type TemplateContext = {
	login: string;
	confirmUrl?: string;
	expiresMinutes?: number;
	when?: string;
};

const baseStyle =
	'font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; color: #111; line-height: 1.5;';

function shell(title: string, bodyHtml: string, ctaButton?: string) {
	return `<!doctype html>
<html><body style="${baseStyle}">
  <div style="max-width:560px;margin:32px auto;padding:24px;border:1px solid #eee;border-radius:8px;">
    <h2 style="margin:0 0 16px 0;">${title}</h2>
    ${bodyHtml}
    ${ctaButton ?? ''}
    <p style="font-size:12px;color:#666;margin-top:32px;">If you did not request this, you can safely ignore this email.</p>
  </div>
</body></html>`;
}

function ctaButton(url: string, label: string) {
	return `<p style="margin:24px 0;">
    <a href="${url}" style="display:inline-block;padding:12px 20px;background:#111;color:#fff;text-decoration:none;border-radius:6px;font-weight:600;">${label}</a>
  </p>
  <p style="font-size:12px;color:#666;word-break:break-all;">Or paste this link in your browser: ${url}</p>`;
}

export function deletionRequestEmail(ctx: Required<Pick<TemplateContext, 'login' | 'confirmUrl' | 'expiresMinutes'>>) {
	const subject = 'Confirm your account deletion';
	const html = shell(
		'Confirm account deletion',
		`<p>Hi <strong>${ctx.login}</strong>,</p>
     <p>You asked to delete your ft_transcendence account. This action is <strong>permanent</strong> and will remove your profile, connections, and active sessions.</p>
     <p>The link below is valid for ${ctx.expiresMinutes} minutes:</p>`,
		ctaButton(ctx.confirmUrl, 'Confirm deletion'),
	);
	const text = `Hi ${ctx.login},

You asked to delete your ft_transcendence account. This is permanent.

Confirm here (valid ${ctx.expiresMinutes} minutes):
${ctx.confirmUrl}

If you did not request this, ignore this email.`;
	return { subject, html, text };
}

export function anonymizationRequestEmail(ctx: Required<Pick<TemplateContext, 'login' | 'confirmUrl' | 'expiresMinutes'>>) {
	const subject = 'Confirm anonymization of your data';
	const html = shell(
		'Confirm anonymization',
		`<p>Hi <strong>${ctx.login}</strong>,</p>
     <p>You asked to anonymize your personal data. Your account will remain, but identifying information (name, email, profile fields) will be replaced with placeholders.</p>
     <p>The link below is valid for ${ctx.expiresMinutes} minutes:</p>`,
		ctaButton(ctx.confirmUrl, 'Confirm anonymization'),
	);
	const text = `Hi ${ctx.login},

You asked to anonymize your data. Your account will remain, but identifying fields will be replaced.

Confirm here (valid ${ctx.expiresMinutes} minutes):
${ctx.confirmUrl}`;
	return { subject, html, text };
}

export function exportNotificationEmail(ctx: Required<Pick<TemplateContext, 'login' | 'when'>>) {
	const subject = 'Your data export is ready';
	const html = shell(
		'Your data export is ready',
		`<p>Hi <strong>${ctx.login}</strong>,</p>
     <p>You exported a copy of your ft_transcendence data on ${ctx.when}. The download was provided directly to your browser.</p>
     <p>If this wasn't you, change your password immediately and revoke active sessions from your settings.</p>`,
	);
	const text = `Hi ${ctx.login},

You exported a copy of your ft_transcendence data on ${ctx.when}.
If this wasn't you, revoke your sessions immediately.`;
	return { subject, html, text };
}

export function deletionCompletedEmail(ctx: Required<Pick<TemplateContext, 'login' | 'when'>>) {
	const subject = 'Your account has been deleted';
	const html = shell(
		'Account deleted',
		`<p>Hi <strong>${ctx.login}</strong>,</p>
     <p>Your ft_transcendence account was deleted on ${ctx.when}. All sessions, profile data, and connections were removed.</p>`,
	);
	const text = `Hi ${ctx.login},

Your ft_transcendence account was deleted on ${ctx.when}.`;
	return { subject, html, text };
}

export function anonymizationCompletedEmail(ctx: Required<Pick<TemplateContext, 'login' | 'when'>>) {
	const subject = 'Your data has been anonymized';
	const html = shell(
		'Data anonymized',
		`<p>Hi <strong>${ctx.login}</strong>,</p>
     <p>Your personal data was anonymized on ${ctx.when}. Identifying fields have been replaced with placeholders.</p>`,
	);
	const text = `Hi ${ctx.login},

Your data was anonymized on ${ctx.when}.`;
	return { subject, html, text };
}
