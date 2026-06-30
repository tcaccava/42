#!/bin/bash
set -e

# wp-cli is a phar run directly, so WP_CLI_PHP_ARGS is ignored. Wrap it to raise
# PHP's memory limit, otherwise extracting WordPress hits the 128M default cap.
wp() { php -d memory_limit=512M /usr/local/bin/wp "$@"; }

DB_PASSWORD="$(cat "${MYSQL_PASSWORD_FILE}")"
WP_ADMIN_PASSWORD="$(cat "${WP_ADMIN_PASSWORD_FILE}")"
WP_USER_PASSWORD="$(cat "${WP_USER_PASSWORD_FILE}")"

cd /var/www/html

# Wait until MariaDB accepts connections.
for i in $(seq 1 60); do
    if mariadb -h "${MYSQL_HOST}" -u "${MYSQL_USER}" -p"${DB_PASSWORD}" \
                -e "USE ${MYSQL_DATABASE};" >/dev/null 2>&1; then
        break
    fi
    sleep 1
done

# Install + configure WordPress only on first run.
if [ ! -f wp-config.php ]; then
    wp core download --allow-root --quiet

    wp config create --allow-root \
        --dbname="${MYSQL_DATABASE}" \
        --dbuser="${MYSQL_USER}" \
        --dbpass="${DB_PASSWORD}" \
        --dbhost="${MYSQL_HOST}:3306"

    wp core install --allow-root \
        --url="${WP_URL}" \
        --title="${WP_TITLE}" \
        --admin_user="${WP_ADMIN_USER}" \
        --admin_password="${WP_ADMIN_PASSWORD}" \
        --admin_email="${WP_ADMIN_EMAIL}" \
        --skip-email

    wp user create --allow-root \
        "${WP_USER}" "${WP_USER_EMAIL}" \
        --role=author \
        --user_pass="${WP_USER_PASSWORD}"

    chown -R nobody:nobody /var/www/html
fi

exec "$@"
