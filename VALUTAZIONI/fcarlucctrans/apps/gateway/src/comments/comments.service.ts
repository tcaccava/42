import { Injectable } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import axios from 'axios';
import { buildProxyAxiosConfig } from '../proxy-forward';

@Injectable()
export class CommentsService {
	private serviceUrl: string;

	constructor(private readonly configService: ConfigService) {
		const serviceHost: string = configService.getOrThrow(
			'COMMENTS_SERVICE_HOST',
		);
		const servicePort: string = configService.getOrThrow(
			'COMMENTS_SERVICE_PORT',
		);

		this.serviceUrl = `http://${serviceHost}:${servicePort}`;
	}

	async forward(
		path: string,
		method: string,
		body: any,
		headers: any,
	): Promise<object> {
		const config = buildProxyAxiosConfig(
			this.serviceUrl,
			path,
			method,
			body,
			headers,
		);

		const response = await axios(config);
		// eslint-disable-next-line @typescript-eslint/no-unsafe-return
		return response.data;
	}
}