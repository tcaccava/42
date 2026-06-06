'use client';

import { User } from '@/types';
import type { GradedProjectCard } from '@/types/feed';
import { ProfileProjects } from '@/components/ProfileProjects';
import {
  Mail,
  Calendar,
  MapPin,
  Book,
  Edit2,
  Github,
  Linkedin,
  Globe,
  UserCircle,
  Save,
  Upload,
} from 'lucide-react';
import Image from 'next/image';
import { Button } from '@/components/Button';
import { ConnectButton } from '@/components/ConnectButton';
import { ActiveStatusBadge } from '@/components/ActiveStatusBadge';
import { useState } from 'react';
import { local } from '@/utilities';
import type { ConnectionRelation } from '@/types/connection';
import { updateMyEditableProfile } from '@/server/user-profile';
import { AlertBanner } from '@/components/ui/layout';
import { compressImageFile } from '@/lib/compress-image';

const MAX_COVER_SIZE = 2 * 1024 * 1024;
const ALLOWED_COVER_MIME = ['image/png', 'image/jpeg', 'image/webp', 'image/gif'];

type ProfileProps = {
  user: User;
  projects: GradedProjectCard[];
  projectsPageCount: number;
  isOwnProfile?: boolean;
  connectionRelation?: ConnectionRelation;
  connectionId?: string;
  connectionCount?: number;
  isActive?: boolean;
};

interface EditFormData {
  bio: string;
  username: string;
  website: string;
  github: string;
  linkedin: string;
  coverImage: string;
  location: string;
  campus: string;
}

function InfoBadge({ icon, label, value }: { icon: React.ReactNode; label: string; value: React.ReactNode }) {
  if (!value && value !== 0 && value !== '') return null;
  return (
    <div className="flex items-center gap-2 px-3 py-2 bg-gray-50 rounded-lg border border-gray-200 min-w-[140px]">
      <div className="text-gray-500 text-sm">{icon}</div>
      <div className="text-xs flex-1">
        <p className="font-semibold text-gray-500 uppercase mb-0.5">{label}</p>
        <div className="font-medium text-gray-900">{value}</div>
      </div>
    </div>
  );
}

function SocialIcon({ icon, label, url, isEditing, value, onChange }: {
  icon: React.ReactNode
  label: string
  url?: string
  isEditing?: boolean
  value?: string
  onChange?: (val: string) => void
}) {
  if (isEditing) {
    return (
      <div className="flex items-center gap-2 w-full">
         <div className="text-gray-500 text-sm w-4" aria-hidden="true">{icon}</div>
         <input
          type="url"
          value={value || ''}
          onChange={(e) => onChange?.(e.target.value)}
          className="flex-1 px-2 py-1 text-sm border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-teal-500"
          placeholder={label}
        />
      </div>
    );
  }

  if (!url) return null;

  return (
    <a
      href={url}
      target="_blank"
      rel="noopener noreferrer"
      className="text-gray-400 hover:text-gray-900 transition-colors"
      aria-label={label}
    >
      {icon}
    </a>
  );
}

export function Profile({
  user,
  projects,
  projectsPageCount,
  isOwnProfile = false,
  connectionRelation = 'none',
  connectionId,
  connectionCount,
  isActive,
}: ProfileProps) {
  const [isEditing, setIsEditing] = useState(false);
  const [isSaving, setIsSaving] = useState(false);
  const [saveError, setSaveError] = useState<string | null>(null);
  const [coverError, setCoverError] = useState('');
  const [displayUser, setDisplayUser] = useState(user);
  const [editData, setEditData] = useState<EditFormData>({
    bio: user.bio || '',
    username: user.username || user.login || '',
    website: user.website || '',
    github: user.github || '',
    linkedin: user.linkedin || '',
    coverImage: user.coverImage || '',
    location: user.location || '',
    campus: user.campus || '',
  });

  const handleCoverFile = async (file: File) => {
    if (file.size > MAX_COVER_SIZE) {
      setCoverError('File is too big. Max 2 MB.');
      return;
    }
    if (!ALLOWED_COVER_MIME.includes(file.type)) {
      setCoverError('Unsupported file format. Use only PNG/JPEG/WebP/GIF.');
      return;
    }
    setCoverError('');
    try {
      const dataUrl = await compressImageFile(file, { maxHeight: 480 });
      setEditData((prev) => ({ ...prev, coverImage: dataUrl }));
    } catch {
      setCoverError('Could not process image.');
    }
  };

  const resetEditData = () => {
    setEditData({
      bio: displayUser.bio || '',
      username: displayUser.username || displayUser.login || '',
      website: displayUser.website || '',
      github: displayUser.github || '',
      linkedin: displayUser.linkedin || '',
      coverImage: displayUser.coverImage || '',
      location: displayUser.location || '',
      campus: displayUser.campus || '',
    });
  };

  const handleCancel = () => {
    resetEditData();
    setSaveError(null);
    setCoverError('');
    setIsEditing(false);
  };

  const handleSave = async () => {
    if (!isOwnProfile) return;
    setIsSaving(true);
    setSaveError(null);
    const result = await updateMyEditableProfile({
      bio: editData.bio,
      username: editData.username,
      website: editData.website,
      github: editData.github,
      linkedin: editData.linkedin,
      campus: editData.campus,
      coverImage: editData.coverImage || undefined,
    });

    if (result.status !== 'success') {
      setSaveError(result.message ?? 'Failed to save profile.');
      setIsSaving(false);
      return;
    }

    setDisplayUser({
      ...displayUser,
      bio: editData.bio,
      username: editData.username,
      website: editData.website,
      github: editData.github,
      linkedin: editData.linkedin,
      coverImage: editData.coverImage,
      campus: editData.campus,
    });
    setIsEditing(false);
    setIsSaving(false);
  };

  const poolDisplay = displayUser.pool_month && displayUser.pool_year ? `${displayUser.pool_month} ${displayUser.pool_year}` : 'N/A';
  const fullName = `${displayUser.first_name || ''} ${displayUser.last_name || ''}`.trim();
  const hasSocialLinks = displayUser.website || displayUser.github || displayUser.linkedin;
  const t = local.useLocalization().t;
  const coverSrc = isEditing ? editData.coverImage : displayUser.coverImage;

  return (
    <div className="max-w-4xl mx-auto space-y-6 pb-8">
      {saveError ? <AlertBanner kind="error">{saveError}</AlertBanner> : null}
      {/* Main Profile Card */}
      <div className="bg-white rounded-2xl shadow-lg overflow-hidden">
        {/* Cover Image */}
        <div className="relative h-48 bg-gradient-to-r from-blue-500 via-teal-500 to-green-500 overflow-hidden">
          {coverSrc ? (
            // eslint-disable-next-line @next/next/no-img-element
            <img
              src={coverSrc}
              alt=""
              className="absolute inset-0 h-full w-full object-cover"
            />
          ) : null}
          {isEditing ? (
            <div className="absolute inset-0 flex items-center justify-center bg-black/40 z-10">
              <label className="cursor-pointer flex flex-col items-center gap-2 hover:scale-110 transition-transform">
                <Upload className="w-8 h-8 text-white" />
                <span className="text-white font-medium text-sm">{t.coverClickUpload}</span>
                <input
                  type="file"
                  className="hidden"
                  accept="image/png,image/jpeg,image/webp,image/gif"
                  onChange={(e) => {
                    const file = e.target.files?.[0];
                    if (file) void handleCoverFile(file);
                  }}
                />
              </label>
            </div>
          ) : null}
        </div>
        {coverError && isEditing ? (
          <p className="px-8 pt-2 text-sm text-red-600">{coverError}</p>
        ) : null}

        {/* Content */}
        <div className="px-8 pb-8 relative">

          {/* Top Row: Avatar | Info Area | Edit Button */}
          <div className="flex flex-col sm:flex-row gap-6 -mt-16 items-start">

            {/* Avatar with Ping */}
            <div className="relative flex-shrink-0 z-10 w-32 h-32 sm:w-40 sm:h-40">
              <div className="relative w-full h-full rounded-full border-4 border-white shadow-xl bg-white overflow-hidden">
                <Image
                  src={displayUser.image || '/globe.svg'}
                  alt={displayUser.login}
                  fill
                  className="object-cover"
                />
              </div>
              {/* Online indicator */}
              {(isActive ?? displayUser.isActive) ? (
                <ActiveStatusBadge
                  isActive={isActive ?? displayUser.isActive}
                  className="bottom-2 right-2 h-5 w-5"
                />
              ) : null}
            </div>

            {/* User Info Area */}
            <div className="flex-1 pt-4 sm:pt-8 md:pt-16 w-full text-center sm:text-left">
              <div className="flex flex-col sm:flex-row sm:items-start justify-between gap-4 w-full">

                {/* Full Name & Socials */}
                <div className="flex flex-col gap-1 w-full sm:w-auto">
                   <h1 className="text-3xl sm:text-4xl font-bold text-gray-900 leading-tight">
                     {fullName || displayUser.login}
                   </h1>

                   <div className="flex items-center justify-center sm:justify-start gap-3 flex-wrap">
                      <p className="text-gray-500 font-medium text-lg">@{displayUser.login}</p>

                      {/* Social Icons next to username */}
                      {!isEditing && hasSocialLinks && (
                        <div className="flex items-center gap-2">
                            {displayUser.website && <SocialIcon icon={<Globe className="w-4 h-4" />} url={displayUser.website} label="Website" />}
                            {displayUser.github && <SocialIcon icon={<Github className="w-4 h-4" />} url={displayUser.github} label="GitHub" />}
                            {displayUser.linkedin && <SocialIcon icon={<Linkedin className="w-4 h-4" />} url={displayUser.linkedin} label="LinkedIn" />}
                        </div>
                      )}
                   </div>
                </div>

                {/* Edit / Connect */}
                <div className="mt-4 shrink-0 self-center sm:mt-2 sm:self-start">
                  {!isOwnProfile && displayUser.userId && (
                    <ConnectButton
                      targetUserId={displayUser.userId}
                      targetLogin={displayUser.login}
                      relation={connectionRelation}
                      connectionId={connectionId}
                      connectionCount={connectionCount}
                    />
                  )}
                  {isOwnProfile && !isEditing && (
                    <Button
                      onClick={() => setIsEditing(true)}
                      size="sm"
                      variant="secondary"
                      className="hover:bg-gray-100 border border-gray-300 hover:text-red-600 transition-colors"
                    >
                      <Edit2 className="w-4 h-4 mr-2 text-black" />
                      {t.editProfile}
                    </Button>
                  )}
                </div>
              </div>
            </div>
          </div>

          {/* Edit Mode: Social Links Form */}
          {isEditing && (
             <div className="my-6 p-6 bg-gray-50 rounded-xl border border-gray-200 shadow-sm transition-all duration-300">
                <h3 className="text-xs font-bold text-gray-500 uppercase tracking-wider mb-4 flex items-center gap-2">
                  <Globe className="w-3 h-3" />
                  {t.socialPresence}
                </h3>
                <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
                  <SocialIcon
                    icon={<Globe className="w-4 h-4" />}
                    label="Website"
                    isEditing
                    value={editData.website}
                    onChange={(val) => setEditData({ ...editData, website: val })}
                  />
                  <SocialIcon
                    icon={<Github className="w-4 h-4" />}
                    label="GitHub"
                    isEditing
                    value={editData.github}
                    onChange={(val) => setEditData({ ...editData, github: val })}
                  />
                  <SocialIcon
                    icon={<Linkedin className="w-4 h-4" />}
                    label="LinkedIn"
                    isEditing
                    value={editData.linkedin}
                    onChange={(val) => setEditData({ ...editData, linkedin: val })}
                  />
                </div>
             </div>
          )}

          {/* Bio Section */}
          <div className="mt-6 mb-8">
             {isEditing ? (
              <div className="space-y-3 transition-all duration-300 delay-75">
                <label className="text-xs font-bold text-gray-500 uppercase tracking-wider flex items-center gap-2">
                  <UserCircle className="w-3 h-3" />
                  {t.aboutMe}
                </label>
                <textarea
                  value={editData.bio}
                  onChange={(e) => setEditData({ ...editData, bio: e.target.value })}
                  placeholder={t.aboutMePlaceholder}
                  className="w-full px-4 py-3 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-teal-500/50 focus:border-teal-500 resize-none bg-gray-50 focus:bg-white transition-all shadow-sm text-gray-700 text-sm leading-relaxed"
                  rows={4}
                />
              </div>
            ) : (
               <div className="relative group">
                  <div className="absolute -inset-1 bg-gradient-to-r from-teal-50 to-blue-50 rounded-lg opacity-0 group-hover:opacity-100 transition-opacity duration-500" />
                  <div className="relative text-sm max-w-none text-gray-600 leading-relaxed p-2">
                    {displayUser.bio || 'No bio provided.'}
                  </div>
               </div>
            )}
          </div>

          {/* Divider */}
          <div className="h-px bg-gray-100 my-8 w-full" />

          {/* 42 Data Section */}
          <div>
            <div className="flex flex-wrap justify-center gap-4">
              <InfoBadge icon={<Mail className="w-4 h-4" />} label="Email" value={displayUser.email} />

              {/* Campus Editable Box */}
              <InfoBadge
                icon={<MapPin className="w-4 h-4" />}
                label="Campus"
                value={
                  isEditing ? (
                    <select
                      value={editData.campus}
                      onChange={(e) => setEditData({...editData, campus: e.target.value})}
                      className="w-full bg-transparent border-b border-gray-300 focus:border-teal-500 focus:outline-none text-gray-900 px-1 py-0.5"
                    >
                      <option value="" disabled>{t.selectCampus}</option>
                      <option value="42 Paris">42 {t.paris}</option>
                      <option value="42 Rome">42 {t.rome}</option>
                      <option value="42 Florence">42 {t.florence}</option>
                      <option value="42 Heilbronn">42 {t.heilbronn}</option>
                      <option value="42 Barcelona">42 {t.barcelona}</option>
                      <option value="42 London">42 {t.london}</option>
                      <option value="42 Berlin">42 {t.berlin}</option>
                    </select>
                  ) : (displayUser.campus || 'Unknown')
                }
              />

              <InfoBadge icon={<Calendar className="w-4 h-4" />} label="Pool" value={poolDisplay} />
              <InfoBadge icon={<Book className="w-4 h-4" />} label="Rank" value={displayUser.kind} />
            </div>
          </div>

           {/* Edit Actions Footer */}
           {isEditing && (
            <div className="mt-8 flex items-center justify-end gap-3 pt-6 border-t border-gray-100">
              <Button
                onClick={handleCancel}
                variant="ghost"
                className="text-gray-500 hover:text-gray-700"
                disabled={isSaving}
              >
                {t.cancel}
              </Button>
              <Button onClick={handleSave} disabled={isSaving}>
                <Save className="w-4 h-4 mr-2" />
                {t.saveChanges}
              </Button>
            </div>
          )}
        </div>
      </div>


      {/* Projects Section */}
      <div className="rounded-2xl bg-white p-6 shadow-lg sm:p-8">
        <h2 className="mb-6 text-2xl font-bold text-gray-900">{t.projects}</h2>
        <ProfileProjects
          userLogin={displayUser.login}
          initialProjects={projects}
          initialPageCount={projectsPageCount}
          isOwnProfile={isOwnProfile}
        />
      </div>
    </div>
  );
}
