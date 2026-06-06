import {
	Body,
	Controller,
	Delete,
	Get,
	BadRequestException,
	InternalServerErrorException,
	NotFoundException,
	NotImplementedException,
	ParseIntPipe,
	Post,
	Put,
	Query,
	UnauthorizedException,
	UploadedFile,
	UploadedFiles,
	UseInterceptors,
} from '@nestjs/common';
import { AppService } from './project.service';
import { Project } from '../prisma/client/client';
import { GDPRResponse, ProjectCardData, SearchResult } from './responses';
import * as dto from './dto';
import { AnyFilesInterceptor, FileInterceptor } from '@nestjs/platform-express';
import { S3Service } from './s3.service';
import { Score } from '../prisma/client/browser';
import 'multer'; // This line exists so my IDE stops crying like baby about a type that actually exists with no problem.

const SCREENSHOT_MIMETYPES = [
	'image/png', 'image/jpeg', 'image/webp', 'image/gif'
];

const ARCHIVE_MIMETYPES = [
	'application/zip',
	'application/x-zip-compressed',
	'application/x-tar',
	'application/gzip',
	'application/x-gzip',
];

@Controller('projects')
export class AppController {
	constructor(
		private readonly appService: AppService,
		private readonly s3Service: S3Service,
	) {}

	@Get('byId')
	async getProject(@Query('id', ParseIntPipe) id: number): Promise<Project> {
		console.log('Received project GET request');
		const maybeObject = await this.appService.getProjectById(id);
		if (maybeObject === null) {
			throw new InternalServerErrorException(
				`Project with id ${id} was not found`,
			);
		}
		const screenshotKeys: Array<string> | undefined =
			maybeObject.screenshots?.valueOf() as Array<string>;
		if (screenshotKeys) {
			maybeObject.screenshots = await Promise.all(
				screenshotKeys?.map(async (key) => await this.s3Service.getFileUrl(key)),
			);
		}
		if (maybeObject.source) {
			maybeObject.source = await this.s3Service.getFileUrl(maybeObject.source);
		}
		return maybeObject;
	}

	@Get('fromUser')
	async getAllUserProjects(
		@Query('id') id: string,
	): Promise<Array<Project>> {
		return this.appService.searchProjects({
			userId: id,
		})[0];
	}

	@Post('search')
	async searchProjects(
		@Body() body: dto.ProjectSearchDTO,
	): Promise<SearchResult> {
		const [projects, pageCount, totalCount] =
			await this.appService.searchProjects(body);
		const cards = await Promise.all(
			projects.map(async (project) => {
				const card = ProjectCardData.fromModel(project);
				const screenshotKeys = project.screenshots?.valueOf() as
					| string[]
					| undefined;
				if (screenshotKeys?.length) {
					card.screenshotImages = await Promise.all(
						screenshotKeys.map(async (key) =>
							this.s3Service.getFileUrl(key),
						),
					);
					card.coverImage = card.screenshotImages[0];
				}
				return card;
			}),
		);
		return {
			cards: cards,
			pageCount: pageCount,
			totalCount: totalCount,
		};
	}

	@Post('secure/new')
	async newProject(
		@Body() dto: dto.ProjectCreateDTO,
		@Query('userId') userId: string
	): Promise<Project> {
		if (!dto.repositoryUrl?.trim()) {
			throw new BadRequestException('repositoryUrl is required');
		}
		const project = await this.appService.createProject(
			userId,
			dto.repositoryUrl,
			dto.subject,
		);
		return project;
	}

	@Put('secure/update/screenshots')
	@UseInterceptors(AnyFilesInterceptor())
	async uploadScreenshots(
		@UploadedFiles() fileHandles: Array<Express.Multer.File>,
		@Query('id', ParseIntPipe) id: number,
		@Query('userId') userId: string,
	): Promise<Project> {
		if (!fileHandles) {
			throw new InternalServerErrorException('fileHandles is empty');
		}

		const project = await this.appService.getProjectById(id);
		if (!project) {
			throw new InternalServerErrorException(`Project with id ${id} was not found.`);
		}
		if (project.user != userId) {
			throw new UnauthorizedException(`Mismatch between provided and project user ids.`);
		}

		const keys = await Promise.all(
			fileHandles.map(async (handle) => {
				if (!SCREENSHOT_MIMETYPES.includes(handle.mimetype)) {
					throw new InternalServerErrorException('Invalid screenshot mime type.');
				};
				return this.s3Service.uploadFile(userId, handle);
			}),
		);

		return await this.appService.updateProjectScreenshots(id, keys);
	}

	@Put('secure/update/source')
	@UseInterceptors(FileInterceptor('archive'))
	// eslint-disable-next-line @typescript-eslint/require-await
	async uploadSourceArchive(
		// eslint-disable-next-line @typescript-eslint/no-unused-vars
		@UploadedFile() handle: Express.Multer.File,
		// eslint-disable-next-line @typescript-eslint/no-unused-vars
		@Query('id', ParseIntPipe) id: number,
		// eslint-disable-next-line @typescript-eslint/no-unused-vars
		@Query('userId') userId: string,
	): Promise<object> {
		if (!handle) {
			throw new InternalServerErrorException('handle is missing.');
		}

		const project = await this.appService.getProjectById(id);
		if (!project) {
			throw new InternalServerErrorException(`Project with id ${id} was not found.`);
		}
		if (project.user != userId) {
			throw new UnauthorizedException(`Mismatch between provided and project user ids.`);
		}

		if (!ARCHIVE_MIMETYPES.includes(handle.mimetype)) {
			throw new InternalServerErrorException('Invalid archive mime type.');
		}

		const key = await this.s3Service.uploadFile(userId, handle);

		return await this.appService.updateProjectArchive(id, key);
	}

	@Post('secure/clear/screenshots')
	@UseInterceptors(AnyFilesInterceptor())
	async clearScreenshots(
		@Query('id', ParseIntPipe) id: number,
		@Query('userId') userId: string,
	): Promise<boolean> {
		const project = await this.appService.getProjectById(id);
		if (!project) {
			throw new InternalServerErrorException(`Project with id ${id} was not found.`);
		}
		if (project.user != userId) {
			throw new UnauthorizedException(`Mismatch between provided and project user ids.`);
		}

		if (!project.screenshots || (project.screenshots.valueOf() as string[]).length == 0) {
			return true;
		}

		for (const key of project.screenshots as string[]) {
			await this.s3Service.deleteFile(key);
		}
		await this.appService.updateProjectScreenshots(id, []);
		return true;
	}

	@Post('secure/clear/source')
	@UseInterceptors(AnyFilesInterceptor())
	async clearSource(
		@Query('id', ParseIntPipe) id: number,
		@Query('userId') userId: string,
	): Promise<boolean> {
		const project = await this.appService.getProjectById(id);
		if (!project) {
			throw new InternalServerErrorException(`Project with id ${id} was not found.`);
		}
		if (project.user != userId) {
			throw new UnauthorizedException(`Mismatch between provided and project user ids.`);
		}

		if (!project.source) {
			return true;
		}

		await this.s3Service.deleteFile(project.source);
		await this.appService.updateProjectArchive(id, null);
		return true;
	}

	@Post('secure/update')
	async updateProjectData(
		@Body() data: dto.ProjectUpdateDTO,
		@Query('userId') userId: string,
	): Promise<Project> {
		const project = await this.appService.getProjectById(data.id);
		if (!project) {
			throw new InternalServerErrorException(`Project with id ${data.id} was not found.`);
		}
		if (project.user != userId) {
			throw new UnauthorizedException(`Mismatch between provided and project user ids.`);
		}
		const updatedProject = this.appService.updateProject(data.id, data);
		return updatedProject;
	}

	@Delete('secure/byId')
	async deleteProjectById(
		@Query('id', ParseIntPipe) id: number,
		// eslint-disable-next-line @typescript-eslint/no-unused-vars
		@Query('userId') userId: string,
	): Promise<boolean> {
		const project = await this.appService.getProjectById(id);
		if (!project) {
			throw new InternalServerErrorException(`Project with id ${id} was not found.`);
		}
		if (project.user != userId) {
			throw new UnauthorizedException(`Mismatch between provided and project user ids.`);
		}

		const screenshotKeys = project.screenshots?.valueOf() as string[] | undefined;
		if (screenshotKeys?.length) {
			for (const key of screenshotKeys) {
				await this.s3Service.deleteFile(key);
			}
		}
		if (project.source) {
			await this.s3Service.deleteFile(project.source);
		}

		return this.appService.deleteProjectById(id);
	}

	@Delete('secure/fromUser')
	async deleteAllProjectsFromUser(
		@Query('userId') userId: string,
	): Promise<boolean> {
		const [projects] = await this.appService.searchProjects({ userId });

		for (const project of projects) {
			const screenshotKeys = project.screenshots?.valueOf() as
				| string[]
				| undefined;
			if (screenshotKeys?.length) {
				for (const key of screenshotKeys) {
					await this.s3Service.deleteFile(key);
				}
			}
			if (project.source) {
				await this.s3Service.deleteFile(project.source);
			}
		}

		return this.appService.deleteProjectsFromUser(userId);
	}

	@Put('secure/reassignUser')
	async reassignUserProjects(
		@Query('from') fromLogin: string,
		@Query('to') toLogin: string,
	): Promise<{ updated: number }> {
		if (!fromLogin || !toLogin) {
			throw new InternalServerErrorException('from and to login are required');
		}
		const updated = await this.appService.reassignProjects(fromLogin, toLogin);
		return { updated };
	}

	@Post('secure/grade')
	async gradeProject(
		@Body() gradeDto: dto.GradeDTO,
		@Query('userId') userId: string,
	): Promise<Score> {
		return this.appService.gradeProject(userId, gradeDto);
	}

	@Get('secure/grade')
	async getGrade(
		@Query('projectId', ParseIntPipe) projectId: number,
		// eslint-disable-next-line @typescript-eslint/no-unused-vars
		@Query('userId') userId: string,
	): Promise<Score> {
		const grade = await this.appService.getOwnGrade(userId, projectId);
		if (!grade) {
			throw new NotFoundException('Grade not found for this project');
		}
		return grade;
	}

	@Get('grade/total')
	async getTotalGrade(
		@Query('id', ParseIntPipe) id: number,
	): Promise<dto.ProjectGradeDTO> {
		return this.appService.getProjectGrade(id);
	}

	@Get('secure/gdpr')
	async getAllUserData(
		@Query('userId') userId: string,
	): Promise<GDPRResponse | null> {
		const [foundProjects, _] = await this.appService.searchProjects({
			userId: userId,
		});

		const projects: any[] = await Promise.all(foundProjects.map(async project => {
			const screenshotKeys: Array<string> | undefined =
				project.screenshots?.valueOf() as Array<string>;
			
			const screenshotUrls = screenshotKeys ? await Promise.all(
				screenshotKeys?.map(async (key) => await this.s3Service.getFileUrl(key)),
			) : null;

			const sourceUrl = project.source ? await this.s3Service.getFileUrl(project.source) : null;

			return {
				...project,
				screenshotUrls,
				sourceUrl
			}
		}));

		const grades = await this.appService.getOwnGrades(userId);

		return {
			projects: projects,
			grades: grades,
		};
	}
}
