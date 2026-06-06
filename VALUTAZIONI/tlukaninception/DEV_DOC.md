# Developer Documentation

## Prerequisites

- Docker Engine (20.10+)
- Docker Compose plugin (`docker compose` command)
- `sudo` access on the host machine
- `make`

## Environment setup from scratch

### 1. Create the secrets directory and files

```bash
mkdir -p secrets
echo "your_db_password"      > secrets/db_password.txt
echo "your_db_root_password" > secrets/db_root_password.txt
echo "your_wp_admin_password" > secrets/credentials.txt
```

### 2. Create the `.env` file

Create `srcs/.env` with the following variables:

```env
DOMAIN_NAME=tlukan.42.fr

# MariaDB
MYSQL_DATABASE=wordpress
MYSQL_USER=wpuser

# WordPress
WP_TITLE=My WordPress Site
WP_ADMIN_USER=wpmaster
WP_ADMIN_EMAIL=admin@tlukan.42.fr
WP_USER=wpeditor
WP_USER_EMAIL=editor@tlukan.42.fr
WP_USER_PASSWORD=your_user_password
```

> Note: `WP_ADMIN_USER` must not contain `admin`, `Admin`, `administrator`, or `Administrator`.

### 3. Add the domain to `/etc/hosts`

```bash
echo "127.0.0.1 tlukan.42.fr" | sudo tee -a /etc/hosts
```

## Build and launch

```bash
make
```

This will:
1. Create `/home/tlukan/data/mariadb` and `/home/tlukan/data/wordpress` on the host
2. Build the three Docker images (nginx, wordpress, mariadb)
3. Start all containers in detached mode

## Useful commands

### Container management

```bash
docker ps                        # list running containers
docker logs nginx                # view nginx logs
docker logs wordpress            # view wordpress logs
docker logs mariadb              # view mariadb logs
docker exec -it wordpress bash   # open shell in wordpress container
docker exec -it mariadb bash     # open shell in mariadb container
```

### Volume management

```bash
docker volume ls                 # list all volumes
docker volume inspect inception_db_data          # inspect DB volume
docker volume inspect inception_wordpress_files  # inspect WP volume
```

### Rebuild a single service

```bash
docker compose -f srcs/docker-compose.yaml build wordpress
docker compose -f srcs/docker-compose.yaml up -d wordpress
```

### Full reset

```bash
make fclean   # removes containers, images, volumes, and host data
make re       # fclean + full rebuild
```

## Data persistence

Data is stored in two locations on the host machine:

| Path | Contents |
|---|---|
| `/home/tlukan/data/mariadb` | MariaDB database files |
| `/home/tlukan/data/wordpress` | WordPress core files and uploads |

These directories are mounted into the containers via Docker named volumes (`db_data` and `wordpress_files`) defined in `srcs/docker-compose.yaml`.

Data survives `make down` but is deleted by `make fclean`.

## Project structure

```
inception/
├── Makefile
├── README.md
├── USER_DOC.md
├── DEV_DOC.md
├── secrets/
│   ├── credentials.txt       # WordPress admin password (git-ignored)
│   ├── db_password.txt       # MariaDB user password (git-ignored)
│   └── db_root_password.txt  # MariaDB root password (git-ignored)
└── srcs/
    ├── .env                  # Environment variables (git-ignored)
    ├── docker-compose.yaml
    └── requirements/
        ├── nginx/
        │   ├── Dockerfile
        │   ├── conf/nginx.conf
        │   └── tools/setup.sh
        ├── wordpress/
        │   ├── Dockerfile
        │   ├── conf/www.conf
        │   └── tools/wp-setup.sh
        └── mariadb/
            ├── Dockerfile
            ├── conf/my.cnf
            └── tools/db-setup.sh
```
