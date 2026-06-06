'use client';

import { useState } from 'react';
import { useRouter } from 'next/navigation';
import type { User } from '@/types';
import type { ProjectData } from '@/types/project';
import { ProjectForm, type ProjectFormValues } from '@/components/ProjectForm';
import { AlertBanner } from '@/components/ui/layout';
import {
  clearProjectScreenshots,
  clearProjectSource,
  updateProject,
} from '@/server/project';
import { uploadScreenshots, uploadSource } from '@/server/projectClient';
import { deleteProjectClient } from '@/lib/projects-client';
import { urlToScreenshotFile } from '@/lib/project-files';

export function EditProjectClient({
  user,
  project,
}: {
  user: User;
  project: ProjectData;
}) {
  const router = useRouter();
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const initialScreenshotUrls = project.screenshots ?? [];

  const persistUploads = async (projectId: number, values: ProjectFormValues) => {
    const screenshotsChanged =
      values.existingScreenshotUrls.length !== initialScreenshotUrls.length ||
      values.screenshotFiles.length > 0;

    if (screenshotsChanged) {
      const hasAnyScreenshots =
        values.existingScreenshotUrls.length > 0 || values.screenshotFiles.length > 0;

      if (!hasAnyScreenshots) {
        const cleared = await clearProjectScreenshots(projectId);
        if (!cleared) throw new Error('Failed to clear project screenshots.');
      } else {
        const cleared = await clearProjectScreenshots(projectId);
        if (!cleared) throw new Error('Failed to update project screenshots.');

        const keptFiles = await Promise.all(
          values.existingScreenshotUrls.map((url, index) =>
            urlToScreenshotFile(url, index),
          ),
        );
        const allFiles = [...keptFiles, ...values.screenshotFiles];

        if (allFiles.length > 0) {
          const uploaded = await uploadScreenshots(projectId, allFiles, () => {});
          if (!uploaded) throw new Error('Failed to upload project screenshots.');
        }
      }
    }

    if (values.removeExistingArchive && !values.archiveFile) {
      const cleared = await clearProjectSource(projectId);
      if (!cleared) throw new Error('Failed to remove project archive.');
    } else if (values.archiveFile) {
      const uploaded = await uploadSource(projectId, values.archiveFile, () => {});
      if (!uploaded) throw new Error('Failed to upload project source archive.');
    }
  };

  return (
    <>
      {error ? (
        <div className="mx-auto max-w-2xl px-4 pt-4">
          <AlertBanner kind="error">{error}</AlertBanner>
        </div>
      ) : null}
      <ProjectForm
        currentUser={user}
        projectSubject={project.subject}
        initialRepositoryUrl={project.repositoryUrl || project.title}
        initialDescription={project.description}
        initialTags={project.tags ?? []}
        existingScreenshots={project.screenshots}
        existingSourceUrl={project.source}
        isSubmitting={isSubmitting}
        onCancel={() => router.push(`/project/${project.projectId}`)}
        onSubmit={async (values) => {
          setError(null);
          setIsSubmitting(true);
          try {
            const updated = await updateProject({
              id: project.projectId,
              repositoryUrl: values.repositoryUrl,
              description: values.description,
              tagList: values.tags,
            });

            if (!updated) {
              setError('Could not update project.');
              return;
            }

            await persistUploads(project.projectId, values);
            router.push(`/project/${project.projectId}`);
            router.refresh();
          } catch (err) {
            setError(err instanceof Error ? err.message : 'Could not update project.');
          } finally {
            setIsSubmitting(false);
          }
        }}
        onDelete={async () => {
          setError(null);
          setIsSubmitting(true);
          try {
            const deleted = await deleteProjectClient(project.projectId);
            if (!deleted) {
              setError('Could not delete project.');
              return;
            }
            router.push('/search');
            router.refresh();
          } catch (err) {
            setError(err instanceof Error ? err.message : 'Could not delete project.');
          } finally {
            setIsSubmitting(false);
          }
        }}
      />
    </>
  );
}
