'use client';

import { useState } from 'react';
import { useRouter } from 'next/navigation';
import { Trash2, UserMinus, AlertTriangle } from 'lucide-react';
import { Button } from '@/components/Button';
import { AlertBanner, PageHeader, PageShell, SectionCard } from '@/components/ui/layout';
import { confirmAnonymization, confirmDeletion } from '@/server/gdpr';
import { useLocalization } from '@/utilities/localization';

type Props = {
  type: 'delete' | 'anonymize';
  token: string;
};

export function ConfirmAction({ type, token }: Props) {
  const router = useRouter();
  const { t } = useLocalization();
  const [pending, setPending] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [done, setDone] = useState(false);

  const config =
    type === 'delete'
      ? {
          icon: <Trash2 className="h-6 w-6 text-red-600" />,
          title: t.gdprConfirmDeleteTitle,
          subtitle: t.gdprConfirmDeleteBody,
          cta: t.gdprConfirmDeleteCta,
          tone: 'danger' as const,
          successTitle: t.gdprConfirmDeleteSuccessTitle,
          successBody: t.gdprConfirmDeleteSuccessBody,
        }
      : {
          icon: <UserMinus className="h-6 w-6 text-[var(--hvr-background)]" />,
          title: t.gdprConfirmAnonymizeTitle,
          subtitle: t.gdprConfirmAnonymizeBody,
          cta: t.gdprConfirmAnonymizeCta,
          tone: 'default' as const,
          successTitle: t.gdprConfirmAnonymizeSuccessTitle,
          successBody: t.gdprConfirmAnonymizeSuccessBody,
        };

  if (!token) {
    return (
      <PageShell maxWidth="2xl">
        <SectionCard tone="danger">
          <ConfirmHeader
            icon={<AlertTriangle className="h-6 w-6 text-red-600" />}
            title={t.gdprConfirmMissingToken}
            subtitle={t.gdprConfirmMissingTokenBody}
          />
          <div className="mt-6">
            <Button variant="secondary" onClick={() => router.replace('/settings/privacy')}>
              {t.privacyAndData}
            </Button>
          </div>
        </SectionCard>
      </PageShell>
    );
  }

  const handleConfirm = async () => {
    setPending(true);
    setError(null);
    try {
      const action =
        type === 'delete' ? confirmDeletion : confirmAnonymization;
      const result = await action(token);
      if (result.status !== 'success') {
        setError(result.message ?? t.gdprConfirmFailed);
        return;
      }
      setDone(true);
      window.location.href = '/api/logout';
    } finally {
      setPending(false);
    }
  };

  if (done) {
    return (
      <PageShell maxWidth="2xl">
        <SectionCard>
          <ConfirmHeader
            icon={config.icon}
            title={config.successTitle}
            subtitle={config.successBody}
          />
          <p className="mt-6 text-sm text-foreground/60">
            {t.gdprConfirmRedirecting}
          </p>
        </SectionCard>
      </PageShell>
    );
  }

  return (
    <PageShell maxWidth="2xl">
      <PageHeader
        icon={config.icon}
        title={config.title}
        description={config.subtitle}
      />
      {error && <AlertBanner kind="error" className="mb-6">{error}</AlertBanner>}
      <SectionCard tone={config.tone}>
        <div className="flex flex-wrap gap-3">
          <Button
            variant={type === 'delete' ? 'danger' : 'primary'}
            onClick={handleConfirm}
            disabled={pending}
            className="inline-flex gap-2"
          >
            {config.icon}
            {pending ? t.gdprConfirmProcessing : config.cta}
          </Button>
          <Button
            variant="secondary"
            onClick={() => router.replace('/settings/privacy')}
            disabled={pending}
          >
            {t.cancel}
          </Button>
        </div>
      </SectionCard>
    </PageShell>
  );
}

function ConfirmHeader({
  icon,
  title,
  subtitle,
}: {
  icon: React.ReactNode;
  title: string;
  subtitle: string;
}) {
  return (
    <div className="flex items-start gap-3">
      <div className="rounded-lg border border-border bg-darker-background p-2 shadow-sm">
        {icon}
      </div>
      <div>
        <h1 className="text-lg font-semibold text-foreground">{title}</h1>
        <p className="mt-1 text-sm text-foreground/70">{subtitle}</p>
      </div>
    </div>
  );
}
