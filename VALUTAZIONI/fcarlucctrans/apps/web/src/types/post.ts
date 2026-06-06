/**
 * Post & Comment Types
 * Defines post, comment, and reaction structures for the feed
 */

export interface Reaction {
	type: 'like' | 'heart' | 'fire' | 'clap';
	users: string[]; // Stores logins/usernames
}

export interface Comment {
	id: string;
	login: string;
	image: string;
	campus: string;
	text: string;
	timestamp: string;
	reactions: Reaction[];
}

export interface Post {
	id: string;
	login: string;
	image: string;
	campus: string;
	projectName: string;
	screenshot: string;
	description: string;
	likes: number;
	comments: Comment[];
	timestamp: string;
	tags: string[];
	rating?: number | null; // average or user's rating (optional)
}
