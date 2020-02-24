/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:50:06 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/14 22:45:29 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	size;

	if (!*little)
		return ((char *)big);
	size = ft_strlen(little);
	while (*big && len-- >= size)
	{
		if (*big == *little && ft_memcmp(big, little, size) == 0)
			return ((char *)big);
		big++;
	}
	return (NULL);
}
