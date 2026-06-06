import { Module } from '@nestjs/common';
import { ConfigModule } from '@nestjs/config';
import { AppService } from './app.service';
import { AppController } from './app.controller';
import { AuthModule } from './auth/auth.module';
import { ProjectModule } from './project/project.module';
import { UserModule } from './user/user.module';
import { GdprModule } from './gdpr/gdpr.module';
import { PublicApiModule } from './public-api/public-api.module';
import { EventsModule } from './events/events.module';
import path from 'path';
import { CommentsModule } from './comments/comments.module';
import { S3Service } from './s3.service';

@Module({
	imports: [
		ConfigModule.forRoot({
			isGlobal: true,
			envFilePath: path.join(__dirname, '../../../.env'),
		}),
		AuthModule,
		ProjectModule,
		CommentsModule,
		UserModule,
		GdprModule,
		PublicApiModule,
		EventsModule,
	],
	controllers: [AppController],
	providers: [AppService, S3Service],
})
export class AppModule {}
