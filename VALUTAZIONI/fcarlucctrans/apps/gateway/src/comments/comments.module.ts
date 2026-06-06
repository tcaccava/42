import { Module } from '@nestjs/common';
import { CommentsService } from './comments.service';
import { CommentsController } from './comments.controller';
import { ProxyGuard } from '../proxy.guard';
import { ConfigService } from '@nestjs/config';
import { JwtModule } from '@nestjs/jwt';

@Module({
	imports: [
		JwtModule.registerAsync({
			useFactory: () => ({}),
			inject: [],
		}),
	],
	controllers: [CommentsController],
	providers: [CommentsService, ProxyGuard, ConfigService],
	exports: [CommentsService, ProxyGuard],
})
export class CommentsModule {}