import {
	Controller,
	Res,
	Req,
	UploadedFiles,
	UseInterceptors,
	All,
	InternalServerErrorException,
	UseGuards,
} from '@nestjs/common';
import { AnyFilesInterceptor } from '@nestjs/platform-express/multer';
import { ProjectService } from './project.service';
import type { Response, Request } from 'express';
import 'multer'; // Yet again, nannying my IDE so it stops yelling at me for no reason.
import { ProxyGuard } from '../proxy.guard';
import { memoryStorage } from 'multer';
import 'form-data';

@Controller('projects')
export class ProjectController {
	constructor(
		private readonly projectGateway: ProjectService,
		// private readonly logger: Logger
	) {
	}

	private extractUserId(request: Request): string | null {
		const user = (request as any).user
		if (!user) {
			return null;
		}
		return user.login;
	}

	private reconstructQuery(query: any): string {
		const entries = Object.entries(query);
		if (entries.length == 0) {
			return '';
		}
		return '?' + entries.map(([k, v]) => {
			if (v && v != '') {
				return `${k}=${v}`;
			}
			return `${k}`
		}).join('&');
	}

	@All('*path')
	@UseInterceptors(AnyFilesInterceptor({
		storage: memoryStorage(),
		limits: {
			fileSize: 10 * 1024 * 1024,
		}
	}))
	@UseGuards(ProxyGuard)
	async projectProxy(
		@Req() request: Request,
		@Res() response: Response,
		@UploadedFiles() fileHandles: Array<Express.Multer.File>,
	) {
		try {
			// eslint-disable-next-line @typescript-eslint/no-unused-vars
			const [_service, ...restPath] = request.path.split('/').filter((s) => s != '');

			// If the request path contains secure segment, then add userId to the query
			const query = request.query;
			if (restPath.find((s) => s == 'secure')) {
				const userId = this.extractUserId(request);
				if (!userId) {
					throw new InternalServerErrorException(`Unable to extract user id from the request.`);
				}
				// Also check if user has decided to sneak-in their own userId parameter in query
				query['userId'] = userId;
			}
			
			const queryStr = this.reconstructQuery(query);
			const path = `/projects/${restPath.join('/')}${queryStr}`;
			let body: any = undefined;
			const headers = request.headers;
			if (fileHandles != undefined && fileHandles.length > 0) {
				const formData = new FormData();
				fileHandles.forEach((file) =>
					formData.append(
						file.fieldname,
						new Blob([Buffer.from(file.buffer)], {
							type: file.mimetype,
						}),
						file.originalname,
					),
				);
				body = formData;
			} else {
				// eslint-disable-next-line @typescript-eslint/no-unsafe-assignment
				body = request.body;
			}
			const data = await this.projectGateway.forward(
				path,
				request.method,
				body,
				headers,
			);
			response.json(data);
		} catch (error: any) {
			throw new InternalServerErrorException(
				// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
				`API call failed: ${JSON.stringify(error)}`,
			);
		}
	}
}
