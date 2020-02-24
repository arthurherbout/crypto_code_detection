/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/31 18:52:58 by magouin           #+#    #+#             */
/*   Updated: 2016/08/31 18:53:03 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <libft.h>

static t_gnl	*ft_lstdict(t_gnl *lst, const int n)
{
	while (lst)
	{
		if (lst->fd == n)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

static t_gnl	*gnl_add_elem(t_gnl **list, const int fd)
{
	t_gnl	*beginning;
	t_gnl	*elem;

	elem = mallocp(sizeof(t_gnl));
	elem->fd = fd;
	elem->line = NULL;
	elem->next = NULL;
	if (!(*list))
	{
		*list = elem;
		return (elem);
	}
	beginning = (*list);
	while ((*list)->next)
		(*list) = (*list)->next;
	(*list)->next = elem;
	*list = beginning;
	return (elem);
}

static char		*strjoin_secure(char *s1, char *s2)
{
	char	*join;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
	{
		join = ft_strdup(s2);
		return (join);
	}
	if (s2 == NULL)
	{
		join = ft_strdup(s1);
		ft_strdel(&s1);
		return (join);
	}
	if (s1 != NULL && s2 != NULL)
	{
		join = ft_strjoin(s1, s2);
		ft_strdel(&s1);
		return (join);
	}
	return (NULL);
}

static int		newline_detected(char *stbuff, char **line,
		int flag, t_gnl **elem)
{
	char			*nl;

	if (flag == 0)
	{
		nl = ft_strchr(stbuff, '\n');
		*line = ft_strsub(stbuff, 0, nl - stbuff);
		ft_strcpy(stbuff, nl + 1);
		return (1);
	}
	else
	{
		*line = ft_strdup((*elem)->line);
		ft_strdel(&((*elem)->line));
		return (1);
	}
}

int				get_next_line(const int fd, char **line)
{
	char			buff[BUFF_SIZE + 1];
	static t_gnl	*lst = NULL;
	t_gnl			*elem;
	int				ret;

	if (line == NULL || fd < 0)
		return (-1);
	if ((elem = ft_lstdict(lst, fd)) == NULL)
		elem = gnl_add_elem(&lst, fd);
	if ((elem->line && ft_strchr(elem->line, '\n') != NULL)
			&& elem->line && elem->line[0])
		return (newline_detected(elem->line, line, 0, NULL));
	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if ((elem->line = strjoin_secure(elem->line, buff)) == NULL)
			return (-1);
		if (ft_strchr(elem->line, '\n') != NULL)
			return (newline_detected(elem->line, line, 0, NULL));
	}
	if (ret == -1)
		return (-1);
	if (elem->line && *(elem->line))
		return (newline_detected(NULL, line, 1, &elem));
	return (0);
}
