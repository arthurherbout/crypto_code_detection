/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 17:10:13 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 08:29:40 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strtrim(char const *s)
{
	int	i;
	int	len;
	int	stay;

	if (!s)
		return (NULL);
	i = 0;
	len = 0;
	while (s[i] == '\n' || s[i] == ' ' || s[i] == '\t')
		i++;
	stay = i;
	i = (int)ft_strlen(s) - 1;
	while ((s[i - len] == '\n' || s[i - len] == ' ' || s[i - len] == '\t' ||
				s[i - len] == '\0') && (i - len > 0))
		len++;
	if (i == len)
		return (ft_strdup(""));
	return (ft_strsub(s, stay, i - stay - len + 1));
}
