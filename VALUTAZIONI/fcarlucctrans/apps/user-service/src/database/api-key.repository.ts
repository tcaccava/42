import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';

@Injectable()
export class ApiKeyRepository {
	constructor(private readonly prisma: PrismaService) {}

	findByHash(keyHash: string) {
		return this.prisma.apiKey.findUnique({ where: { keyHash } });
	}

	create(data: {
		name: string;
		keyPrefix: string;
		keyHash: string;
		requestsPerMinute?: number;
	}) {
		return this.prisma.apiKey.create({ data });
	}

	listActive() {
		return this.prisma.apiKey.findMany({
			where: { isActive: true },
			select: {
				id: true,
				name: true,
				keyPrefix: true,
				requestsPerMinute: true,
				createdAt: true,
			},
			orderBy: { createdAt: 'desc' },
		});
	}
}
