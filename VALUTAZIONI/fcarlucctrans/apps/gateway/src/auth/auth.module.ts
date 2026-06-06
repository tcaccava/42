import { Module } from '@nestjs/common';
import { JwtModule } from '@nestjs/jwt';
import { ConfigService } from '@nestjs/config';
import { ClientsModule, Transport } from '@nestjs/microservices';
import { AuthService } from './auth.service';
import { AuthController } from './auth.controller';
import { AuthGuard } from './auth.guard';

@Module({
	imports: [
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
		]),
		JwtModule.registerAsync({
			useFactory: () => ({}),
			inject: [],
		}),
	],
	controllers: [AuthController],
	providers: [AuthService, AuthGuard],
	exports: [AuthService, AuthGuard, JwtModule]
})
export class AuthModule {}
