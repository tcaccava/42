#!/bin/bash

# Legge i secrets dai file
DB_PASSWORD=$(cat /run/secrets/db_password)
DB_ROOT_PASSWORD=$(cat /run/secrets/db_root_password)

# Avvia MariaDB in modalità sicura per configurarlo
mysqld_safe &
sleep 3

# Imposta la password di root e crea il database e l'utente
mysql -u root << EOF
ALTER USER 'root'@'localhost' IDENTIFIED BY '${DB_ROOT_PASSWORD}';
CREATE DATABASE IF NOT EXISTS ${MYSQL_DATABASE};
CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${DB_PASSWORD}';
GRANT ALL PRIVILEGES ON ${MYSQL_DATABASE}.* TO '${MYSQL_USER}'@'%';
FLUSH PRIVILEGES;
EOF

# Ferma MariaDB e lo fa ripartire in foreground (come processo principale)
mysqladmin -u root -p"${DB_ROOT_PASSWORD}" shutdown
exec mysqld --user=mysql