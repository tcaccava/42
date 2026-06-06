FROM postgres:16-alpine

# Copy init scripts into the standard entrypoint directory.
# This avoids bind-mounting from NFS home directories (which can cause "Stale file handle").
COPY ./init/*.sql /docker-entrypoint-initdb.d/
