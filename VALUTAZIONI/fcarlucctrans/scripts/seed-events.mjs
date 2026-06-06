#!/usr/bin/env node
/**
 * Seed campus events + participants in user_service.
 *
 * Usage: node scripts/seed-events.mjs [--reset]
 */

import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import path from 'node:path';

const require = createRequire(import.meta.url);
const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.join(__dirname, '..');

const { USERS, EVENTS } = require('./seed-data.js');

const USER_DB_URL =
  process.env.USER_DATABASE_URL ??
  'postgres://postgres:postgres@localhost:5432/user_service?schema=public';

const reset = process.argv.includes('--reset');

async function main() {
  const { PrismaClient: UserPrisma } = await import(
    path.join(root, 'apps/user-service/src/generated/prisma/index.js')
  );

  const userDb = new UserPrisma({ datasources: { db: { url: USER_DB_URL } } });
  const loginToUser = new Map(USERS.map((u) => [u.login, u]));

  try {
    await userDb.$connect();

    const eventIds = EVENTS.map((e) => e.id);

    if (reset) {
      await userDb.eventParticipant.deleteMany({
        where: { eventId: { in: eventIds } },
      });
      await userDb.event.deleteMany({ where: { id: { in: eventIds } } });
    }

    console.log(`Upserting ${EVENTS.length} events...`);

    for (const event of EVENTS) {
      const organizer = loginToUser.get(event.organizerLogin);
      if (!organizer) {
        console.warn(`  skip event "${event.title}" — unknown organizer`);
        continue;
      }

      await userDb.event.upsert({
        where: { id: event.id },
        create: {
          id: event.id,
          title: event.title,
          description: event.description,
          organizerLogin: organizer.login,
          organizerUserId: organizer.uuid,
          organizerImage: organizer.image,
          campus: event.campus,
          date: event.date,
          time: event.time,
          location: event.location,
          maxParticipants: event.maxParticipants,
          tags: event.tags,
          coverImage: event.coverImage,
        },
        update: {
          title: event.title,
          description: event.description,
          organizerLogin: organizer.login,
          organizerUserId: organizer.uuid,
          organizerImage: organizer.image,
          campus: event.campus,
          date: event.date,
          time: event.time,
          location: event.location,
          maxParticipants: event.maxParticipants,
          tags: event.tags,
          coverImage: event.coverImage,
        },
      });

      await userDb.eventParticipant.deleteMany({ where: { eventId: event.id } });

      for (const login of event.participants ?? []) {
        const participant = loginToUser.get(login);
        if (!participant) continue;
        await userDb.eventParticipant.create({
          data: { eventId: event.id, login: participant.login },
        });
      }

      console.log(`  ${event.title} (@${event.organizerLogin})`);
    }

    console.log('\n✅ Events seed complete\n');
  } finally {
    await userDb.$disconnect();
  }
}

main().catch((err) => {
  console.error('Events seed failed:', err);
  process.exit(1);
});
