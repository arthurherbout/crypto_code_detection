/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 17:41:37 by pbondoer          #+#    #+#             */
/*   Updated: 2017/01/18 00:22:14 by lemon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "fillit.h"

/*
** Allocates an empty map string to populate with our pieces.
*/

char	*empty_map(int size)
{
	char	*str;
	int		x;
	int		y;

	str = ft_strnew((size + 1) * (size));
	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			str[y * (size + 1) + x] = '.';
			x++;
		}
		str[y * (size + 1) + x] = '\n';
		y++;
	}
	return (str);
}

/*
** Displays a map as resolved by the program.
*/

void	print(t_etris *t, int count, int size)
{
	char	*str;
	int		x;
	int		y;

	str = empty_map(size);
	while (count > 0)
	{
		y = 0;
		while (y < t->height)
		{
			x = 0;
			while (x < t->width)
			{
				if ((t->value >> (16 * (y + 1) - 1 - x)) & 1)
					str[(t->y + y) * (size + 1) + x + t->x] = t->id;
				x++;
			}
			y++;
		}
		t++;
		count--;
	}
	ft_putstr(str);
	ft_strdel(&str);
}

/*
** Utility die function
*/

int		die(char *str)
{
	ft_putendl(str);
	return (1);
}

/*
** Entry point for our application
*/

int		main(int argc, char **argv)
{
	t_etris		tetris[MAX_TETRI + 1];
	uint16_t	map[16];
	int			count;
	int			size;

	if (argc != 2)
		return (die("usage: ./fillit [input_file]"));
	ft_bzero(tetris, sizeof(t_etris) * (MAX_TETRI + 1));
	if ((count = read_tetri(open(argv[1], O_RDONLY), tetris)) == 0)
		return (die("error"));
	ft_bzero(map, sizeof(uint16_t) * 16);
	if ((size = solve(tetris, count, map)) == 0)
		return (die("error"));
	print(tetris, count, size);
	return (0);
}
