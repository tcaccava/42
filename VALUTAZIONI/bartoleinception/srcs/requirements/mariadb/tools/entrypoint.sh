#!/bin/sh
set -e

DB_ROOT_PASSWORD="$(cat "${MYSQL_ROOT_PASSWORD_FILE}")"
DB_PASSWORD="$(cat "${MYSQL_PASSWORD_FILE}")"

# First-run init: only when the data dir is empty.
if [ ! -d /var/lib/mysql/mysql ]; then
    mysql_install_db --user=mysql --datadir=/var/lib/mysql >/dev/null

    # Boot a temporary server to run our init SQL, then shut it down.
    mysqld --user=mysql --skip-networking --socket=/var/run/mysqld/mysqld.sock &
    pid="$!"

    # Wait for it to accept connections.
    for i in $(seq 1 30); do
        mysqladmin --socket=/var/run/mysqld/mysqld.sock ping >/dev/null 2>&1 && break
        sleep 1
    done

    mysql --socket=/var/run/mysqld/mysqld.sock <<SQL
CREATE DATABASE IF NOT EXISTS \`${MYSQL_DATABASE}\`;
CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${DB_PASSWORD}';
GRANT ALL PRIVILEGES ON \`${MYSQL_DATABASE}\`.* TO '${MYSQL_USER}'@'%';
ALTER USER 'root'@'localhost' IDENTIFIED BY '${DB_ROOT_PASSWORD}';
DELETE FROM mysql.user WHERE User='';
DROP DATABASE IF EXISTS test;
FLUSH PRIVILEGES;
SQL

    mysqladmin --socket=/var/run/mysqld/mysqld.sock -uroot -p"${DB_ROOT_PASSWORD}" shutdown
    wait "$pid"
fi

# Replace shell with mysqld so it becomes PID 1.
exec "$@"
