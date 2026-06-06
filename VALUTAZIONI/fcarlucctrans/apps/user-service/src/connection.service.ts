import { BadRequestException, ForbiddenException, Injectable, NotFoundException } from '@nestjs/common';
import { ConnectionStatus } from './generated/prisma/index.js';
import { ConnectionRepository } from './database/connection.repository';

export type ConnectionWithPeer = {
	id: string;
	status: ConnectionStatus;
	requesterId: string;
	addresseeId: string;
	peerId: string;
	direction: 'outgoing' | 'incoming';
	createdAt: Date;
	updatedAt: Date;
};

export type ConnectionRelation =
	| 'none'
	| 'connected'
	| 'pending_outgoing'
	| 'pending_incoming'
	| 'self';

@Injectable()
export class ConnectionService {
	constructor(private readonly connectionRepository: ConnectionRepository) {}

	private toConnectionWithPeer(
		connection: {
			id: string;
			status: ConnectionStatus;
			requesterId: string;
			addresseeId: string;
			createdAt: Date;
			updatedAt: Date;
		},
		userId: string,
	): ConnectionWithPeer {
		const isOutgoing = connection.requesterId === userId;
		return {
			id: connection.id,
			status: connection.status,
			requesterId: connection.requesterId,
			addresseeId: connection.addresseeId,
			peerId: isOutgoing ? connection.addresseeId : connection.requesterId,
			direction: isOutgoing ? 'outgoing' : 'incoming',
			createdAt: connection.createdAt,
			updatedAt: connection.updatedAt,
		};
	}

	async sendRequest(requesterId: string, addresseeId: string) {
		if (!requesterId || !addresseeId) {
			throw new BadRequestException('requesterId and addresseeId are required');
		}
		if (requesterId === addresseeId) {
			throw new BadRequestException('Cannot connect with yourself');
		}

		const existing = await this.connectionRepository.findBetween(requesterId, addresseeId);
		if (existing) {
			if (existing.status === ConnectionStatus.ACCEPTED) {
				throw new BadRequestException('Already connected');
			}
			if (existing.status === ConnectionStatus.PENDING) {
				if (existing.requesterId === addresseeId && existing.addresseeId === requesterId) {
					const accepted = await this.connectionRepository.updateStatus(
						existing.id,
						ConnectionStatus.ACCEPTED,
					);
					return {
						status: 'success' as const,
						connection: this.toConnectionWithPeer(accepted, requesterId),
						autoAccepted: true,
					};
				}
				throw new BadRequestException('Connection request already pending');
			}
			if (existing.status === ConnectionStatus.REJECTED) {
				if (existing.requesterId === requesterId) {
					const updated = await this.connectionRepository.updateStatus(
						existing.id,
						ConnectionStatus.PENDING,
					);
					return {
						status: 'success' as const,
						connection: this.toConnectionWithPeer(updated, requesterId),
					};
				}
				await this.connectionRepository.delete(existing.id);
			}
		}

		const connection = await this.connectionRepository.create(requesterId, addresseeId);
		return {
			status: 'success' as const,
			connection: this.toConnectionWithPeer(connection, requesterId),
		};
	}

	async acceptRequest(userId: string, connectionId: string) {
		const connection = await this.getConnectionOrThrow(connectionId);
		if (connection.addresseeId !== userId) {
			throw new ForbiddenException('Only the recipient can accept this request');
		}
		if (connection.status !== ConnectionStatus.PENDING) {
			throw new BadRequestException('Connection is not pending');
		}

		const updated = await this.connectionRepository.updateStatus(
			connectionId,
			ConnectionStatus.ACCEPTED,
		);
		return {
			status: 'success' as const,
			connection: this.toConnectionWithPeer(updated, userId),
		};
	}

	async rejectRequest(userId: string, connectionId: string) {
		const connection = await this.getConnectionOrThrow(connectionId);
		if (connection.addresseeId !== userId) {
			throw new ForbiddenException('Only the recipient can reject this request');
		}
		if (connection.status !== ConnectionStatus.PENDING) {
			throw new BadRequestException('Connection is not pending');
		}

		const updated = await this.connectionRepository.updateStatus(
			connectionId,
			ConnectionStatus.REJECTED,
		);
		return {
			status: 'success' as const,
			connection: this.toConnectionWithPeer(updated, userId),
		};
	}

	async cancelRequest(userId: string, connectionId: string) {
		const connection = await this.getConnectionOrThrow(connectionId);
		if (connection.requesterId !== userId) {
			throw new ForbiddenException('Only the sender can cancel this request');
		}
		if (connection.status !== ConnectionStatus.PENDING) {
			throw new BadRequestException('Connection is not pending');
		}

		await this.connectionRepository.delete(connectionId);
		return { status: 'success' as const };
	}

	async removeConnection(userId: string, connectionId: string) {
		const connection = await this.getConnectionOrThrow(connectionId);
		if (connection.requesterId !== userId && connection.addresseeId !== userId) {
			throw new ForbiddenException('Not allowed to remove this connection');
		}
		if (connection.status !== ConnectionStatus.ACCEPTED) {
			throw new BadRequestException('Can only remove accepted connections');
		}

		await this.connectionRepository.delete(connectionId);
		return { status: 'success' as const };
	}

	async listConnections(userId: string, status?: ConnectionStatus) {
		if (!userId) throw new BadRequestException('userId is required');

		const connections = await this.connectionRepository.listForUser(userId, status);
		return {
			status: 'success' as const,
			connections: connections.map((c) => this.toConnectionWithPeer(c, userId)),
		};
	}

	async getConnectionStatus(userId: string, otherUserId: string): Promise<{
		status: 'success';
		relation: ConnectionRelation;
		connectionId?: string;
	}> {
		if (!userId || !otherUserId) {
			throw new BadRequestException('userId and otherUserId are required');
		}
		if (userId === otherUserId) {
			return { status: 'success', relation: 'self' };
		}

		const connection = await this.connectionRepository.findBetween(userId, otherUserId);
		if (!connection) {
			return { status: 'success', relation: 'none' };
		}

		if (connection.status === ConnectionStatus.ACCEPTED) {
			return { status: 'success', relation: 'connected', connectionId: connection.id };
		}

		if (connection.status === ConnectionStatus.PENDING) {
			if (connection.requesterId === userId) {
				return {
					status: 'success',
					relation: 'pending_outgoing',
					connectionId: connection.id,
				};
			}
			return {
				status: 'success',
				relation: 'pending_incoming',
				connectionId: connection.id,
			};
		}

		return { status: 'success', relation: 'none' };
	}

	async getConnectionCount(userId: string) {
		const count = await this.connectionRepository.countAccepted(userId);
		return { status: 'success' as const, count };
	}

	private async getConnectionOrThrow(connectionId: string) {
		const connection = await this.connectionRepository.findById(connectionId);
		if (!connection) throw new NotFoundException('Connection not found');
		return connection;
	}
}
