import type { AxiosRequestConfig } from 'axios';

function pickHeader(headers: Record<string, unknown>, name: string): string | undefined {
	const direct = headers[name] ?? headers[name.toLowerCase()];
	return typeof direct === 'string' && direct.length > 0 ? direct : undefined;
}

function shouldSendBody(method: string, body: unknown): boolean {
	const upper = method.toUpperCase();
	if (upper === 'GET' || upper === 'HEAD' || upper === 'DELETE') {
		return false;
	}
	if (body === undefined || body === null) {
		return false;
	}
	if (typeof FormData !== 'undefined' && body instanceof FormData) {
		return true;
	}
	if (typeof body === 'object' && Object.keys(body as object).length === 0) {
		return false;
	}
	return true;
}

export function buildProxyAxiosConfig(
	baseUrl: string,
	path: string,
	method: string,
	body: unknown,
	headers: Record<string, unknown>,
): AxiosRequestConfig {
	const safeHeaders: Record<string, string> = {};
	for (const name of ['cookie', 'authorization', 'content-type', 'accept'] as const) {
		const value = pickHeader(headers, name);
		if (value) safeHeaders[name] = value;
	}

	const config: AxiosRequestConfig = {
		method,
		url: baseUrl + path,
		headers: safeHeaders,
	};

	if (shouldSendBody(method, body)) {
		config.data = body;
	}

	return config;
}
