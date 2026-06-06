import { Inject, Injectable, Logger } from '@nestjs/common';
import { ClientProxy } from '@nestjs/microservices';
import { lastValueFrom } from 'rxjs';

@Injectable()
export class PublicApiService {
	private readonly logger = new Logger(PublicApiService.name);

	constructor(@Inject('USER_SERVICE') private readonly userClient: ClientProxy) {}

	list(limit?: number, offset?: number) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'public_resource_list' }, { limit, offset }),
		);
	}

	get(id: string) {
		return lastValueFrom(this.userClient.send({ cmd: 'public_resource_get' }, { id }));
	}

	create(body: { title: string; content?: string; tags?: string[] }) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'public_resource_create' }, body),
		);
	}

	update(
		id: string,
		body: { title?: string; content?: string; tags?: string[] },
	) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'public_resource_update' }, { id, ...body }),
		);
	}

	remove(id: string) {
		return lastValueFrom(
			this.userClient.send({ cmd: 'public_resource_delete' }, { id }),
		);
	}
}
