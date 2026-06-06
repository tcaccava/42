import { Controller, Get } from '@nestjs/common';

/**
 * Legacy GDPR zip endpoints (`POST /gdpr`, `DELETE /gdpr`) were removed.
 * Use the authenticated GDPR module instead:
 *   GET/POST /api/user/me/export|delete-request|anonymize-request|*-confirm
 */
@Controller()
export class AppController {
	@Get('health')
	health() {
		return { status: 'ok', service: 'gateway' };
	}
}
