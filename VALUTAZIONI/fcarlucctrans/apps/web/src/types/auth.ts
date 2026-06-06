/**
 * Authentication Types
 * Defines user data and auth response structures
 */

export interface User {
	userId?: string;
	login: string;
	email: string;
	image?: string;
	first_name?: string;
	last_name?: string;
	displayname?: string;
	pool_month?: string;
	pool_year?: number;
	location?: string;
	wallet?: number;
	correction_point?: number;
	grade?: string;
	kind?: string;
	status?: string;
	createdAt?: string;
	updatedAt?: string;
	// Profile editable fields
	bio?: string;
	username?: string;
	website?: string;
	github?: string;
	linkedin?: string;
	campus?: string;
	coverImage?: string;
	/** True when the user has a valid access-token session in the database */
	isActive?: boolean;
}

export interface AuthResponse {
	status: 'success' | 'error' | 'unauthorized';
	user?: User;
	message?: string;
}
