import { Body, Controller, Delete, Get, InternalServerErrorException, NotImplementedException, ParseIntPipe, Post, Put, Query, UnauthorizedException } from '@nestjs/common';
import { CommentService } from './comment.service';
import * as dto from './dto';
// import { Comment, User } from '../prisma/client';
import { Comment, User } from '@prisma/client';

@Controller('comments')
export class CommentController {
	constructor(private readonly appService: CommentService) {}

	async authorize(commentId: number, userId: string) {
		const comment = await this.appService.getComment(commentId);
		if (!comment) {
			throw new InternalServerErrorException(`Comment with id ${commentId} was not found.`);
		}
		if (comment.userId != userId) {
			throw new UnauthorizedException(`Mismatch between provided and comment user ids.`);
		}
	}

	@Get('comment')
	async getComment(@Query('id', ParseIntPipe) id: number): Promise<Comment> {
		const comment = await this.appService.getComment(id);
		if (comment == null) {
			throw new InternalServerErrorException('Specified comment was not found.');
		}
		return comment;
	}

	@Post('secure/comment')
	async uploadComment(
		@Body() uploadDto: any,
		@Query('userId') userId: string,
	): Promise<Comment> {
		if ('projectId' in uploadDto) {
			return this.appService.createComment(userId, uploadDto as dto.CreateCommentDTO);
		}
		else if ('id' in uploadDto) {
			await this.authorize(uploadDto.id, userId);
			return this.appService.updateComment(uploadDto as dto.UpdateCommentDTO);
		}
		else {
			throw new InternalServerErrorException('Invalid DTO.');
		}
	}

	@Delete('secure/comment')
	async deleteComment(
		@Query('id', ParseIntPipe) id: number,
		@Query('userId') userId: string,
	): Promise<boolean> {
		await this.authorize(id, userId);
		return this.appService.deleteComment(id);
	}

	@Get('project')
	async getComments(@Query('projectId', ParseIntPipe) projectId: number): Promise<Array<Comment>> {
		return this.appService.getComments(projectId);
	}

	@Post('secure/user')
	async upsertUser(
		@Body() userUpdateDto: dto.UploadUserDTO,
		@Query('userId') userId: string,
	): Promise<User> {
		if (userUpdateDto.id != userId) {
			throw new UnauthorizedException('User id mismatch in DTO and query.');
		}
		return this.appService.upsertUser(userUpdateDto);
	}

	@Delete('secure/user')
	async deleteUser(
		@Query('userId') userId: string,
	): Promise<boolean> {
		return this.appService.deleteUser(userId);
	}

	@Put('secure/rename-user')
	async renameUser(
		@Query('from') fromLogin: string,
		@Query('to') toLogin: string,
	): Promise<{ updated: number }> {
		if (!fromLogin || !toLogin) {
			throw new InternalServerErrorException('from and to login are required');
		}
		return this.appService.renameUser(fromLogin, toLogin);
	}

	@Delete('secure/project')
	async deleteProject(
		@Query('id', ParseIntPipe) projectId: number,
		@Query('userId') userId: string,
	): Promise<boolean> {
		return this.appService.deleteProject(projectId);
	}

	@Get('secure/gdpr')
	async getGDPR(
		@Query('userId') userId: string,
	): Promise<Comment[]> {
		return await this.appService.getGDPRData(userId);
	}
}
