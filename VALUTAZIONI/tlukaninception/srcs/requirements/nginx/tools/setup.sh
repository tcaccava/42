#!/bin/bash
# Genera certificato self-signed se non esiste
if [ ! -f /etc/ssl/certs/inception.crt ]; then
    openssl req -x509 -nodes -days 365 \
        -newkey rsa:2048 \
        -keyout /etc/ssl/private/inception.key \
        -out /etc/ssl/certs/inception.crt \
        -subj "/C=IT/ST=Italy/L=Rome/O=42/CN=${DOMAIN_NAME}"
fi

# Avvia NGINX in foreground
exec nginx -g "daemon off;"