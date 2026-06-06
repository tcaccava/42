import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';

export type PublicResourceInput = {
	title: string;
	content?: string | null;
	tags?: string[];
};

@Injectable()
export class PublicResourceRepository {
	constructor(private readonly prisma: PrismaService) {}

	findAll(limit = 50, offset = 0) {
		return this.prisma.publicResource.findMany({
			take: limit,
			skip: offset,
			orderBy: { createdAt: 'desc' },
		});
	}

	count() {
		return this.prisma.publicResource.count();
	}

	findById(id: string) {
		return this.prisma.publicResource.findUnique({ where: { id } });
	}

	create(data: PublicResourceInput) {
		return this.prisma.publicResource.create({
			data: {
				title: data.title,
				content: data.content ?? null,
				tags: data.tags ?? [],
			},
		});
	}

	update(id: string, data: Partial<PublicResourceInput>) {
		return this.prisma.publicResource.update({
			where: { id },
			data: {
				...(data.title !== undefined ? { title: data.title } : {}),
				...(data.content !== undefined ? { content: data.content } : {}),
				...(data.tags !== undefined ? { tags: data.tags } : {}),
			},
		});
	}

	delete(id: string) {
		return this.prisma.publicResource.delete({ where: { id } });
	}
}
