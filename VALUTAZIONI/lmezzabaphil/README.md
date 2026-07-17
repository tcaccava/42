# *This project has been created as part of the 42 curriculum by lmezzaba.*

# Codexion

## Description

**Codexion** is a multithreaded synchronization project developed in **C** using the POSIX Threads (**pthreads**) library.

The goal of the project is to simulate a collaborative workspace where multiple coders compete for a limited number of shared USB dongles required to compile their code.

Each coder repeatedly performs the following cycle:

- Acquire two USB dongles
- Compile
- Debug
- Refactor

The simulation terminates when either:

- Every coder has completed the required number of compilations.
- One coder burns out because they failed to start compiling before their deadline.

This project focuses on concurrent programming, synchronization, scheduling algorithms, resource management and race-condition prevention.

---

# Features

- POSIX Threads (`pthread`)
- Mutex synchronization
- Condition variables
- Serialized logging
- FIFO scheduling
- Earliest Deadline First (EDF) scheduling
- Custom binary heap priority queue
- USB dongle cooldown management
- Dedicated monitor thread
- Safe input parsing
- Memory leak free

---

# Project Structure

```text
.
├── include/
│   └── codexion.h
├── src/
│   ├── main.c
│   ├── parser.c
│   ├── coder.c
│   ├── monitor.c
│   ├── monitor_utils.c
│   ├── heap.c
│   ├── utils.c
│   └── utils2.c
├── Makefile
└── README.md
```

---

# Instructions

## Compilation

Compile the project:

```bash
make
```

Remove object files:

```bash
make clean
```

Remove executable and object files:

```bash
make fclean
```

Recompile everything:

```bash
make re
```

---

## Usage

```bash
./codexion \
number_of_coders \
time_to_burnout \
time_to_compile \
time_to_debug \
time_to_refactor \
number_of_compiles_required \
dongle_cooldown \
scheduler
```

Example:

```bash
./codexion 5 800 200 200 200 5 50 fifo
```

or

```bash
./codexion 5 800 200 200 200 5 50 edf
```

to check for race conditions:
```bash
cc -Wall -Wextra -pthread -fsanitize=thread -g -Iinclude src/*.c -o codexion_tsan
```

### Arguments

| Argument | Description |
|----------|-------------|
| `number_of_coders` | Number of coder threads |
| `time_to_burnout` | Maximum time before a coder burns out (ms) |
| `time_to_compile` | Compile duration (ms) |
| `time_to_debug` | Debug duration (ms) |
| `time_to_refactor` | Refactor duration (ms) |
| `number_of_compiles_required` | Number of compilations required before ending the simulation |
| `dongle_cooldown` | Cooldown time after releasing a dongle (ms) |
| `scheduler` | Scheduling policy (`fifo` or `edf`) |

---

# Architecture

The project is divided into four main components.

## Parser

Responsible for:

- validating command-line arguments;
- safely parsing numeric values;
- validating the scheduler;
- rejecting invalid inputs.

---

## Coder Threads

Each coder is represented by a POSIX thread.

Every coder repeatedly:

1. Requests two USB dongles.
2. Compiles.
3. Releases both dongles.
4. Debugs.
5. Refactors.

---

## Monitor Thread

A dedicated monitor thread continuously:

- checks burnout deadlines;
- assigns available dongles;
- wakes waiting coders;
- stops the simulation when required.

---

## Scheduler

Every request is stored inside a custom binary heap.

Two scheduling policies are implemented:

### FIFO

Requests are processed according to their arrival time.

### EDF (Earliest Deadline First)

Requests are processed according to the earliest burnout deadline.

If two deadlines are equal, the implementation breaks ties using:

1. request arrival time;
2. coder ID.

This guarantees deterministic behaviour.

---

# Blocking Cases Handled

## Deadlock Prevention

Coders never lock dongles directly.

Instead:

- every request is inserted into a shared priority queue;
- the monitor thread grants access only when both dongles are simultaneously available;
- coders remain blocked until the monitor wakes them.

This centralized resource allocation removes circular waits and prevents deadlocks.

---

## Starvation Prevention

FIFO guarantees fair ordering.

EDF always prioritizes the coder with the earliest deadline, minimizing the probability of starvation when the parameters are feasible.

---

## Dongle Cooldown

Every dongle stores the timestamp of its last release.

Before assigning a dongle, the monitor checks:

```c
current_time - last_used >= dongle_cooldown
```

This guarantees the mandatory cooldown period required by the subject.

---

## Precise Burnout Detection

A dedicated monitor periodically checks every coder.

If:

```c
current_time - last_compile_start > time_to_burnout
```

the simulation immediately stops and prints:

```text
timestamp coder burned out
```

---

## Log Serialization

All output is protected by a dedicated logging mutex.

This guarantees:

- no interleaved messages;
- thread-safe output;
- deterministic logging.

---

# Thread Synchronization Mechanisms

## pthread_mutex_t

Mutexes protect all shared resources, including:

- global simulation state;
- priority queue;
- USB dongles;
- stop flag;
- logging.

Mutexes prevent race conditions between concurrent threads.

---

## pthread_cond_t

Each coder owns a condition variable.

The workflow is:

1. the coder inserts its request into the priority queue;
2. the coder blocks using `pthread_cond_wait()`;
3. the monitor assigns both dongles;
4. the monitor wakes the selected coder using `pthread_cond_signal()`.


---

## Monitor Thread

Instead of allowing every thread to compete directly for dongles, a dedicated monitor performs all scheduling decisions.

This simplifies synchronization while guaranteeing thread-safe access to shared resources.

---

## Custom Priority Queue

The scheduler is implemented using a custom binary heap.

The heap supports:

- insertion;
- removal of the highest-priority request.

Priority depends on the selected scheduling algorithm (FIFO or EDF).

No external priority queue implementation is used.

---

# AI Usage

Artificial Intelligence was used only as a learning and productivity assistant.

AI was used to:

- clarify project requirements;
- proofread the README.
- Norminette compilance.

---

# Resources

## POSIX Threads

https://www.geeksforgeeks.org/operating-systems/posix-threads-in-os/

---

# Author

**lmezzaba**

42 School — Codexion
