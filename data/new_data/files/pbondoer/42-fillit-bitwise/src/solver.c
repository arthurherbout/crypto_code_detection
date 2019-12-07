/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/02 15:19:32 by pbondoer          #+#    #+#             */
/*   Updated: 2017/02/03 02:11:59 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"
#include "fillit.h"
#include <stdio.h>

/*
** Checks for a piece
*/

inline int	check_piece(const t_etris *tetris, uint16_t *map)
{
	return (!(*(uint64_t *)(map + tetris->y) & (tetris->value >> tetris->x)));
}

/*
** Switches off a piece
*/

inline void	toggle_piece(const t_etris *tetris, uint16_t *map)
{
	*(uint64_t *)(map + tetris->y) ^= tetris->value >> tetris->x;
}

/*
** Backtracking implementation of the solver.
*/

int			solve_map(t_etris *tetris, const int size, uint16_t *map)
{
	int pos;

	if (tetris->id == 0)
		return (1);
	pos = (tetris->last ? (tetris->last->x + tetris->last->y * size) : 0);
	tetris->y = pos / size;
	while (tetris->y <= size - tetris->height)
	{
		tetris->x = (tetris->y == pos / size ? pos % size : 0);
		while (tetris->x <= size - tetris->width)
		{
			if (check_piece(tetris, map))
			{
				toggle_piece(tetris, map);
				if (solve_map(tetris + 1, size, map))
					return (1);
				toggle_piece(tetris, map);
			}
			tetris->x++;
		}
		tetris->y++;
	}
	tetris->x = 0;
	tetris->y = 0;
	return (0);
}

/*
** Tries to solve maps starting from the smallest possible size.
*/

int			solve(t_etris *tetris, const int count, uint16_t *map)
{
	int		size;

	size = 2;
	while (size * size < count * 4)
		size++;
	while (!solve_map(tetris, size, map) && size <= 16)
	{
		ft_bzero(map, sizeof(uint16_t) * 16);
		size++;
	}
	return (size == 17 ? 0 : size);
}
