import {
	BadRequestException,
	Body,
	Controller,
	Delete,
	Get,
	NotFoundException,
	Param,
	Post,
	Put,
	Query,
	Res,
	UseGuards,
} from '@nestjs/common';
import type { Response } from 'express';
import { ApiKeyGuard } from './api-key.guard';
import { publicOpenApiSpec } from './openapi.spec';
import { PublicApiService } from './public-api.service';

@Controller('api/v1')
export class PublicApiController {
	constructor(private readonly publicApiService: PublicApiService) {}

	/** Documentation — no API key required. */
	@Get('openapi.json')
	getOpenApiSpec(@Res() res: Response) {
		res.setHeader('Content-Type', 'application/json');
		return res.json(publicOpenApiSpec);
	}

	@Get('docs')
	getDocsRedirect(@Res() res: Response) {
		return res.redirect(302, '/api/v1/openapi.json');
	}

	@UseGuards(ApiKeyGuard)
	@Get('resources')
	async list(
		@Query('limit') limit?: string,
		@Query('offset') offset?: string,
	) {
		const parsedLimit = limit ? Math.min(Number(limit) || 50, 100) : 50;
		const parsedOffset = offset ? Math.max(Number(offset) || 0, 0) : 0;

		const result = await this.publicApiService.list(parsedLimit, parsedOffset);
		if (result.status !== 'success') {
			throw new BadRequestException(result.message ?? 'Failed to list resources');
		}
		return result;
	}

	@UseGuards(ApiKeyGuard)
	@Get('resources/:id')
	async get(@Param('id') id: string) {
		const result = await this.publicApiService.get(id);
		if (result.status !== 'success') {
			throw new NotFoundException(result.message ?? 'Resource not found');
		}
		return result;
	}

	@UseGuards(ApiKeyGuard)
	@Post('resources')
	async create(
		@Body() body: { title?: string; content?: string; tags?: string[] },
		@Res({ passthrough: true }) res: Response,
	) {
		if (!body?.title?.trim()) {
			throw new BadRequestException('title is required');
		}

		const result = await this.publicApiService.create({
			title: body.title,
			content: body.content,
			tags: body.tags,
		});

		if (result.status !== 'success') {
			throw new BadRequestException(result.message ?? 'Failed to create resource');
		}

		res.status(201);
		return result;
	}

	@UseGuards(ApiKeyGuard)
	@Put('resources/:id')
	async update(
		@Param('id') id: string,
		@Body() body: { title?: string; content?: string; tags?: string[] },
	) {
		const result = await this.publicApiService.update(id, body);
		if (result.status !== 'success') {
			throw new NotFoundException(result.message ?? 'Resource not found');
		}
		return result;
	}

	@UseGuards(ApiKeyGuard)
	@Delete('resources/:id')
	async remove(@Param('id') id: string) {
		const result = await this.publicApiService.remove(id);
		if (result.status !== 'success') {
			throw new NotFoundException(result.message ?? 'Resource not found');
		}
		return result;
	}
}
