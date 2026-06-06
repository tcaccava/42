import {
	Injectable,
	InternalServerErrorException,
	Logger,
	OnModuleInit,
} from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';
import {
	S3Client,
	PutObjectCommand,
	GetObjectCommand,
	DeleteObjectCommand,
	PutBucketLifecycleConfigurationCommand,
} from '@aws-sdk/client-s3';
import { v4 as uuidv4 } from 'uuid';

@Injectable()
export class S3Service implements OnModuleInit {
	private readonly logger = new Logger(S3Service.name);
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

	async onModuleInit() {
		// Docs:
		// https://docs.aws.amazon.com/AmazonS3/latest/API/API_PutBucketLifecycleConfiguration.html
		const command = new PutBucketLifecycleConfigurationCommand({
			Bucket: this.bucketName,
			LifecycleConfiguration: {
				Rules: [
					{
						ID: 'Expire GDPR Data',
						Status: 'Enabled',
						Filter: {
							Prefix: 'gdpr/',
						},
						Expiration: {
							Days: 1,
						},
					},
				],
			},
		});

		try {
			await this.client.send(command);
		} catch (error) {
			const message =
				error instanceof Error ? error.message : 'unknown S3 error';
			this.logger.warn(
				`S3 lifecycle setup skipped (${message}). GDPR file uploads need object storage running at ${this.serverAddress}.`,
			);
		}
	}

	public async uploadFile(userId: string, file: File): Promise<string> {
		try {
			const key = `gdpr/${userId}/${uuidv4()}/${file.name}`;
			const command = new PutObjectCommand({
				Bucket: this.bucketName,
				Key: key,
				Body: await file.bytes(),
				ContentType: file.type,
				ACL: 'public-read',

				Metadata: {
					originalName: file.name,
				},
			});

			await this.client.send(command);

			return await this.getFileUrl(key);
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
