import {
	BadRequestException,
	Body,
	Controller,
	Delete,
	Get,
	Param,
	Patch,
	Post,
	Query,
	Req,
	UnauthorizedException,
	UseGuards,
} from '@nestjs/common';
import { AuthGuard } from '../auth/auth.guard';
import { EventsService, type CreateEventBody, type UpdateEventBody } from './events.service';

@Controller('api/events')
export class EventsController {
	constructor(private readonly eventsService: EventsService) {}

	private getActor(req: any) {
		const userId = req?.user?.sub;
		const login = req?.user?.login;
		if (!userId) throw new UnauthorizedException('Missing user id');
		if (!login) throw new UnauthorizedException('Missing user login');
		return { userId, login };
	}

	@UseGuards(AuthGuard)
	@Get()
	listEvents() {
		return this.eventsService.listEvents();
	}

	@UseGuards(AuthGuard)
	@Get('search')
	searchEvents(
		@Query('q') query?: string,
		@Query('pageNumber') pageNumber?: string,
		@Query('pageSize') pageSize?: string,
	) {
		const pn = Math.max(0, Number.parseInt(pageNumber ?? '0', 10) || 0);
		const ps = Math.min(50, Math.max(1, Number.parseInt(pageSize ?? '6', 10) || 6));
		return this.eventsService.searchEvents(query?.trim() ?? '', pn, ps);
	}

	@UseGuards(AuthGuard)
	@Get(':id')
	getEvent(@Param('id') id: string) {
		return this.eventsService.getEventById(id);
	}

	@UseGuards(AuthGuard)
	@Post()
	createEvent(@Req() req: any, @Body() body: CreateEventBody) {
		const { userId, login } = this.getActor(req);
		if (!body?.title?.trim()) throw new BadRequestException('Title is required');
		return this.eventsService.createEvent(userId, login, body);
	}

	@UseGuards(AuthGuard)
	@Patch(':id')
	updateEvent(@Req() req: any, @Param('id') id: string, @Body() body: UpdateEventBody) {
		const { login } = this.getActor(req);
		return this.eventsService.updateEvent(id, login, body);
	}

	@UseGuards(AuthGuard)
	@Delete(':id')
	deleteEvent(@Req() req: any, @Param('id') id: string) {
		const { login } = this.getActor(req);
		return this.eventsService.deleteEvent(id, login);
	}

	@UseGuards(AuthGuard)
	@Post(':id/join')
	joinEvent(@Req() req: any, @Param('id') id: string) {
		const { login } = this.getActor(req);
		return this.eventsService.joinEvent(id, login);
	}

	@UseGuards(AuthGuard)
	@Post(':id/leave')
	leaveEvent(@Req() req: any, @Param('id') id: string) {
		const { login } = this.getActor(req);
		return this.eventsService.leaveEvent(id, login);
	}
}
