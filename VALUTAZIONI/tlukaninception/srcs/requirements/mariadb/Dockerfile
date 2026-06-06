FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    mariadb-server \
    && rm -rf /var/lib/apt/lists/*

# Copia configurazione e script
COPY conf/my.cnf /etc/mysql/conf.d/my.cnf
COPY tools/db-setup.sh /usr/local/bin/db-setup.sh
RUN chmod +x /usr/local/bin/db-setup.sh

# Crea directory e imposta permessi
RUN mkdir -p /var/lib/mysql /run/mysqld \
    && chown -R mysql:mysql /var/lib/mysql /run/mysqld

EXPOSE 3306

ENTRYPOINT ["/usr/local/bin/db-setup.sh"]