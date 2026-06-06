/**
 * Lightweight OpenAPI 3.0 description served at GET /api/v1/openapi.json
 * (public, no API key — so integrators can read the contract first).
 */
export const publicOpenApiSpec = {
	openapi: '3.0.3',
	info: {
		title: 'ft_transcendence Public API',
		version: '1.0.0',
		description:
			'REST API for PublicResource CRUD. All /resources routes require the X-API-Key header.',
	},
	servers: [{ url: '/api/v1', description: 'Gateway' }],
	components: {
		securitySchemes: {
			ApiKeyAuth: {
				type: 'apiKey',
				in: 'header',
				name: 'X-API-Key',
			},
		},
		schemas: {
			PublicResource: {
				type: 'object',
				properties: {
					id: { type: 'string', format: 'uuid' },
					title: { type: 'string' },
					content: { type: 'string', nullable: true },
					tags: { type: 'array', items: { type: 'string' } },
					createdAt: { type: 'string', format: 'date-time' },
					updatedAt: { type: 'string', format: 'date-time' },
				},
			},
			CreateResource: {
				type: 'object',
				required: ['title'],
				properties: {
					title: { type: 'string' },
					content: { type: 'string' },
					tags: { type: 'array', items: { type: 'string' } },
				},
			},
		},
	},
	paths: {
		'/resources': {
			get: {
				summary: 'List resources',
				security: [{ ApiKeyAuth: [] }],
				parameters: [
					{ name: 'limit', in: 'query', schema: { type: 'integer', default: 50 } },
					{ name: 'offset', in: 'query', schema: { type: 'integer', default: 0 } },
				],
				responses: { '200': { description: 'Paginated list' } },
			},
			post: {
				summary: 'Create resource',
				security: [{ ApiKeyAuth: [] }],
				requestBody: {
					content: {
						'application/json': {
							schema: { $ref: '#/components/schemas/CreateResource' },
						},
					},
				},
				responses: { '201': { description: 'Created' } },
			},
		},
		'/resources/{id}': {
			get: {
				summary: 'Get one resource',
				security: [{ ApiKeyAuth: [] }],
				parameters: [
					{ name: 'id', in: 'path', required: true, schema: { type: 'string', format: 'uuid' } },
				],
				responses: { '200': { description: 'OK' }, '404': { description: 'Not found' } },
			},
			put: {
				summary: 'Update resource',
				security: [{ ApiKeyAuth: [] }],
				parameters: [
					{ name: 'id', in: 'path', required: true, schema: { type: 'string', format: 'uuid' } },
				],
				requestBody: {
					content: {
						'application/json': {
							schema: { $ref: '#/components/schemas/CreateResource' },
						},
					},
				},
				responses: { '200': { description: 'Updated' } },
			},
			delete: {
				summary: 'Delete resource',
				security: [{ ApiKeyAuth: [] }],
				parameters: [
					{ name: 'id', in: 'path', required: true, schema: { type: 'string', format: 'uuid' } },
				],
				responses: { '200': { description: 'Deleted' } },
			},
		},
	},
} as const;
