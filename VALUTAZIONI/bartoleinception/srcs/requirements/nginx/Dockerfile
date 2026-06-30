FROM alpine:3.24.0

ARG DOMAIN_NAME=fbartole.42.fr

RUN apk update && apk add --no-cache \
        nginx \
        openssl \
    && rm -rf /var/cache/apk/*

RUN mkdir -p /etc/nginx/ssl /var/www/html /run/nginx \
    && openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
        -subj "/C=IT/ST=Italy/L=Rome/O=42/OU=Inception/CN=${DOMAIN_NAME}" \
        -keyout /etc/nginx/ssl/${DOMAIN_NAME}.key \
        -out    /etc/nginx/ssl/${DOMAIN_NAME}.crt

COPY conf/nginx.conf /etc/nginx/nginx.conf

EXPOSE 443

CMD ["nginx", "-g", "daemon off;"]
