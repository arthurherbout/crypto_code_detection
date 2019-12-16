/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:48:56 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/12/19 02:05:31 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	char	*str;

	str = s1;
	while (str && *str)
		str++;
	while (s2 && *s2 && n > 0)
	{
		*str = *s2;
		str++;
		s2++;
		n--;
	}
	*str = '\0';
	return (s1);
}
