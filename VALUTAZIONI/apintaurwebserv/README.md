# webserv

> A non-blocking HTTP/1.1 server written from scratch in C++98 — the 42 Common Core project where sockets, the HTTP RFCs, and an NGINX-style configuration file all click into place.

| | |
|---|---|
| **Authors** | [apintaur](https://profile.intra.42.fr/users/apintaur) · [ahabdelr](https://profile.intra.42.fr/users/ahabdelr) |
| **Cursus** | 42 / 1337 Common Core |
| **Language** | C++98 |
| **Norm** | 42 Norm |

## Table of contents

- [About](#about)
- [Build & run](#build--run)
- [How it works](#how-it-works)
- [Configuration file](#configuration-file)
- [Mandatory part](#mandatory-part)
- [Bonus part](#bonus-part)
- [Testing](#testing)
- [Resources](#resources)
- [Authors](#authors)

## About

`webserv` is a from-scratch HTTP/1.1 server: no third-party HTTP library and no forked copy of an existing server, just raw sockets, a hand-written request parser and response builder, and a single event loop juggling every client at once. Its behaviour is driven by an NGINX-style configuration file (ports, virtual hosts, routes, error pages, CGI...), and it's built to survive a real browser, `curl`, raw `telnet` traffic, and a stress test without ever leaking, crashing, or hanging.

## Build & run

```sh
make
./webserv [configuration_file]
```

If no configuration file is given, `webserv` falls back to its default path. `make re` and `make fclean` behave as usual; `make clean` removes object files only.

## How it works

**Server core & event loop.** Every configured `host:port` opens one non-blocking listening socket (`fcntl(fd, F_SETFL, O_NONBLOCK)` — the only flag the subject allows). A single multiplexing call — `poll()`, `select()`, `epoll()` or `kqueue()`, depending on the implementation — watches every listening and connected socket for read *and* write readiness in the same pass. No `read`/`recv`/`write`/`send` ever fires outside of a "this fd is ready" notification, and `errno` is never inspected right after one — both are instant disqualifiers at evaluation.

**Parsing a request.** Bytes are fed into the parser incrementally as they arrive, walking the request line, headers, and optional body, and switching state as soon as enough bytes are available. This lets the server reject a malformed request (`400`) without waiting for the whole thing to land, and copes naturally with chunked transfer encoding.

**Building a response.** Once a request is understood, a status line, the relevant RFC 7230–7235 headers, and a body are assembled — from a static file, a generated directory listing, or the output of a CGI script — and queued to go out on the next writable tick of the event loop.

**CGI.** A request mapped to a CGI route is handed to a forked child — the only sanctioned use of `fork()` in this project. The requested script becomes the child's first argument, the classic CGI meta-variables are exported, and the result streams back over a pipe; chunked input is un-chunked before the CGI sees it, and EOF marks the end of the body in either direction whenever no `Content-Length` is present.

## Configuration file

The syntax mirrors NGINX's `server` blocks. A minimal example:

```nginx
server {
    listen               127.0.0.1:8080;
    server_name          webserv.local;
    client_max_body_size 10M;

    error_page  404          /errors/404.html;
    error_page  500 502 503  /errors/50x.html;

    location / {
        root          ./www;
        index         index.html;
        allow_methods GET POST;
        autoindex     off;
    }

    location /upload {
        root          ./www/uploads;
        allow_methods POST DELETE;
        upload_store  ./www/uploads;
    }

    location /cgi-bin {
        root          ./www/cgi-bin;
        cgi_extension .php;
        cgi_path      /usr/bin/php-cgi;
    }
}
```

Several `server` blocks can share the same `host:port`; the first one declared becomes the default for any request that doesn't match a `server_name`.

## Mandatory part

Straight from the subject — tick items off as they're implemented and evaluated:

- [ ] HTTP server written entirely in C++98.
- [ ] Conditionally RFC 7230–7235 (HTTP/1.1) compliant — only this header subset is required: `Accept-Charset`, `Accept-Language`, `Allow`, `Authorization`, `Content-Language`, `Content-Length`, `Content-Location`, `Content-Type`, `Date`, `Host`, `Last-Modified`, `Location`, `Referer`, `Retry-After`, `Server`, `Transfer-Encoding`, `User-Agent`, `WWW-Authenticate`.
- [ ] Non-blocking I/O: exactly **one** multiplexing call (`select`/`poll`/`epoll`/`kqueue`) handles every read and write, listening sockets included, watching for read- and write-readiness together.
- [ ] No client request ever hangs forever; misbehaving clients get disconnected instead.
- [ ] `read`/`recv`/`write`/`send` only ever happen right after the multiplexer reports that fd as ready — never blind.
- [ ] `errno` is never inspected immediately after a read or write.
- [ ] Built-in default error pages are served whenever the configuration doesn't supply its own.
- [ ] No memory leaks, no crashes — including under simulated out-of-memory conditions, once startup itself succeeded.
- [ ] `fork()` is used for nothing except spawning CGI.
- [ ] Only `iostream`, `string`, `vector`, `list`, `queue`, `stack`, `map` and `algorithm` are used from the STL.
- [ ] A configuration file path is accepted as an optional first CLI argument, falling back to a default path otherwise.
- [ ] The configuration file supports, NGINX-style: per-server `listen` host:port, optional `server_name`(s), one default server per host:port, default error pages, a client body size limit, and per-route rules for allowed methods, root mapping, directory-listing toggle, default index file, CGI-by-extension, and upload storage location.

## Bonus part

Only worth attempting once the mandatory part is airtight — and not all of these are required:

- [ ] Plugins (compression, charset conversion, ...) that can be loaded/unloaded from the terminal at runtime. *(repeatable — more plugin kinds, more credit)*
- [ ] A worker model (processes or threads) that isn't spawned per client and can serve an unbounded number of requests.
- [ ] Configurable worker count.
- [ ] Regex-based routes.
- [ ] A reverse proxy to another HTTP/HTTPS server.
- [ ] An in-process scripting module (e.g. PHP) that generates pages without forking an external CGI binary. *(repeatable)*

## Testing

Resilience is the main thing being graded here: the server is expected to keep running no matter what it's thrown, and it gets stress-tested as part of evaluation. Worth covering before submitting:

- Open it in a real browser and click around — a `GET`, a form `POST`, a few broken links.
- Talk to it directly with `telnet`/`nc` to see the raw protocol: multi-packet requests, slow clients, malformed lines.
- Compare ambiguous behaviour (e.g. how `server_name` resolves) against `nginx` running an equivalent configuration.
- Stress-test with `siege` or `ab` to surface leaks, hangs, or crashes under load.
- Write your own tests in whatever language is fastest to iterate in (Python, Go, even C++) rather than leaning on a single tester script — a server that only survives one tester is fragile by definition.

## Resources

- [RFC 9110 — HTTP Semantics](https://www.rfc-editor.org/rfc/rfc9110)
- [RFC 9112 — HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9112)
- [RFC 3875 — The Common Gateway Interface (CGI)](https://www.rfc-editor.org/rfc/rfc3875)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [NGINX documentation](https://nginx.org/en/docs/)

## Authors

| Login | Profile |
|---|---|
| **apintaur** | <https://profile.intra.42.fr/users/apintaur> |
| **ahabdelr** | <https://profile.intra.42.fr/users/ahabdelr> |

---

Built as part of the 42 Common Core. Educational project — not intended for production use.
