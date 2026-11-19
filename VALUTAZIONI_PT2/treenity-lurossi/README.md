*This project has been created as part of the 42 curriculum by lurossi, lmezzaba.*

# treenity

## Description

**treenity** is a small message queue system, inspired by Apache Kafka, written in Go.

Producers publish messages to named **topics**; consumers **subscribe** to a topic and
receive its messages in order. Every message has a **key**, a **value** and an
**offset** (its position in the topic, starting at 0). Consumers can filter messages by
**key prefix**, remember where they stopped (their **committed offset**) and resume from
there after a disconnection, or replay the topic from any offset.

The project is made of two programs that run on the same machine and communicate only
through **named pipes (FIFOs)**:

- `server` keeps all topics, messages and client metadata in memory and serves requests;
- `client` is a single executable with five modes: `create`, `list`, `produce`,
  `subscribe` and `info`.

## Instructions

### Requirements

- Linux
- Go 1.18 or later

### Build

```sh
make        # builds ./server and ./client
make test   # runs the unit tests
make clean  # removes the binaries
make re     # clean + build
```

### Run the server

```sh
./server
# /tmp/operator.server.1337      <- the IPC identifier to give to the clients
```

The server stops on `SIGINT` (Ctrl+C) or `SIGTERM`: it notifies the connected
subscribers, waits at most 5 seconds for the last commits and removes every FIFO it
created.

### Use the client

```sh
IPC=/tmp/operator.server.1337

./client $IPC create user_events
# topic created

./client $IPC list
# user_events

./client $IPC subscribe user_events client0 --prefix user.create
# subscribed to user_events
# (keeps running and prints every matching message)

printf 'user.create:{"user":"reach"}\nuser.delete:{"user":"froz"}\n' \
    | ./client $IPC produce user_events

./client $IPC info client0
# {"client":"client0","topic":"user_events","offset":1,"prefix":"user.create","ipc":"/tmp/operator.server.1337.client0"}
```

| Command | Usage |
|---|---|
| `create` | `./client <ipc> create <topic>` |
| `list` | `./client <ipc> list` |
| `produce` | `./client <ipc> produce <topic> [--raw]` — reads messages from stdin until EOF |
| `subscribe` | `./client <ipc> subscribe <topic> <name> [--prefix <p>] [--offset <n>] [--raw]` |
| `info` | `./client <ipc> info <name>` |

Topic and subscriber names must match `^[a-zA-Z0-9_.-]{1,32}$`.

**Text format** (default): one message per line, `key:value`, split on the first `:`.

**Raw format** (`--raw`), little-endian 32-bit integers:

- producer input: `[keysize][key][valuesize][value]`
- subscriber output: `[offset][keysize][key][valuesize][value]`

Key and value together are limited to 1024 bytes.

**Offsets.** A new subscriber starts at offset 0. A returning subscriber (same name)
resumes from its last committed offset. `--offset N` always starts from `N`. After
printing a message the client commits `offset + 1`, the next offset it wants to read.

**Exit codes.** `0` success, `1` invalid arguments or connection failure, `2` topic or
client error (topic not found, topic already exists, subscriber already active, unknown
client), `3` IPC error (server disconnected). When several errors apply, the precedence
is `1 > 3 > 2`. Only the official output goes to stdout; every error goes to stderr.

## Architecture

### Goroutines

The server is a single process organised around goroutines:

- **main loop** (`Serve`): reads the main FIFO, splits it into lines and dispatches
  each request. Replies are opened with `O_NONBLOCK` and retried for at most 2 seconds,
  so a client that died before reading its answer cannot block the server.
- **one goroutine per topic**: it is the only writer of the topic's message log. It
  receives new messages on a buffered channel (`incoming`), assigns their offset,
  appends them and looks up the matching subscribers in the prefix index.
- **one goroutine per active subscriber**: it owns a personal queue and writes the
  messages to the subscriber's FIFO. The topic goroutine only *enqueues*, which never
  blocks, so a slow or frozen subscriber slows down only its own goroutine, never the
  topic or the server.
- **one goroutine for signals**: it starts the graceful shutdown.

### Synchronization

| Shared state | Protection |
|---|---|
| topic table, creation order, `closing` flag | `topicsMutex` (`sync.RWMutex`) |
| client metadata hashmap | `clientsMutex` (`sync.RWMutex`) |
| a topic's messages and its subscriber index | the topic's own `sync.RWMutex` |
| a subscriber's queue and FIFO | the subscriber's `sync.Mutex` |
| new messages for a topic | the `incoming` channel (single receiver) |

Two rules keep the system correct:

- **No message lost or duplicated on subscribe.** The backlog (old messages from the
  start offset) and the registration in the prefix index happen under the same topic
  lock. The backlog is put at the head of the subscriber's queue, so older messages are
  always delivered before newer ones, in offset order.
- **No send on a closed channel at shutdown.** Producers send to `incoming` while
  holding `topicsMutex` in read mode and only if `closing` is false. Shutdown takes the
  lock in write mode, sets `closing` and only then closes the channels.

### Shutdown

On `SIGINT`/`SIGTERM` the server stops accepting messages, queues a *shutdown*
notification for every subscriber (they exit with code 0), waits up to 5 seconds for
the goroutines to finish while still accepting commits, then closes and removes the
main FIFO and all subscriber FIFOs.

## IPC Choice

We chose **named pipes (FIFOs)**.

- They are fully supported by Go's standard library: `syscall.Mkfifo` creates them and
  they are then used as ordinary files, also with Go's runtime poller (a blocked read
  is released when the file is closed). POSIX and System V message queues would have
  required cgo or raw system calls.
- They have a visible name in `/tmp`, which makes them easy to inspect and to clean up.
- Writes of at most `PIPE_BUF` bytes (4096 on Linux) are **atomic**: since one request
  is always written with a single `write` and stays well below that size (messages are
  limited to 1024 bytes), lines from different producers can never be interleaved.

### Bidirectional communication

A FIFO only goes one way, so the system uses several of them:

| FIFO | Created by | Direction | Used for |
|---|---|---|---|
| `/tmp/operator.server.<pid>` | server | clients → server | every request, message, commit and disconnect |
| `/tmp/operator.server.<pid>.@<client-pid>` | client | server → client | the single reply to a request |
| `/tmp/operator.server.<pid>.<name>` | server | server → subscriber | the messages delivered to a subscriber |

Each request carries the path of the FIFO where the client waits for the answer, like a
return address on an envelope. The `@` cannot appear in a valid name, so a temporary
reply FIFO can never collide with a subscriber FIFO.

The **protocol** is one JSON object per line (types in `internal/protocol.go`):
`Request` (client → server), `Response` (`ok`, exit `code`, `data`, `error`) and
`Delivery` (`offset`, `key`, `value`, `shutdown`). Keys and values are `[]byte`, which
`encoding/json` encodes in base64, so binary data and newlines never break the line
framing.

The server opens its own FIFO in `O_RDWR` so it never receives EOF when clients come
and go. A subscriber, on the contrary, opens its FIFO read-only: if the server dies, the
subscriber gets EOF and exits with code 3. The server writes to subscriber FIFOs
write-only, so a write to a subscriber that died gets `EPIPE`: the subscriber is marked
inactive and its name becomes available again.

## Data Structures

### Client metadata: hashmap

Client metadata (`client`, `topic`, committed `offset`, `prefix`, `ipc`, `active`) is
stored in a generic hashmap written from scratch (`internal/utils/hashmap.go`):

- **hash function**: djb2 (`h = h*33 + byte`), simple and well distributed for short
  ASCII names;
- **collisions**: separate chaining — every bucket is a linked list of entries, new
  entries are inserted at the head;
- **resizing**: 64 buckets at start, doubled when the load factor exceeds 0.75, so
  chains stay short and lookups are O(1) on average.

The same hashmap stores the topics.

### Prefix matching: prefix index

Each topic keeps its subscribers in a **prefix index** (`internal/server/prefixindex.go`):

- subscribers with an **empty prefix** go into a direct list and receive every message
  without any lookup;
- the others are stored in a hashmap from **prefix → subscribers**.

To find the subscribers of a message with key `user.create`, the index looks up every
prefix of the key: `u`, `us`, `use`, … `user.create`. Each lookup is O(1), so a message
costs O(length of the key) lookups, independently of the number of subscribers, and
keys are at most 1024 bytes.

We chose it over a trie because it reuses the hashmap we already needed, it is much
simpler to implement and to test, and for keys this short it is as fast in practice.
A trie would save memory when many prefixes share a long common beginning, which is not
the typical case here.

## Testing

The tests use Go's standard `testing` package and run with:

```sh
make test          # or: go test -v ./...
```

The command exits with a non-zero status if a test fails.

**Prefix matching** (`internal/server/prefixindex_test.go`):

| Area | Tests |
|---|---|
| basic matching | exact match, key longer or shorter than the prefix, different and unrelated keys, prefix in the middle of the key, case sensitivity, empty key, nested prefixes, several subscribers on the same prefix, binary keys |
| empty prefix | receives every key; stored in the direct list, not in the map |
| operations | add, remove (including removing the last subscriber of a prefix), find, list |
| edge cases | empty index, single subscriber, 10,000 subscribers |
| integration | 300 subscribers with mixed prefixes (empty, short, long, binary), some removed, compared with a brute-force `strings.HasPrefix` on 2,000 random keys; backlog filtering by prefix and start offset |

**Hashmap** (`internal/utils/hashmap_test.go`): insertion and lookup, overwrite,
collisions (keys forced into the same bucket, deleted at the head, middle and tail of
the chain), deleting a missing key, resizing with 100,000 keys, empty map.

The whole system was also tested by hand and with the `ft_fish` program provided with
the subject: several producers and subscribers at the same time,
subscribers killed with `kill -9`, frozen with `SIGSTOP`, and server shutdowns under
load.

## Resources

- [fifo(7)](https://man7.org/linux/man-pages/man7/fifo.7.html) and
  [pipe(7)](https://man7.org/linux/man-pages/man7/pipe.7.html) — FIFO semantics,
  `O_NONBLOCK`, `ENXIO`, `EPIPE`, atomicity of `PIPE_BUF` writes
- [signal(7)](https://man7.org/linux/man-pages/man7/signal.7.html)
- [Go standard library](https://pkg.go.dev/std) — `os`, `os/signal`, `syscall`, `bufio`,
  `encoding/json`, `encoding/binary`, `sync`, `testing`
- [Effective Go](https://go.dev/doc/effective_go) and
  [A Tour of Go](https://go.dev/tour/)
- [Apache Kafka — Introduction](https://kafka.apache.org/documentation/#introduction) —
  topics, offsets and consumer commits
- [Conventional Commits](https://www.conventionalcommits.org/)

### Use of AI

We used Claude (Anthropic) as an assistant during the project:

- **Learning**: explanations of Go (types, pointers, goroutines, channels, JSON,
  `encoding/binary`), of FIFOs and signals, and of the protocol design (reply FIFO,
  framing, offsets and commits).
- **Client** (`cmd/client`): written by lurossi with the AI as a tutor, reviewing the
  code step by step. Some parts were written by the AI and then studied and integrated:
  the connection and request helpers, raw input parsing and raw output encoding, and the
  graceful `Ctrl+C` handling of `subscribe`.
- **Server** (`internal/server`): written by lmezzaba. The AI reviewed it, reproduced
  three bugs (a deadlock with dead subscribers, a panic at shutdown, out-of-order
  delivery) and wrote the fixes (per-subscriber queues, the shutdown guard, the
  `--offset` filter, the release of dead subscribers' names).
- **README**: drafted by the AI and reviewed by both of us.

Every part generated with the AI was read, tested and discussed by both of us before
being committed.