import { NestFactory } from '@nestjs/core';
import { ConfigService } from '@nestjs/config';
import { MicroserviceOptions, Transport } from '@nestjs/microservices';
import { AppModule } from './app.module';

async function bootstrap() {
  // Create a Nest application to access DI (ConfigService)
  const app = await NestFactory.create(AppModule);
  const configService = app.get(ConfigService);

  // Read host/port from env via ConfigService (non-null assertions assume env is set)
  const host = configService.get('AUTH_SERVICE_HOST')!;
  const port = parseInt(configService.get('AUTH_SERVICE_PORT')!, 10);

  // Attach the TCP microservice using the configured host/port
  app.connectMicroservice<MicroserviceOptions>({
    transport: Transport.TCP,
    options: { host, port },
  });

  await app.startAllMicroservices();
  // No HTTP server needed here; the microservice is now listening
  // Optionally keep the app running without HTTP listen
  // await app.listen(0);
  // Log for visibility
  // eslint-disable-next-line no-console
  console.log(`Auth microservice listening on ${host}:${port}`);
}

bootstrap();