/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:42:19 by brabo-hi          #+#    #+#             */
/*   Updated: 2018/01/11 03:54:01 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	words(int n)
{
	int c;

	c = (n < 0) ? 2 : 1;
	while (n && (n /= 10))
		c++;
	return (c);
}

char		*ft_itoa(int n)
{
	char	*ret;
	int		neg;
	int		index;

	index = 0;
	ret = ft_strnew(words(n));
	if (!ret)
		return (NULL);
	neg = (n < 0) ? 1 : 0;
	while (n)
	{
		ret[index++] = (n % 10 > 0) ? (n % 10 + '0') : (n % 10 * (-1) + '0');
		n /= 10;
	}
	if (neg)
		ret[index++] = '-';
	if (ret[0] == 0)
		ret[0] = '0';
	return (ft_strrev(ret));
}
