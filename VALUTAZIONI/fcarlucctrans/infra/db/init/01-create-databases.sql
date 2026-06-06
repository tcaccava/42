-- Create dedicated databases per microservice.
-- Runs only when the Postgres data volume is initialized for the first time.

\connect postgres

SELECT 'CREATE DATABASE auth_service'
WHERE NOT EXISTS (SELECT FROM pg_database WHERE datname = 'auth_service')\gexec

SELECT 'CREATE DATABASE user_service'
WHERE NOT EXISTS (SELECT FROM pg_database WHERE datname = 'user_service')\gexec
