import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';

@Injectable()
export class RefreshSessionRepository {
	constructor(private readonly prisma: PrismaService) {}

	create(userId: string, tokenHash: string, expiresAt: Date) {
		return this.prisma.refreshSession.create({
			data: { userId, tokenHash, expiresAt },
		});
	}

	findByTokenHash(tokenHash: string) {
		return this.prisma.refreshSession.findUnique({ where: { tokenHash } });
	}

	markReplaced(id: string, replacedById: string) {
		return this.prisma.refreshSession.update({
			where: { id },
			data: { replacedById },
		});
	}

	deleteByTokenHash(tokenHash: string) {
		return this.prisma.refreshSession.deleteMany({ where: { tokenHash } });
	}

	deleteAllForUser(userId: string) {
		return this.prisma.refreshSession.deleteMany({ where: { userId } });
	}

	deleteExpired() {
		return this.prisma.refreshSession.deleteMany({
			where: { expiresAt: { lt: new Date() } },
		});
	}
}
