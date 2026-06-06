import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';

@Injectable()
export class SessionRepository {
	constructor(private readonly prisma: PrismaService) {}

	create(userId: string, accessJti: string, expiresAt: Date) {
		return this.prisma.userSession.create({
			data: { userId, accessJti, expiresAt },
		});
	}

	findByAccessJti(accessJti: string) {
		return this.prisma.userSession.findUnique({ where: { accessJti } });
	}

	deleteByAccessJti(accessJti: string) {
		return this.prisma.userSession.deleteMany({ where: { accessJti } });
	}

	deleteAllForUser(userId: string) {
		return this.prisma.userSession.deleteMany({ where: { userId } });
	}

	deleteExpired() {
		return this.prisma.userSession.deleteMany({
			where: { expiresAt: { lt: new Date() } },
		});
	}

	hasActiveSession(userId: string) {
		return this.prisma.userSession.findFirst({
			where: {
				userId,
				expiresAt: { gt: new Date() },
			},
			select: { id: true },
		});
	}

	findActiveUserIds(userIds: string[]) {
		if (!userIds.length) return Promise.resolve([]);
		return this.prisma.userSession.findMany({
			where: {
				userId: { in: userIds },
				expiresAt: { gt: new Date() },
			},
			select: { userId: true },
			distinct: ['userId'],
		});
	}
}
