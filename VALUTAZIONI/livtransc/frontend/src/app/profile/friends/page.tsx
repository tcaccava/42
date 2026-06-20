'use client';

import { useEffect, useState } from 'react';
import Link from 'next/link';
import { useTranslation } from "@/hooks/useTranslation";

interface Friend {
  id: number;
  friendship_id: number;
  username: string;
  is_online?: boolean;
  emoji?: string;
}

interface FriendRequest {
  id: number;
  from_user: string;
  from_user_id: number;
  emoji?: string;
}

interface PendingRequest {
  id: number;
  to_user: string;
  to_user_id: number;
  emoji?: string;
}

const API_BASE = process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8000';

export default function FriendsPage() {
  const { t } = useTranslation();
  const [friends, setFriends] = useState<Friend[]>([]);
  const [friendRequests, setFriendRequests] = useState<FriendRequest[]>([]);
  const [pendingRequests, setPendingRequests] = useState<PendingRequest[]>([]);
  const [searchQuery, setSearchQuery] = useState('');
  const [lastSearchQuery, setLastSearchQuery] = useState('');
  const [searchResults, setSearchResults] = useState<Friend[]>([]);
  const [hasSearched, setHasSearched] = useState(false);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  // Carica i dati degli amici
  useEffect(() => {
    const fetchFriends = async () => {
      try {
        const response = await fetch(`${API_BASE}/api/friends`,
          { credentials: 'include' }
        );

        if (!response.ok) throw new Error('Error loading friends');

        const data = await response.json();

        // Map backend response to frontend format
        const friendsData = data.friends || {};

        // Accepted friends
        const acceptedFriends = Array.isArray(friendsData.accepted) ? friendsData.accepted.map((f: any) => ({
          friendship_id: f.id,
          id: f.user_id,
          username: f.username,
          is_online: f.online,
          emoji: f.emoji
        })) : [];

        // Friend requests received
        const requests = Array.isArray(friendsData.received_pending) ? friendsData.received_pending.map((r: any) => ({
          id: r.id,
          from_user_id: r.user_id,
          from_user: r.username,
          emoji: r.emoji
        })) : [];

        // Pending requests sent
        const pending = Array.isArray(friendsData.sent_pending) ? friendsData.sent_pending.map((p: any) => ({
          id: p.id,
          to_user_id: p.user_id,
          to_user: p.username,
          emoji: p.emoji
        })) : [];

        setFriends(acceptedFriends);
        setFriendRequests(requests);
        setPendingRequests(pending);
      } catch (err) {
        setError(err instanceof Error ? err.message : 'Unknown error');
      } finally {
        setLoading(false);
      }
    };

    fetchFriends();
  }, []);

  // Ricerca amici
  const handleSearch = async () => {
    if (!searchQuery.trim()) {
      setSearchResults([]);
      setHasSearched(false);
      return;
    }

    try {
      const response = await fetch(
        `${API_BASE}/api/search_users?search=${encodeURIComponent(searchQuery)}`,
        { credentials: 'include' }
      );

      if (!response.ok) throw new Error('Error searching');

      const data = await response.json();
      const users = Array.isArray(data.users) ? data.users : [];
      setSearchResults(users);
      setLastSearchQuery(searchQuery);
      setHasSearched(true);
    } catch (err) {
      console.error('Search error:', err);
    }
  };

  // Ricerca amici in tempo reale ad ogni lettera
  useEffect(() => {
    const timer = setTimeout(() => {
      handleSearch();
    }, 300);
    return () => clearTimeout(timer);
  }, [searchQuery]);

  // Invia richiesta di amicizia
  const handleAddFriend = async (userId: number) => {
    try {
      const response = await fetch(`${API_BASE}/api/send_request`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        credentials: 'include',
        body: JSON.stringify({ user2: userId }),
      });

      if (!response.ok) throw new Error('Error sending request');

      const data = await response.json();

      setPendingRequests([
        ...pendingRequests,
        { id: data.request_id, to_user: searchResults.find((u) => u.id === userId)?.username || '', to_user_id: userId },
      ]);
    } catch (err) {
      console.error('Error:', err);
    }
  };

  // Accetta richiesta di amicizia
  const handleAcceptRequest = async (requestId: number, userId: number) => {
    try {
      const response = await fetch(`${API_BASE}/api/accept_request`, {
        method: 'POST',
        credentials: 'include',
        body: JSON.stringify({ request_id: requestId })
      });

      if (!response.ok) throw new Error('Error accepting request');

      const request = friendRequests.find((r) => r.id === requestId);
      if (request) {
        setFriends([...friends, { friendship_id: requestId, id: userId, username: request.from_user }]);
      }
      setFriendRequests(friendRequests.filter((r) => r.id !== requestId));
    } catch (err) {
      console.error('Error:', err);
    }
  };

  // Rifiuta richiesta di amicizia
  const handleRejectRequest = async (requestId: number) => {
    try {
      const response = await fetch(`${API_BASE}/api/remove_friend`, {
        method: 'DELETE',
        credentials: 'include',
        body: JSON.stringify({ request_id: requestId }),
      });

      if (!response.ok) throw new Error('Error rejecting request');

      setFriendRequests(friendRequests.filter((r) => r.id !== requestId));
    } catch (err) {
      console.error('Error:', err);
    }
  };

  // Rimuovi amico
  const handleRemoveFriend = async (friendshipId: number, userId: number) => {
    try {
      const response = await fetch(`${API_BASE}/api/remove_friend`, {
        method: 'DELETE',
        credentials: 'include',
        body: JSON.stringify({ request_id: friendshipId }),
      });

      if (!response.ok) throw new Error('Error removing friend');

      setFriends(friends.filter((f) => f.id !== userId));
    } catch (err) {
      console.error('Error:', err);
    }
  };

  // Annulla richiesta in sospeso
  const handleCancelPending = async (requestId: number) => {
    try {
      const response = await fetch(`${API_BASE}/api/remove_friend`, {
        method: 'DELETE',
        credentials: 'include',
        body: JSON.stringify({ request_id: requestId })
      });

      if (!response.ok) throw new Error('Error canceling request');

      setPendingRequests(pendingRequests.filter((r) => r.id !== requestId));
    } catch (err) {
      console.error('Error:', err);
    }
  };

  if (loading) return <div className="flex justify-center items-center h-full">Loading...</div>;

  return (
    <div className="bg-[#0F172A] text-[#F8FAFC] p-6">
      <div className="max-w-4xl mx-auto">
        <div className="mb-8 flex items-center justify-between">
          <h1 className="text-3xl font-bold">{t('friends.title')}</h1>
          <Link href="/profile">
            <button className="px-5 py-2 rounded-lg bg-[#334155] hover:bg-[#475569] text-white font-bold transition cursor-pointer flex items-center gap-2">
              <span>←</span> {t('profile.back_to_profile')}
            </button>
          </Link>
        </div>

        {error && <div className="bg-red-600 p-4 rounded mb-6">{error}</div>}

        {/* Search bar */}
        <div className="bg-gray-800 p-6 rounded-lg mb-8">
          <h2 className="text-xl font-semibold mb-4">🔍 {t('friends.search_users')}</h2>
          <div className="flex gap-3">
            <input
              type="text"
              value={searchQuery}
              onChange={(e) => setSearchQuery(e.target.value)}
              placeholder={t('friends.placeholder')}
              className="flex-1 px-4 py-2 bg-gray-700 rounded border border-gray-600 focus:border-blue-500 focus:outline-none"
            />
            <button
              onClick={handleSearch}
              className="px-6 py-2 bg-blue-600 hover:bg-blue-700 rounded font-semibold transition cursor-pointer"
            >
              {t('friends.search_button')}
            </button>
          </div>

          {/* Search results */}
          {hasSearched && (
            <div className="mt-6 pt-6 border-t border-gray-700">
              <h3 className="text-lg font-semibold mb-4">{t('friends.search_results')}</h3>
              {searchResults.length > 0 ? (
                <div className="space-y-3">
                  {searchResults.map((user) => (
                    <div key={`search-${user.id}-${user.username}`} className="flex justify-between items-center bg-gray-700 p-4 rounded">
                      <Link href={`/user/${user.username}`}>
                        <span className="font-medium hover:text-[#FACC15] transition cursor-pointer">{user.username}</span>
                      </Link>
                      <div className="flex gap-2">
                        {friends.some(f => f.id === user.id) ? (
                          <span className="text-gray-400 text-sm italic">{t('friends.already_friends')}</span>
                        ) : friendRequests.some(r => r.from_user_id === user.id) ? (
                          <button
                            onClick={() => {
                              const req = friendRequests.find(r => r.from_user_id === user.id);
                              if (req) handleAcceptRequest(req.id, user.id);
                            }}
                            className="px-4 py-1 bg-green-600 hover:bg-green-700 rounded text-sm transition"
                          >
                            {t('navbar.accept')}
                          </button>
                        ) : pendingRequests.some(p => p.to_user_id === user.id) ? (
                          <button
                            onClick={() => {
                              const req = pendingRequests.find(p => p.to_user_id === user.id);
                              if (req) handleCancelPending(req.id);
                            }}
                            className="px-4 py-1 bg-yellow-600 hover:bg-yellow-700 rounded text-sm transition"
                          >
                            {t('friends.cancel')}
                          </button>
                        ) : (
                          <button
                            onClick={() => handleAddFriend(user.id)}
                            className="px-4 py-1 bg-blue-600 hover:bg-blue-700 rounded text-sm transition"
                          >
                            {t('friends.add_friend')}
                          </button>
                        )}
                      </div>
                    </div>
                  ))}
                </div>
              ) : (
                <p className="text-gray-400">{t('friends.no_results')} "{lastSearchQuery}"</p>
              )}
            </div>
          )}
        </div>

        {/* Friends */}
        <div className="bg-gray-800 p-6 rounded-lg mb-8">
          <h2 className="text-xl font-semibold mb-4">{t('friends.friends')} ({friends.length})</h2>
          {friends.length === 0 ? (
            <p className="text-gray-400">{t('friends.no_friends')}</p>
          ) : (
            <div className="space-y-3">
              {friends.map((friend) => (
                <div key={`friend-${friend.friendship_id}-${friend.id}`} className="flex justify-between items-center bg-gray-700 p-4 rounded">
                  <div className="flex items-center gap-3">
                    <span className={`inline-block w-3 h-3 rounded-full ${friend.is_online ? 'bg-green-500' : 'bg-gray-500'}`}></span>
                    <Link href={`/user/${friend.username}`}>
                      <span className="font-medium hover:text-[#FACC15] transition cursor-pointer">{friend.emoji} {friend.username}</span>
                    </Link>
                  </div>
                  <div className="flex gap-2">
                    <Link href={`/user/${friend.username}`}>
                      <button className="px-4 py-1 bg-blue-600 hover:bg-blue-700 rounded text-sm transition">
                        {t('navbar.profile')}
                      </button>
                    </Link>
                    <button
                      onClick={() => handleRemoveFriend(friend.friendship_id, friend.id)}
                      className="px-4 py-1 bg-red-600 hover:bg-red-700 rounded text-sm transition"
                    >
                      {t('friends.remove')}
                    </button>
                  </div>
                </div>
              ))}
            </div>
          )}
        </div>

        {/* Received friend requests */}
        <div className="bg-gray-800 p-6 rounded-lg mb-8">
          <h2 className="text-xl font-semibold mb-4">{t('friends.received_requests')} ({friendRequests.length})</h2>
          {friendRequests.length === 0 ? (
            <p className="text-gray-400">{t('friends.no_requests')}</p>
          ) : (
            <div className="space-y-3">
              {friendRequests.map((request) => (
                <div key={`request-${request.id}-${request.from_user_id}`} className="flex justify-between items-center bg-gray-700 p-4 rounded">
                  <div className="flex items-center gap-3">
                    <Link href={`/user/${request.from_user}`}>
                      <span className="font-medium hover:text-[#FACC15] transition cursor-pointer">{request.emoji} {request.from_user}</span>
                    </Link>
                  </div>
                  <div className="flex gap-2">
                    <button
                      onClick={() => handleAcceptRequest(request.id, request.from_user_id)}
                      className="px-4 py-1 bg-green-600 hover:bg-green-700 rounded text-sm transition"
                    >
                      {t('navbar.accept')}
                    </button>
                    <button
                      onClick={() => handleRejectRequest(request.id)}
                      className="px-4 py-1 bg-red-600 hover:bg-red-700 rounded text-sm transition"
                    >
                      {t('navbar.reject')}
                    </button>
                  </div>
                </div>
              ))}
            </div>
          )}
        </div>

        {/* Pending requests */}
        <div className="bg-gray-800 p-6 rounded-lg">
          <h2 className="text-xl font-semibold mb-4">{t('friends.sent_requests')} ({pendingRequests.length})</h2>
          {pendingRequests.length === 0 ? (
            <p className="text-gray-400">{t('friends.no_pending_requests')}</p>
          ) : (
            <div className="space-y-3">
              {pendingRequests.map((request) => (
                <div key={`pending-${request.id}-${request.to_user_id}`} className="flex justify-between items-center bg-gray-700 p-4 rounded">
                  <div className="flex items-center gap-3">
                    <Link href={`/user/${request.to_user}`}>
                      <span className="font-medium hover:text-[#FACC15] transition cursor-pointer">{request.emoji} {request.to_user}</span>
                    </Link>
                  </div>
                  <button
                    onClick={() => handleCancelPending(request.id)}
                    className="px-4 py-1 bg-yellow-600 hover:bg-yellow-700 rounded text-sm transition"
                  >
                    {t('friends.cancel')}
                  </button>
                </div>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}
