import { Module } from '@nestjs/common';
import { ClientsModule, Transport } from '@nestjs/microservices';
import { ConfigService } from '@nestjs/config';
import { AuthModule } from '../auth/auth.module';
import { GdprController } from './gdpr.controller';
import { GdprService } from './gdpr.service';

@Module({
	imports: [
		AuthModule,
		ClientsModule.registerAsync([
			{
				name: 'AUTH_SERVICE',
				useFactory: (configService: ConfigService) => ({
					transport: Transport.TCP,
					options: {
						host: configService.get('AUTH_SERVICE_HOST'),
						port: configService.get('AUTH_SERVICE_PORT'),
					},
				}),
				inject: [ConfigService],
			},
			{
				name: 'USER_SERVICE',
				useFactory: (configService: ConfigService) => ({
					transport: Transport.TCP,
					options: {
						host: configService.get('USER_SERVICE_HOST'),
						port: configService.get('USER_SERVICE_PORT'),
					},
				}),
				inject: [ConfigService],
			},
		]),
	],
	controllers: [GdprController],
	providers: [GdprService],
})
export class GdprModule {}
