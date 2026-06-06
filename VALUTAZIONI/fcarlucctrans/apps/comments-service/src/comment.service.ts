import { Injectable, NotImplementedException } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { PrismaBetterSqlite3 } from '@prisma/adapter-better-sqlite3';
// import { Comment, PrismaClient, User } from '../prisma/client';
import { Comment, PrismaClient, User } from '@prisma/client';
import { CreateCommentDTO, UpdateCommentDTO, UploadUserDTO } from './dto';
import { resolveSqliteDatabaseUrl } from './sqlite-url';

@Injectable()
export class CommentService {
	private readonly dbClient: PrismaClient;
	constructor(configService: ConfigService) {
		const adapter = new PrismaBetterSqlite3({
			url: resolveSqliteDatabaseUrl(
				configService,
				__dirname,
				'COMMENTS_DATABASE_URL',
				'apps/comments-service/dev.db',
			),
		});
		this.dbClient = new PrismaClient({ adapter });
	}

	async createComment(userId: string, dto: CreateCommentDTO): Promise<Comment> {
		await this.dbClient.user.upsert({
			where: { id: userId },
			create: { id: userId, name: userId },
			update: {},
		});
		return this.dbClient.comment.create({
			data: {
				userId: userId,
				commentText: dto.text,
				projectId: dto.projectId,
			},
			include: {
				user: true,
			},
		});
	}

	async updateComment(dto: UpdateCommentDTO): Promise<Comment> {
		return this.dbClient.comment.update({
			data: {
				commentText: dto.text,
			},
			where: {
				id: dto.id,
			}
		});
	}

	async getComment(id: number): Promise<Comment | null> {
		// const kekw = this.dbClient.
		return this.dbClient.comment.findUnique({
			// relationLoadStrategy: 'join',
			include: {
				user: true,
			},
			where: {
				id: id,
			},
		});
	}

	async getComments(projectId: number): Promise<Array<Comment>> {
		return this.dbClient.comment.findMany({
			where: {
				projectId: projectId,
			},
			orderBy: {
				createdAt: 'asc',
			},
			include: {
				user: true,
			},
		});
	}

	async deleteComment(id: number): Promise<boolean> {
		const deleteResult = await this.dbClient.comment.deleteMany({
			where: {
				id: id,
			}
		});
		return deleteResult.count > 0;
	}

	async getUser(id: string): Promise<User | null> {
		return this.dbClient.user.findUnique({
			where: {
				id: id,
			}
		});
	}

	async upsertUser(dto: UploadUserDTO): Promise<User> {
		return this.dbClient.user.upsert({
			create: {
				id: dto.id,
				name: dto.name,
			},
			update: {
				name: dto.name,
			},
			where: {
				id: dto.id,
			},
		});
	}

	async deleteUser(id: string): Promise<boolean> {
		const deleteResult = await this.dbClient.user.deleteMany({
			where: {
				id: id,
			}
		});
		await this.dbClient.comment.deleteMany({ // Not sure if this is necessary.
			where: {
				userId: id,
			}
		});
		return deleteResult.count > 0;
	}

	async renameUser(fromLogin: string, toLogin: string): Promise<{ updated: number }> {
		const updated = await this.dbClient.$transaction(async (tx) => {
			const commentUpdate = await tx.comment.updateMany({
				where: { userId: fromLogin },
				data: { userId: toLogin },
			});
			await tx.user.upsert({
				where: { id: toLogin },
				create: { id: toLogin, name: toLogin },
				update: { name: toLogin },
			});
			await tx.user.deleteMany({ where: { id: fromLogin } });
			return commentUpdate.count;
		});
		return { updated };
	}

	async deleteProject(id: number): Promise<boolean> {
		const deleteResult = await this.dbClient.comment.deleteMany({
			where: {
				projectId: id,
			}
		});
		return deleteResult.count > 0;
	}

	async getGDPRData(userId: string): Promise<Comment[]> {
		return this.dbClient.comment.findMany({
			where: {
				userId: userId,
			}
		});
	}
}
