import { Module } from '@nestjs/common';
import { ConfigModule } from '@nestjs/config';
import path from 'path';
import { CommentController } from './comment.controller';
import { CommentService } from './comment.service';

@Module({
	imports: [
		ConfigModule.forRoot({
			isGlobal: true,
			envFilePath: path.join(__dirname, '../../../../.env'),
		}),
	],
	controllers: [CommentController],
	providers: [CommentService],
})
export class CommentModule {}
