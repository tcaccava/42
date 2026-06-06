import { Injectable, Logger } from '@nestjs/common';
import { PrismaService } from '../database/prisma.service';

type GdprResult<T = Record<string, unknown>> = T & {
	status: 'success' | 'error';
	message?: string;
};

@Injectable()
export class GdprService {
	private readonly logger = new Logger(GdprService.name);

	constructor(private readonly prisma: PrismaService) {}

	/** GDPR right of access — return everything user-service holds about a user. */
	async exportUserData(userId: string): Promise<GdprResult<{ data?: unknown }>> {
		if (!userId) return { status: 'error', message: 'userId is required' };

		const profile = await this.prisma.userProfile.findUnique({ where: { userId } });
		const login = profile?.login;

		const [connections, organizedEvents, eventParticipations] = await Promise.all([
			this.prisma.connection.findMany({
				where: { OR: [{ requesterId: userId }, { addresseeId: userId }] },
				orderBy: { updatedAt: 'desc' },
			}),
			this.prisma.event.findMany({
				where: { organizerUserId: userId },
				orderBy: { createdAt: 'desc' },
			}),
			login
				? this.prisma.eventParticipant.findMany({ where: { login } })
				: Promise.resolve([]),
		]);

		return {
			status: 'success',
			data: {
				profile: profile ?? null,
				connections,
				eventsOrganized: organizedEvents,
				eventParticipations,
			},
		};
	}

	/** Hard delete: remove the user's profile and any connection involving them. */
	async deleteUserData(userId: string): Promise<GdprResult> {
		if (!userId) return { status: 'error', message: 'userId is required' };

		const profile = await this.prisma.userProfile.findUnique({ where: { userId } });
		const login = profile?.login;

		await this.prisma.$transaction([
			this.prisma.event.deleteMany({ where: { organizerUserId: userId } }),
			...(login
				? [this.prisma.eventParticipant.deleteMany({ where: { login } })]
				: []),
			this.prisma.connection.deleteMany({
				where: { OR: [{ requesterId: userId }, { addresseeId: userId }] },
			}),
			this.prisma.userProfile.deleteMany({ where: { userId } }),
		]);

		return { status: 'success' };
	}

	/**
	 * Anonymize the profile: keep the row so connections still resolve, but
	 * scrub everything that could re-identify the user.
	 */
	async anonymizeUserData(
		userId: string,
		newLogin: string,
	): Promise<GdprResult> {
		if (!userId) return { status: 'error', message: 'userId is required' };

		const exists = await this.prisma.userProfile.findUnique({ where: { userId } });
		if (!exists) {
			// No profile yet; nothing to anonymize on this side.
			return { status: 'success' };
		}

		await this.prisma.$transaction([
			this.prisma.userProfile.update({
				where: { userId },
				data: {
					login: newLogin,
					bio: null,
					username: null,
					website: null,
					github: null,
					linkedin: null,
					campus: null,
				},
			}),
			this.prisma.event.updateMany({
				where: { organizerUserId: userId },
				data: { organizerLogin: newLogin },
			}),
			this.prisma.eventParticipant.updateMany({
				where: { login: exists.login },
				data: { login: newLogin },
			}),
		]);

		return { status: 'success' };
	}
}
