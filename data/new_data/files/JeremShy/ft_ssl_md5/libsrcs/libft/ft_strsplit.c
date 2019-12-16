/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JeremShy <JeremShy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 18:24:48 by magouin           #+#    #+#             */
/*   Updated: 2015/12/19 18:55:31 by JeremShy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>

static	int		ft_strcount(char const *s, char const c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

static	int		poulet(size_t i, size_t j, char const *s, char c)
{
	while (s[i + j] != c && s[i + j] != '\0')
		j++;
	return (j);
}

static	char	**ft_fln(char const *s, char **res, size_t *x, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	*x = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		j = 0;
		j = poulet(i, j, s, c);
		if (s[i + j] != '\0' || s[i + j - 1] != c)
		{
			res[*x] = ft_strsub(s, i, j);
			*x = *x + 1;
		}
		i += j;
	}
	return (res);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**res;
	size_t	x;

	res = mallocp((ft_strcount(s, c) + 1) * sizeof(char*));
	if (!s || !res)
		return (NULL);
	if (ft_strcount(s, c) == 0)
	{
		res[0] = NULL;
		return (res);
	}
	res = ft_fln(s, res, &x, c);
	res[x] = NULL;
	return (res);
}
