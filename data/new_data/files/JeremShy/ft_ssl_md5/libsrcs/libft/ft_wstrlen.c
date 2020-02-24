/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wstrlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/15 12:39:54 by magouin           #+#    #+#             */
/*   Updated: 2016/01/18 11:31:45 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

size_t	ft_wstrlen(const char *s)
{
	size_t	i;
	int		b;
	int		count;

	i = 0;
	count = 0;
	b = 0;
	while (s[i] != '\0')
	{
		if (s[i] > 0)
			count++;
		else
			b++;
		if (b == 3)
		{
			count++;
			b = 0;
		}
		i++;
	}
	return (count);
}
