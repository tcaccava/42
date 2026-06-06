#!/usr/bin/env node
/**
 * Seed demo projects in project-service SQLite (+ optional S3 screenshots).
 *
 * Usage: node scripts/seed-projects.mjs [--reset] [--skip-s3]
 */

import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import path from 'node:path';
import fs from 'node:fs';

const require = createRequire(import.meta.url);
const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.join(__dirname, '..');

function requirePackage(name) {
  for (const base of [
    root,
    path.join(root, 'apps/project-service'),
  ]) {
    try {
      return require(require.resolve(name, { paths: [base] }));
    } catch {
      // try next
    }
  }
  throw new Error(`Package not found: ${name}. Run npm install from repo root.`);
}

const { USERS, PROJECTS, projectShotUrl } = require('./seed-data.js');
const { fetchImageBuffer } = require('./seed-images.js');

const reset = process.argv.includes('--reset');
const skipS3 = process.argv.includes('--skip-s3');

function resolveSqlitePath() {
  const raw = process.env.PROJECT_DATABASE_URL ?? 'file:./apps/project-service/dev.db';
  if (!raw.startsWith('file:')) {
    throw new Error(`Unsupported PROJECT_DATABASE_URL: ${raw}`);
  }

  const filePath = raw.slice('file:'.length);
  if (path.isAbsolute(filePath)) {
    return filePath;
  }

  const normalized = filePath.replace(/^\.\//, '');
  return normalized.startsWith('apps/')
    ? path.join(root, normalized)
    : path.join(root, 'apps/project-service', normalized);
}

function parseRepositoryUrl(input) {
  const trimmed = input.trim();
  let url;
  try {
    url = new URL(trimmed);
  } catch {
    return null;
  }
  if (url.protocol !== 'https:') return null;
  const host = url.hostname.toLowerCase();
  const parts = url.pathname.replace(/\/+$/, '').split('/').filter(Boolean);
  if (parts.length < 2) return null;

  if (host === 'github.com' && parts.length === 2) {
    const [owner, repo] = parts;
    return {
      normalized: `https://github.com/${owner}/${repo}`,
      slug: `${owner}/${repo}`,
    };
  }
  if (host === 'gitlab.com') {
    const repo = parts[parts.length - 1];
    const owner = parts.slice(0, -1).join('/');
    return {
      normalized: `https://gitlab.com/${owner}/${repo}`,
      slug: `${owner}/${repo}`,
    };
  }
  return null;
}

async function createS3Client() {
  const endpoint = process.env.S3_ENDPOINT;
  const bucket = process.env.S3_BUCKET;
  const key = process.env.S3_KEY;
  const secret = process.env.S3_SECRET;
  const region = process.env.S3_REGION ?? 'us-east-1';

  if (!endpoint || !bucket || !key || !secret) {
    return null;
  }

  const { S3Client, PutObjectCommand } = await import(
    require.resolve('@aws-sdk/client-s3', { paths: [root, path.join(root, 'apps/project-service')] }),
  );

  return {
    client: new S3Client({
      endpoint,
      region,
      credentials: { accessKeyId: key, secretAccessKey: secret },
      forcePathStyle: true,
    }),
    PutObjectCommand,
    bucket,
  };
}

async function uploadScreenshot(s3, userId, photoId) {
  const url = projectShotUrl(photoId);
  const { buffer, contentType } = await fetchImageBuffer(url);
  const objectKey = `seed/${userId}/${photoId.replace(/[^a-z0-9-]/gi, '_')}.jpg`;

  await s3.client.send(
    new s3.PutObjectCommand({
      Bucket: s3.bucket,
      Key: objectKey,
      Body: buffer,
      ContentType: contentType,
      ACL: 'public-read',
    }),
  );

  return objectKey;
}

async function main() {
  const dbPath = resolveSqlitePath();
  fs.mkdirSync(path.dirname(dbPath), { recursive: true });

  if (!fs.existsSync(dbPath)) {
    console.error(`SQLite database not found: ${dbPath}`);
    console.error('Run make dev-migrate (or make up) before seeding projects.');
    process.exit(1);
  }

  const Database = requirePackage('better-sqlite3');
  const db = new Database(dbPath);

  const loginToUuid = new Map(USERS.map((u) => [u.login, u.uuid]));
  const seedUserIds = USERS.map((u) => u.uuid);

  let s3 = null;
  if (!skipS3) {
    try {
      s3 = await createS3Client();
      if (!s3) {
        console.log('S3 env not set — projects will have no screenshots.');
      }
    } catch (err) {
      console.warn('S3 client unavailable — skipping screenshots:', err.message);
    }
  }

  if (reset) {
    console.log('Resetting seed projects...');
    const placeholders = seedUserIds.map(() => '?').join(', ');
    db.prepare(`DELETE FROM Score WHERE userId IN (${placeholders})`).run(...seedUserIds);
    db.prepare(`DELETE FROM Project WHERE user IN (${placeholders})`).run(...seedUserIds);
  }

  const findProject = db.prepare(
    'SELECT id FROM Project WHERE user = ? AND repositoryUrl = ? LIMIT 1',
  );
  const insertProject = db.prepare(`
    INSERT INTO Project (title, repositoryUrl, subject, description, screenshots, tags, user, createdAt)
    VALUES (@title, @repositoryUrl, @subject, @description, @screenshots, @tags, @user, @createdAt)
  `);
  const updateProject = db.prepare(`
    UPDATE Project
    SET title = @title, subject = @subject, description = @description,
        screenshots = @screenshots, tags = @tags, createdAt = @createdAt
    WHERE id = @id
  `);

  console.log(`Upserting ${PROJECTS.length} projects...`);

  for (const item of PROJECTS) {
    const userId = loginToUuid.get(item.login);
    if (!userId) continue;

    const parsed = parseRepositoryUrl(item.repositoryUrl);
    if (!parsed) {
      console.warn(`  skip invalid repo URL: ${item.repositoryUrl}`);
      continue;
    }

    let screenshotKeys = null;
    if (s3) {
      try {
        const key = await uploadScreenshot(s3, userId, item.photoId);
        screenshotKeys = [key];
      } catch (err) {
        console.warn(`  screenshot upload failed for ${parsed.slug}: ${err.message}`);
      }
    }

    const row = {
      title: parsed.slug,
      repositoryUrl: parsed.normalized,
      subject: item.subject,
      description: item.description,
      tags: JSON.stringify(item.tags),
      screenshots: screenshotKeys ? JSON.stringify(screenshotKeys) : null,
      user: userId,
      createdAt: new Date(Date.now() - item.daysAgo * 86400000).toISOString(),
    };

    const existing = findProject.get(userId, parsed.normalized);
    if (existing) {
      updateProject.run({ ...row, id: existing.id });
      console.log(`  updated ${parsed.slug} (@${item.login})`);
    } else {
      insertProject.run(row);
      console.log(`  created ${parsed.slug} (@${item.login})`);
    }
  }

  const countRow = db
    .prepare(
      `SELECT COUNT(*) AS n FROM Project WHERE user IN (${seedUserIds.map(() => '?').join(', ')})`,
    )
    .get(...seedUserIds);

  db.close();
  console.log(`\n✅ Projects seed complete (${countRow.n} demo projects)\n`);
}

main().catch((err) => {
  console.error('Projects seed failed:', err);
  process.exit(1);
});
