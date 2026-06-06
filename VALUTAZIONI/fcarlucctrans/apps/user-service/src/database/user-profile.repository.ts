import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';
import type { EditableProfileFields } from '../app.service';

@Injectable()
export class UserProfileRepository {
	constructor(private readonly prisma: PrismaService) {}

	findByUserId(userId: string) {
		return this.prisma.userProfile.findUnique({ where: { userId } });
	}

	upsertByUserId(userId: string, login: string, patch: EditableProfileFields) {
		return this.prisma.userProfile.upsert({
			where: { userId },
			create: {
				userId,
				login,
				bio: patch.bio,
				username: patch.username,
				website: patch.website,
				github: patch.github,
				linkedin: patch.linkedin,
				campus: patch.campus,
				coverImage: patch.coverImage,
			},
			update: {
				login,
				bio: patch.bio,
				username: patch.username,
				website: patch.website,
				github: patch.github,
				linkedin: patch.linkedin,
				campus: patch.campus,
				coverImage: patch.coverImage,
			},
		});
	}
}
