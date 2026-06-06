import { ConfigService } from '@nestjs/config';
import { mkdirSync } from 'fs';
import path from 'path';

/** See apps/project-service/src/sqlite-url.ts */
export function resolveSqliteDatabaseUrl(
	config: ConfigService,
	fromDir: string,
	envKey: 'PROJECT_DATABASE_URL' | 'COMMENTS_DATABASE_URL',
	defaultDbRelativeToRepo: string,
): string {
	const packageRoot = path.join(fromDir, '../..');
	const repoRoot = path.join(packageRoot, '../..');
	const raw =
		config.get<string>(envKey) ?? config.get<string>('DATABASE_URL');

	let absolutePath: string;
	if (!raw?.startsWith('file:')) {
		absolutePath = path.join(repoRoot, defaultDbRelativeToRepo);
	} else {
		const filePath = raw.slice('file:'.length);
		if (path.isAbsolute(filePath)) {
			absolutePath = filePath;
		} else {
			const normalized = filePath.replace(/^\.\//, '');
			if (normalized.startsWith('apps/')) {
				absolutePath = path.join(repoRoot, normalized);
			} else {
				absolutePath = path.join(packageRoot, normalized);
			}
		}
	}

	mkdirSync(path.dirname(absolutePath), { recursive: true });
	return `file:${absolutePath}`;
}
