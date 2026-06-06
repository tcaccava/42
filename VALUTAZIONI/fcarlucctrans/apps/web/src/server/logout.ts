/**
 * Logout is a normal link to the gateway (`getGatewayLogoutUrl` in Header).
 * Server Actions cannot redirect the browser to another origin (e.g. :3000 → :4000);
 * DevTools shows that as `logout (blocked:origin)`.
 */

export { getGatewayLogoutUrl } from '@/lib/logout-url';
