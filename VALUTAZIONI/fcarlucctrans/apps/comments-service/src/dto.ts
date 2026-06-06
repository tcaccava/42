// The year is 2026. Typescript still has no fucking way of checking for an interface at runtime,
// thus these are classes and not interfaces. Amazing.
export interface CreateCommentDTO {
	text: string;
	projectId: number;
}

export interface UpdateCommentDTO {
	id: number;
	text: string;
};

export interface UploadCommentDTO {
	id?: number;
	text: string;
	projectId?: number;
}

export interface UploadUserDTO {
	id: string,
	name: string,
};
