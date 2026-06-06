/**
 * Event Types
 * Defines event structure for the events page
 */

export interface Event {
	id: string;
	title: string;
	description: string;
	organizer: string;
	organizerImage: string;
	campus: string;
	date: string;
	time: string;
	location: string;
	participants: string[];
	maxParticipants: number;
	tags: string[];
	coverImage?: string;
}
