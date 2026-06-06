import { Inject, Injectable, Logger } from '@nestjs/common';
import { ClientProxy } from '@nestjs/microservices';
import { lastValueFrom } from 'rxjs';

export type EventDto = {
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
};

export type EventActionResult = {
	status: 'success' | 'error';
	event?: EventDto;
	events?: EventDto[];
	pageCount?: number;
	totalCount?: number;
	message?: string;
};

export type CreateEventBody = {
	title: string;
	description: string;
	organizerImage?: string;
	campus: string;
	date: string;
	time: string;
	location: string;
	maxParticipants: number;
	tags: string[];
	coverImage?: string;
};

export type UpdateEventBody = {
	title?: string;
	description?: string;
	campus?: string;
	date?: string;
	time?: string;
	location?: string;
	maxParticipants?: number;
	tags?: string[];
	coverImage?: string;
};

@Injectable()
export class EventsService {
	private readonly logger = new Logger(EventsService.name);

	constructor(@Inject('USER_SERVICE') private readonly userClient: ClientProxy) {}

	async listEvents(): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_list' }, {}),
			);
		} catch (error) {
			this.logger.error('User microservice error (list events):', error);
			return { status: 'error', message: 'Failed to list events' };
		}
	}

	async searchEvents(
		query: string,
		pageNumber = 0,
		pageSize = 6,
	): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send(
					{ cmd: 'event_search' },
					{ query, pageNumber, pageSize },
				),
			);
		} catch (error) {
			this.logger.error('User microservice error (search events):', error);
			return {
				status: 'error',
				message: 'Failed to search events',
				events: [],
				pageCount: 0,
				totalCount: 0,
			};
		}
	}

	async getEventById(id: string): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_get' }, { id }),
			);
		} catch (error) {
			this.logger.error('User microservice error (get event):', error);
			return { status: 'error', message: 'Failed to fetch event' };
		}
	}

	async createEvent(
		userId: string,
		login: string,
		input: CreateEventBody,
	): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_create' }, { userId, login, input }),
			);
		} catch (error) {
			this.logger.error('User microservice error (create event):', error);
			return { status: 'error', message: 'Failed to create event' };
		}
	}

	async updateEvent(
		eventId: string,
		login: string,
		patch: UpdateEventBody,
	): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_update' }, { eventId, login, patch }),
			);
		} catch (error) {
			this.logger.error('User microservice error (update event):', error);
			return { status: 'error', message: 'Failed to update event' };
		}
	}

	async deleteEvent(eventId: string, login: string): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_delete' }, { eventId, login }),
			);
		} catch (error) {
			this.logger.error('User microservice error (delete event):', error);
			return { status: 'error', message: 'Failed to delete event' };
		}
	}

	async joinEvent(eventId: string, login: string): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_join' }, { eventId, login }),
			);
		} catch (error) {
			this.logger.error('User microservice error (join event):', error);
			return { status: 'error', message: 'Failed to join event' };
		}
	}

	async leaveEvent(eventId: string, login: string): Promise<EventActionResult> {
		try {
			return await lastValueFrom<EventActionResult>(
				this.userClient.send({ cmd: 'event_leave' }, { eventId, login }),
			);
		} catch (error) {
			this.logger.error('User microservice error (leave event):', error);
			return { status: 'error', message: 'Failed to leave event' };
		}
	}
}
