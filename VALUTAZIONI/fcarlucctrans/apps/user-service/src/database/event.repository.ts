import { Injectable } from '@nestjs/common';
import { PrismaService } from './prisma.service';

const eventInclude = { participants: true } as const;

export type CreateEventData = {
  title: string;
  description: string;
  organizerLogin: string;
  organizerUserId: string;
  organizerImage?: string;
  campus: string;
  date: string;
  time: string;
  location: string;
  maxParticipants: number;
  tags: string[];
  coverImage?: string;
};

export type UpdateEventData = {
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
export class EventRepository {
  constructor(private readonly prisma: PrismaService) {}

  findAll() {
    return this.prisma.event.findMany({
      include: eventInclude,
      orderBy: [{ date: 'asc' }, { time: 'asc' }],
    });
  }

  search(query: string | undefined, pageNumber = 0, pageSize = 6) {
    const trimmed = query?.trim();
    const where =
      trimmed && trimmed.length >= 2
        ? {
            OR: [
              { title: { contains: trimmed, mode: 'insensitive' as const } },
              { description: { contains: trimmed, mode: 'insensitive' as const } },
              { location: { contains: trimmed, mode: 'insensitive' as const } },
              { campus: { contains: trimmed, mode: 'insensitive' as const } },
              { organizerLogin: { contains: trimmed, mode: 'insensitive' as const } },
            ],
          }
        : {};

    return Promise.all([
      this.prisma.event.findMany({
        where,
        include: eventInclude,
        orderBy: [{ date: 'asc' }, { time: 'asc' }],
        skip: pageNumber * pageSize,
        take: pageSize,
      }),
      this.prisma.event.count({ where }),
    ]);
  }

  findById(id: string) {
    return this.prisma.event.findUnique({
      where: { id },
      include: eventInclude,
    });
  }

  create(data: CreateEventData) {
    return this.prisma.event.create({
      data: {
        ...data,
        participants: {
          create: { login: data.organizerLogin },
        },
      },
      include: eventInclude,
    });
  }

  update(id: string, data: UpdateEventData) {
    return this.prisma.event.update({
      where: { id },
      data,
      include: eventInclude,
    });
  }

  delete(id: string) {
    return this.prisma.event.delete({ where: { id } });
  }

  addParticipant(eventId: string, login: string) {
    return this.prisma.eventParticipant.create({
      data: { eventId, login },
    });
  }

  removeParticipant(eventId: string, login: string) {
    return this.prisma.eventParticipant.deleteMany({
      where: { eventId, login },
    });
  }
}
