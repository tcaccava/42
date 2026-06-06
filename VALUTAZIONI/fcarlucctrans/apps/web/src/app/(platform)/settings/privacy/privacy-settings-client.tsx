'use client';

import { Shield } from 'lucide-react';
import { LegalLinks } from '@/components/legal/LegalLinks';
import { PrivacyPanel } from '@/components/PrivacyPanel';
import { SettingsNav } from '@/components/SettingsNav';
import { PageHeader, PageShell } from '@/components/ui/layout';
import { useLocalization } from '@/utilities/localization';

export function PrivacySettingsClient({ userEmail }: { userEmail: string }) {
  const { t } = useLocalization();

  return (
    <PageShell>
      <PageHeader
        icon={<Shield className="h-5 w-5 text-[var(--hvr-background)]" />}
        title={t.privacyAndData}
        description={t.privacyDescription}
      />
      <SettingsNav />
      <p className="mb-6 text-sm text-foreground/70">
        {t.privacyPolicyReadAlso}{' '}
        <LegalLinks exclude="privacy-policy" linkClassName="font-medium text-teal-600 underline-offset-2 hover:underline" />
      </p>
      <PrivacyPanel userEmail={userEmail} />
    </PageShell>
  );
}
