FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    php8.0 \
    php8.0-fpm \
    php8.0-mysql \
    php8.0-curl \
    php8.0-gd \
    php8.0-mbstring \
    php8.0-xml \
    php8.0-zip \
    mariadb-client \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Installa WP-CLI
RUN curl -o /usr/local/bin/wp \
    https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar \
    && chmod +x /usr/local/bin/wp

# Crea la directory web e imposta permessi
RUN mkdir -p /var/www/html /run/php \
    && chown -R www-data:www-data /var/www/html

# Copia configurazione php-fpm
COPY conf/www.conf /etc/php/8.0/fpm/pool.d/www.conf

# Copia ed esegui lo script di setup
COPY tools/wp-setup.sh /usr/local/bin/wp-setup.sh
RUN chmod +x /usr/local/bin/wp-setup.sh

EXPOSE 9000

ENTRYPOINT ["/usr/local/bin/wp-setup.sh"]