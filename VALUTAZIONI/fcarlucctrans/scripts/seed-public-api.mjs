#!/usr/bin/env node
/**
 * Seed a development API key and sample public resources.
 *
 * Usage:
 *   npm run db:seed:public-api
 *
 * Prints the raw API key once — store it in .env as PUBLIC_API_KEY for curl tests.
 */

import crypto from 'node:crypto';
import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import path from 'node:path';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.join(__dirname, '..');

const USER_DB_URL =
  process.env.USER_DATABASE_URL ??
  'postgres://postgres:postgres@localhost:5432/user_service?schema=public';

function hashApiKey(rawKey) {
  return crypto.createHash('sha256').update(rawKey).digest('hex');
}

async function main() {
  const { PrismaClient } = await import(
    path.join(root, 'apps/user-service/src/generated/prisma/index.js')
  );

  const db = new PrismaClient({ datasources: { db: { url: USER_DB_URL } } });

  try {
    await db.$connect();

    const rawKey =
      process.env.PUBLIC_API_DEV_KEY ??
      `ft_dev_${crypto.randomBytes(24).toString('hex')}`;
    const keyPrefix = rawKey.slice(0, 12);
    const keyHash = hashApiKey(rawKey);

    await db.apiKey.deleteMany({ where: { name: 'Development key' } });
    await db.apiKey.create({
      data: {
        name: 'Development key',
        keyPrefix,
        keyHash,
        requestsPerMinute: 120,
        isActive: true,
      },
    });

    const existing = await db.publicResource.count();
    if (existing === 0) {
      await db.publicResource.createMany({
        data: [
          {
            title: 'Welcome to the Public API',
            content: 'This resource was created by the seed script.',
            tags: ['docs', 'seed'],
          },
          {
            title: '42 Transcendence',
            content: 'Example payload for GET /api/v1/resources',
            tags: ['example'],
          },
        ],
      });
    }

    console.log('\n✅ Public API seed complete\n');
    console.log('API key (add to .env as PUBLIC_API_KEY=…):');
    console.log(`  ${rawKey}\n`);
    console.log('Gateway base URL (default): http://localhost:4000');
    console.log('See PUBLIC_API.md for curl examples.\n');
  } finally {
    await db.$disconnect();
  }
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
