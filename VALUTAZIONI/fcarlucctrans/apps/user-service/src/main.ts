import { NestFactory } from '@nestjs/core';
import { ConfigService } from '@nestjs/config';
import { MicroserviceOptions, Transport } from '@nestjs/microservices';
import { AppModule } from './app.module';

async function bootstrap() {
  const app = await NestFactory.create(AppModule);
  const configService = app.get(ConfigService);

  const host = configService.get('USER_SERVICE_HOST')!;
  const port = parseInt(configService.get('USER_SERVICE_PORT')!, 10);

  app.connectMicroservice<MicroserviceOptions>({
    transport: Transport.TCP,
    options: { host, port },
  });

  await app.startAllMicroservices();
  // eslint-disable-next-line no-console
  console.log(`User microservice listening on ${host}:${port}`);
}

bootstrap();
