import { NestFactory } from '@nestjs/core';
import { AppModule } from './app.module';
import { ConfigService } from '@nestjs/config';
import cookieParser from 'cookie-parser';
import { json, urlencoded } from 'express';
import { waitForTcp } from './bootstrap/wait-for-tcp';

/** Profile covers and similar fields may include base64 data URLs (see web bodySizeLimit). */
const JSON_BODY_LIMIT = '5mb';

async function bootstrap() {
	const app = await NestFactory.create(AppModule, { bodyParser: false });
	const configService = app.get(ConfigService);

	const wait = (hostKey: string, portKey: string, label: string) => {
		const host = configService.get<string>(hostKey) ?? '127.0.0.1';
		const port = parseInt(configService.get<string>(portKey) ?? '0', 10);
		return waitForTcp(host, port, { label });
	};

	await wait('AUTH_SERVICE_HOST', 'AUTH_SERVICE_PORT', 'auth-service');
	await wait('USER_SERVICE_HOST', 'USER_SERVICE_PORT', 'user-service');
	await wait('PROJECT_SERVICE_HOST', 'PROJECT_SERVICE_PORT', 'project-service');
	await wait('COMMENTS_SERVICE_HOST', 'COMMENTS_SERVICE_PORT', 'comments-service');

	app.enableCors({
		origin: configService.get('WEBAPP_URL'),
		credentials: true,
	});

	// Behind nginx TLS termination in Docker (make up)
	app.getHttpAdapter().getInstance().set('trust proxy', 1);

	app.use(json({ limit: JSON_BODY_LIMIT }));
	app.use(urlencoded({ extended: true, limit: JSON_BODY_LIMIT }));
	app.use(cookieParser());

	const host = configService.get('GATEWAY_HOST')!;
	const port = parseInt(configService.get('GATEWAY_PORT')!, 10);
	await app.listen(port, host);
}
bootstrap();
