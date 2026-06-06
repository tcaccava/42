import { Injectable, BadRequestException, InternalServerErrorException, NotImplementedException } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { PrismaClient, Project, Subject, Score } from '../prisma/client/client';
import { PrismaBetterSqlite3 } from '@prisma/adapter-better-sqlite3';
import { resolveSqliteDatabaseUrl } from './sqlite-url';
import {
	GradeDTO,
	ProjectGradeDTO,
	ProjectSearchDTO,
	ProjectUpdateDTO,
	SortMethod,
} from './dto';
import { parsePublicRepositoryUrl } from './repository-url';

@Injectable()
export class AppService extends PrismaClient {
	constructor(configService: ConfigService) {
		const adapter = new PrismaBetterSqlite3({
			url: resolveSqliteDatabaseUrl(
				configService,
				__dirname,
				'PROJECT_DATABASE_URL',
				'apps/project-service/dev.db',
			),
		});
		super({ adapter });
	}

	async getProjectById(id: number): Promise<Project | null> {
		return this.project.findUnique({
			where: { id: id },
		});
	}

	// "Why don't we support case insensitive sorting?: https://github.com/prisma/prisma/issues/5068" LMFAO
	async searchProjects(
		dto: ProjectSearchDTO,
	): Promise<[Array<Project>, number, number]> {
		const textQuery =
			'title' in dto && dto.title != null && dto.title.trim() !== ''
				? dto.title.trim()
				: null;

		const where: {
			OR?: Array<
				| { title: { contains: string } }
				| { repositoryUrl: { contains: string } }
			>;
			subject?: Subject;
			user?: string;
		} = {};

		if (textQuery) {
			where.OR = [
				{ title: { contains: textQuery } },
				{ repositoryUrl: { contains: textQuery } },
			];
		}
		if ('subject' in dto && dto.subject != null) {
			where.subject = dto.subject;
		}
		if ('userId' in dto && dto.userId != null) {
			where.user = dto.userId;
		}

		let orderDirection = 'descending' in dto && dto.descending ? 'desc' : 'asc';
		let orderQuery;
		if (!dto.sortBy) {
			orderQuery = { createdAt: orderDirection };
		} else {
			switch (dto.sortBy) {
				case SortMethod.Grade: {
					orderDirection = orderDirection == 'asc' ? 'desc' : 'asc';
					throw new NotImplementedException();
				}
				case SortMethod.Date: {
					orderQuery = { createdAt: orderDirection };
					break;
				}
				case SortMethod.Alpha: {
					orderQuery = { title: orderDirection };
					break;
				}
			}
		}

		const pageSize = dto.pageSize;
		const pageNumber = dto.pageNumber ?? 0;
		const hasTagFilter =
			'tags' in dto && dto.tags != null && dto.tags.length > 0;

		if (!hasTagFilter && pageSize) {
			const [projects, totalCount] = await Promise.all([
				this.project.findMany({
					where,
					orderBy: orderQuery,
					skip: pageNumber * pageSize,
					take: pageSize,
				}),
				this.project.count({ where }),
			]);
			const pageCount =
				totalCount === 0 ? 0 : Math.ceil(totalCount / pageSize);
			return [projects, pageCount, totalCount];
		}

		let projects: Array<Project> = await this.project.findMany({
			where,
			orderBy: orderQuery,
		});

		if (hasTagFilter) {
			const tags = dto.tags ?? [];
			projects = projects.filter((item: Project) => {
				const itemTags = (item.tags as Array<string>) ?? [];
				return tags.reduce((val, tag) => val || itemTags.includes(tag), false);
			});
		}

		const totalCount = projects.length;
		if (pageSize) {
			const pageCount =
				totalCount === 0 ? 0 : Math.ceil(totalCount / pageSize);
			const startIndex = pageNumber * pageSize;
			projects = projects.slice(startIndex, startIndex + pageSize);
			return [projects, pageCount, totalCount];
		}

		return [projects, 0, totalCount];
	}

	async createProject(
		userId: string,
		repositoryUrl: string,
		subject: Subject,
	): Promise<Project> {
		const parsed = parsePublicRepositoryUrl(repositoryUrl);
		if (!parsed) {
			throw new BadRequestException('Invalid public repository URL');
		}

		return this.project.create({
			data: {
				title: parsed.slug,
				repositoryUrl: parsed.normalized,
				subject: subject,
				user: userId,
			},
		});
	}

	async updateProject(
		projectId: number,
		dto: ProjectUpdateDTO,
	): Promise<Project> {
		const updateTable: any = {};
		if (dto.description != null) {
			// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
			updateTable.description = dto.description;
		}

		if (dto.tagList) {
			// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
			updateTable.tags = dto.tagList;
		}

		if (dto.repositoryUrl != null) {
			const parsed = parsePublicRepositoryUrl(dto.repositoryUrl);
			if (!parsed) {
				throw new BadRequestException('Invalid public repository URL');
			}
			// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
			updateTable.repositoryUrl = parsed.normalized;
			// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
			updateTable.title = parsed.slug;
		}

		return this.project.update({
			// eslint-disable-next-line @typescript-eslint/no-unsafe-assignment
			data: updateTable,
			where: {
				id: projectId,
			},
		});
	}

	async deleteProjectById(projectId: number): Promise<boolean> {
		const deleteResult = await this.project.deleteMany({
			where: {
				id: projectId,
			},
		});
		return deleteResult.count > 0;
	}

	async deleteProjectsFromUser(userId: string): Promise<boolean> {
		return (
			(
				await this.project.deleteMany({
					where: {
						user: userId,
					},
				})
			).count > 0
		);
	}

	async reassignProjects(fromLogin: string, toLogin: string): Promise<number> {
		const result = await this.project.updateMany({
			where: { user: fromLogin },
			data: { user: toLogin },
		});
		return result.count;
	}

	async updateProjectScreenshots(
		id: number,
		keyList: Array<string>,
	): Promise<Project> {
		const updateTable: any = {};
		// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
		updateTable.screenshots = keyList;
		JSON.stringify(keyList).replace(/\\/g, ''); // What?
		return this.project.update({
			// eslint-disable-next-line @typescript-eslint/no-unsafe-assignment
			data: updateTable,
			where: {
				id: id,
			},
		});
	}

	async updateProjectArchive(
		id: number,
		key: string | null,
	): Promise<Project> {
		// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
		return this.project.update({
			// eslint-disable-next-line @typescript-eslint/no-unsafe-assignment
			data: {
				source: key,
			},
			where: {
				id: id,
			},
		});
	}

	async gradeProject(userId: string, gradeDto: GradeDTO): Promise<Score> {
		const project = await this.getProjectById(gradeDto.projectId);
		if (project == null) {
			throw new InternalServerErrorException(
				`Project with id ${gradeDto.projectId} was not found.`,
			);
		}

		return this.score.upsert({
			create: {
				userId: userId,
				projectId: gradeDto.projectId,
				score: gradeDto.score,
				favourite: gradeDto.favourite,
			},
			update: {
				score: gradeDto.score,
				favourite: gradeDto.favourite,
			},
			where: {
				userId_projectId: {
					userId,
					projectId: gradeDto.projectId,
				},
			},
		});
	}

	async getOwnGrade(userId: string, projectId: number): Promise<Score | null> {
		return this.score.findUnique({
			where: {
				userId_projectId: {
					userId,
					projectId,
				},
			},
		});
	}

	async getOwnGrades(userId: string): Promise<Score[]> {
		return this.score.findMany({
			where: {
				userId: userId,
			},
		});
	}

	async getProjectGrade(projectId: number): Promise<ProjectGradeDTO> {
		const project = await this.getProjectById(projectId);
		if (project == null) {
			throw new InternalServerErrorException(
				`Project with id ${projectId} was not found.`,
			);
		}

		const total = await this.score.aggregate({
			_avg: { score: true },
			_count: { score: true },
			where: { projectId: projectId },
		});
		const favouriteCount = await this.score.count({
			where: { projectId: projectId, favourite: true },
		});
		return {
			projectId: projectId,
			averageScore: total._avg.score,
			gradeCount: total._count.score,
			favouriteCount,
		};
	}
}
