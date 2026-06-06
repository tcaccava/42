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
import { UserService } from './user.service';
import type { EditableProfilePatch } from './user.service';

@Controller('api/user')
export class UserController {
	constructor(private readonly userService: UserService) {}

	private getActor(req: any) {
		const userId = req?.user?.sub;
		const login = req?.user?.login;
		if (!userId) throw new UnauthorizedException('Missing user id');
		if (!login) throw new UnauthorizedException('Missing user login');
		return { userId, login };
	}

	@UseGuards(AuthGuard)
	@Get('profile')
	async getMyProfile(@Req() req: any) {
		const { userId, login } = this.getActor(req);
		return this.userService.getProfileByUserId(userId, login);
	}

	@UseGuards(AuthGuard)
	@Patch('profile')
	async updateMyProfile(@Req() req: any, @Body() body: EditableProfilePatch) {
		const { userId, login } = this.getActor(req);
		if (!body || typeof body !== 'object') throw new BadRequestException('Invalid payload');
		return this.userService.upsertProfileByUserId(userId, login, body);
	}

	@UseGuards(AuthGuard)
	@Get('directory')
	async listDirectory(
		@Req() req: any,
		@Query('pageNumber') pageNumber?: string,
		@Query('pageSize') pageSize?: string,
	) {
		const { userId } = this.getActor(req);
		const pn = Math.max(0, Number.parseInt(pageNumber ?? '0', 10) || 0);
		const ps = Math.min(50, Math.max(1, Number.parseInt(pageSize ?? '6', 10) || 6));
		return this.userService.listDirectory(userId, pn, ps);
	}

	@UseGuards(AuthGuard)
	@Get('search')
	async searchUsers(
		@Req() req: any,
		@Query('q') query: string,
		@Query('pageNumber') pageNumber?: string,
		@Query('pageSize') pageSize?: string,
	) {
		const { userId } = this.getActor(req);
		if (!query?.trim()) {
			return { status: 'success', users: [], pageCount: 0, totalCount: 0 };
		}
		const pn = Math.max(0, Number.parseInt(pageNumber ?? '0', 10) || 0);
		const ps = Math.min(50, Math.max(1, Number.parseInt(pageSize ?? '6', 10) || 6));
		return this.userService.searchUsers(query.trim(), userId, pn, ps);
	}

	@UseGuards(AuthGuard)
	@Get('by-login/:login')
	async getProfileByLogin(@Req() req: any, @Param('login') login: string) {
		const { userId, login: viewerLogin } = this.getActor(req);
		if (!login?.trim()) throw new BadRequestException('Login is required');
		return this.userService.getPublicProfileByLogin(login.trim(), userId, viewerLogin);
	}

	@UseGuards(AuthGuard)
	@Get('connections')
	async listConnections(@Req() req: any, @Query('status') status?: 'PENDING' | 'ACCEPTED') {
		const { userId } = this.getActor(req);
		return this.userService.listConnections(userId, status);
	}

	@UseGuards(AuthGuard)
	@Post('connections')
	async sendConnection(@Req() req: any, @Body() body: { targetUserId?: string }) {
		const { userId } = this.getActor(req);
		if (!body?.targetUserId) throw new BadRequestException('targetUserId is required');
		return this.userService.sendConnection(userId, body.targetUserId);
	}

	@UseGuards(AuthGuard)
	@Post('connections/:id/accept')
	async acceptConnection(@Req() req: any, @Param('id') connectionId: string) {
		const { userId } = this.getActor(req);
		return this.userService.acceptConnection(userId, connectionId);
	}

	@UseGuards(AuthGuard)
	@Post('connections/:id/reject')
	async rejectConnection(@Req() req: any, @Param('id') connectionId: string) {
		const { userId } = this.getActor(req);
		return this.userService.rejectConnection(userId, connectionId);
	}

	@UseGuards(AuthGuard)
	@Delete('connections/:id')
	async cancelOrRemoveConnection(@Req() req: any, @Param('id') connectionId: string) {
		const { userId } = this.getActor(req);
		const list = await this.userService.listConnections(userId);
		const connection = (list.connections ?? []).find((c) => c.id === connectionId);
		if (!connection) throw new BadRequestException('Connection not found');

		if (connection.status === 'PENDING' && connection.direction === 'outgoing') {
			return this.userService.cancelConnection(userId, connectionId);
		}
		if (connection.status === 'ACCEPTED') {
			return this.userService.removeConnection(userId, connectionId);
		}
		if (connection.status === 'PENDING' && connection.direction === 'incoming') {
			return this.userService.rejectConnection(userId, connectionId);
		}

		throw new BadRequestException('Cannot modify this connection');
	}
}
