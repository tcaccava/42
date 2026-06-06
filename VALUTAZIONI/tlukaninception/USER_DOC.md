# User Documentation

## Services provided

This stack runs three services:

| Service | Role | Access |
|---|---|---|
| NGINX | Web server / entrypoint | Port 443 (HTTPS) |
| WordPress + php-fpm | Website and CMS | Via NGINX |
| MariaDB | Database | Internal only |

Only NGINX is reachable from outside. WordPress and MariaDB are internal to the Docker network.

## Start and stop the project

From the root of the repository:

```bash
make        # build images and start all containers
make down   # stop and remove containers (data is preserved)
make clean  # stop containers, remove images and volumes
make fclean # full reset including host data directories
make re     # full reset and restart
```

## Access the website

Add the following line to your `/etc/hosts` file if not already present:
```
127.0.0.1 tlukan.42.fr
```

Then open your browser and go to:
- **Website**: `https://tlukan.42.fr`
- **Admin panel**: `https://tlukan.42.fr/wp-admin`

The browser will warn about a self-signed certificate — this is expected. Accept the exception to proceed.

## Credentials

Credentials are stored in the `secrets/` directory at the root of the project:

| File | Contents |
|---|---|
| `secrets/credentials.txt` | WordPress admin password |
| `secrets/db_password.txt` | MariaDB user password |
| `secrets/db_root_password.txt` | MariaDB root password |

WordPress usernames and other settings are defined in `srcs/.env`.

**These files must never be committed to git.**

## Check that services are running

```bash
docker ps
```

You should see three containers running: `nginx`, `wordpress`, `mariadb`.

To check logs for a specific container:
```bash
docker logs nginx
docker logs wordpress
docker logs mariadb
```

To check that NGINX is responding:
```bash
curl -k https://tlukan.42.fr
```

The `-k` flag skips certificate verification for self-signed certificates.
