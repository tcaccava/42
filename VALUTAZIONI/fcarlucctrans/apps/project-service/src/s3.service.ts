import { Injectable, InternalServerErrorException } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';
import {
	S3Client,
	PutObjectCommand,
	GetObjectCommand,
	DeleteObjectCommand,
} from '@aws-sdk/client-s3';
import { v4 as uuidv4 } from 'uuid';

@Injectable()
export class S3Service {
	private client: S3Client;
	private bucketName: string;
	private serverAddress: string;

	public constructor(private readonly configService: ConfigService) {
		const notFound = (key: string): never => {
			throw new Error(`${key} not found in environment variables/config`);
		};

		this.bucketName =
			this.configService.get('S3_BUCKET') ?? notFound('S3_BUCKET');

		this.client = new S3Client({
			endpoint:
				this.configService.get('S3_ENDPOINT') ?? notFound('S3_ENDPOINT'),
			region: this.configService.get('S3_REGION') ?? notFound('S3_REGION'),
			credentials: {
				accessKeyId: this.configService.get('S3_KEY') ?? notFound('S3_KEY'),
				secretAccessKey:
					this.configService.get('S3_SECRET') ?? notFound('S3_SECRET'),
			},
			forcePathStyle: true,
		});

		this.serverAddress =
			this.configService.get('S3_ENDPOINT') ?? notFound('S3_ENDPOINT');
	}

	public async uploadFile(userId: string, file: Express.Multer.File): Promise<string> {
		try {
			const key = `uploads/${userId}/${uuidv4()}/${file.originalname}`;
			const command = new PutObjectCommand({
				Bucket: this.bucketName,
				Key: key,
				Body: file.buffer,
				ContentType: file.mimetype,
				ACL: 'public-read',

				Metadata: {
					originalName: file.originalname,
				},
			});

			await this.client.send(command);

			return key;
		} catch (error) {
			throw new InternalServerErrorException(error);
		}
	}

	public async getFileUrl(key: string) {
		try {
			const publicBase = this.configService
				.get<string>('S3_PUBLIC_BASE_URL')
				?.replace(/\/$/, '');
			if (publicBase) {
				return `${publicBase}/${this.bucketName}/${key}`;
			}

			const command = new GetObjectCommand({
				Bucket: this.bucketName,
				Key: key,
			});

			const url = await getSignedUrl(this.client, command, {
				expiresIn: 60 * 60 * 24, // 24 hours
			});

			return url;
		} catch (error) {
			throw new InternalServerErrorException(error);
		}
	}

	public async deleteFile(key: string) {
		try {
			const command = new DeleteObjectCommand({
				Bucket: this.bucketName,
				Key: key,
			});

			const result = await this.client.send(command);
			return result.DeleteMarker;
		} catch (error) {
			throw new InternalServerErrorException(error);
		}
	}

	// public async newFileUploadUrl(
	// 	userId: string,
	// 	filename: string,
	// 	mimetype: string,
	// 	fileSize: number,
	// ): Promise<{key: string, url: string}> {
	// 	try {
	// 		const key = `uploads/${userId}/${uuidv4()}/${filename}`;
	// 		const command = new PutObjectCommand({
	// 			Bucket: this.bucketName,
	// 			Key: key,
	// 			ContentType: mimetype,
	// 			ContentLength: fileSize,
	// 		});

	// 		const url = await getSignedUrl(this.client, command, {
	// 			expiresIn: 60 * 15, // 15 minutes
	// 		});

	// 		return {
	// 			key: key,
	// 			url: url,
	// 		};
	// 	}
	// 	catch (error) {
	// 		throw new InternalServerErrorException(error);
	// 	}
	// }
}
