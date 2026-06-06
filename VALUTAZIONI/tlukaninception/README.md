*This project has been created as part of the 42 curriculum by tlukan.*

# Inception

## Description

Inception is a system administration project that uses Docker to set up a small infrastructure composed of multiple services running in dedicated containers. The goal is to understand containerization, service orchestration, and best practices for running isolated processes.

The stack includes:
- **NGINX** — the only entrypoint, listening on port 443 with TLSv1.2/TLSv1.3 only
- **WordPress + php-fpm** — the web application, without nginx
- **MariaDB** — the database backend, without nginx

All images are built from scratch using custom Dockerfiles based on Debian Bullseye. No pre-built images from DockerHub are used (except the base OS).

## Instructions

### Prerequisites

- Docker and Docker Compose installed
- `sudo` access to create directories under `/home/tlukan/data`
- Add the domain to `/etc/hosts`:
  ```
  127.0.0.1 tlukan.42.fr
  ```
- Create `srcs/.env` with the required environment variables (see DEV_DOC.md)
- Create the secret files under `secrets/` (see DEV_DOC.md)

### Build and run

```bash
make        # creates data directories, builds images, starts containers
make down   # stops and removes containers
make clean  # stops containers and removes volumes and images
make fclean # full cleanup including host data directories
make re     # fclean + all
```

### Access

- Website: `https://tlukan.42.fr`
- WordPress admin panel: `https://tlukan.42.fr/wp-admin`

## Project Description

### How Docker is used

The project uses Docker Compose to orchestrate three containers that communicate over a dedicated bridge network (`inception_network`). Each service has its own Dockerfile and runs a single process in the foreground as PID 1. Two named volumes persist data across container restarts: one for the WordPress files and one for the MariaDB database.

### Design choices

- **Base image**: Debian Bullseye (penultimate stable Debian release as required)
- **TLS**: Self-signed certificate generated at container startup via `openssl`, with only TLSv1.2 and TLSv1.3 allowed
- **Secrets**: Sensitive values (database passwords, admin credentials) are stored in Docker secrets mounted at `/run/secrets/` inside containers — never passed as plain environment variables
- **Restart policy**: `unless-stopped` ensures containers restart on crash but not on explicit `docker stop`

### Virtual Machines vs Docker

| | Virtual Machine | Docker |
|---|---|---|
| Isolation | Full OS, hardware-level | Process-level, shared kernel |
| Resource usage | Heavy (GBs of RAM/disk) | Lightweight (MBs) |
| Startup time | Minutes | Seconds |
| Portability | Limited | High (image = reproducible environment) |
| Use case | Full OS environments, strong isolation | Microservices, CI/CD, reproducible builds |

Docker containers share the host kernel and isolate processes using Linux namespaces and cgroups. VMs run a full guest OS on top of a hypervisor.

### Secrets vs Environment Variables

| | Secrets | Environment Variables |
|---|---|---|
| Storage | Files mounted at `/run/secrets/` | Injected into process environment |
| Visibility | Not visible in `docker inspect` | Visible in `docker inspect` |
| Use case | Passwords, API keys, tokens | Non-sensitive config (domain, usernames) |
| Risk | Low — file access controlled | Higher — environment can be leaked in logs |

This project uses Docker secrets for passwords and credentials, and `.env` for non-sensitive configuration such as domain name and WordPress site title.

### Docker Network vs Host Network

| | Docker Network (bridge) | Host Network |
|---|---|---|
| Isolation | Containers have their own network namespace | Container shares host network stack |
| Port mapping | Explicit — only published ports are accessible | All ports exposed on host directly |
| Security | Better — containers cannot reach host services by default | Weaker — no network isolation |
| Inter-container communication | By service name (DNS resolution) | Via localhost |

This project uses a bridge network (`inception_network`). Only port 443 is published to the host via NGINX. `network: host` is explicitly forbidden by the subject.

### Docker Volumes vs Bind Mounts

| | Named Volumes | Bind Mounts |
|---|---|---|
| Management | Managed by Docker | Managed by the user |
| Declaration | Declared in `docker-compose.yml` | Path directly mapped |
| Portability | Higher | Lower (depends on host path existing) |
| Use case | Persistent data (DB, files) | Development (live code editing) |

This project uses named volumes (`db_data`, `wordpress_files`) configured with the local driver to store data at `/home/tlukan/data` on the host, as required by the subject.

## Resources

### Documentation

- [Docker official documentation](https://docs.docker.com/)
- [Docker Compose reference](https://docs.docker.com/compose/)
- [NGINX documentation](https://nginx.org/en/docs/)
- [WordPress CLI (WP-CLI)](https://wp-cli.org/)
- [MariaDB documentation](https://mariadb.com/kb/en/documentation/)
- [OpenSSL — generating certificates](https://www.openssl.org/docs/manmaster/man1/openssl-req.html)
- [PHP-FPM configuration](https://www.php.net/manual/en/install.fpm.configuration.php)
- [Docker secrets](https://docs.docker.com/engine/swarm/secrets/)
- [Best practices for writing Dockerfiles](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)
- [PID 1 and signal handling in containers](https://cloud.google.com/architecture/best-practices-for-building-containers#signal-handling)

### AI usage

Claude Code (claude.ai) was used during this project for the following tasks:
- Reviewing the project structure against the subject requirements
- Identifying configuration bugs (wrong file references, missing environment variables)
- Explaining concepts: differences between VMs and Docker, secrets vs env vars, network modes, volume types
- Drafting and reviewing documentation files (README.md, USER_DOC.md, DEV_DOC.md)

All generated content was reviewed, understood, and validated before being included in the project.
