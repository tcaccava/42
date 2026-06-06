import { Subject } from '../prisma/client/enums';

export enum SortMethod {
	Grade = 'grade',
	Date = 'date',
	Alpha = 'alpha',
}

export interface ProjectSearchDTO {
	title?: string;
	subject?: Subject;
	tags?: Array<string>;
	userId?: string;
	sortBy?: SortMethod;
	descending?: boolean;
	pageNumber?: number;
	pageSize?: number;
}

export interface ProjectUploadDTO {
	title: string;
	subject: Subject;
	description: string;
	tagsList: string;
	//...
}

export interface ExampleDTO {
	thing: string;
}

export interface ProjectCreateDTO {
	repositoryUrl: string;
	subject: Subject;
}

export interface ProjectUpdateDTO {
	id: number;
	repositoryUrl: string | null;
	description: string | null;
	tagList: Array<string> | null;
}

export interface GradeDTO {
	projectId: number;
	score: number;
	favourite: boolean;
}

export interface ProjectGradeDTO {
	projectId: number;
	averageScore: number | null;
	gradeCount: number;
	favouriteCount: number;
}

export interface ScreenshotUrlRequestDTO {
	projectId: number;
	files: Array<{
		id: string,
		name: string,
		size: number,
		mime: string,
	}>;
}
