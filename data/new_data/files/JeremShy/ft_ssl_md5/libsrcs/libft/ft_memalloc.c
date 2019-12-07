/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 13:22:30 by magouin           #+#    #+#             */
/*   Updated: 2016/09/01 23:54:30 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>

void	*ft_memalloc(size_t size)
{
	size_t	i;
	char	*res;

	res = (char*)mallocp(size);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = '\0';
		i++;
	}
	return ((void*)res);
}
