import net from 'net';

export type WaitForTcpOptions = {
	timeoutMs?: number;
	intervalMs?: number;
	label?: string;
};

/**
 * Block until a TCP port accepts connections (or timeout).
 * Used so the gateway does not serve traffic while auth/user microservices
 * are still booting under `turbo run dev`.
 */
export async function waitForTcp(
	host: string,
	port: number,
	options: WaitForTcpOptions = {},
): Promise<void> {
	const { timeoutMs = 60_000, intervalMs = 500, label = `${host}:${port}` } =
		options;

	// `localhost` can resolve to ::1 while a service only listens on IPv4.
	const targets =
		host === 'localhost' ? ['127.0.0.1', '::1'] : [host];

	const started = Date.now();

	while (Date.now() - started < timeoutMs) {
		for (const target of targets) {
			const ok = await tryConnect(target, port);
			if (ok) {
				// eslint-disable-next-line no-console
				console.log(`[gateway] ${label} is ready (${target}:${port})`);
				return;
			}
		}
		await sleep(intervalMs);
	}

	throw new Error(
		`[gateway] Timed out after ${timeoutMs}ms waiting for ${label}`,
	);
}

function tryConnect(host: string, port: number): Promise<boolean> {
	return new Promise((resolve) => {
		const socket = net.connect({ host, port });
		const done = (success: boolean) => {
			socket.removeAllListeners();
			socket.destroy();
			resolve(success);
		};
		socket.setTimeout(1000);
		socket.once('connect', () => done(true));
		socket.once('timeout', () => done(false));
		socket.once('error', () => done(false));
	});
}

function sleep(ms: number) {
	return new Promise((resolve) => setTimeout(resolve, ms));
}
