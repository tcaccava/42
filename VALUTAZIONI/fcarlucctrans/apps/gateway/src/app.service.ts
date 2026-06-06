import { Injectable, NotImplementedException } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { BlobWriter } from '@zip.js/zip.js';
import axios, { AxiosRequestConfig } from 'axios';

@Injectable()
export class AppService {
	private projectServiceUrl: string;
	private commentServiceUrl: string;

	constructor(private readonly configService: ConfigService) {
		const projectServiceHost: string = configService.getOrThrow(
			'PROJECT_SERVICE_HOST',
		);
		const projectServicePort: string = configService.getOrThrow(
			'PROJECT_SERVICE_PORT',
		);

		this.projectServiceUrl = `http://${projectServiceHost}:${projectServicePort}`;

		const commentServiceHost: string = configService.getOrThrow(
			'COMMENTS_SERVICE_HOST',
		);
		const commentServicePort: string = configService.getOrThrow(
			'COMMENTS_SERVICE_PORT',
		);

		this.commentServiceUrl = `http://${commentServiceHost}:${commentServicePort}`;
	}

	async requestProjectGDPRData(userId: string): Promise<any> {
		const response = await axios({
			method: 'GET',
			url: this.projectServiceUrl + `/projects/secure/gdpr?userId=${userId}`,
		});

		const commentResponse = await axios({
			method: 'GET',
			url: this.commentServiceUrl + `/comments/secure/gdpr?userId=${userId}`,
		});

		return {
			comments: commentResponse.data,
			...response.data
		};
	}

	async downloadFile(url: string): Promise<File> {
		const response = await axios({
			method: 'GET',
			url: url,
			responseType: 'arraybuffer',
		});

		// Okay, I don't know how to get the name of the incoming file.
		// This header supposedly works for now, but in the future if this ever breaks,
		// I'll have to implement a whole system for randomly generating filenames, as well as
		// picking the appropriate file extensions based on those names.
		const fileName = response.headers['x-amz-meta-originalname']; // or content disposition
		const fileType = response.headers['content-type'];

		const fileBuffer = Buffer.from(response.data);

		const file = new File([fileBuffer], fileName,{
			type: fileType,
		});

		return file;
	}

	async deleteGDPRData(userId: string): Promise<any> {
		const response = await axios({
			method: 'DELETE',
			url: this.projectServiceUrl + `/projects/secure/fromUser?userId=${userId}`,
		});

		const commentResponse = await axios({
			method: 'DELETE',
			url: this.commentServiceUrl + `/comments/secure/user?userId=${userId}`,
		});

		return response.status == 200 && commentResponse.status == 200;
	}
}

