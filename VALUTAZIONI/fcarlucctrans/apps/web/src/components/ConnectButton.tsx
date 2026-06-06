'use client';

import { useState, useTransition } from 'react';
import { UserPlus, UserCheck, Clock, X, Loader2 } from 'lucide-react';
import { Button } from '@/components/Button';
import type { ConnectionRelation } from '@/types/connection';
import {
  sendConnectionRequest,
  acceptConnectionRequest,
  removeOrCancelConnection,
} from '@/server/connections';
import { revalidateProfilePaths } from '@/server/users';

type ConnectButtonProps = {
  targetUserId: string;
  targetLogin: string;
  relation: ConnectionRelation;
  connectionId?: string;
  connectionCount?: number;
};

export function ConnectButton({
  targetUserId,
  targetLogin,
  relation,
  connectionId,
  connectionCount,
}: ConnectButtonProps) {
  const [currentRelation, setCurrentRelation] = useState(relation);
  const [currentConnectionId, setCurrentConnectionId] = useState(connectionId);
  const [error, setError] = useState<string | null>(null);
  const [isPending, startTransition] = useTransition();

  const refresh = async () => {
    await revalidateProfilePaths(targetLogin);
  };

  const handleConnect = () => {
    setError(null);
    startTransition(async () => {
      const res = await sendConnectionRequest(targetUserId);
      if (res.status !== 'success') {
        setError(res.message ?? 'Failed to send request');
        return;
      }
      if (res.autoAccepted) {
        setCurrentRelation('connected');
      } else {
        setCurrentRelation('pending_outgoing');
      }
      setCurrentConnectionId(res.connection?.id);
      await refresh();
    });
  };

  const handleAccept = () => {
    if (!currentConnectionId) return;
    setError(null);
    startTransition(async () => {
      const res = await acceptConnectionRequest(currentConnectionId);
      if (res.status !== 'success') {
        setError(res.message ?? 'Failed to accept');
        return;
      }
      setCurrentRelation('connected');
      await refresh();
    });
  };

  const handleWithdrawOrRemove = () => {
    if (!currentConnectionId) return;
    setError(null);
    startTransition(async () => {
      const res = await removeOrCancelConnection(currentConnectionId);
      if (res.status !== 'success') {
        setError(res.message ?? 'Failed to update connection');
        return;
      }
      setCurrentRelation('none');
      setCurrentConnectionId(undefined);
      await refresh();
    });
  };

  if (currentRelation === 'self') {
    return (
      <div className="text-sm text-gray-500">
        {typeof connectionCount === 'number' ? (
          <span className="font-medium text-gray-700">{connectionCount} connections</span>
        ) : null}
      </div>
    );
  }

  return (
    <div className="flex flex-col items-start gap-2">
      {currentRelation === 'none' && (
        <Button onClick={handleConnect} disabled={isPending} size="sm">
          {isPending ? (
            <Loader2 className="w-4 h-4 mr-2 animate-spin" />
          ) : (
            <UserPlus className="w-4 h-4 mr-2" />
          )}
          Connect
        </Button>
      )}

      {currentRelation === 'pending_outgoing' && (
        <Button
          onClick={handleWithdrawOrRemove}
          disabled={isPending}
          size="sm"
          variant="secondary"
        >
          {isPending ? (
            <Loader2 className="w-4 h-4 mr-2 animate-spin" />
          ) : (
            <Clock className="w-4 h-4 mr-2" />
          )}
          Pending — Withdraw
        </Button>
      )}

      {currentRelation === 'pending_incoming' && (
        <div className="flex gap-2">
          <Button onClick={handleAccept} disabled={isPending} size="sm">
            {isPending ? (
              <Loader2 className="w-4 h-4 mr-2 animate-spin" />
            ) : (
              <UserCheck className="w-4 h-4 mr-2" />
            )}
            Accept
          </Button>
          <Button
            onClick={handleWithdrawOrRemove}
            disabled={isPending}
            size="sm"
            variant="ghost"
          >
            <X className="w-4 h-4 mr-2" />
            Ignore
          </Button>
        </div>
      )}

      {currentRelation === 'connected' && (
        <Button
          onClick={handleWithdrawOrRemove}
          disabled={isPending}
          size="sm"
          variant="secondary"
        >
          {isPending ? (
            <Loader2 className="w-4 h-4 mr-2 animate-spin" />
          ) : (
            <UserCheck className="w-4 h-4 mr-2" />
          )}
          Connected — Remove
        </Button>
      )}

      {error && <p className="text-sm text-red-600">{error}</p>}
    </div>
  );
}
