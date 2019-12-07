/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_new_prefix.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 00:29:57 by brabo-hi          #+#    #+#             */
/*   Updated: 2018/07/10 00:30:07 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_str_new_prefix(char *origin, char c, size_t n)
{
	char	*out;
	char	*cpy;
	int		i;

	if (!n)
		return (origin);
	i = 0;
	cpy = origin;
	if (!(out = ft_memalloc(ft_strlen(origin) + n + 1)))
		return (NULL);
	while (n--)
		out[i++] = c;
	while (origin && *origin)
		out[i++] = *origin++;
	out[i] = 0;
	return (out);
}
