/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:51:11 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/10 13:36:31 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *s)
{
	char const	*str;

	if (s == NULL)
		return (NULL);
	while (TRIM(*s))
		s++;
	if (*s == '\0')
		return (ft_strnew(0));
	str = s + ft_strlen(s) - 1;
	while (TRIM(*str))
		str--;
	return (ft_strsub(s, 0, str - s + 1));
}
