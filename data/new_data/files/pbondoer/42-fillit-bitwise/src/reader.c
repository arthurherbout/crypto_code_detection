/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/15 17:43:27 by pbondoer          #+#    #+#             */
/*   Updated: 2017/02/03 01:51:47 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "fillit.h"

/*
** Utility function to get min-max values for get_piece.
** pos is a 4 dimensional array encoded as: xMin 0, xMax 1, yMin 2, yMax 3
*/

void	min_max(const char *str, char *m)
{
	unsigned char i;

	i = 0;
	m[0] = 3;
	m[1] = 0;
	m[2] = 3;
	m[3] = 0;
	while (i < 20)
	{
		if (str[i] == '#')
		{
			if (i % 5 < m[0])
				m[0] = i % 5;
			if (i % 5 > m[1])
				m[1] = i % 5;
			if (i / 5 < m[2])
				m[2] = i / 5;
			if (i / 5 > m[3])
				m[3] = i / 5;
		}
		i++;
	}
}

/*
** Reads a piece from a valid chunk and puts that in the t_etris structure.
*/

t_etris	get_piece(const char *str, const char id)
{
	t_etris	tetris;
	char	m[4];
	int		x;
	int		y;

	min_max(str, m);
	tetris.width = m[1] - m[0] + 1;
	tetris.height = m[3] - m[2] + 1;
	tetris.id = id;
	tetris.value = 0;
	tetris.last = NULL;
	y = 0;
	while (y < tetris.height)
	{
		x = 0;
		while (x < tetris.width)
		{
			if (str[(m[0] + x) + (m[2] + y) * 5] == '#')
				tetris.value |= (1L << (16 * (y + 1) - 1 - x));
			x++;
		}
		y++;
	}
	return (tetris);
}

/*
** Checks connection counts, if we have 6 or 8 connections, the tetrimino is
** valid. Otherwise, our tetrimino is not contiguous.
** (This of course assumes we know this tetrimino has only 4 blocks)
*/

int		check_connection(const char *str)
{
	int block;
	int i;

	block = 0;
	i = 0;
	while (i < 20)
	{
		if (str[i] == '#')
		{
			if ((i + 1) < 20 && str[i + 1] == '#')
				block++;
			if ((i - 1) >= 0 && str[i - 1] == '#')
				block++;
			if ((i + 5) < 20 && str[i + 5] == '#')
				block++;
			if ((i - 5) >= 0 && str[i - 5] == '#')
				block++;
		}
		i++;
	}
	return (block == 6 || block == 8);
}

/*
** Checks character counts and that chunk format is valid.
*/

int		check_counts(const char *str, const int count)
{
	int i;
	int blocks;

	blocks = 0;
	i = 0;
	while (i < 20)
	{
		if (i % 5 < 4)
		{
			if (!(str[i] == '#' || str[i] == '.'))
				return (1);
			if (str[i] == '#' && ++blocks > 4)
				return (2);
		}
		else if (str[i] != '\n')
			return (3);
		i++;
	}
	if (count == 21 && str[20] != '\n')
		return (4);
	if (!check_connection(str))
		return (5);
	return (0);
}

/*
** Read tetriminos from fd and put them in our tetrimino array.
** We use 21 sized reads to read piece by piece since there are
** 4 lines made of 4 chars (+ newline) = 20 chars + sep. newline = 21 chars
** (don't forget that \0)
*/

int		read_tetri(const int fd, t_etris *tetris)
{
	char	buf[22];
	int		count;
	int		i;
	int		j;
	char	cur;

	cur = 'A';
	i = 0;
	while ((count = read(fd, buf, 21)) >= 20)
	{
		if (check_counts(buf, count) != 0)
			return (0);
		tetris[i] = get_piece(buf, cur++);
		j = i - 1;
		while (j >= 0)
		{
			if (tetris[j].value == tetris[i].value)
				tetris[i].last = tetris + j;
			j--;
		}
		i++;
	}
	if (count != 0)
		return (0);
	return (cur - 'A');
}
