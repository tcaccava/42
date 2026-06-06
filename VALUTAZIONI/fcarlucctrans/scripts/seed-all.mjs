#!/usr/bin/env node
/**
 * Populate all demo databases in one command.
 *
 * Usage:
 *   npm run db:seed
 *   npm run db:seed -- --reset
 *   npm run db:seed -- --skip-s3
 *
 * Steps: users + connections → events → projects (+ S3) → public API key
 */

import { spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import path from 'node:path';
import { loadEnv } from './load-env.mjs';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.join(__dirname, '..');

loadEnv(root);

const extraArgs = process.argv.slice(2);
const steps = [
  { name: 'Users & connections', script: 'seed-databases.mjs' },
  { name: 'Events', script: 'seed-events.mjs' },
  { name: 'Projects', script: 'seed-projects.mjs' },
  { name: 'Public API', script: 'seed-public-api.mjs' },
];

console.log('\n🌱 Seeding all demo data...\n');

for (const step of steps) {
  console.log(`── ${step.name} ──`);
  const result = spawnSync(process.execPath, [path.join(__dirname, step.script), ...extraArgs], {
    cwd: root,
    stdio: 'inherit',
    env: process.env,
  });

  if (result.status !== 0) {
    console.error(`\n❌ Seed stopped at: ${step.name}\n`);
    process.exit(result.status ?? 1);
  }
  console.log('');
}

console.log('✅ All seeds finished.\n');
console.log('Next steps:');
console.log('  make dev   # or make up');
console.log('  Sign in with 42 OAuth');
console.log('  Search → browse students, events, and paginated projects');
console.log('  Copy PUBLIC_API_KEY from output above into .env if needed\n');
