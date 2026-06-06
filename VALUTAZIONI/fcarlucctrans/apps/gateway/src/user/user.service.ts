import { Inject, Injectable, Logger } from '@nestjs/common';
import { ClientProxy } from '@nestjs/microservices';
import { lastValueFrom } from 'rxjs';

export type EditableProfilePatch = {
	bio?: string;
	username?: string;
	website?: string;
	github?: string;
	linkedin?: string;
	campus?: string;
	coverImage?: string;
};

export interface UserProfileResponse {
	status: 'success' | 'error';
	profile?: {
		userId?: string;
		login: string;
		bio?: string | null;
		username?: string | null;
		website?: string | null;
		github?: string | null;
		linkedin?: string | null;
		campus?: string | null;
		coverImage?: string | null;
		createdAt?: string;
		updatedAt?: string;
	};
	message?: string;
}

export type ConnectionRelation =
	| 'none'
	| 'connected'
	| 'pending_outgoing'
	| 'pending_incoming'
	| 'self';

export type ConnectionItem = {
	id: string;
	status: string;
	requesterId: string;
	addresseeId: string;
	peerId: string;
	direction: 'outgoing' | 'incoming';
	peer?: {
		userId: string;
		login: string;
		image?: string | null;
		displayname?: string | null;
		first_name?: string | null;
		last_name?: string | null;
	};
	createdAt: string;
	updatedAt: string;
};

@Injectable()
export class UserService {
	private readonly logger = new Logger(UserService.name);

	constructor(
		@Inject('USER_SERVICE') private readonly userClient: ClientProxy,
		@Inject('AUTH_SERVICE') private readonly authClient: ClientProxy,
	) {}

	async getProfileByUserId(userId: string, login: string): Promise<UserProfileResponse> {
		try {
			return await lastValueFrom<UserProfileResponse>(
				this.userClient.send({ cmd: 'user_profile_get' }, { userId, login }),
			);
		} catch (error) {
			this.logger.error('User microservice error (get profile):', error);
			return { status: 'error', message: 'Failed to fetch user profile' };
		}
	}

	async upsertProfileByUserId(
		userId: string,
		login: string,
		patch: EditableProfilePatch,
	): Promise<UserProfileResponse> {
		const allowed: EditableProfilePatch = {
			bio: patch.bio,
			username: patch.username,
			website: patch.website,
			github: patch.github,
			linkedin: patch.linkedin,
			campus: patch.campus,
			coverImage: patch.coverImage,
		};

		try {
			return await lastValueFrom<UserProfileResponse>(
				this.userClient.send({ cmd: 'user_profile_upsert' }, { userId, login, profile: allowed }),
			);
		} catch (error) {
			this.logger.error('User microservice error (upsert profile):', error);
			return { status: 'error', message: 'Failed to update user profile' };
		}
	}

	async listDirectory(viewerId?: string, pageNumber = 0, pageSize = 6) {
		try {
			const result = await lastValueFrom<any>(
				this.authClient.send(
					{ cmd: 'user_directory' },
					{ excludeUuid: viewerId, pageNumber, pageSize },
				),
			);
			if (result.status !== 'success' || !viewerId || !result.users?.length) {
				return result;
			}
			return {
				...result,
				users: await this.attachConnectionStatuses(viewerId, result.users),
			};
		} catch (error) {
			this.logger.error('Auth microservice error (directory):', error);
			return {
				status: 'error',
				message: 'Failed to list users',
				users: [],
				pageCount: 0,
				totalCount: 0,
			};
		}
	}

	async searchUsers(query: string, viewerId?: string, pageNumber = 0, pageSize = 6) {
		try {
			const result = await lastValueFrom<any>(
				this.authClient.send(
					{ cmd: 'user_search' },
					{ query, excludeUuid: viewerId, pageNumber, pageSize },
				),
			);
			if (result.status !== 'success' || !viewerId || !result.users?.length) {
				return result;
			}
			return {
				...result,
				users: await this.attachConnectionStatuses(viewerId, result.users),
			};
		} catch (error) {
			this.logger.error('Auth microservice error (search):', error);
			return {
				status: 'error',
				message: 'Failed to search users',
				users: [],
				pageCount: 0,
				totalCount: 0,
			};
		}
	}

	private async attachConnectionStatuses(
		viewerId: string,
		users: Array<{ userId: string }>,
	) {
		const statuses = await Promise.all(
			users.map((user) =>
				this.getConnectionStatus(viewerId, user.userId).catch(() => ({
					relation: 'none' as ConnectionRelation,
				})),
			),
		);

		return users.map((user, index) => ({
			...user,
			connection: {
				relation: statuses[index]?.relation ?? 'none',
				connectionId: statuses[index]?.connectionId,
			},
		}));
	}

	async getPublicProfileByLogin(login: string, viewerId: string, viewerLogin: string) {
		try {
			const authResult = await lastValueFrom<any>(
				this.authClient.send({ cmd: 'user_find_by_login' }, { login }),
			);
			if (authResult.status !== 'success' || !authResult.user) {
				return { status: 'error', message: 'User not found' };
			}

			const target = authResult.user;
			const [profileResult, relationResult, countResult, activeResult] = await Promise.all([
				lastValueFrom<any>(
					this.userClient.send(
						{ cmd: 'user_profile_get' },
						{ userId: target.userId, login: target.login },
					),
				),
				lastValueFrom<any>(
					this.userClient.send(
						{ cmd: 'user_connection_status' },
						{ userId: viewerId, otherUserId: target.userId },
					),
				),
				lastValueFrom<any>(
					this.userClient.send({ cmd: 'user_connection_count' }, { userId: target.userId }),
				),
				lastValueFrom<{ status: string; isActive?: boolean }>(
					this.authClient.send({ cmd: 'user_is_active' }, { userId: target.userId }),
				),
			]);

			const profile = profileResult?.profile ?? {};
			const isOwnProfile = target.userId === viewerId;

			return {
				status: 'success',
				user: {
					...target,
					bio: profile.bio ?? undefined,
					username: profile.username ?? undefined,
					website: profile.website ?? undefined,
					github: profile.github ?? undefined,
					linkedin: profile.linkedin ?? undefined,
					campus: profile.campus ?? target.location ?? undefined,
					coverImage: profile.coverImage ?? undefined,
				},
				isOwnProfile,
				connection: {
					relation: relationResult?.relation ?? 'none',
					connectionId: relationResult?.connectionId,
				},
				connectionCount: countResult?.count ?? 0,
				isActive: activeResult?.status === 'success' && Boolean(activeResult.isActive),
			};
		} catch (error) {
			this.logger.error('Error loading public profile:', error);
			return { status: 'error', message: 'Failed to load profile' };
		}
	}

	async sendConnection(requesterId: string, targetUserId: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'user_connection_send' }, { requesterId, addresseeId: targetUserId }),
		);
	}

	async acceptConnection(userId: string, connectionId: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'user_connection_accept' }, { userId, connectionId }),
		);
	}

	async rejectConnection(userId: string, connectionId: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'user_connection_reject' }, { userId, connectionId }),
		);
	}

	async cancelConnection(userId: string, connectionId: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'user_connection_cancel' }, { userId, connectionId }),
		);
	}

	async removeConnection(userId: string, connectionId: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'user_connection_remove' }, { userId, connectionId }),
		);
	}

	async getConnectionStatus(userId: string, otherUserId: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'user_connection_status' }, { userId, otherUserId }),
		);
	}

	async listConnections(
		userId: string,
		status?: 'PENDING' | 'ACCEPTED',
	): Promise<{ status: string; connections: ConnectionItem[]; message?: string }> {
		try {
			const result = await lastValueFrom<any>(
				this.userClient.send({ cmd: 'user_connection_list' }, { userId, status }),
			);

			if (result.status !== 'success' || !result.connections?.length) {
				return { status: 'success', connections: [] as ConnectionItem[] };
			}

			const peerIds = result.connections.map((c: { peerId: string }) => c.peerId);
			const usersResult = await lastValueFrom<any>(
				this.authClient.send({ cmd: 'user_find_by_uuids' }, { uuids: peerIds }),
			);

			const userMap = new Map<string, any>(
				(usersResult.users ?? []).map((u: { userId: string }) => [u.userId, u]),
			);

			const connections: ConnectionItem[] = result.connections.map((c: any) => ({
				...c,
				createdAt: c.createdAt,
				updatedAt: c.updatedAt,
				peer: userMap.get(c.peerId),
			}));

			return { status: 'success', connections };
		} catch (error) {
			this.logger.error('Error listing connections:', error);
			return { status: 'error', message: 'Failed to list connections', connections: [] };
		}
	}
}
