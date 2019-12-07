/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 10:23:51 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 09:48:42 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t i;
	size_t j;

	i = 0;
	while (s1[i] != '\0' && i < n)
	{
		j = 0;
		while (s1[i + j] == s2[j] && s1[i + j] && s2 && i + j < n)
			j++;
		if (s2[j] == '\0')
			return ((char*)(s1 + i));
		i++;
	}
	if (s1[0] == '\0' && s2[0] == '\0')
		return ((char*)s1);
	return (NULL);
}
