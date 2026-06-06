'use server';

import { ProjectData, ProjectKind } from "@/types/project";
import type { ProjectGradeStats, UserProjectGrade } from "@/types/feed";
import { ProjectCard, SearchQuery } from "@/types/search";
import { cookies } from "next/headers";
import { getApiBaseUrl, joinApiUrl } from "./api-url";
import { fetchWithAuth } from "./http";

export async function getCookieHeader(): Promise<string> {
	const cookieStore = await cookies();
	return cookieStore
	  .getAll()
	  .map((c) => `${c.name}=${c.value}`)
	  .join('; ');
}

function buildSearchBody(query: SearchQuery): Record<string, unknown> {
	const body: Record<string, unknown> = {
		title: query.title,
		tags: query.tags,
		sortBy: query.sortBy,
		descending: query.descending,
		pageSize: query.pageSize,
		pageNumber: query.pageNumber,
	};
	if (query.subject != null) {
		body.subject = query.subject;
	}
	if (query.userId) {
		body.userId = query.userId;
	}
	return body;
}

export type ProjectSearchBase = Omit<SearchQuery, 'pageNumber' | 'pageSize'>;

export async function searchProjectsSlice(
	query: ProjectSearchBase,
	offset: number,
	limit: number,
): Promise<[ProjectCard[], boolean, string | undefined]> {
	const fetchSize = offset + limit + 1;
	const [cards, , , error] = await searchProjects({
		...query,
		pageNumber: 0,
		pageSize: fetchSize,
	});
	if (error) {
		return [[], false, error];
	}
	const hasMore = cards.length > offset + limit;
	return [cards.slice(offset, offset + limit), hasMore, undefined];
}

export async function searchProjects(
	query: SearchQuery,
): Promise<[ProjectCard[], number, number, string | undefined]> {
	try {
		const base = await getApiBaseUrl();
		const response = await fetch(joinApiUrl(base, '/projects/search'), {
			body: JSON.stringify(buildSearchBody(query)),
			method: 'POST',
			headers: {
				"Content-Type": "application/json",
			},
			cache: 'no-store',
		});

		if (!response.ok) {
			const body = await response.text();
			console.error(`Project search failed (${response.status}):`, body);
			return [[], 0, 0, `Search unavailable (${response.status})`];
		}
		const result = (await response.json()) as {
			cards?: ProjectCard[];
			pageCount?: number;
			totalCount?: number;
		};
		return [
			result.cards ?? [],
			result.pageCount ?? 0,
			result.totalCount ??
				(result.pageCount && query.pageSize
					? Math.max(
							(result.pageCount - 1) * query.pageSize +
								(result.cards?.length ?? 0),
							result.cards?.length ?? 0,
						)
					: (result.cards?.length ?? 0)),
			undefined,
		];
	}
	catch (error) {
		const message = error instanceof Error ? error.message : 'Search failed';
		console.error('Error when requesting a project search:', message);
		return [[], 0, 0, message];
	}
}

async function gatewayUrl(path: string): Promise<string> {
	return joinApiUrl(await getApiBaseUrl(), path);
}

export async function newProject(
	kind: ProjectKind,
	repositoryUrl: string,
): Promise<object | null> {
	try {
		const response = await fetchWithAuth('/projects/secure/new', {
			body: JSON.stringify({
				subject: kind,
				repositoryUrl: repositoryUrl,
			}),
			method: 'POST',
			headers: {
				"Content-Type": "application/json",
			},
		});

		if (!response.ok) throw new Error(`Failed to create a new project: ${JSON.stringify(await response.json())}`);
		return response.json();
	}
	catch (error) {
		console.error('Error when creating a new project:', error);
		return null;
	}
}

export async function getProject(id: number): Promise<ProjectData | null> {
	try {
		const response = await fetch(await gatewayUrl(`/projects/byId?id=${id}`), {
			method: 'GET',
			headers: {}
		});

		if (!response.ok) throw new Error(`Failed to retrieve a project: ${await response.json()}`);
		const raw = await response.json();
		return {
			projectId: raw.id,
			userId: raw.user,
			title: raw.title,
			repositoryUrl: raw.repositoryUrl ?? '',
			subject: raw.subject,
			description: raw.description,
			screenshots: raw.screenshots,
			tags: raw.tags,
			source: raw.source,
		}
	}
	catch (error) {
		console.error('Error when retrieving a project:', error);
		return null;
	}
}

export async function updateProject(dto: any): Promise<object | null> {
	try {
		const response = await fetchWithAuth('/projects/secure/update', {
			body: JSON.stringify(dto),
			method: 'POST',
			headers: {
				"Content-Type": "application/json",
			},
		});

		if (!response.ok) throw new Error(`Failed to update a project: ${JSON.stringify(await response.json())}`);
		return response.json();
	}
	catch (error) {
		console.error('Error when updating a project:', error);
		return null;
	}
}

export async function deleteProject(id: number): Promise<boolean> {
	try {
		const response = await fetchWithAuth(`/projects/secure/byId?id=${id}`, {
			method: 'DELETE',
		});

		if (!response.ok) throw new Error(`Failed to delete a project: ${JSON.stringify(await response.json())}`);
		return await response.json();
	}
	catch (error) {
		console.error('Error when deleting a project:', error);
		return false;
	}
}

export async function clearProjectScreenshots(id: number): Promise<boolean> {
	try {
		const response = await fetchWithAuth(`/projects/secure/clear/screenshots?id=${id}`, {
			method: 'POST',
		});

		if (!response.ok) throw new Error(`Failed to clear project screenshots: ${JSON.stringify(await response.json())}`);
		return await response.json();
	}
	catch (error) {
		console.error('Error when clearing project screenshots:', error);
		return false;
	}
}

export async function clearProjectSource(id: number): Promise<boolean> {
	try {
		const response = await fetchWithAuth(`/projects/secure/clear/source?id=${id}`, {
			method: 'POST',
		});

		if (!response.ok) throw new Error(`Failed to clear project source: ${JSON.stringify(await response.json())}`);
		return await response.json();
	}
	catch (error) {
		console.error('Error when clearing project source:', error);
		return false;
	}
}

export async function getProjectGradeStats(
	projectId: number,
): Promise<ProjectGradeStats | null> {
	try {
		const base = await getApiBaseUrl();
		const response = await fetch(
			joinApiUrl(base, `/projects/grade/total?id=${projectId}`),
			{ cache: 'no-store' },
		);
		if (!response.ok) return null;
		return (await response.json()) as ProjectGradeStats;
	} catch (error) {
		console.error('Error when fetching project grade stats:', error);
		return null;
	}
}

export async function getMyProjectGrade(
	projectId: number,
): Promise<UserProjectGrade | null> {
	try {
		const response = await fetchWithAuth(
			`/projects/secure/grade?projectId=${projectId}`,
			{ method: 'GET' },
		);
		if (!response.ok) return null;
		return (await response.json()) as UserProjectGrade;
	} catch (error) {
		return null;
	}
}
