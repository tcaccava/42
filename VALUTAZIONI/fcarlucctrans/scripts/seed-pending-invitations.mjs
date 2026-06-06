#!/usr/bin/env node
/**
 * Create incoming connection requests FROM seed users TO your real 42 account.
 *
 * Your account must exist in auth_service (sign in once via OAuth first).
 *
 * Usage:
 *   npm run db:seed:invitations -- your_42_login
 *   SEED_TARGET_LOGIN=your_42_login npm run db:seed:invitations
 */

import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import path from 'node:path';

const require = createRequire(import.meta.url);
const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.join(__dirname, '..');

const { USERS } = require('./seed-data.js');

const AUTH_DB_URL =
  process.env.AUTH_DATABASE_URL ??
  'postgres://postgres:postgres@localhost:5432/auth_service?schema=public';
const USER_DB_URL =
  process.env.USER_DATABASE_URL ??
  'postgres://postgres:postgres@localhost:5432/user_service?schema=public';

const targetLogin = (
  process.argv[2] ??
  process.env.SEED_TARGET_LOGIN ??
  ''
).trim();

/** Seed users who will send you a pending invitation */
const INVITERS = ['jdoe', 'asmith', 'frossi'];

async function main() {
  if (!targetLogin) {
    console.error('Usage: npm run db:seed:invitations -- <your_42_login>');
    console.error('Example: npm run db:seed:invitations -- flaviano');
    process.exit(1);
  }

  const { PrismaClient: AuthPrisma } = await import(
    path.join(root, 'apps/auth-service/src/generated/prisma/index.js')
  );
  const { PrismaClient: UserPrisma, ConnectionStatus } = await import(
    path.join(root, 'apps/user-service/src/generated/prisma/index.js')
  );

  const auth = new AuthPrisma({ datasources: { db: { url: AUTH_DB_URL } } });
  const userDb = new UserPrisma({ datasources: { db: { url: USER_DB_URL } } });

  try {
    const target = await auth.user.findUnique({ where: { login: targetLogin } });
    if (!target) {
      console.error(`\nNo user "@${targetLogin}" in auth_service.`);
      console.error('Sign in once with 42 OAuth, then run this script again.\n');
      process.exit(1);
    }

    const loginToUuid = new Map(USERS.map((u) => [u.login, u.uuid]));
    console.log(`Creating invitations for @${targetLogin} (${target.uuid})...\n`);

    for (const fromLogin of INVITERS) {
      const requesterId = loginToUuid.get(fromLogin);
      if (!requesterId) continue;
      if (fromLogin === targetLogin) continue;

      await userDb.connection.upsert({
        where: {
          requesterId_addresseeId: { requesterId, addresseeId: target.uuid },
        },
        create: {
          requesterId,
          addresseeId: target.uuid,
          status: ConnectionStatus.PENDING,
        },
        update: { status: ConnectionStatus.PENDING },
      });
      console.log(`  @${fromLogin} → @${targetLogin} (PENDING)`);
    }

    console.log('\n✅ Done. Open the app → My Network (users icon) → Accept invitations.\n');
  } finally {
    await auth.$disconnect();
    await userDb.$disconnect();
  }
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
