'use client';

import { useState, type DragEvent } from 'react';
import { useRouter } from 'next/navigation';
import { GemIcon, X } from 'lucide-react';
import Image from 'next/image';
import type { User } from '@/types';
import type { ProjectKind, ScreenshotFileData } from '@/types/project';
import { ProjectKindValues } from '@/types/project';
import { Input, Textarea } from '@/components/Input';
import { Button } from '@/components/Button';
import { Badge } from '@/components/Badge';
import { local } from '@/utilities';
import { AlertBanner, PageHeader, PageShell, SectionCard } from '@/components/ui/layout';
import { FormLabel, SelectField, dropZoneClassName } from '@/components/ui/form';
import { StepProgressBar } from '@/components/ui/step-progress';
import { newProject, updateProject } from '@/server/project';
import { uploadScreenshots, uploadSource } from '@/server/projectClient';
import { parsePublicRepositoryUrl } from '@/lib/repository-url';

const STEPS = ['Project details', 'Files', 'Tags & publish'];

const MAX_FILE_SIZE = 2 * 1024 * 1024;
const ALLOWED_SCREENSHOT_MIME = ['image/png', 'image/jpeg', 'image/webp', 'image/gif'];
const ALLOWED_ARCHIVE_MIME = [
  'application/zip',
  'application/x-zip-compressed',
  'application/x-tar',
  'application/gzip',
  'application/x-gzip',
];

type WizardFormValues = {
  subject: string;
  repositoryUrl: string;
  description: string;
  tags: string[];
  screenshotFiles: ScreenshotFileData[];
  archiveFile: File | null;
};

const emptyValues: WizardFormValues = {
  subject: '',
  repositoryUrl: '',
  description: '',
  tags: [],
  screenshotFiles: [],
  archiveFile: null,
};

export function CreateProjectClient({ user }: { user: User }) {
  const router = useRouter();
  const t = local.useLocalization().t;
  const [step, setStep] = useState(1);
  const [values, setValues] = useState<WizardFormValues>(emptyValues);
  const [tagInput, setTagInput] = useState('');
  const [screenshotError, setScreenshotError] = useState('');
  const [archiveError, setArchiveError] = useState('');
  const [stepError, setStepError] = useState<string | null>(null);
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [uploadProgress, setUploadProgress] = useState<number | null>(null);

  const setField = <K extends keyof WizardFormValues>(key: K, value: WizardFormValues[K]) => {
    setValues((prev) => ({ ...prev, [key]: value }));
  };

  const validateStep = (targetStep: number): string | null => {
    if (targetStep >= 2) {
      if (!values.subject) return t.projectSelect;
      if (!parsePublicRepositoryUrl(values.repositoryUrl)) {
        return t.projectRepositoryUrlInvalid;
      }
      if (!values.description.trim()) return 'Description is required.';
    }
    return null;
  };

  const goNext = () => {
    const error = validateStep(step + 1);
    if (error) {
      setStepError(error);
      return;
    }
    setStepError(null);
    setStep((s) => Math.min(s + 1, STEPS.length));
  };

  const goBack = () => {
    setStepError(null);
    setStep((s) => Math.max(s - 1, 1));
  };

  const handleAddTag = () => {
    if (tagInput.trim() && !values.tags.includes(tagInput.trim())) {
      setField('tags', [...values.tags, tagInput.trim()]);
      setTagInput('');
    }
  };

  const addScreenshotFiles = (fileList: FileList) => {
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
      setValues((prev) => ({
        ...prev,
        screenshotFiles: [...prev.screenshotFiles, { data: file, name: file.name }],
      }));
    }
  };

  const setArchiveFromFile = (file: File) => {
    if (file.size > MAX_FILE_SIZE) {
      setArchiveError('File is too big. Max 2 MB.');
      return;
    }
    if (!ALLOWED_ARCHIVE_MIME.includes(file.type)) {
      setArchiveError('Unsupported file format. Use only zip/tar/tar.gz/tar.xz.');
      return;
    }
    setArchiveError('');
    setField('archiveFile', file);
  };

  const removeScreenshot = (index: number) => {
    setValues((prev) => ({
      ...prev,
      screenshotFiles: prev.screenshotFiles.filter((_, i) => i !== index),
    }));
  };

  const removeArchive = () => {
    setArchiveError('');
    setField('archiveFile', null);
  };

  const handlePublish = async () => {
    const error = validateStep(3);
    if (error) {
      setStepError(error);
      setStep(1);
      return;
    }

    setStepError(null);
    setIsSubmitting(true);
    setUploadProgress(0);

    try {
      const parsedRepo = parsePublicRepositoryUrl(values.repositoryUrl);
      if (!parsedRepo) {
        setStepError(t.projectRepositoryUrlInvalid);
        setStep(1);
        return;
      }

      setUploadProgress(0.15);
      const created = (await newProject(
        values.subject as ProjectKind,
        parsedRepo.normalized,
      )) as { id?: number } | null;

      if (!created?.id) {
        setStepError('Could not create project. Please try again.');
        return;
      }

      setUploadProgress(0.35);
      const updated = await updateProject({
        id: created.id,
        repositoryUrl: parsedRepo.normalized,
        description: values.description.trim(),
        tagList: values.tags,
      });

      if (!updated) {
        setStepError('Project was created but details could not be saved.');
        return;
      }

      if (values.screenshotFiles.length > 0) {
        setUploadProgress(0.55);
        const uploaded = await uploadScreenshots(
          created.id,
          values.screenshotFiles,
          (event) => {
            if (event.total) {
              setUploadProgress(0.55 + (event.loaded / event.total) * 0.25);
            }
          },
        );
        if (!uploaded) throw new Error('Failed to upload project screenshots.');
      }

      if (values.archiveFile) {
        setUploadProgress(0.85);
        const uploaded = await uploadSource(created.id, values.archiveFile, (event) => {
          if (event.total) {
            setUploadProgress(0.85 + (event.loaded / event.total) * 0.15);
          }
        });
        if (!uploaded) throw new Error('Failed to upload project source archive.');
      }

      setUploadProgress(1);
      router.push(`/project/${created.id}`);
      router.refresh();
    } catch (err) {
      setStepError(err instanceof Error ? err.message : 'Could not publish project.');
    } finally {
      setIsSubmitting(false);
      setUploadProgress(null);
    }
  };

  return (
    <PageShell maxWidth="2xl">
      <PageHeader
        icon={<GemIcon className="h-5 w-5 text-teal-600" />}
        title={t.shareProject}
        description="Complete each step to publish your 42 project."
      />

      {stepError ? <AlertBanner kind="error">{stepError}</AlertBanner> : null}

      <SectionCard>
        <StepProgressBar
          steps={STEPS}
          currentStep={step}
          uploadProgress={isSubmitting ? uploadProgress : null}
        />

        <div className="mb-6 flex items-center gap-3 border-b border-gray-200 pb-4">
          <Image
            src={user.image || '/globe.svg'}
            alt={user.login}
            width={40}
            height={40}
            className="h-10 w-10 rounded-full object-cover"
          />
          <div>
            <p className="font-medium text-gray-900">{user.login}</p>
            <p className="text-sm text-gray-600">{user.campus}</p>
          </div>
        </div>

        {step === 1 && (
          <div className="space-y-4">
            <SelectField
              id="project-subject"
              label={t.projectSubject}
              value={values.subject}
              onChange={(e) => setField('subject', e.target.value)}
              required
            >
              <option value="">-- {t.select42Project} --</option>
              {ProjectKindValues.map((p) => (
                <option key={p} value={p}>
                  {p}
                </option>
              ))}
            </SelectField>

            <Input
              label={t.projectRepositoryUrl}
              id="project-repository-url"
              type="url"
              inputMode="url"
              placeholder={t.projectRepositoryUrlPlaceholder}
              value={values.repositoryUrl}
              onChange={(e) => setField('repositoryUrl', e.target.value)}
              required
            />
            <p className="text-sm text-foreground/60">{t.projectRepositoryUrlHelp}</p>

            <Textarea
              label="Description"
              id="description"
              placeholder="Tell us about your project..."
              value={values.description}
              onChange={(e) => setField('description', e.target.value)}
              rows={4}
              required
            />
          </div>
        )}

        {step === 2 && (
          <div className="space-y-6">
            <div>
              <FormLabel>{t.screenshot}</FormLabel>
              <div
                onDragOver={(e) => e.preventDefault()}
                onDrop={(e: DragEvent) => {
                  e.preventDefault();
                  if (e.dataTransfer?.files) addScreenshotFiles(e.dataTransfer.files);
                }}
                className={dropZoneClassName}
                onClick={() => document.getElementById('wizard-screenshot-input')?.click()}
              >
                <div className="text-sm text-gray-500">{t.dragDropScreens}.</div>
              </div>
              <input
                id="wizard-screenshot-input"
                type="file"
                accept="image/png,image/jpeg,image/webp,image/gif"
                className="hidden"
                multiple
                onChange={(e) => {
                  if (e.target.files) addScreenshotFiles(e.target.files);
                }}
              />
              {screenshotError ? (
                <p className="mt-1 text-sm text-red-600">{screenshotError}</p>
              ) : null}
              {values.screenshotFiles.length > 0 ? (
                <div className="mt-3 space-y-2">
                  {values.screenshotFiles.map((file, index) => (
                    <div
                      key={`${file.name}-${index}`}
                      className="flex items-center justify-between rounded-lg border border-gray-200 bg-gray-50 px-3 py-2"
                    >
                      <span className="truncate text-sm text-gray-700">{file.name}</span>
                      <button
                        type="button"
                        onClick={() => removeScreenshot(index)}
                        className="ml-3 shrink-0 text-sm font-medium text-red-600 hover:text-red-700"
                      >
                        {t.delete}
                      </button>
                    </div>
                  ))}
                </div>
              ) : null}
            </div>

            <div>
              <FormLabel>{t.projectArchive}</FormLabel>
              <div
                onDragOver={(e) => e.preventDefault()}
                onDrop={(e: DragEvent) => {
                  e.preventDefault();
                  const file = e.dataTransfer?.files?.[0];
                  if (file) setArchiveFromFile(file);
                }}
                className={dropZoneClassName}
                onClick={() => document.getElementById('wizard-archive-input')?.click()}
              >
                <div className="text-sm text-gray-500">{t.dragDropArchive}.</div>
              </div>
              <input
                id="wizard-archive-input"
                type="file"
                accept=".zip,.tar,.tar.gz,.tar.xz"
                className="hidden"
                onChange={(e) => {
                  const file = e.target.files?.[0];
                  if (file) setArchiveFromFile(file);
                }}
              />
              {archiveError ? (
                <p className="mt-1 text-sm text-red-600">{archiveError}</p>
              ) : null}
              {values.archiveFile ? (
                <div className="mt-3 flex items-center justify-between rounded-lg border border-gray-200 bg-gray-50 px-3 py-2">
                  <span className="truncate text-sm text-gray-700">{values.archiveFile.name}</span>
                  <button
                    type="button"
                    onClick={removeArchive}
                    className="ml-3 shrink-0 text-sm font-medium text-red-600 hover:text-red-700"
                  >
                    {t.delete}
                  </button>
                </div>
              ) : null}
            </div>
          </div>
        )}

        {step === 3 && (
          <div className="space-y-4">
            <div className="rounded-xl border border-gray-200 bg-gray-50 p-4 text-sm text-gray-700">
              <p>
                <span className="font-semibold text-gray-900">Subject:</span> {values.subject}
              </p>
              <p className="mt-1">
                <span className="font-semibold text-gray-900">Repository:</span>{' '}
                {parsePublicRepositoryUrl(values.repositoryUrl)?.normalized ??
                  values.repositoryUrl}
              </p>
              <p className="mt-1">
                <span className="font-semibold text-gray-900">Description:</span>{' '}
                {values.description}
              </p>
              <p className="mt-1">
                <span className="font-semibold text-gray-900">Screenshots:</span>{' '}
                {values.screenshotFiles.length}
              </p>
              <p className="mt-1">
                <span className="font-semibold text-gray-900">Source archive:</span>{' '}
                {values.archiveFile ? values.archiveFile.name : 'None'}
              </p>
            </div>

            <div>
              <FormLabel>Tags</FormLabel>
              <div className="flex gap-2">
                <Input
                  type="text"
                  placeholder={t.addTagPlaceholder}
                  value={tagInput}
                  onChange={(e) => setTagInput(e.target.value)}
                  onKeyDown={(e) =>
                    e.key === 'Enter' && (e.preventDefault(), handleAddTag())
                  }
                />
                <Button type="button" onClick={handleAddTag} variant="secondary">
                  {t.add}
                </Button>
              </div>
              {values.tags.length > 0 && (
                <div className="mt-3 flex flex-wrap gap-2">
                  {values.tags.map((tag) => (
                    <Badge key={tag} variant="tag">
                      {tag}
                      <button
                        type="button"
                        onClick={() =>
                          setField(
                            'tags',
                            values.tags.filter((item) => item !== tag),
                          )
                        }
                        className="ml-1 hover:text-red-600"
                      >
                        <X className="h-3 w-3" />
                      </button>
                    </Badge>
                  ))}
                </div>
              )}
            </div>
          </div>
        )}

        <div className="mt-8 flex flex-col gap-3 border-t border-gray-100 pt-6 sm:flex-row">
          {step > 1 ? (
            <Button
              type="button"
              variant="secondary"
              className="w-full sm:flex-1"
              onClick={goBack}
              disabled={isSubmitting}
            >
              Back
            </Button>
          ) : (
            <Button
              type="button"
              variant="secondary"
              className="w-full sm:flex-1"
              onClick={() => router.push('/feed')}
              disabled={isSubmitting}
            >
              Cancel
            </Button>
          )}

          {step < STEPS.length ? (
            <Button type="button" className="w-full sm:flex-1" onClick={goNext} disabled={isSubmitting}>
              Next
            </Button>
          ) : (
            <Button
              type="button"
              className="w-full sm:flex-1"
              onClick={() => void handlePublish()}
              disabled={isSubmitting}
            >
              <GemIcon className="mr-2 h-4 w-4" />
              {isSubmitting ? t.submitting : t.createPost}
            </Button>
          )}
        </div>
      </SectionCard>
    </PageShell>
  );
}
