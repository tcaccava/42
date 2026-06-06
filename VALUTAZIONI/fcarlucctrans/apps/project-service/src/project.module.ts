import { Module } from '@nestjs/common';
import { AppController } from './project.controller';
import { AppService } from './project.service';
import { ConfigModule } from '@nestjs/config';
import { S3Service } from './s3.service';
import { MulterModule } from '@nestjs/platform-express';
import path from 'path';

@Module({
  imports: [ConfigModule.forRoot({
    isGlobal: true,
    envFilePath: path.join(__dirname, '../../../../.env'),
  }), MulterModule.register({
    limits: { fileSize: 10 * 1024 * 1024 }, // 10MB limit
  })],
  controllers: [AppController],
  providers: [AppService, S3Service],
})
export class AppModule {}
