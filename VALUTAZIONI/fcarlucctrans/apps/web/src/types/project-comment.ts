//(Sara) my project page is based on these two types; tried to match names with the backend
//but I'm not quite sure

export type Project = {
	userId: string,
	authorAlias: string,
	authorName: string,
	profilePic: string,
	projectTitle: string,
	subject: string,
	screenshots: string[],
	projectDescription?: string
	comments?: Comment[]
}

export type Comment = {
	id: string,
	authorAlias: string,
	authorName: string,
	profilePic?: string,
	content: string,
	replyTo?: string,
	replies?: Comment[]
	createdAt: string,
	updatedAt: string
}
