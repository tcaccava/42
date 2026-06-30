*This project has been created as part of the 42 curriculum by fbartole.*

# Inception

## Description

Inception is a System Administration project at 42. The goal is to build, from
scratch, a small containerised web infrastructure that runs inside a Virtual
Machine and is orchestrated with `docker compose`.

The stack is composed of three custom-built containers, each running a single
service:

- **NGINX** — reverse proxy, the only entry point on port 443, TLSv1.2/1.3 only
- **WordPress + php-fpm** — application server (no nginx inside)
- **MariaDB** — database (no nginx inside)

Two named Docker volumes persist the database and the WordPress files on the
host under `/home/fbartole/data/`. A dedicated bridge network connects the
three containers. The domain `fbartole.42.fr` points to the local IP.

## Instructions

Prerequisites: a Linux VM (Debian or Ubuntu recommended) with `docker`,
`docker compose` and `make` installed.

```sh
# 1. clone
git clone <repo-url> inception && cd inception

# 2. fill secrets (4 files in secrets/, one password per file, no trailing data)
$EDITOR secrets/db_root_password.txt
$EDITOR secrets/db_password.txt
$EDITOR secrets/wp_admin_password.txt
$EDITOR secrets/wp_user_password.txt

# 3. review env
$EDITOR srcs/.env        # copy from srcs/.env.example if missing

# 4. build and start
make                     # creates /home/fbartole/data, updates /etc/hosts, builds & starts

# 5. open https://fbartole.42.fr in a browser
```

Other Makefile targets: `make down`, `make logs`, `make ps`, `make clean`,
`make fclean`, `make re`.

## Project description

### Use of Docker

Each service is described by its own `Dockerfile` under
`srcs/requirements/<service>/`, built from `alpine:3.24.0` (penultimate stable
Alpine at the time of writing). No image is pulled ready-made from Docker Hub.
The build, network, volumes, secrets and restart policy are declared in
`srcs/docker-compose.yml`. The Makefile at the project root drives the whole
pipeline.

### Virtual Machines vs Docker

A VM virtualises an entire operating system on top of a hypervisor: it has its
own kernel, its own init system and a large memory/disk footprint. A Docker
container shares the host kernel and isolates only the user-space (filesystem,
PIDs, network namespace). Containers start in milliseconds, weigh tens of MB
and are reproducible from a `Dockerfile`. The 42 subject still requires the
whole stack to be hosted inside a VM, so we get both: the VM provides
operating-system level isolation from the host, Docker provides service-level
isolation inside the VM.

### Secrets vs Environment variables

Environment variables (`srcs/.env`) are convenient for non-sensitive
configuration: domain name, database name, WordPress title, user names. They
are visible in `docker inspect` and in process listings, so they are a bad
place for passwords. Docker secrets, by contrast, are mounted as read-only
files inside `/run/secrets/` and are not exposed through `inspect`. In this
project all passwords (`db_root_password`, `db_password`, `wp_admin_password`,
`wp_user_password`) live in `secrets/*.txt` files that are git-ignored.

### Docker Network vs Host Network

`network: host` would share the host's network namespace with the containers,
breaking isolation and making port collisions possible. A custom **bridge**
network (`inception`) gives each container its own IP, lets containers talk to
each other by service name (`mariadb`, `wordpress`, `nginx`) and exposes to
the host only what is explicitly published — here only nginx's 443.

### Docker Volumes vs Bind Mounts

A bind mount maps an arbitrary host path into the container and is tightly
coupled to the host's filesystem layout. A named volume is managed by Docker,
gets a stable name, can be inspected/backed up via the Docker CLI and is the
canonical way to persist stateful data. The subject mandates named volumes for
both `db_data` and `wp_data`; the volumes are configured with `driver: local`
and `driver_opts` so their backing storage lives under `/home/fbartole/data/`.

## Resources

- Docker docs — https://docs.docker.com/
- docker compose reference — https://docs.docker.com/compose/compose-file/
- WordPress wp-cli — https://developer.wordpress.org/cli/commands/
- Alpine Linux packages — https://pkgs.alpinelinux.org/

AI was used to scaffold the initial project tree, generate the first draft of
the Dockerfiles / entrypoints / nginx config, and to review the README and
docs. Every generated file has been read, edited and validated by the author.
