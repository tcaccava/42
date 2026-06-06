export enum ProjectKind {
	libft = 'libft',
	ft_printf = 'ft_printf',
	get_next_line = 'get_next_line',
	minishell = 'minishell',
	push_swap = 'push_swap',
	minitalk = 'minitalk',
	cub3d = 'cub3d',
	philosophers = 'philosophers',
	pipex = 'pipex',
	so_long = 'so_long',
};

// Idk how to do it cleaner, so for-loop it is :P
let pkValues = [];
for (const value in ProjectKind) {
	pkValues.push(value);
}
export const ProjectKindValues = pkValues;

export type ScreenshotFileData = {
  data: File,
  name: string,
};

export type ProjectData = {
	projectId: number,
	userId: string,
	title: string,
	repositoryUrl: string,
	subject: ProjectKind,
	description: string,
	screenshots: string[] | null,
	source: string | null,
	tags: string[] | null,
};
