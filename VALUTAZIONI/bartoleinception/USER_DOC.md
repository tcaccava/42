# User documentation

## What the stack provides

| Service     | Role                                         |
|-------------|----------------------------------------------|
| NGINX       | HTTPS entry point on port 443 (TLSv1.2/1.3)  |
| WordPress   | Web application served via php-fpm           |
| MariaDB     | Persistent storage for the WordPress site    |

Only NGINX is reachable from outside the Docker network.

## Start and stop

From the project root:

| Action                              | Command          |
|-------------------------------------|------------------|
| Build and start everything          | `make`           |
| Stop containers (keep volumes)      | `make down`      |
| Stop without removing containers    | `make stop`      |
| Start previously stopped containers | `make start`     |
| Tail logs                           | `make logs`      |
| List running containers             | `make ps`        |
| Stop + remove project volumes       | `make clean`     |
| Full reset (volumes + host data)    | `make fclean`    |
| Rebuild from scratch                | `make re`        |

## Accessing the website and the admin panel

1. Make sure `127.0.0.1 fbartole.42.fr` is present in `/etc/hosts`
   (the Makefile adds it on first run).
2. Open `https://fbartole.42.fr` in a browser. Accept the self-signed
   certificate warning.
3. The admin panel is at `https://fbartole.42.fr/wp-admin`. Sign in with the
   administrator credentials configured in `secrets/wp_admin_password.txt`
   and `srcs/.env` (`WP_ADMIN_USER`).

## Credentials

All passwords live in plain text files under `secrets/`, one password per
file, no trailing whitespace:

- `secrets/db_root_password.txt`   — MariaDB root
- `secrets/db_password.txt`        — MariaDB user (`MYSQL_USER` in .env)
- `secrets/wp_admin_password.txt`  — WordPress administrator
- `secrets/wp_user_password.txt`   — WordPress secondary user

These files are git-ignored. Rotate a password by editing the file then
running `make re`.

## Checking that everything is running

```sh
make ps                                # all three containers must be "Up"
curl -kI https://fbartole.42.fr        # expect HTTP/2 200 (or 301 to /wp-...)
docker exec mariadb mariadb -uroot \
    -p"$(cat secrets/db_root_password.txt)" -e "SHOW DATABASES;"
```

If `make ps` shows a container restarting in a loop, inspect its logs with
`docker logs <container>` (for example `docker logs wordpress`).
