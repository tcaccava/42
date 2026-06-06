import { NestFactory } from '@nestjs/core';
import { CommentModule } from './comment.module';

async function bootstrap() {
	const app = await NestFactory.create(CommentModule);
	await app.listen(process.env.PORT ?? 4004);
}
void bootstrap();
