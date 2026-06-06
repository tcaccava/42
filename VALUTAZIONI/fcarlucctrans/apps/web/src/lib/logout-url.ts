/** Full-page logout URL on the gateway (browser navigation, not Server Action redirect). */
export function getGatewayLogoutUrl(): string {
	const base = (process.env.NEXT_PUBLIC_API_URL ?? 'http://localhost:4000').replace(
		/\/$/,
		'',
	);
	return `${base}/api/auth/logout`;
}
