import type { Project } from "@/types/project-comment"
import Link from "next/link"
import Title from "@/components/title"
import { getProject } from "@/server/project"
import { fetchServerProfile } from "@/server/auth"
import { Button } from "@/components/Button"
import Comments from "@/components/Comments"
import { getComments } from "@/server/comment"
import { CommentData } from "@/types/comment"
import ProjectPage from "@/components/ProjectPage"

export default async function ProjectPageServerComp(pageParams: { params: Promise<{
	id: string
}>
}) {
	const projectId = parseInt((await pageParams.params).id);
	const projectData = await getProject(projectId);
	const comments = await getComments(projectId);
	const profile = await fetchServerProfile();
	// const authorIsHere = profile.user?.login == projectData?.userId;

	return (
		<ProjectPage 
			projectId={projectId}
			projectData={projectData ?? undefined}
			comments={comments ?? []}
			profile={profile}
		/>
	)
}
