import { Controller, Logger } from '@nestjs/common';
import { MessagePattern, Payload } from '@nestjs/microservices';
import { AppService } from './app.service';
import { GdprService } from './gdpr/gdpr.service';

@Controller()
export class AppController {
	private readonly logger = new Logger(AppController.name);

	constructor(
		private readonly appService: AppService,
		private readonly gdprService: GdprService,
	) {}

	@MessagePattern({ cmd: 'validate_42_user' })
	async validate42User(@Payload() data: { code: string }) {
		try {
			this.logger.log(`Validating 42 user with code: ${data.code.substring(0, 10)}...`);
			return await this.appService.validate42User(data.code);
		} catch (error) {
			this.logger.error('Error validating 42 user:', error);
			return { status: 'error', message: 'Failed to validate user' };
		}
	}

	@MessagePattern({ cmd: 'verify_jwt' })
	async verifyJwt(@Payload() data: { token: string }) {
		try {
			this.logger.log('Verifying JWT token');
			return await this.appService.verifyJwtToken(data.token);
		} catch (error) {
			this.logger.error('Error verifying JWT:', error);
			return { status: 'error', message: 'Failed to verify token' };
		}
	}

	@MessagePattern({ cmd: 'refresh_token' })
	async refreshToken(@Payload() data: { refreshToken: string }) {
		try {
			this.logger.log('Refreshing tokens');
			return await this.appService.refreshTokens(data.refreshToken);
		} catch (error) {
			this.logger.error('Error refreshing tokens:', error);
			return { status: 'error', message: 'Failed to refresh tokens' };
		}
	}

	@MessagePattern({ cmd: 'session_validate' })
	async validateSession(@Payload() data: { accessJti: string }) {
		try {
			return await this.appService.validateAccessSession(data.accessJti);
		} catch (error) {
			this.logger.error('Error validating session:', error);
			return { status: 'error', message: 'Failed to validate session' };
		}
	}

	@MessagePattern({ cmd: 'session_revoke' })
	async revokeSession(@Payload() data: { accessToken?: string; refreshToken?: string }) {
		try {
			const results = await Promise.all([
				data.accessToken
					? this.appService.revokeAccessSession(data.accessToken)
					: Promise.resolve({ status: 'success' }),
				data.refreshToken
					? this.appService.revokeRefreshSession(data.refreshToken)
					: Promise.resolve({ status: 'success' }),
			]);
			const failed = results.find((r) => r.status !== 'success');
			return failed ?? { status: 'success' };
		} catch (error) {
			this.logger.error('Error revoking session:', error);
			return { status: 'error', message: 'Failed to revoke session' };
		}
	}

	@MessagePattern({ cmd: 'user_is_active' })
	async isUserActive(@Payload() data: { userId: string }) {
		try {
			return await this.appService.isUserActive(data.userId);
		} catch (error) {
			this.logger.error('Error checking user active status:', error);
			return { status: 'error', message: 'Failed to check active status' };
		}
	}

	@MessagePattern({ cmd: 'user_directory' })
	async listDirectory(
		@Payload()
		data: { excludeUuid?: string; pageNumber?: number; pageSize?: number },
	) {
		try {
			return await this.appService.listDirectory(
				data.excludeUuid,
				data.pageNumber ?? 0,
				data.pageSize ?? 6,
			);
		} catch (error) {
			this.logger.error('Error listing user directory:', error);
			return { status: 'error', message: 'Failed to list users' };
		}
	}

	@MessagePattern({ cmd: 'user_search' })
	async searchUsers(
		@Payload()
		data: {
			query: string;
			excludeUuid?: string;
			pageNumber?: number;
			pageSize?: number;
		},
	) {
		try {
			return await this.appService.searchUsers(
				data.query,
				data.excludeUuid,
				data.pageNumber ?? 0,
				data.pageSize ?? 6,
			);
		} catch (error) {
			this.logger.error('Error searching users:', error);
			return { status: 'error', message: 'Failed to search users' };
		}
	}

	@MessagePattern({ cmd: 'user_find_by_login' })
	async findUserByLogin(@Payload() data: { login: string }) {
		try {
			return await this.appService.findPublicUserByLogin(data.login);
		} catch (error) {
			this.logger.error('Error finding user by login:', error);
			return { status: 'error', message: 'Failed to find user' };
		}
	}

	@MessagePattern({ cmd: 'user_find_by_uuids' })
	async findUsersByUuids(@Payload() data: { uuids: string[] }) {
		try {
			return await this.appService.findUsersByUuids(data.uuids ?? []);
		} catch (error) {
			this.logger.error('Error finding users by uuids:', error);
			return { status: 'error', message: 'Failed to find users' };
		}
	}

	@MessagePattern({ cmd: 'gdpr_export_auth' })
	async exportAuthData(@Payload() data: { userId: string }) {
		try {
			return await this.gdprService.exportAuthData(data.userId);
		} catch (error) {
			this.logger.error('Error exporting auth data:', error);
			return { status: 'error', message: 'Failed to export data' };
		}
	}

	@MessagePattern({ cmd: 'gdpr_request_deletion' })
	async requestDeletion(@Payload() data: { userId: string }) {
		try {
			return await this.gdprService.requestDeletion(data.userId);
		} catch (error) {
			this.logger.error('Error requesting deletion:', error);
			return { status: 'error', message: 'Failed to request deletion' };
		}
	}

	@MessagePattern({ cmd: 'gdpr_confirm_deletion' })
	async confirmDeletion(@Payload() data: { token: string }) {
		try {
			return await this.gdprService.confirmDeletion(data.token);
		} catch (error) {
			this.logger.error('Error confirming deletion:', error);
			return { status: 'error', message: 'Failed to confirm deletion' };
		}
	}

	@MessagePattern({ cmd: 'gdpr_request_anonymization' })
	async requestAnonymization(@Payload() data: { userId: string }) {
		try {
			return await this.gdprService.requestAnonymization(data.userId);
		} catch (error) {
			this.logger.error('Error requesting anonymization:', error);
			return { status: 'error', message: 'Failed to request anonymization' };
		}
	}

	@MessagePattern({ cmd: 'gdpr_confirm_anonymization' })
	async confirmAnonymization(@Payload() data: { token: string }) {
		try {
			return await this.gdprService.confirmAnonymization(data.token);
		} catch (error) {
			this.logger.error('Error confirming anonymization:', error);
			return { status: 'error', message: 'Failed to confirm anonymization' };
		}
	}
}
