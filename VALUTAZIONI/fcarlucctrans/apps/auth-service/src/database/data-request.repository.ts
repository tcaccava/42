import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';
import type { DataRequestType } from '../generated/prisma/index.js';

@Injectable()
export class DataRequestRepository {
	constructor(private readonly prisma: PrismaService) {}

	create(userId: string, type: DataRequestType, token: string, expiresAt: Date) {
		return this.prisma.dataRequest.create({
			data: { userId, type, token, expiresAt },
		});
	}

	findActiveByToken(token: string) {
		return this.prisma.dataRequest.findUnique({ where: { token } });
	}

	markCompleted(id: string) {
		return this.prisma.dataRequest.update({
			where: { id },
			data: { status: 'COMPLETED', consumedAt: new Date() },
		});
	}

	deleteExpired() {
		return this.prisma.dataRequest.deleteMany({
			where: {
				OR: [
					{ expiresAt: { lt: new Date() } },
					{ status: { in: ['COMPLETED', 'EXPIRED'] } },
				],
			},
		});
	}

	cancelPendingForUser(userId: string, type: DataRequestType) {
		return this.prisma.dataRequest.deleteMany({
			where: { userId, type, status: 'PENDING' },
		});
	}
}
