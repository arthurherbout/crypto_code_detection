/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:47:56 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/12 17:43:06 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsts)
{
	size_t	i;
	size_t	dst_size;
	size_t	src_size;

	dst_size = ft_strlen(dst);
	src_size = ft_strlen(src);
	i = dst_size;
	if (dsts < dst_size + 1)
		return (dsts + src_size);
	while (*src && i + 1 < dsts)
		dst[i++] = *src++;
	dst[i] = '\0';
	return (dst_size + src_size);
}
