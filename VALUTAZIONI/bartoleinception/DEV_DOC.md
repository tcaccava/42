# Developer documentation

## Setting up the environment from scratch

### Host (the VM)

- Debian 12 or Ubuntu 22.04 inside VirtualBox / UTM
- Required packages: `docker.io`, `docker-compose-plugin` (or `docker-compose`),
  `make`, `sudo`
- The user running `make` must be in the `docker` group, or must use `sudo`

### Configuration files

1. `srcs/.env` — copy `srcs/.env.example`, adjust `DOMAIN_NAME` and the user
   names. Never commit it.
2. `secrets/*.txt` — four files, one password per file. Never commit them.
3. `/etc/hosts` — the Makefile appends `127.0.0.1 fbartole.42.fr` on first
   run; verify it is present.
4. `/home/fbartole/data/{db,wordpress}` — created by the Makefile, used as
   the backing storage for the two named volumes.

## Building and launching

```sh
make            # alias for `make up`
make build      # docker compose build only
make up         # build + run detached
make down       # stop and remove containers
make logs       # follow logs of every service
make ps         # show container status
make clean      # down + remove project volumes
make fclean     # clean + prune images/volumes + wipe /home/fbartole/data
make re         # fclean + up
```

The actual compose invocation is:

```sh
docker compose -f srcs/docker-compose.yml --env-file srcs/.env <subcommand>
```

## Managing containers and volumes

```sh
docker ps -a
docker logs <container>
docker exec -it <container> sh           # shell into a running container
docker volume ls
docker volume inspect srcs_db_data       # path on host, mountpoint, options
docker network inspect srcs_inception
```

To force a clean rebuild of a single service:

```sh
docker compose -f srcs/docker-compose.yml build --no-cache wordpress
docker compose -f srcs/docker-compose.yml up -d wordpress
```

## Where the data lives

| Volume       | Host path                          | Mounted inside container |
|--------------|------------------------------------|--------------------------|
| `db_data`    | `/home/fbartole/data/db`           | `/var/lib/mysql`         |
| `wp_data`    | `/home/fbartole/data/wordpress`    | `/var/www/html`          |

Both are declared with `driver: local` + `driver_opts` in
`srcs/docker-compose.yml`, so Docker manages them as named volumes while the
backing files are stored under the host paths above. Removing the containers
does not delete the data; only `make fclean` (or `docker volume rm`) does.

The MariaDB entrypoint initialises the database on first run (when
`/var/lib/mysql/mysql` is missing) and exits the init phase before exec-ing
into `mysqld` so the daemon becomes PID 1. The WordPress entrypoint
downloads core, generates `wp-config.php`, runs `wp core install` and creates
the two users on first run only; subsequent runs skip directly to `php-fpm`.
