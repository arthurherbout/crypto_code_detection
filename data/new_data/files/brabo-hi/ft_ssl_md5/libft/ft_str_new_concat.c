/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_new_concat.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 00:29:29 by brabo-hi          #+#    #+#             */
/*   Updated: 2018/07/10 00:29:49 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_str_new_concat(char *origin, char *str)
{
	char    *out;
	int     i;

	if (!str)
		return (origin);
	i = 0;
	if (!(out = ft_memalloc(ft_strlen((const char *)str)
	                        + ft_strlen((const char *)origin) + 1)))
		return (NULL);
	while (origin && *origin)
		out[i++] = *origin++;
	while (str && *str)
		 out[i++] = *str++;
	out[i] = 0;
	return (out);
}
