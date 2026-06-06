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
import { CommentsService } from './comments.service';
import type { Response, Request } from 'express';
import { ProxyGuard } from '../proxy.guard';

@Controller('comments')
export class CommentsController {
	
	constructor(
		private readonly projectGateway: CommentsService,
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
	@UseInterceptors(AnyFilesInterceptor())
	@UseGuards(ProxyGuard)
	async projectProxy(
		@Req() request: Request,
		@Res() response: Response
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
			// const path = `/comments/${restPath.join('/')}`;
			const path = `/comments/${restPath.join('/')}${queryStr}`;
			const data = await this.projectGateway.forward(
				path,
				request.method,
				request.body,
				request.headers,
			);
			response.json(data);
		} catch (error: any) {
			throw new InternalServerErrorException(
				// eslint-disable-next-line @typescript-eslint/no-unsafe-member-access
				`API call failed: ${JSON.stringify(error?.response?.data)}`,
			);
		}
	}
}