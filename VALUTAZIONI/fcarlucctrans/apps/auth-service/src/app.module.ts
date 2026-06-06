import { Module } from '@nestjs/common';
import { JwtModule } from '@nestjs/jwt';
import { HttpModule } from '@nestjs/axios';
import { ConfigModule, ConfigService } from '@nestjs/config';
import { AppService } from './app.service';
import { AppController } from './app.controller';
import { PrismaService } from './database/prisma.service';
import { UserRepository } from './database/user.repository';
import { SessionRepository } from './database/session.repository';
import { RefreshSessionRepository } from './database/refresh-session.repository';
import { DataRequestRepository } from './database/data-request.repository';
import { MailerService } from './email/mailer.service';
import { GdprService } from './gdpr/gdpr.service';
import { getAccessTokenExpiresInSec } from './auth/token-ttl';
import path from 'path';
import { readFileSync } from 'fs';

@Module({
	imports: [
		ConfigModule.forRoot({
			isGlobal: true,
			envFilePath: [
				path.join(__dirname, '../../../.env'),
				path.join(__dirname, '../../../apps/auth-service/prisma/.env'),
			],
		}),
		JwtModule.registerAsync({
			imports: [ConfigModule],
			useFactory: async (configService: ConfigService) => {
				const privateKeyPath = configService.get('JWT_PRIVATE_KEY_PATH');
				const kid = configService.get('JWT_KID');
				const privateKeyInline = configService.get('JWT_PRIVATE_KEY');
				let privateKey: string | undefined;

				if (privateKeyInline && privateKeyInline.includes('-----BEGIN')) {
					privateKey = privateKeyInline;
				} else if (privateKeyPath) {
					const rootDir = path.join(__dirname, '../../../');
					const resolvedPath = path.isAbsolute(privateKeyPath)
						? privateKeyPath
						: path.join(rootDir, privateKeyPath);
					privateKey = readFileSync(resolvedPath, 'utf8');
				}

				if (!privateKey) {
					throw new Error('Missing JWT private key (JWT_PRIVATE_KEY_PATH or JWT_PRIVATE_KEY)');
				}

				return {
					privateKey,
					signOptions: {
						algorithm: 'RS256',
						expiresIn: getAccessTokenExpiresInSec(configService),
						keyid: kid,
					},
				};
			},
			inject: [ConfigService],
		}),
		HttpModule
	],
	controllers: [AppController],
	providers: [
		AppService,
		PrismaService,
		UserRepository,
		SessionRepository,
		RefreshSessionRepository,
		DataRequestRepository,
		MailerService,
		GdprService,
	],
})
export class AppModule {}
