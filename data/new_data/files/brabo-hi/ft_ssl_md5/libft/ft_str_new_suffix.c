/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_new_suffix.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 00:30:11 by brabo-hi          #+#    #+#             */
/*   Updated: 2018/07/10 00:30:17 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *ft_str_new_suffix(char *origin, char c, size_t n)
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
	while (origin && *origin)
		out[i++] = *origin++;
	while (n--)
		out[i++] = c;
	out[i] = 0;
	return (out);
}
