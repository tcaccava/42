import { Project, Score } from '../prisma/client/client';
import { Subject } from '../prisma/client/enums';

export interface Error {
	message: string;
}

export class ProjectCardData {
	id: number | undefined;
	title: string | undefined;
	repositoryUrl: string | undefined;
	shortDescription: string | undefined;
	userId: string | undefined;
	subject: Subject | undefined;
	tags: string[] | undefined;
	coverImage: string | undefined;
	screenshotImages: string[] | undefined;
	createdAt: string | undefined;

	static fromModel(model: Project): ProjectCardData {
		const cardData = new ProjectCardData();
		cardData.id = model.id;
		cardData.title = model.title;
		cardData.repositoryUrl = model.repositoryUrl || undefined;
		if (model.description) {
			cardData.shortDescription =
				model.description.length > 16
					? `${model.description.substring(0, 16)}...`
					: model.description;
		} else {
			cardData.shortDescription = '';
		}
		cardData.userId = model.user;
		cardData.subject = model.subject;
		cardData.tags = model.tags ? (model.tags as string[]) : [];
		cardData.createdAt = model.createdAt?.toISOString?.() ?? undefined;
		return cardData;
	}
}

export interface SearchResult {
	cards: ProjectCardData[];
	pageCount: number;
	totalCount: number;
}

export interface GDPRResponse {
	projects: any[],
	grades: Score[],
}