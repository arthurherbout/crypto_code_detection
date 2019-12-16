/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brabo-hi <brabo-hi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:15:00 by brabo-hi          #+#    #+#             */
/*   Updated: 2017/11/12 14:06:55 by brabo-hi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*current;
	t_list	*copy;

	current = f(lst);
	copy = current;
	if (current == NULL)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
		copy->next = f(lst);
		if (copy->next == NULL)
			return (NULL);
		copy = copy->next;
	}
	return (current);
}
