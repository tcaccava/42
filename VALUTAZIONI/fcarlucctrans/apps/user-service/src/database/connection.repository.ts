import { Injectable } from '@nestjs/common';
import { ConnectionStatus } from '../generated/prisma/index.js';
import { PrismaService } from './prisma.service';

@Injectable()
export class ConnectionRepository {
	constructor(private readonly prisma: PrismaService) {}

	findById(id: string) {
		return this.prisma.connection.findUnique({ where: { id } });
	}

	findBetween(userA: string, userB: string) {
		return this.prisma.connection.findFirst({
			where: {
				OR: [
					{ requesterId: userA, addresseeId: userB },
					{ requesterId: userB, addresseeId: userA },
				],
			},
		});
	}

	create(requesterId: string, addresseeId: string) {
		return this.prisma.connection.create({
			data: { requesterId, addresseeId, status: ConnectionStatus.PENDING },
		});
	}

	updateStatus(id: string, status: ConnectionStatus) {
		return this.prisma.connection.update({
			where: { id },
			data: { status },
		});
	}

	delete(id: string) {
		return this.prisma.connection.delete({ where: { id } });
	}

	listForUser(userId: string, status?: ConnectionStatus) {
		const statusFilter = status ? { status } : {};
		return this.prisma.connection.findMany({
			where: {
				...statusFilter,
				OR: [{ requesterId: userId }, { addresseeId: userId }],
			},
			orderBy: { updatedAt: 'desc' },
		});
	}

	countAccepted(userId: string) {
		return this.prisma.connection.count({
			where: {
				status: ConnectionStatus.ACCEPTED,
				OR: [{ requesterId: userId }, { addresseeId: userId }],
			},
		});
	}
}
