import { Inject, Injectable, Logger } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { ClientProxy } from '@nestjs/microservices';
import axios from 'axios';
import { lastValueFrom } from 'rxjs';

type Result<T = Record<string, unknown>> = T & {
	status: 'success' | 'error';
	message?: string;
};

/** Auth JWT uses `sub` (uuid). Project/comments services key data by 42 login. */
export type GdprUserScope = {
	uuid: string;
	login: string;
};

@Injectable()
export class GdprService {
	private readonly logger = new Logger(GdprService.name);

	constructor(
		@Inject('AUTH_SERVICE') private readonly authClient: ClientProxy,
		@Inject('USER_SERVICE') private readonly userClient: ClientProxy,
		private readonly config: ConfigService,
	) {}

	private projectServiceUrl(): string {
		const host = this.config.getOrThrow<string>('PROJECT_SERVICE_HOST');
		const port = this.config.getOrThrow<string>('PROJECT_SERVICE_PORT');
		return `http://${host}:${port}`;
	}

	private commentsServiceUrl(): string {
		const host = this.config.getOrThrow<string>('COMMENTS_SERVICE_HOST');
		const port = this.config.getOrThrow<string>('COMMENTS_SERVICE_PORT');
		return `http://${host}:${port}`;
	}

	private async fetchProjectExport(login: string): Promise<unknown | null> {
		try {
			const res = await axios.get(
				`${this.projectServiceUrl()}/projects/secure/gdpr`,
				{ params: { userId: login }, timeout: 15000 },
			);
			return res.data;
		} catch (err) {
			this.logger.warn(`Project export failed for ${login}`, err);
			return null;
		}
	}

	private async fetchCommentsExport(login: string): Promise<unknown | null> {
		try {
			const res = await axios.get(
				`${this.commentsServiceUrl()}/comments/secure/gdpr`,
				{ params: { userId: login }, timeout: 15000 },
			);
			return res.data;
		} catch (err) {
			this.logger.warn(`Comments export failed for ${login}`, err);
			return null;
		}
	}

	private async purgeProjectData(login: string): Promise<void> {
		try {
			await axios.delete(
				`${this.projectServiceUrl()}/projects/secure/fromUser`,
				{ params: { userId: login }, timeout: 30000 },
			);
		} catch (err) {
			this.logger.warn(`Project delete failed for ${login}`, err);
		}
	}

	private async purgeCommentsData(login: string): Promise<void> {
		try {
			await axios.delete(
				`${this.commentsServiceUrl()}/comments/secure/user`,
				{ params: { userId: login }, timeout: 15000 },
			);
		} catch (err) {
			this.logger.warn(`Comments delete failed for ${login}`, err);
		}
	}

	private async reassignProjectOwner(fromLogin: string, toLogin: string): Promise<void> {
		try {
			await axios.put(
				`${this.projectServiceUrl()}/projects/secure/reassignUser`,
				undefined,
				{ params: { from: fromLogin, to: toLogin }, timeout: 15000 },
			);
		} catch (err) {
			this.logger.warn(
				`Project reassign failed from ${fromLogin} to ${toLogin}`,
				err,
			);
		}
	}

	private async renameCommentsUser(fromLogin: string, toLogin: string): Promise<void> {
		try {
			await axios.put(
				`${this.commentsServiceUrl()}/comments/secure/rename-user`,
				undefined,
				{ params: { from: fromLogin, to: toLogin }, timeout: 15000 },
			);
		} catch (err) {
			this.logger.warn(
				`Comments rename failed from ${fromLogin} to ${toLogin}`,
				err,
			);
		}
	}

	/** Aggregate everything stored across services for a user. */
	async exportAll(scope: GdprUserScope): Promise<Result<{ data?: unknown }>> {
		try {
			const warnings: string[] = [];
			const [authResult, userResult, projectData, commentsData] =
				await Promise.all([
					lastValueFrom<Result<{ data?: unknown }>>(
						this.authClient.send({ cmd: 'gdpr_export_auth' }, { userId: scope.uuid }),
					),
					lastValueFrom<Result<{ data?: unknown }>>(
						this.userClient.send({ cmd: 'gdpr_export_user' }, { userId: scope.uuid }),
					),
					this.fetchProjectExport(scope.login),
					this.fetchCommentsExport(scope.login),
				]);

			if (authResult.status !== 'success') {
				return { status: 'error', message: authResult.message ?? 'Auth export failed' };
			}
			if (userResult.status !== 'success') {
				warnings.push('userService: export failed');
			}
			if (projectData === null) {
				warnings.push('projectService: export failed');
			}
			if (commentsData === null) {
				warnings.push('commentsService: export failed');
			}

			return {
				status: 'success',
				data: {
					exportedAt: new Date().toISOString(),
					schemaVersion: 3,
					user: { uuid: scope.uuid, login: scope.login },
					warnings,
					authService: authResult.data,
					userService: userResult.status === 'success' ? userResult.data : null,
					projectService: projectData,
					commentsService: commentsData,
				},
			};
		} catch (err) {
			this.logger.error('Error exporting all data', err);
			return { status: 'error', message: 'Failed to export data' };
		}
	}

	async requestDeletion(uuid: string): Promise<Result<{ delivered?: boolean }>> {
		try {
			return await lastValueFrom(
				this.authClient.send({ cmd: 'gdpr_request_deletion' }, { userId: uuid }),
			);
		} catch (err) {
			this.logger.error('Error requesting deletion', err);
			return { status: 'error', message: 'Failed to request deletion' };
		}
	}

	async confirmDeletion(token: string): Promise<Result<{ login?: string }>> {
		try {
			const authResult = await lastValueFrom<
				Result<{ userId?: string; login?: string; previousLogin?: string }>
			>(this.authClient.send({ cmd: 'gdpr_confirm_deletion' }, { token }));

			if (authResult.status !== 'success' || !authResult.userId || !authResult.login) {
				return authResult;
			}

			const login = authResult.login;

			try {
				await lastValueFrom(
					this.userClient.send({ cmd: 'gdpr_delete_user' }, { userId: authResult.userId }),
				);
			} catch (err) {
				this.logger.warn(`User-service delete failed for ${authResult.userId}`, err);
			}

			await Promise.all([
				this.purgeProjectData(login),
				this.purgeCommentsData(login),
			]);

			return { status: 'success', login };
		} catch (err) {
			this.logger.error('Error confirming deletion', err);
			return { status: 'error', message: 'Failed to confirm deletion' };
		}
	}

	async requestAnonymization(uuid: string): Promise<Result<{ delivered?: boolean }>> {
		try {
			return await lastValueFrom(
				this.authClient.send({ cmd: 'gdpr_request_anonymization' }, { userId: uuid }),
			);
		} catch (err) {
			this.logger.error('Error requesting anonymization', err);
			return { status: 'error', message: 'Failed to request anonymization' };
		}
	}

	async confirmAnonymization(token: string): Promise<Result<{ login?: string }>> {
		try {
			const authResult = await lastValueFrom<
				Result<{ userId?: string; login?: string; previousLogin?: string }>
			>(this.authClient.send({ cmd: 'gdpr_confirm_anonymization' }, { token }));

			if (
				authResult.status !== 'success' ||
				!authResult.userId ||
				!authResult.login ||
				!authResult.previousLogin
			) {
				return authResult;
			}

			try {
				await lastValueFrom(
					this.userClient.send(
						{ cmd: 'gdpr_anonymize_user' },
						{ userId: authResult.userId, newLogin: authResult.login },
					),
				);
			} catch (err) {
				this.logger.warn(`User-service anonymize failed for ${authResult.userId}`, err);
			}

			await Promise.all([
				this.reassignProjectOwner(authResult.previousLogin, authResult.login),
				this.renameCommentsUser(authResult.previousLogin, authResult.login),
			]);

			return { status: 'success', login: authResult.login };
		} catch (err) {
			this.logger.error('Error confirming anonymization', err);
			return { status: 'error', message: 'Failed to confirm anonymization' };
		}
	}
}
