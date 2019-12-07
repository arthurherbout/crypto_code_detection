/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alloc_str_with_char.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/30 09:14:31 by magouin           #+#    #+#             */
/*   Updated: 2016/01/07 12:30:46 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_alloc_str_with_char(int c)
{
	char	*ret;

	ret = (char*)mallocp(2 * sizeof(char));
	ret[0] = c;
	ret[1] = '\0';
	return (ret);
}
