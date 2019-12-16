/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 02:20:34 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/07 15:49:33 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	char	*ret;
	size_t	i;

	i = -1;
	ret = dst;
	while (++i < len)
	{
		if (*(src))
			*(dst++) = *(src++);
		else
			*(dst++) = 0;
	}
	return (ret);
}
