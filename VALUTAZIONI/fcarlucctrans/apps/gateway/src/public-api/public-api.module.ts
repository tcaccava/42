import { Module } from '@nestjs/common';
import { ClientsModule, Transport } from '@nestjs/microservices';
import { ConfigService } from '@nestjs/config';
import { PublicApiController } from './public-api.controller';
import { PublicApiService } from './public-api.service';
import { ApiKeyGuard } from './api-key.guard';

@Module({
	imports: [
		ClientsModule.registerAsync([
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
	controllers: [PublicApiController],
	providers: [PublicApiService, ApiKeyGuard],
})
export class PublicApiModule {}
