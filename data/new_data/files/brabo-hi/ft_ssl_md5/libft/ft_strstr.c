/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:50:40 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/12 13:11:54 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;

	i = 0;
	if (!*needle)
		return ((char *)(haystack));
	while (*haystack)
	{
		i = 0;
		while (haystack[i] == needle[i])
			if (!needle[++i])
				return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
