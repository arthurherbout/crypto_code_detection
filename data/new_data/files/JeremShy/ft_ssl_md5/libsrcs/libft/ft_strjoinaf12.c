/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinaf12.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:42:25 by magouin           #+#    #+#             */
/*   Updated: 2016/01/05 13:42:48 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoinaf12(char *s1, char *s2)
{
	int		i;
	size_t	len;
	char	*res;

	if (!s1 && s2)
		return (ft_strdup(s2));
	if (!s2 && s1)
		return (ft_strdup(s1));
	if (!s1 && !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = (char*)mallocp(len * sizeof(char));
	if (!res)
		return (NULL);
	i = -1;
	while (s1[++i] != '\0')
		res[i] = s1[i];
	len = i;
	i = -1;
	while (s2[++i] != '\0')
		res[len + i] = s2[i];
	res[len + i] = '\0';
	free(s1);
	free(s2);
	return (res);
}
