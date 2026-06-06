'use client';

import { ProjectData } from '@/types/project';
import { local } from '@/utilities';
import { CommentData } from '@/types/comment';
import { AuthResponse } from '@/types';
import Comments from '@/components/Comments';
import { PageShell, SectionCard } from '@/components/ui/layout';
import { LinkButton } from '@/components/ui/link-button';
import { Badge } from '@/components/Badge';
import { ScreenshotCarousel } from '@/components/ScreenshotCarousel';
import { Download, FolderKanban, Pencil } from 'lucide-react';

const descriptionContentClass =
  'min-w-0 break-words text-base leading-relaxed text-gray-700 ' +
  '[&_p]:mb-3 [&_ul]:mb-3 [&_ul]:list-disc [&_ul]:pl-5 [&_ol]:mb-3 [&_ol]:list-decimal [&_ol]:pl-5 ' +
  '[&_img]:max-w-full [&_img]:rounded-lg [&_pre]:overflow-x-auto [&_pre]:rounded-lg [&_pre]:bg-gray-50 [&_pre]:p-3 ' +
  '[&_table]:block [&_table]:max-w-full [&_table]:overflow-x-auto [&_a]:break-all [&_a]:text-teal-700';

export default function ProjectPage(params: {
  projectId: number;
  projectData?: ProjectData;
  comments: CommentData[];
  profile: AuthResponse;
}) {
  const t = local.useLocalization().t;
  const projectData = params.projectData;
  if (!projectData) {
    return `${t.couldntFindProject} ${params.projectId}`;
  }

  const comments = params.comments;
  const profile = params.profile;
  const authorIsHere = profile.user?.login === projectData.userId;
  const projectId = params.projectId;
  const screenshots = projectData.screenshots ?? [];

  return (
    <PageShell maxWidth="4xl" className="space-y-6">
      <SectionCard contentClassName="p-5 sm:p-6">
        <div className="flex flex-col gap-4 sm:flex-row sm:items-start sm:justify-between">
          <div className="min-w-0 flex-1">
            <div className="mb-2 flex flex-wrap items-center gap-2">
              <Badge variant="status" className="uppercase tracking-wide">
                {projectData.subject}
              </Badge>
              {projectData.tags?.map((tag) => (
                <Badge key={tag} variant="tag">
                  {tag}
                </Badge>
              ))}
            </div>
            <div className="flex items-start gap-3">
              <div className="hidden rounded-xl bg-teal-50 p-2.5 text-teal-600 sm:block">
                <FolderKanban className="h-6 w-6" aria-hidden="true" />
              </div>
              <div className="min-w-0">
                <h1 className="text-2xl font-bold text-gray-900 sm:text-3xl">
                  {projectData.subject}
                </h1>
                {projectData.repositoryUrl ? (
                  <a
                    href={projectData.repositoryUrl}
                    target="_blank"
                    rel="noopener noreferrer"
                    className="mt-2 inline-block break-all font-mono text-sm text-teal-700 hover:underline"
                  >
                    {projectData.repositoryUrl}
                  </a>
                ) : projectData.title ? (
                  <p className="mt-2 text-sm text-gray-600">{projectData.title}</p>
                ) : null}
              </div>
            </div>
          </div>

          <div className="flex shrink-0 flex-wrap gap-2 sm:justify-end">
            {authorIsHere && (
              <LinkButton
                href={`/create/edit?projectId=${projectId}`}
                className="inline-flex gap-2"
              >
                <Pencil className="h-4 w-4" />
                {t.editProject}
              </LinkButton>
            )}
            {projectData.source && (
              <LinkButton
                href={projectData.source}
                variant="secondary"
                className="inline-flex gap-2"
              >
                <Download className="h-4 w-4" />
                {t.downloadProject}
              </LinkButton>
            )}
          </div>
        </div>
      </SectionCard>

      <div className="grid min-w-0 gap-6 lg:grid-cols-[minmax(0,1fr)_minmax(280px,360px)] lg:items-start">
        <div className="flex min-w-0 flex-col gap-6">
          <SectionCard title="Project Description" className="min-w-0">
            <div
              className={descriptionContentClass}
              dangerouslySetInnerHTML={{
                __html: projectData.description || '<p class="text-gray-500">No description yet.</p>',
              }}
            />
          </SectionCard>

          <SectionCard title="Comments" className="min-w-0" contentClassName="p-4 sm:p-5">
            <Comments
              comments={comments ?? []}
              projectId={projectData.projectId}
              userData={profile.user!}
            />
          </SectionCard>
        </div>

        <SectionCard
          title="Screenshots"
          description={
            screenshots.length > 1
              ? `${screenshots.length} images — use arrows or thumbnails to browse`
              : undefined
          }
          className="min-w-0 lg:sticky lg:top-6"
          contentClassName="p-4 sm:p-5"
        >
          <ScreenshotCarousel
            images={screenshots}
            altPrefix={projectData.title || 'Project'}
          />
        </SectionCard>
      </div>
    </PageShell>
  );
}
