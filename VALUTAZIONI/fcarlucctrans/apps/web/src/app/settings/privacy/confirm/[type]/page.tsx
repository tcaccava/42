import { notFound } from 'next/navigation';
import { ConfirmAction } from '@/components/ConfirmAction';

type Params = { type: string };
type SearchParams = { token?: string };

export const dynamic = 'force-dynamic';

/**
 * Confirmation page for GDPR delete / anonymize.
 *
 * Lives OUTSIDE the (platform) auth-guarded segment on purpose: the user
 * may follow the email link from a browser where they aren't currently
 * logged in (different device, incognito, after logout). The confirmation
 * is authorized by the one-time token, not by the cookie.
 */
export default async function ConfirmPage(props: {
  params: Promise<Params>;
  searchParams: Promise<SearchParams>;
}) {
  const { type } = await props.params;
  const { token } = await props.searchParams;

  if (type !== 'delete' && type !== 'anonymize') {
    notFound();
  }

  return (
    <div className="min-h-screen bg-gray-50">
      <ConfirmAction type={type} token={token ?? ''} />
    </div>
  );
}
