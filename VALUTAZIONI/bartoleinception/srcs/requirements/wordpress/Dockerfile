FROM alpine:3.24.0

RUN apk update && apk add --no-cache \
        php83 \
        php83-fpm \
        php83-mysqli \
        php83-json \
        php83-curl \
        php83-dom \
        php83-exif \
        php83-fileinfo \
        php83-mbstring \
        php83-openssl \
        php83-xml \
        php83-zip \
        php83-phar \
        php83-iconv \
        php83-tokenizer \
        php83-session \
        php83-ctype \
        curl \
        bash \
        mariadb-client \
    && ln -sf /usr/sbin/php-fpm83 /usr/sbin/php-fpm \
    && ln -sf /usr/bin/php83 /usr/bin/php \
    && rm -rf /var/cache/apk/*

# wp-cli
RUN curl -fsSL -o /usr/local/bin/wp \
        https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar \
    && chmod +x /usr/local/bin/wp

COPY conf/www.conf /etc/php83/php-fpm.d/www.conf
COPY tools/entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

RUN mkdir -p /var/www/html /run/php \
    && chown -R nobody:nobody /var/www/html /run/php

WORKDIR /var/www/html

EXPOSE 9000

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
CMD ["php-fpm", "-F"]
