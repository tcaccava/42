#!/usr/bin/env node
/**
 * Populate auth_service + user_service for local connection testing.
 *
 * Usage:
 *   npm run db:seed
 *   npm run db:seed -- --reset    # clear seed users & connections first
 *
 * Prerequisites:
 *   - Postgres running (infra/db/docker-compose.yml)
 *   - Migrations applied on both services
 *   - npm run prisma:generate
 */

import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import path from 'node:path';

const require = createRequire(import.meta.url);
const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.join(__dirname, '..');

const {
  USERS,
  CONNECTIONS,
  ONLINE_LOGINS,
  OFFLINE_EXPIRED_LOGINS,
} = require('./seed-data.js');

const ONLINE_SET = new Set(ONLINE_LOGINS);
const EXPIRED_SET = new Set(OFFLINE_EXPIRED_LOGINS);
const ACCESS_TOKEN_TTL_MS = 24 * 60 * 60 * 1000; // 24h for local testing

function seedAccessJti(userUuid) {
  return `seed-session-${userUuid}`;
}

const AUTH_DB_URL =
  process.env.AUTH_DATABASE_URL ??
  'postgres://postgres:postgres@localhost:5432/auth_service?schema=public';
const USER_DB_URL =
  process.env.USER_DATABASE_URL ??
  'postgres://postgres:postgres@localhost:5432/user_service?schema=public';

const reset = process.argv.includes('--reset');

async function main() {
  const { PrismaClient: AuthPrisma } = await import(
    path.join(root, 'apps/auth-service/src/generated/prisma/index.js')
  );
  const { PrismaClient: UserPrisma, ConnectionStatus } = await import(
    path.join(root, 'apps/user-service/src/generated/prisma/index.js')
  );

  const auth = new AuthPrisma({ datasources: { db: { url: AUTH_DB_URL } } });
  const userDb = new UserPrisma({ datasources: { db: { url: USER_DB_URL } } });

  try {
    await auth.$connect();
    await userDb.$connect();

    if (reset) {
      console.log('Resetting seed data...');
      const uuids = USERS.map((u) => u.uuid);

      await userDb.connection.deleteMany({
        where: {
          OR: [{ requesterId: { in: uuids } }, { addresseeId: { in: uuids } }],
        },
      });
      await userDb.userProfile.deleteMany({ where: { userId: { in: uuids } } });
      await auth.userSession.deleteMany({ where: { userId: { in: uuids } } });
      await auth.user.deleteMany({ where: { uuid: { in: uuids } } });
    }

    console.log(`Upserting ${USERS.length} users into auth_service + user_service...`);
    const loginToUuid = new Map();

    for (const u of USERS) {
      loginToUuid.set(u.login, u.uuid);

      await auth.user.upsert({
        where: { login: u.login },
        create: {
          uuid: u.uuid,
          login: u.login,
          email: u.email,
          image: u.image,
          first_name: u.first_name,
          last_name: u.last_name,
          displayname: u.displayname,
          pool_month: u.pool_month,
          pool_year: u.pool_year,
          location: u.location,
          grade: u.grade,
          kind: u.kind,
          status: u.status,
        },
        update: {
          email: u.email,
          image: u.image,
          first_name: u.first_name,
          last_name: u.last_name,
          displayname: u.displayname,
          pool_month: u.pool_month,
          pool_year: u.pool_year,
          location: u.location,
          grade: u.grade,
          kind: u.kind,
          status: u.status,
        },
      });

      await userDb.userProfile.upsert({
        where: { userId: u.uuid },
        create: {
          userId: u.uuid,
          login: u.login,
          bio: u.bio,
          username: u.username,
          website: u.website ?? null,
          github: u.github ?? null,
          linkedin: u.linkedin ?? null,
          campus: u.campus,
          coverImage: u.coverImage ?? null,
        },
        update: {
          login: u.login,
          bio: u.bio,
          username: u.username,
          website: u.website ?? null,
          github: u.github ?? null,
          linkedin: u.linkedin ?? null,
          campus: u.campus,
          coverImage: u.coverImage ?? null,
        },
      });
    }

    const uuids = USERS.map((u) => u.uuid);
    await auth.userSession.deleteMany({ where: { userId: { in: uuids } } });

    console.log('Seeding access-token sessions (active / offline)...');
    const now = Date.now();
    for (const u of USERS) {
      const accessJti = seedAccessJti(u.uuid);

      if (ONLINE_SET.has(u.login)) {
        await auth.userSession.create({
          data: {
            userId: u.uuid,
            accessJti,
            expiresAt: new Date(now + ACCESS_TOKEN_TTL_MS),
          },
        });
        console.log(`  🟢 ONLINE   @${u.login}`);
      } else if (EXPIRED_SET.has(u.login)) {
        await auth.userSession.create({
          data: {
            userId: u.uuid,
            accessJti,
            expiresAt: new Date(now - 60 * 60 * 1000),
          },
        });
        console.log(`  ⚫ OFFLINE  @${u.login} (expired session)`);
      } else {
        console.log(`  ⚫ OFFLINE  @${u.login} (no session)`);
      }
    }

    console.log(`\nUpserting ${CONNECTIONS.length} connections...`);
    for (const [fromLogin, toLogin, status] of CONNECTIONS) {
      const requesterId = loginToUuid.get(fromLogin);
      const addresseeId = loginToUuid.get(toLogin);
      if (!requesterId || !addresseeId) {
        console.warn(`  skip unknown login pair: ${fromLogin} -> ${toLogin}`);
        continue;
      }

      const prismaStatus =
        status === 'ACCEPTED' ? ConnectionStatus.ACCEPTED : ConnectionStatus.PENDING;

      await userDb.connection.upsert({
        where: {
          requesterId_addresseeId: { requesterId, addresseeId },
        },
        create: {
          requesterId,
          addresseeId,
          status: prismaStatus,
        },
        update: { status: prismaStatus },
      });
      console.log(`  ${fromLogin} -> ${toLogin} (${status})`);
    }

    const pendingCount = await userDb.connection.count({
      where: { status: ConnectionStatus.PENDING },
    });
    const acceptedCount = await userDb.connection.count({
      where: { status: ConnectionStatus.ACCEPTED },
    });

    const activeSessionCount = await auth.userSession.count({
      where: { userId: { in: uuids }, expiresAt: { gt: new Date() } },
    });

    console.log('\n✅ Seed complete\n');
    console.log('── Active badge (green dot) ──');
    console.log(`  ONLINE (${ONLINE_LOGINS.length}): ${ONLINE_LOGINS.map((l) => '@' + l).join(', ')}`);
    console.log(
      `  OFFLINE (${USERS.length - ONLINE_LOGINS.length}): no dot — expired: ${OFFLINE_EXPIRED_LOGINS.map((l) => '@' + l).join(', ') || '—'}; no session: @frossi, @gkim`,
    );
    console.log(`  Valid sessions in DB: ${activeSessionCount}\n`);

    console.log('── Users (search these logins) ──');
    for (const u of USERS) {
      const status = ONLINE_SET.has(u.login)
        ? '🟢 online'
        : EXPIRED_SET.has(u.login)
          ? '⚫ expired'
          : '⚫ offline';
      console.log(`  @${u.login.padEnd(10)} ${u.first_name} ${u.last_name} — ${status}`);
    }

    console.log('\n── Pre-seeded connections ──');
    console.log('  PENDING  asmith → bwilson   (accept as @bwilson)');
    console.log('  PENDING  jdoe → cgarcia     (accept as @cgarcia)');
    console.log('  PENDING  frossi → dyoon     (accept as @dyoon)');
    console.log('  ACCEPTED dyoon ↔ epark, frossi ↔ gkim, asmith ↔ dyoon');
    console.log(`\n  DB totals: ${pendingCount} pending, ${acceptedCount} accepted\n`);

    console.log('── How to test ──');
    console.log('  1. npm run dev');
    console.log('  2. Sign in with your real 42 account (you get your own green dot)');
    console.log('  3. Search → open @jdoe or @asmith → green dot on their avatar');
    console.log('  4. Open @bwilson or @gkim → no green dot');
    console.log('  5. My Network → accept connection invitations\n');
  } finally {
    await auth.$disconnect();
    await userDb.$disconnect();
  }
}

main().catch((err) => {
  console.error('Seed failed:', err);
  process.exit(1);
});
