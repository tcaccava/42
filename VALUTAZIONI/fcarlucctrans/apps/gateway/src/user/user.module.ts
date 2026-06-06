import { Module } from '@nestjs/common';
import { ClientsModule, Transport } from '@nestjs/microservices';
import { ConfigService } from '@nestjs/config';
import { UserController } from './user.controller';
import { UserService } from './user.service';
import { AuthModule } from '../auth/auth.module';

@Module({
	imports: [
		AuthModule,
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
		]),
	],
	controllers: [UserController],
	providers: [UserService],
	exports: [UserService],
})
export class UserModule {}
