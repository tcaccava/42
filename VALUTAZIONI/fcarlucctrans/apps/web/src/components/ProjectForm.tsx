'use client';

import { useState, type DragEvent } from 'react';
import { User } from '@/types';
import { FileArchive, GemIcon, ImageIcon, Trash2, X } from 'lucide-react';
import Image from 'next/image';
import { Input, Textarea } from '@/components/Input';
import { Button } from '@/components/Button';
import { Badge } from '@/components/Badge';
import { local } from '@/utilities';
import { PageHeader, PageShell, SectionCard } from '@/components/ui/layout';
import { FormLabel, dropZoneClassName } from '@/components/ui/form';
import { archiveLabelFromUrl } from '@/lib/project-files';
import { parsePublicRepositoryUrl } from '@/lib/repository-url';
import type { ScreenshotFileData } from '@/types/project';

export type ProjectFormValues = {
  subject: string;
  repositoryUrl: string;
  description: string;
  tags: string[];
  screenshotFiles: ScreenshotFileData[];
  existingScreenshotUrls: string[];
  archiveFile: File | null;
  removeExistingArchive: boolean;
};

type ProjectFormProps = {
  currentUser: User;
  projectSubject: string;
  initialRepositoryUrl: string;
  initialDescription: string;
  initialTags: string[];
  existingScreenshots?: string[] | null;
  existingSourceUrl?: string | null;
  isSubmitting?: boolean;
  onSubmit: (values: ProjectFormValues) => void | Promise<void>;
  onCancel: () => void;
  onDelete?: () => void | Promise<void>;
};

const MAX_FILE_SIZE = 2 * 1024 * 1024;
const ALLOWED_SCREENSHOT_MIME = ['image/png', 'image/jpeg', 'image/webp', 'image/gif'];
const ALLOWED_ARCHIVE_MIME = [
  'application/zip',
  'application/x-zip-compressed',
  'application/x-tar',
  'application/gzip',
  'application/x-gzip',
];

export function ProjectForm({
  currentUser,
  projectSubject,
  initialRepositoryUrl,
  initialDescription,
  initialTags,
  existingScreenshots,
  existingSourceUrl,
  isSubmitting = false,
  onSubmit,
  onCancel,
  onDelete,
}: ProjectFormProps) {
  const [repositoryUrl, setRepositoryUrl] = useState(initialRepositoryUrl);
  const [description, setDescription] = useState(initialDescription);
  const [tagInput, setTagInput] = useState('');
  const [tags, setTags] = useState<string[]>(initialTags);
  const [existingScreenshotUrls, setExistingScreenshotUrls] = useState<string[]>(
    existingScreenshots ?? [],
  );
  const [screenshotFiles, setScreenshotFiles] = useState<ScreenshotFileData[]>([]);
  const [archiveFile, setArchiveFile] = useState<File | null>(null);
  const [removeExistingArchive, setRemoveExistingArchive] = useState(false);
  const [screenshotError, setScreenshotError] = useState('');
  const [archiveError, setArchiveError] = useState('');
  const t = local.useLocalization().t;

  const existingArchiveName = archiveLabelFromUrl(existingSourceUrl);
  const showExistingArchive = Boolean(existingArchiveName) && !removeExistingArchive && !archiveFile;

  const handleAddTag = () => {
    if (tagInput.trim() && !tags.includes(tagInput.trim())) {
      setTags([...tags, tagInput.trim()]);
      setTagInput('');
    }
  };

  const handleRemoveTag = (tagToRemove: string) => {
    setTags(tags.filter((tag) => tag !== tagToRemove));
  };

  function addScreenshotFiles(fileList: FileList) {
    for (const file of fileList) {
      if (file.size > MAX_FILE_SIZE) {
        setScreenshotError('File is too big. Max 2 MB.');
        return;
      }
      if (!ALLOWED_SCREENSHOT_MIME.includes(file.type)) {
        setScreenshotError('Unsupported file format. Use only PNG/JPEG/WebP/GIF.');
        return;
      }
      setScreenshotError('');
      setScreenshotFiles((prev) => [...prev, { data: file, name: file.name }]);
    }
  }

  function setArchiveFromFile(file: File) {
    if (file.size > MAX_FILE_SIZE) {
      setArchiveError('File is too big. Max 2 MB.');
      return;
    }
    if (!ALLOWED_ARCHIVE_MIME.includes(file.type)) {
      setArchiveError('Unsupported file format. Use only zip/tar/tar.gz/tar.xz.');
      return;
    }
    setArchiveError('');
    setArchiveFile(file);
    setRemoveExistingArchive(false);
  }

  const removeNewScreenshot = (index: number) => {
    setScreenshotFiles((prev) => prev.filter((_, i) => i !== index));
  };

  const removeExistingScreenshot = (url: string) => {
    setExistingScreenshotUrls((prev) => prev.filter((item) => item !== url));
  };

  const removeArchive = () => {
    setArchiveError('');
    setArchiveFile(null);
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (isSubmitting) return;
    if (!parsePublicRepositoryUrl(repositoryUrl) || !description.trim()) return;

    await onSubmit({
      subject: projectSubject,
      repositoryUrl: parsePublicRepositoryUrl(repositoryUrl)!.normalized,
      description: description.trim(),
      tags,
      screenshotFiles,
      existingScreenshotUrls,
      archiveFile,
      removeExistingArchive,
    });
  };

  return (
    <PageShell maxWidth="2xl">
      <PageHeader
        icon={<GemIcon className="h-5 w-5 text-teal-600" />}
        title={t.updateProject}
        description="Update your project details, screenshots, and source archive."
      />

      <form onSubmit={handleSubmit} className="space-y-6">
        <SectionCard title="Author">
          <div className="flex items-center gap-3">
            <Image
              src={currentUser.image || '/globe.svg'}
              alt={currentUser.login}
              width={44}
              height={44}
              className="h-11 w-11 rounded-full border border-gray-200 object-cover"
            />
            <div>
              <p className="font-medium text-gray-900">{currentUser.login}</p>
              <p className="text-sm text-gray-600">{currentUser.campus ?? '42 Campus'}</p>
            </div>
          </div>
        </SectionCard>

        <SectionCard title="Project details">
          <div className="space-y-4">
            <div>
              <FormLabel>Subject</FormLabel>
              <Badge variant="status" className="mt-1 uppercase tracking-wide">
                {projectSubject}
              </Badge>
            </div>

            <Input
              label={t.projectRepositoryUrl}
              id="project-repository-url"
              type="url"
              inputMode="url"
              placeholder={t.projectRepositoryUrlPlaceholder}
              value={repositoryUrl}
              onChange={(e) => setRepositoryUrl(e.target.value)}
              required
            />
            <p className="text-sm text-gray-500">{t.projectRepositoryUrlHelp}</p>

            <Textarea
              label="Description"
              id="description"
              placeholder="Tell us about your project..."
              value={description}
              onChange={(e) => setDescription(e.target.value)}
              rows={5}
              required
            />
          </div>
        </SectionCard>

        <SectionCard
          title="Screenshots"
          description="Add new images or remove existing ones. Changes apply when you save."
          icon={<ImageIcon className="h-4 w-4 text-teal-600" />}
        >
          <div className="space-y-4">
            {existingScreenshotUrls.length > 0 ? (
              <div className="grid grid-cols-2 gap-3 sm:grid-cols-3">
                {existingScreenshotUrls.map((url) => (
                  <div
                    key={url}
                    className="group relative overflow-hidden rounded-xl border border-gray-200 bg-gray-50"
                  >
                    {/* eslint-disable-next-line @next/next/no-img-element */}
                    <img
                      src={url}
                      alt="Project screenshot"
                      className="aspect-video w-full object-cover"
                    />
                    <button
                      type="button"
                      onClick={() => removeExistingScreenshot(url)}
                      className="absolute right-2 top-2 rounded-full bg-black/60 p-1.5 text-white opacity-0 transition-opacity group-hover:opacity-100 focus:opacity-100"
                      aria-label="Remove screenshot"
                    >
                      <X className="h-4 w-4" />
                    </button>
                  </div>
                ))}
              </div>
            ) : (
              <p className="text-sm text-gray-500">No screenshots yet.</p>
            )}

            <div
              onDragOver={(e) => e.preventDefault()}
              onDrop={(e: DragEvent) => {
                e.preventDefault();
                if (e.dataTransfer?.files) addScreenshotFiles(e.dataTransfer.files);
              }}
              className={dropZoneClassName}
              onClick={() => document.getElementById('project-screenshot-input')?.click()}
            >
              <div className="text-sm text-gray-500">{t.dragDropScreens}.</div>
            </div>
            <input
              id="project-screenshot-input"
              type="file"
              accept="image/png,image/jpeg,image/webp,image/gif"
              className="hidden"
              multiple
              onChange={(e) => {
                if (e.target.files) addScreenshotFiles(e.target.files);
              }}
            />
            {screenshotError ? (
              <p className="text-sm text-red-600">{screenshotError}</p>
            ) : null}

            {screenshotFiles.length > 0 ? (
              <div className="space-y-2">
                {screenshotFiles.map((file, index) => (
                  <div
                    key={`${file.name}-${index}`}
                    className="flex items-center justify-between rounded-lg border border-gray-200 bg-gray-50 px-3 py-2"
                  >
                    <span className="truncate text-sm text-gray-700">{file.name}</span>
                    <button
                      type="button"
                      onClick={() => removeNewScreenshot(index)}
                      className="ml-3 inline-flex shrink-0 items-center gap-1 text-sm font-medium text-red-600 hover:text-red-700"
                    >
                      <Trash2 className="h-3.5 w-3.5" />
                      {t.delete}
                    </button>
                  </div>
                ))}
              </div>
            ) : null}
          </div>
        </SectionCard>

        <SectionCard
          title={t.projectArchive}
          description="Replace or remove the uploaded source archive."
          icon={<FileArchive className="h-4 w-4 text-teal-600" />}
        >
          <div className="space-y-4">
            {showExistingArchive ? (
              <div className="flex items-center justify-between rounded-lg border border-gray-200 bg-gray-50 px-3 py-2">
                <span className="truncate text-sm text-gray-700">{existingArchiveName}</span>
                <button
                  type="button"
                  onClick={() => setRemoveExistingArchive(true)}
                  className="ml-3 inline-flex shrink-0 items-center gap-1 text-sm font-medium text-red-600 hover:text-red-700"
                >
                  <Trash2 className="h-3.5 w-3.5" />
                  {t.delete}
                </button>
              </div>
            ) : null}

            <div
              onDragOver={(e) => e.preventDefault()}
              onDrop={(e: DragEvent) => {
                e.preventDefault();
                const file = e.dataTransfer?.files?.[0];
                if (file) setArchiveFromFile(file);
              }}
              className={dropZoneClassName}
              onClick={() => document.getElementById('project-archive-input')?.click()}
            >
              <div className="text-sm text-gray-500">
                {showExistingArchive ? 'Upload a new archive to replace the current one.' : t.dragDropArchive}
                .
              </div>
            </div>
            <input
              id="project-archive-input"
              type="file"
              accept=".zip,.tar,.tar.gz,.tar.xz"
              className="hidden"
              onChange={(e) => {
                const file = e.target.files?.[0];
                if (file) setArchiveFromFile(file);
              }}
            />
            {archiveError ? <p className="text-sm text-red-600">{archiveError}</p> : null}

            {archiveFile ? (
              <div className="flex items-center justify-between rounded-lg border border-gray-200 bg-gray-50 px-3 py-2">
                <span className="truncate text-sm text-gray-700">{archiveFile.name}</span>
                <button
                  type="button"
                  onClick={removeArchive}
                  className="ml-3 inline-flex shrink-0 items-center gap-1 text-sm font-medium text-red-600 hover:text-red-700"
                >
                  <Trash2 className="h-3.5 w-3.5" />
                  {t.delete}
                </button>
              </div>
            ) : null}
          </div>
        </SectionCard>

        <SectionCard title="Tags">
          <div className="flex gap-2">
            <Input
              type="text"
              placeholder={t.addTagPlaceholder}
              value={tagInput}
              onChange={(e) => setTagInput(e.target.value)}
              onKeyDown={(e) => e.key === 'Enter' && (e.preventDefault(), handleAddTag())}
            />
            <Button type="button" onClick={handleAddTag} variant="secondary">
              {t.add}
            </Button>
          </div>
          {tags.length > 0 ? (
            <div className="mt-3 flex flex-wrap gap-2">
              {tags.map((tag) => (
                <Badge key={tag} variant="tag">
                  {tag}
                  <button
                    type="button"
                    onClick={() => handleRemoveTag(tag)}
                    className="ml-1 hover:text-red-600"
                  >
                    <X className="h-3 w-3" />
                  </button>
                </Badge>
              ))}
            </div>
          ) : (
            <p className="mt-2 text-sm text-gray-500">No tags yet.</p>
          )}
        </SectionCard>

        <SectionCard contentClassName="p-4 sm:p-6">
          <div className="flex flex-col gap-3 sm:flex-row">
            <Button type="submit" className="w-full sm:flex-1" disabled={isSubmitting}>
              <GemIcon className="mr-2 h-4 w-4" />
              {isSubmitting ? t.updating : t.updateProject}
            </Button>
            <Button
              type="button"
              variant="secondary"
              className="w-full sm:flex-1"
              onClick={onCancel}
              disabled={isSubmitting}
            >
              {t.cancel}
            </Button>
          </div>

          {onDelete ? (
            <Button
              type="button"
              variant="danger"
              className="mt-3 w-full"
              disabled={isSubmitting}
              onClick={() => void onDelete()}
            >
              {t.deleteProject}
            </Button>
          ) : null}
        </SectionCard>
      </form>
    </PageShell>
  );
}
