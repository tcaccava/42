/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 18:21:04 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 18:40:14 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	ll;

	if (s == NULL)
		return (NULL);
	ll = ft_strlen(s) - start;
	if ((long long) ll < 0)
		return (ft_calloc(1, 1));
	if (ll > len)
		ll = len;
	sub = (char *)malloc((ll + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	ft_strlcpy(sub, s + start, ll + 1);
	return (sub);
}
