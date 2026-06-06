import { Module } from '@nestjs/common';
import { ProjectService } from './project.service';
import { ProjectController } from './project.controller';
import { ProxyGuard } from '../proxy.guard';
import { JwtModule } from '@nestjs/jwt';
import { ConfigService } from '@nestjs/config';

@Module({
	imports: [
		JwtModule.registerAsync({
			useFactory: () => ({}),
			inject: [],
		}),
	],
	controllers: [ProjectController],
	providers: [ProjectService, ProxyGuard, ConfigService],
	exports: [ProjectService, ProxyGuard],
})
export class ProjectModule {}
