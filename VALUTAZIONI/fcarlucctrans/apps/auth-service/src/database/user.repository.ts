import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';

@Injectable()
export class UserRepository {
  constructor(private readonly prisma: PrismaService) {}

  async upsertUser(profile: {
    login: string;
    email: string;
    image?: string | null;
    first_name?: string | null;
    last_name?: string | null;
    displayname?: string | null;
    pool_month?: string | null;
    pool_year?: number | null;
    location?: string | null;
    wallet?: number | null;
    correction_point?: number | null;
    grade?: string | null;
    kind?: string | null;
    status?: string | null;
  }): Promise<any> {
    const data = { ...profile };
    return this.prisma.user.upsert({
      where: { login: profile.login },
      create: data,
      update: data,
    });
  }

  async findByLogin(login: string): Promise<any | null> {
    return this.prisma.user.findUnique({ where: { login } });
  }

	async findByUuid(uuid: string): Promise<any | null> {
		return this.prisma.user.findUnique({ where: { uuid } });
	}

	searchByLogin(query: string, limit = 20, excludeUuid?: string) {
		return this.searchUsers(query, excludeUuid, 0, limit).then(([users]) => users);
	}

	searchUsers(
		query: string,
		excludeUuid?: string,
		pageNumber = 0,
		pageSize = 6,
	) {
		const trimmed = query.trim();
		const where = {
			AND: [
				excludeUuid ? { uuid: { not: excludeUuid } } : {},
				{
					OR: [
						{ login: { contains: trimmed, mode: 'insensitive' as const } },
						{ first_name: { contains: trimmed, mode: 'insensitive' as const } },
						{ last_name: { contains: trimmed, mode: 'insensitive' as const } },
						{ displayname: { contains: trimmed, mode: 'insensitive' as const } },
					],
				},
			],
		};

		return Promise.all([
			this.prisma.user.findMany({
				where,
				skip: pageNumber * pageSize,
				take: pageSize,
				orderBy: { login: 'asc' },
				select: {
					uuid: true,
					login: true,
					email: true,
					image: true,
					displayname: true,
					first_name: true,
					last_name: true,
					location: true,
				},
			}),
			this.prisma.user.count({ where }),
		]);
	}

	findPublicByLogin(login: string) {
		return this.prisma.user.findUnique({
			where: { login },
			select: {
				uuid: true,
				login: true,
				email: true,
				image: true,
				displayname: true,
				first_name: true,
				last_name: true,
				pool_month: true,
				pool_year: true,
				location: true,
				grade: true,
				kind: true,
				status: true,
			},
		});
	}

	listDirectory(excludeUuid?: string, pageNumber = 0, pageSize = 6) {
		const where = excludeUuid ? { uuid: { not: excludeUuid } } : {};

		return Promise.all([
			this.prisma.user.findMany({
				where,
				skip: pageNumber * pageSize,
				take: pageSize,
				orderBy: { login: 'asc' },
				select: {
					uuid: true,
					login: true,
					email: true,
					image: true,
					displayname: true,
					first_name: true,
					last_name: true,
					location: true,
				},
			}),
			this.prisma.user.count({ where }),
		]);
	}

	findManyByUuids(uuids: string[]) {
		if (!uuids.length) return Promise.resolve([]);
		return this.prisma.user.findMany({
			where: { uuid: { in: uuids } },
			select: {
				uuid: true,
				login: true,
				image: true,
				displayname: true,
				first_name: true,
				last_name: true,
			},
		});
	}

	delete(uuid: string) {
		return this.prisma.user.delete({ where: { uuid } });
	}

	/**
	 * GDPR anonymization: overwrite identifying fields with placeholders.
	 * Keeps the row so referential integrity from other services survives,
	 * but no PII remains.
	 */
	anonymize(uuid: string, overrides: { login: string; email: string }) {
		return this.prisma.user.update({
			where: { uuid },
			data: {
				login: overrides.login,
				email: overrides.email,
				image: null,
				first_name: null,
				last_name: null,
				displayname: null,
				location: null,
				wallet: null,
				correction_point: null,
				grade: null,
				kind: null,
				status: 'anonymized',
			},
		});
	}
}
