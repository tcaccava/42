'use client';

import { useState, useTransition } from 'react';
import Image from 'next/image';
import Link from 'next/link';
import { UserCheck, UserPlus, X, Loader2 } from 'lucide-react';
import { Button } from '@/components/Button';
import type { ConnectionItem } from '@/types/connection';
import {
  acceptConnectionRequest,
  removeOrCancelConnection,
} from '@/server/connections';
import { PageHeader, PageShell, SectionCard } from '@/components/ui/layout';

type ConnectionsManagerProps = {
  pending: ConnectionItem[];
  accepted: ConnectionItem[];
};

function peerName(peer?: ConnectionItem['peer']) {
  if (!peer) return 'Unknown user';
  const full = `${peer.first_name ?? ''} ${peer.last_name ?? ''}`.trim();
  return full || peer.displayname || peer.login;
}

export function ConnectionsManager({ pending, accepted }: ConnectionsManagerProps) {
  const [pendingList, setPendingList] = useState(pending);
  const [acceptedList, setAcceptedList] = useState(accepted);
  const [isPending, startTransition] = useTransition();
  const [activeId, setActiveId] = useState<string | null>(null);

  const runAction = (connectionId: string, action: 'accept' | 'remove') => {
    setActiveId(connectionId);
    startTransition(async () => {
      const res =
        action === 'accept'
          ? await acceptConnectionRequest(connectionId)
          : await removeOrCancelConnection(connectionId);

      if (res.status === 'success') {
        const item = pendingList.find((c) => c.id === connectionId);
        setPendingList((list) => list.filter((c) => c.id !== connectionId));

        if (action === 'accept' && item) {
          setAcceptedList((list) => [
            { ...item, status: 'ACCEPTED', direction: item.direction },
            ...list,
          ]);
        } else if (action === 'remove') {
          setAcceptedList((list) => list.filter((c) => c.id !== connectionId));
        }
      }
      setActiveId(null);
    });
  };

  const incoming = pendingList.filter((c) => c.direction === 'incoming');
  const outgoing = pendingList.filter((c) => c.direction === 'outgoing');

  return (
    <PageShell className="space-y-6">
      <PageHeader
        icon={<UserCheck className="h-5 w-5 text-teal-600" />}
        title="My Network"
        description="Manage connection requests and your professional network."
      />

      {incoming.length > 0 && (
        <SectionCard
          icon={<UserPlus className="h-5 w-5 text-teal-600" />}
          title={`Invitations (${incoming.length})`}
          contentClassName="space-y-3"
        >
          {incoming.map((connection) => (
            <div
              key={connection.id}
              className="flex items-center justify-between gap-4 rounded-xl border border-gray-100 bg-gray-50 p-4"
            >
              <Link
                href={`/profile/${connection.peer?.login ?? ''}`}
                className="flex min-w-0 items-center gap-3"
              >
                <Image
                  src={connection.peer?.image || '/globe.svg'}
                  alt={connection.peer?.login ?? 'user'}
                  width={48}
                  height={48}
                  className="h-12 w-12 rounded-full object-cover"
                />
                <div className="min-w-0">
                  <p className="truncate font-semibold text-gray-900">
                    {peerName(connection.peer)}
                  </p>
                  <p className="text-sm text-gray-500">@{connection.peer?.login}</p>
                </div>
              </Link>
              <div className="flex shrink-0 gap-2">
                <Button
                  size="sm"
                  onClick={() => runAction(connection.id, 'accept')}
                  disabled={isPending && activeId === connection.id}
                >
                  {isPending && activeId === connection.id ? (
                    <Loader2 className="h-4 w-4 animate-spin" />
                  ) : (
                    'Accept'
                  )}
                </Button>
                <Button
                  size="sm"
                  variant="ghost"
                  onClick={() => runAction(connection.id, 'remove')}
                  disabled={isPending && activeId === connection.id}
                >
                  <X className="h-4 w-4" />
                </Button>
              </div>
            </div>
          ))}
        </SectionCard>
      )}

      {outgoing.length > 0 && (
        <SectionCard
          title={`Sent requests (${outgoing.length})`}
          contentClassName="space-y-3"
        >
          {outgoing.map((connection) => (
            <div
              key={connection.id}
              className="flex items-center justify-between gap-4 rounded-xl bg-gray-50 p-4"
            >
              <Link href={`/profile/${connection.peer?.login ?? ''}`} className="flex items-center gap-3">
                <Image
                  src={connection.peer?.image || '/globe.svg'}
                  alt={connection.peer?.login ?? 'user'}
                  width={40}
                  height={40}
                  className="h-10 w-10 rounded-full object-cover"
                />
                <div>
                  <p className="font-medium text-gray-900">{peerName(connection.peer)}</p>
                  <p className="text-sm text-gray-500">Pending</p>
                </div>
              </Link>
              <Button
                size="sm"
                variant="secondary"
                onClick={() => runAction(connection.id, 'remove')}
                disabled={isPending && activeId === connection.id}
              >
                Withdraw
              </Button>
            </div>
          ))}
        </SectionCard>
      )}

      <SectionCard
        icon={<UserCheck className="h-5 w-5 text-teal-600" />}
        title={`Connections (${acceptedList.length})`}
      >
        {acceptedList.length === 0 ? (
          <p className="py-8 text-center text-gray-500">
            No connections yet. Search for students and send a request.
          </p>
        ) : (
          <div className="grid grid-cols-1 gap-3 sm:grid-cols-2">
            {acceptedList.map((connection) => (
              <Link
                key={connection.id}
                href={`/profile/${connection.peer?.login ?? ''}`}
                className="flex items-center gap-3 rounded-xl border border-gray-100 p-4 transition-colors hover:border-teal-200 hover:bg-teal-50/30"
              >
                <Image
                  src={connection.peer?.image || '/globe.svg'}
                  alt={connection.peer?.login ?? 'user'}
                  width={48}
                  height={48}
                  className="h-12 w-12 rounded-full object-cover"
                />
                <div>
                  <p className="font-semibold text-gray-900">{peerName(connection.peer)}</p>
                  <p className="text-sm text-gray-500">@{connection.peer?.login}</p>
                </div>
              </Link>
            ))}
          </div>
        )}
      </SectionCard>
    </PageShell>
  );
}
