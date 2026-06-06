#!/bin/bash
# Setup WordPress automatico con WP-CLI

# Legge i secrets
DB_PASSWORD=$(cat /run/secrets/db_password)
WP_ADMIN_PASSWORD=$(cat /run/secrets/credentials)

# Aspetta che MariaDB sia pronto
echo "Attendo che MariaDB sia pronto..."
until mysqladmin ping -h mariadb -u ${MYSQL_USER} -p"${DB_PASSWORD}" --silent; do
    sleep 2
done
echo "MariaDB pronto."

# Entra nella directory web
cd /var/www/html

# Scarica WordPress solo se non è già installato
if [ ! -f wp-config.php ]; then
    wp core download --allow-root

    # Crea il file di configurazione
    wp config create \
        --dbname="${MYSQL_DATABASE}" \
        --dbuser="${MYSQL_USER}" \
        --dbpass="${DB_PASSWORD}" \
        --dbhost="mariadb:3306" \
        --allow-root

    # Installa WordPress (questo crea il database tables)
    wp core install \
        --url="https://${DOMAIN_NAME}" \
        --title="${WP_TITLE}" \
        --admin_user="${WP_ADMIN_USER}" \
        --admin_password="${WP_ADMIN_PASSWORD}" \
        --admin_email="${WP_ADMIN_EMAIL}" \
        --skip-email \
        --allow-root

    # Crea un secondo utente normale (non admin)
    wp user create \
        "${WP_USER}" "${WP_USER_EMAIL}" \
        --role=author \
        --user_pass="${WP_USER_PASSWORD}" \
        --allow-root
fi

# Avvia php-fpm in foreground
exec php-fpm8.0 -F