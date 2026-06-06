
export type CommentData = {
	id: number,
	user: {
		id: string,
		name: string,
	},
	userId: string,
	commentText: string,
	projectId: number,
	createdAt: string,
	updatedAt: string,
};