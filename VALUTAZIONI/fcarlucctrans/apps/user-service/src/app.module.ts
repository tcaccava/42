import { Module } from '@nestjs/common';
import { ConfigModule } from '@nestjs/config';
import { AppController } from './app.controller';
import { AppService } from './app.service';
import path from 'path';
import { PrismaService } from './database/prisma.service';
import { UserProfileRepository } from './database/user-profile.repository';
import { ConnectionRepository } from './database/connection.repository';
import { ConnectionService } from './connection.service';
import { GdprService } from './gdpr/gdpr.service';
import { ApiKeyRepository } from './database/api-key.repository';
import { PublicResourceRepository } from './database/public-resource.repository';
import { PublicApiService } from './public-api/public-api.service';
import { EventRepository } from './database/event.repository';
import { EventService } from './event.service';

@Module({
  imports: [
		ConfigModule.forRoot({
			isGlobal: true,
			envFilePath: [
				path.join(__dirname, '../../../.env'),
				path.join(__dirname, '../../../apps/user-service/prisma/.env'),
			],
		}),
	],
  controllers: [AppController],
  providers: [
    AppService,
    ConnectionService,
    PrismaService,
    UserProfileRepository,
    ConnectionRepository,
    GdprService,
    ApiKeyRepository,
    PublicResourceRepository,
    PublicApiService,
    EventRepository,
    EventService,
  ],
})
export class AppModule {}
