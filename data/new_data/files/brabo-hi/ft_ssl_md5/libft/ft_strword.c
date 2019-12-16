/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strword.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 23:16:40 by brabo-hi          #+#    #+#             */
/*   Updated: 2018/03/24 22:57:39 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strword(char *str)
{
	char	*dest;
	int		i;
	int		len;

	if (!str)
		return (NULL);
	dest = NULL;
	i = 0;
	len = 0;
	while (str[len] && ft_isalnum(str[len]))
		len++;
	if (!(dest = ft_memalloc(sizeof(len + 1))))
		return (NULL);
	while (i < len)
		dest[i++] = *str++;
	dest[i] = '\0';
	return (dest);
}

char	*ft_strword_dash(char *str)
{
	char	*dest;
	int		i;
	int		len;

	if (!str)
		return (NULL);
	dest = NULL;
	i = 0;
	len = 0;
	while (str[len] && str[len] != '-')
		len++;
	if (!(dest = ft_memalloc(sizeof(len + 1))))
		return (NULL);
	while (i < len)
		dest[i++] = *str++;
	dest[i] = '\0';
	return (dest);
}
