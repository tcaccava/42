import { ProjectKind } from "./project";

export type SearchQuery = {
	title: string;
	subject: ProjectKind | null;
	tags: string[];
	sortBy: SortMethod;
	descending: boolean;
	pageNumber: number;
	pageSize: number;
	userId?: string;
}

export enum SortMethod {
	Date = 'date',
	Alpha = 'alpha',
	// Grade = 'grade', // not supported yet
}

let smValues = [];
for (const value in SortMethod) {
	smValues.push([value, (SortMethod as any)[value]]);
}
export const SortMethodValues = smValues;

export type ProjectCard = {
	id: number;
	title: string;
	repositoryUrl?: string | null;
	shortDescription: string;
	userId: string;
	subject: ProjectKind;
	tags: string[];
	coverImage?: string | null;
	screenshotImages?: string[];
	createdAt?: string | null;
}

export const CARD_COUNT = 5;
export const SEARCH_PAGE_SIZE = 6;
export const FEED_INITIAL_SIZE = 5;
export const FEED_PAGE_SIZE = 5;
export const PROFILE_PROJECTS_PAGE_SIZE = 12;
