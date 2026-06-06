import {
  BadRequestException,
  ForbiddenException,
  Injectable,
  NotFoundException,
} from '@nestjs/common';
import {
  EventRepository,
  type CreateEventData,
  type UpdateEventData,
} from './database/event.repository';

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

function toEventDto(
  event: {
    id: string;
    title: string;
    description: string;
    organizerLogin: string;
    organizerImage: string | null;
    campus: string;
    date: string;
    time: string;
    location: string;
    maxParticipants: number;
    tags: string[];
    coverImage: string | null;
    participants: { login: string }[];
  },
): EventDto {
  return {
    id: event.id,
    title: event.title,
    description: event.description,
    organizer: event.organizerLogin,
    organizerImage: event.organizerImage ?? '',
    campus: event.campus,
    date: event.date,
    time: event.time,
    location: event.location,
    participants: event.participants.map((p) => p.login),
    maxParticipants: event.maxParticipants,
    tags: event.tags,
    coverImage: event.coverImage ?? undefined,
  };
}

@Injectable()
export class EventService {
  constructor(private readonly eventRepository: EventRepository) {}

  async listEvents() {
    const events = await this.eventRepository.findAll();
    return { status: 'success' as const, events: events.map(toEventDto) };
  }

  async searchEvents(query: string, pageNumber = 0, pageSize = 6) {
    const [events, totalCount] = await this.eventRepository.search(
      query,
      pageNumber,
      pageSize,
    );
    const pageCount = totalCount === 0 ? 0 : Math.ceil(totalCount / pageSize);
    return {
      status: 'success' as const,
      events: events.map(toEventDto),
      pageCount,
      totalCount,
    };
  }

  async getEventById(id: string) {
    const event = await this.eventRepository.findById(id);
    if (!event) throw new NotFoundException('Event not found');
    return { status: 'success' as const, event: toEventDto(event) };
  }

  async createEvent(
    userId: string,
    login: string,
    input: Omit<CreateEventData, 'organizerLogin' | 'organizerUserId'>,
  ) {
    const event = await this.eventRepository.create({
      ...input,
      organizerLogin: login,
      organizerUserId: userId,
    });
    return { status: 'success' as const, event: toEventDto(event) };
  }

  async updateEvent(eventId: string, login: string, patch: UpdateEventData) {
    const event = await this.eventRepository.findById(eventId);
    if (!event) throw new NotFoundException('Event not found');
    if (event.organizerLogin !== login) {
      throw new ForbiddenException('Only the organizer can edit this event.');
    }
    if (
      patch.maxParticipants != null &&
      patch.maxParticipants < event.participants.length
    ) {
      throw new BadRequestException(
        `Max participants cannot be below current registrations (${event.participants.length}).`,
      );
    }

    const updated = await this.eventRepository.update(eventId, patch);
    return { status: 'success' as const, event: toEventDto(updated) };
  }

  async deleteEvent(eventId: string, login: string) {
    const event = await this.eventRepository.findById(eventId);
    if (!event) throw new NotFoundException('Event not found');
    if (event.organizerLogin !== login) {
      throw new ForbiddenException('Only the organizer can delete this event.');
    }

    await this.eventRepository.delete(eventId);
    return { status: 'success' as const };
  }

  async joinEvent(eventId: string, login: string) {
    const event = await this.eventRepository.findById(eventId);
    if (!event) throw new NotFoundException('Event not found');

    if (event.participants.some((p) => p.login === login)) {
      return { status: 'success' as const, event: toEventDto(event) };
    }
    if (event.participants.length >= event.maxParticipants) {
      throw new BadRequestException('This event is full.');
    }

    await this.eventRepository.addParticipant(eventId, login);
    const updated = await this.eventRepository.findById(eventId);
    return { status: 'success' as const, event: toEventDto(updated!) };
  }

  async leaveEvent(eventId: string, login: string) {
    const event = await this.eventRepository.findById(eventId);
    if (!event) throw new NotFoundException('Event not found');

    if (!event.participants.some((p) => p.login === login)) {
      return { status: 'success' as const, event: toEventDto(event) };
    }

    await this.eventRepository.removeParticipant(eventId, login);
    const updated = await this.eventRepository.findById(eventId);
    return { status: 'success' as const, event: toEventDto(updated!) };
  }
}
