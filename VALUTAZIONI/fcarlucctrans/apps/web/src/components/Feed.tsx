'use client';

import { useState } from 'react';
import PostCard from '@/components/PostCard';
import { Post, User } from '@/types';
import { EmptyState } from '@/components/ui/layout';
import { Newspaper } from 'lucide-react';
import { local } from '../utilities';

type FeedProps = {
  posts: Post[];
  currentUser: User;
  onLike?: (postId: string, liked: boolean) => void;
  onAddComment?: (postId: string, text: string) => void;
  onReactToComment?: (
    postId: string,
    commentId: string,
    reactionType: 'like' | 'heart' | 'fire' | 'clap',
  ) => void;
};

export function Feed({
  posts: initialPosts,
  currentUser,
  onLike,
  onAddComment,
  onReactToComment,
}: FeedProps) {
  const [posts, setPosts] = useState<Post[]>(initialPosts);
  const t = local.useLocalization().t;

  const handleLike = (postId: string, liked: boolean) => {
    setPosts(
      posts.map((post) =>
        post.id === postId
          ? { ...post, likes: Math.max(0, post.likes + (liked ? 1 : -1)) }
          : post,
      ),
    );
    onLike?.(postId, liked);
  };

  const handleAddReaction = (
    postId: string,
    commentId: string,
    reactionType: 'like' | 'heart' | 'fire' | 'clap',
  ) => {
    onReactToComment?.(postId, commentId, reactionType);
  };

  const handleAddComment = (postId: string, text: string) => {
    const newComment = {
      id: Date.now().toString(),
      login: currentUser.login,
      image: currentUser.image ?? '/globe.svg',
      campus: currentUser.campus ?? '',
      text,
      timestamp: t.justNow ?? 'Just now',
      reactions: [],
    };
    setPosts(
      posts.map((post) =>
        post.id === postId ? { ...post, comments: [...post.comments, newComment] } : post,
      ),
    );
    onAddComment?.(postId, text);
  };

  const handleRate = (postId: string, rating: number) => {
    setPosts(posts.map((post) => (post.id === postId ? { ...post, rating } : post)));
  };

  if (posts.length === 0) {
    return (
      <EmptyState
        icon={<Newspaper className="h-10 w-10 text-gray-400" />}
        message="No posts yet. Publish a project to share it with the community."
      />
    );
  }

  return (
    <div className="space-y-6">
      {posts.map((post) => (
        <PostCard
          key={post.id}
          post={post}
          currentUser={currentUser}
          onAddReaction={handleAddReaction}
          onLike={handleLike}
          onRate={handleRate}
          onAddComment={handleAddComment}
        />
      ))}
    </div>
  );
}
