import { Injectable } from '@nestjs/common';
import crypto from 'crypto';
import { ApiKeyRepository } from '../database/api-key.repository';
import {
	PublicResourceRepository,
	type PublicResourceInput,
} from '../database/public-resource.repository';

function hashApiKey(rawKey: string): string {
	return crypto.createHash('sha256').update(rawKey).digest('hex');
}

@Injectable()
export class PublicApiService {
	constructor(
		private readonly apiKeyRepository: ApiKeyRepository,
		private readonly resourceRepository: PublicResourceRepository,
	) {}

	async validateApiKey(rawKey: string) {
		if (!rawKey?.trim()) {
			return { status: 'error', message: 'API key is required' };
		}

		const keyHash = hashApiKey(rawKey.trim());
		const record = await this.apiKeyRepository.findByHash(keyHash);

		if (!record || !record.isActive) {
			return { status: 'error', message: 'Invalid or inactive API key' };
		}

		return {
			status: 'success',
			key: {
				id: record.id,
				name: record.name,
				prefix: record.keyPrefix,
				requestsPerMinute: record.requestsPerMinute,
			},
		};
	}

	async listResources(limit = 50, offset = 0) {
		const [items, total] = await Promise.all([
			this.resourceRepository.findAll(limit, offset),
			this.resourceRepository.count(),
		]);
		return { status: 'success', items, total, limit, offset };
	}

	async getResource(id: string) {
		const item = await this.resourceRepository.findById(id);
		if (!item) return { status: 'error', message: 'Resource not found' };
		return { status: 'success', item };
	}

	async createResource(body: PublicResourceInput) {
		if (!body?.title?.trim()) {
			return { status: 'error', message: 'title is required' };
		}
		const item = await this.resourceRepository.create({
			title: body.title.trim(),
			content: body.content ?? null,
			tags: body.tags ?? [],
		});
		return { status: 'success', item };
	}

	async updateResource(id: string, body: Partial<PublicResourceInput>) {
		const existing = await this.resourceRepository.findById(id);
		if (!existing) return { status: 'error', message: 'Resource not found' };

		const item = await this.resourceRepository.update(id, {
			...(body.title !== undefined ? { title: body.title.trim() } : {}),
			...(body.content !== undefined ? { content: body.content } : {}),
			...(body.tags !== undefined ? { tags: body.tags } : {}),
		});
		return { status: 'success', item };
	}

	async deleteResource(id: string) {
		const existing = await this.resourceRepository.findById(id);
		if (!existing) return { status: 'error', message: 'Resource not found' };
		await this.resourceRepository.delete(id);
		return { status: 'success', deletedId: id };
	}

	/** Used by seed script only — returns the raw key once. */
	async createApiKey(name: string, requestsPerMinute = 60) {
		const rawKey = `ft_${crypto.randomBytes(24).toString('hex')}`;
		const keyPrefix = rawKey.slice(0, 12);
		const record = await this.apiKeyRepository.create({
			name,
			keyPrefix,
			keyHash: hashApiKey(rawKey),
			requestsPerMinute,
		});
		return { record, rawKey };
	}
}
