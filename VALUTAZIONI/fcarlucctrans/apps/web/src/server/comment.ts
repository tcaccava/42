'use server';

import { CommentData } from '@/types/comment';
import { getApiBaseUrl, joinApiUrl } from './api-url';
import { fetchWithAuth } from './http';

export async function getComments(projectId: number): Promise<CommentData[] | null> {
	try {
		const base = await getApiBaseUrl();
		const response = await fetch(
			joinApiUrl(base, `/comments/project?projectId=${projectId}`),
			{
				method: 'GET',
				cache: 'no-store',
			},
		);

		if (!response.ok) {
			throw new Error(`Failed to retrieve comments: ${await response.text()}`);
		}
		const raw = (await response.json()) as CommentData[];
		return raw ?? [];
	} catch (error) {
		console.error('Error when retrieving comments:', error);
		return null;
	}
}

export async function sendComment(
	projectId: number,
	comment: string,
): Promise<CommentData | null> {
	try {
		const response = await fetchWithAuth('/comments/secure/comment', {
			body: JSON.stringify({
				projectId,
				text: comment,
			}),
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
			},
		});

		if (!response.ok) {
			throw new Error(`Failed to send a comment: ${await response.text()}`);
		}
		return (await response.json()) as CommentData;
	} catch (error) {
		console.error('Error when sending a comment:', error);
		return null;
	}
}

export async function editComment(
	commentId: number,
	comment: string,
): Promise<CommentData | null> {
	try {
		const response = await fetchWithAuth('/comments/secure/comment', {
			body: JSON.stringify({
				id: commentId,
				text: comment,
			}),
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
			},
		});

		if (!response.ok) {
			throw new Error(`Failed to edit a comment: ${await response.text()}`);
		}
		return (await response.json()) as CommentData;
	} catch (error) {
		console.error('Error when editing a comment:', error);
		return null;
	}
}

export async function deleteComment(commentId: number): Promise<boolean> {
	try {
		const response = await fetchWithAuth(
			`/comments/secure/comment?id=${commentId}`,
			{
				method: 'DELETE',
			},
		);

		if (!response.ok) {
			throw new Error(`Failed to delete a comment: ${await response.text()}`);
		}
		return (await response.json()) as boolean;
	} catch (error) {
		console.error('Error when deleting a comment:', error);
		return false;
	}
}

export async function setUser(id: string, name: string): Promise<object | null> {
	try {
		const response = await fetchWithAuth('/comments/secure/user', {
			body: JSON.stringify({
				id,
				name,
			}),
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
			},
		});

		if (!response.ok) {
			throw new Error(`Failed to upsert comment user: ${await response.text()}`);
		}
		return await response.json();
	} catch (error) {
		console.error('Error when upserting comment user:', error);
		return null;
	}
}
