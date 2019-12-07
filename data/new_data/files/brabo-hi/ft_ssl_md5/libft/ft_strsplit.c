/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 22:59:48 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/15 02:40:52 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t		ft_cnt_word(char *str, char c)
{
	size_t i;
	size_t k;

	i = 0;
	k = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c)
		{
			k++;
			while (str[i] != c && str[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (k);
}

static char			**ft_split(char **split, char *str, char c, size_t j)
{
	size_t		i;
	size_t		start;
	size_t		k;

	i = 0;
	start = 0;
	k = 0;
	while (str[i] != '\0' && k < j)
	{
		if (str[i] != c)
		{
			start = i;
			while (str[i] != c && str[i] != '\0')
				i++;
			split[k] = ft_strsub(str, start, i - start);
			k++;
		}
		else
			i++;
	}
	split[k] = NULL;
	return (split);
}

char				**ft_strsplit(char const *str, char c)
{
	char		**split;
	size_t		k;

	if (!str)
		return (NULL);
	k = ft_cnt_word((char*)str, c);
	split = NULL;
	if ((split = (char**)malloc((k + 1) * sizeof(char*))) == NULL)
		return (NULL);
	if (!split)
		return (NULL);
	split = ft_split(split, (char*)str, c, k);
	return (split);
}
