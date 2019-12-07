/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 18:14:49 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 11:46:19 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;
	int		copying;

	i = 0;
	copying = 1;
	while (i < n)
	{
		if (copying)
		{
			if (src[i] != '\0')
				dst[i] = src[i];
			else
				copying = 0;
		}
		if (!copying)
			dst[i] = '\0';
		i++;
	}
	return (dst);
}
