/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 10:52:37 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 12:31:05 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list *ret;

	ret = mallocp(sizeof(t_list));
	if (!ret)
		return (NULL);
	if (content_size == 0 || content == NULL)
	{
		ret->content = NULL;
		ret->content_size = 0;
	}
	else
	{
		ret->content = mallocp(sizeof(content));
		if (ret->content == NULL)
			return (NULL);
		ft_memcpy(ret->content, content, sizeof(content));
		ret->content_size = content_size;
	}
	ret->next = NULL;
	return (ret);
}
