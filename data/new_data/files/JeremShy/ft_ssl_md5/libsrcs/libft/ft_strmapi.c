/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 14:31:51 by magouin           #+#    #+#             */
/*   Updated: 2015/11/24 14:45:30 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	int				len;
	char			*rez;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	rez = (char*)mallocp(len + 1);
	while (s[i] != '\0')
	{
		rez[i] = f(i, s[i]);
		i++;
	}
	rez[i] = '\0';
	return (rez);
}
