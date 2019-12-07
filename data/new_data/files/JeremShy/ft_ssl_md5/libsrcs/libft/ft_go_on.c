/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_go_on.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 14:05:59 by magouin           #+#    #+#             */
/*   Updated: 2016/01/05 14:10:11 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>

void	*ft_go_on(void *str, int i)
{
	char	*tmp;

	tmp = (void*)ft_strdup((char*)str + i);
	free(str);
	return (tmp);
}
