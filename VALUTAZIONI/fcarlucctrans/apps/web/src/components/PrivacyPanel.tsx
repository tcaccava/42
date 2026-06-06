'use client';

import { useState } from 'react';
import { Download, Trash2, UserMinus, Mail, AlertTriangle } from 'lucide-react';
import { Button } from '@/components/Button';
import { Input } from '@/components/Input';
import { AlertBanner, SectionCard } from '@/components/ui/layout';
import {
  downloadMyDataExport,
  requestAccountDeletion,
  requestDataAnonymization,
  type GdprActionResult,
} from '@/server/gdpr';
import { useLocalization } from '@/utilities/localization';

type Banner =
  | { kind: 'success'; text: string }
  | { kind: 'error'; text: string }
  | null;

export function PrivacyPanel({ userEmail }: { userEmail: string }) {
  const { t, tr } = useLocalization();
  const [banner, setBanner] = useState<Banner>(null);
  const [pendingAction, setPendingAction] = useState<
    'export' | 'delete' | 'anonymize' | null
  >(null);
  const [confirmingDelete, setConfirmingDelete] = useState(false);
  const [confirmingAnonymize, setConfirmingAnonymize] = useState(false);
  const [deleteAck, setDeleteAck] = useState('');

  const handleExport = async () => {
    setBanner(null);
    setPendingAction('export');
    try {
      const result = await downloadMyDataExport();
      if (result.status === 'unauthorized') {
        setBanner({ kind: 'error', text: t.gdprSessionExpired });
        return;
      }
      if (result.status !== 'success' || !result.contents) {
        setBanner({
          kind: 'error',
          text: result.message ?? t.gdprExportError,
        });
        return;
      }

      const blob = new Blob([result.contents], { type: 'application/json' });
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url;
      a.download = result.filename ?? 'ft-transcendence-export.json';
      document.body.appendChild(a);
      a.click();
      document.body.removeChild(a);
      URL.revokeObjectURL(url);

      setBanner({
        kind: 'success',
        text: tr('gdprExportSuccess', { email: userEmail }),
      });
    } finally {
      setPendingAction(null);
    }
  };

  const handleSendEmail = async (
    action: 'delete' | 'anonymize',
    server: () => Promise<GdprActionResult>,
  ) => {
    setBanner(null);
    setPendingAction(action);
    try {
      const result = await server();
      if (result.status === 'unauthorized') {
        setBanner({ kind: 'error', text: t.gdprSessionExpired });
        return;
      }
      if (result.status !== 'success') {
        setBanner({
          kind: 'error',
          text: result.message ?? t.gdprRequestError,
        });
        return;
      }

      const note = result.delivered
        ? tr('gdprEmailSent', { email: userEmail })
        : t.gdprEmailQueued;
      setBanner({
        kind: 'success',
        text: `${note} ${t.gdprLinkValidity}`,
      });
      setConfirmingDelete(false);
      setConfirmingAnonymize(false);
      setDeleteAck('');
    } finally {
      setPendingAction(null);
    }
  };

  return (
    <div className="space-y-6">
      {banner && (
        <AlertBanner kind={banner.kind}>{banner.text}</AlertBanner>
      )}

      <SectionCard
        icon={<Download className="h-5 w-5 text-[var(--hvr-background)]" />}
        title={t.gdprExportTitle}
        description={t.gdprExportDescription}
      >
        <Button
          onClick={handleExport}
          disabled={pendingAction !== null}
          className="inline-flex gap-2"
        >
          <Download className="h-4 w-4" />
          {pendingAction === 'export' ? t.gdprExportPreparing : t.gdprExportButton}
        </Button>
      </SectionCard>

      <SectionCard
        icon={<UserMinus className="h-5 w-5 text-[var(--hvr-background)]" />}
        title={t.gdprAnonymizeTitle}
        description={t.gdprAnonymizeDescription}
      >
        {!confirmingAnonymize ? (
          <Button
            variant="secondary"
            onClick={() => {
              setConfirmingAnonymize(true);
              setConfirmingDelete(false);
              setBanner(null);
            }}
            disabled={pendingAction !== null}
            className="inline-flex gap-2"
          >
            <UserMinus className="h-4 w-4" />
            {t.gdprAnonymizeRequest}
          </Button>
        ) : (
          <ConfirmEmailRequest
            email={userEmail}
            pending={pendingAction === 'anonymize'}
            disabled={pendingAction !== null}
            prompt={tr('gdprEmailPrompt', { email: userEmail })}
            sendLabel={t.gdprSendAnonymizeEmail}
            sendingLabel={t.gdprSending}
            cancelLabel={t.cancel}
            onCancel={() => setConfirmingAnonymize(false)}
            onConfirm={() =>
              handleSendEmail('anonymize', requestDataAnonymization)
            }
          />
        )}
      </SectionCard>

      <SectionCard
        icon={<Trash2 className="h-5 w-5 text-red-600" />}
        title={t.gdprDeleteTitle}
        description={t.gdprDeleteDescription}
        tone="danger"
      >
        {!confirmingDelete ? (
          <Button
            variant="danger"
            onClick={() => {
              setConfirmingDelete(true);
              setConfirmingAnonymize(false);
              setBanner(null);
            }}
            disabled={pendingAction !== null}
            className="inline-flex gap-2"
          >
            <Trash2 className="h-4 w-4" />
            {t.gdprDeleteRequest}
          </Button>
        ) : (
          <div className="space-y-4">
            <div className="flex items-start gap-2 rounded-lg border border-red-300/60 bg-red-50/80 p-3 text-sm text-red-900 dark:border-red-500/40 dark:bg-red-950/30 dark:text-red-200">
              <AlertTriangle className="mt-0.5 h-4 w-4 shrink-0" />
              <p>{tr('gdprDeleteAckHelp', { email: userEmail })}</p>
            </div>
            <Input
              value={deleteAck}
              onChange={(e) => setDeleteAck(e.target.value)}
              placeholder={t.gdprDeleteAckPlaceholder}
              disabled={pendingAction !== null}
              className="focus:ring-red-500 focus:border-red-600"
            />
            <div className="flex flex-wrap gap-2">
              <Button
                variant="danger"
                onClick={() => handleSendEmail('delete', requestAccountDeletion)}
                disabled={deleteAck.trim() !== 'DELETE' || pendingAction !== null}
                className="inline-flex gap-2"
              >
                <Mail className="h-4 w-4" />
                {pendingAction === 'delete' ? t.gdprSending : t.gdprSendEmail}
              </Button>
              <Button
                variant="secondary"
                onClick={() => {
                  setConfirmingDelete(false);
                  setDeleteAck('');
                }}
                disabled={pendingAction !== null}
              >
                {t.cancel}
              </Button>
            </div>
          </div>
        )}
      </SectionCard>
    </div>
  );
}

function ConfirmEmailRequest({
  email,
  pending,
  disabled,
  prompt,
  sendLabel,
  sendingLabel,
  cancelLabel,
  onCancel,
  onConfirm,
}: {
  email: string;
  pending: boolean;
  disabled: boolean;
  prompt: string;
  sendLabel: string;
  sendingLabel: string;
  cancelLabel: string;
  onCancel: () => void;
  onConfirm: () => void;
}) {
  return (
    <div className="space-y-3">
      <p className="text-sm text-foreground/80">{prompt}</p>
      <p className="sr-only">{email}</p>
      <div className="flex flex-wrap gap-2">
        <Button
          onClick={onConfirm}
          disabled={disabled}
          className="inline-flex gap-2"
        >
          <Mail className="h-4 w-4" />
          {pending ? sendingLabel : sendLabel}
        </Button>
        <Button variant="secondary" onClick={onCancel} disabled={disabled}>
          {cancelLabel}
        </Button>
      </div>
    </div>
  );
}
