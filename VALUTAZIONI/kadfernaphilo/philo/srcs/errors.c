/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:54:43 by kadferna          #+#    #+#             */
/*   Updated: 2025/05/14 13:54:46 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	std_error(char *msg)
{
	printf("Error\n");
	printf("%s\n", msg);
}

void	cust_error(void)
{
	printf("Error\n");
	printf("Expected 4 or 5 arguments (excluding program name).\n");
	printf("./philo <num of philos> <time will die if does not eat> ");
	printf("<eat time> <sleep time> *num of times to eat* \n");
}
