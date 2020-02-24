/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 11:24:48 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 14:14:36 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *newlist;
	t_list *ret;

	newlist = f(lst);
	ret = newlist;
	while (lst->next != NULL)
	{
		lst = lst->next;
		newlist->next = f(lst);
		newlist = newlist->next;
	}
	newlist = f(lst);
	newlist->next = NULL;
	return (ret);
}
