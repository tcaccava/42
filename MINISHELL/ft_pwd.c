/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:10:43 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:10:44 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	buff[PATH_MAX];

	if (getcwd(buff, PATH_MAX) != NULL)
	{
		printf("%s\n", buff);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}
