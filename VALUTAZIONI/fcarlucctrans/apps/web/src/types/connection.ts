export type ConnectionRelation =
  | 'none'
  | 'connected'
  | 'pending_outgoing'
  | 'pending_incoming'
  | 'self';

export type ConnectionPeer = {
  userId: string;
  login: string;
  image?: string | null;
  displayname?: string | null;
  first_name?: string | null;
  last_name?: string | null;
};

export type ConnectionItem = {
  id: string;
  status: 'PENDING' | 'ACCEPTED' | 'REJECTED';
  requesterId: string;
  addresseeId: string;
  peerId: string;
  direction: 'outgoing' | 'incoming';
  peer?: ConnectionPeer;
  createdAt: string;
  updatedAt: string;
};

export type ConnectionsResponse = {
  status: 'success' | 'error' | 'unauthorized';
  connections?: ConnectionItem[];
  message?: string;
};

export type ConnectionActionResponse = {
  status: 'success' | 'error' | 'unauthorized';
  connection?: ConnectionItem;
  message?: string;
  autoAccepted?: boolean;
};

export type PublicProfileResponse = {
  status: 'success' | 'error' | 'unauthorized';
  user?: {
    userId: string;
    login: string;
    email: string;
    image?: string;
    displayname?: string;
    first_name?: string;
    last_name?: string;
    pool_month?: string;
    pool_year?: number;
    location?: string;
    grade?: string;
    kind?: string;
    status?: string;
    bio?: string;
    username?: string;
    website?: string;
    github?: string;
    linkedin?: string;
    campus?: string;
  };
  isOwnProfile?: boolean;
  connection?: {
    relation: ConnectionRelation;
    connectionId?: string;
  };
  connectionCount?: number;
  isActive?: boolean;
  message?: string;
};
