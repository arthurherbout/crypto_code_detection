/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_resolve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarracc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 11:18:19 by rmarracc          #+#    #+#             */
/*   Updated: 2019/02/10 11:31:29 by rmarracc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"

static unsigned int	g_r[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
static unsigned int	g_t[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static t_md5		ft_fill_m(t_md5 d, unsigned char *padded_addr)
{
	d.stock[0] = 0;
	while (d.stock[0] < d.stock[2])
	{
		d.m[d.stock[0]] = padded_addr[4 * d.stock[0] + 3];
		d.m[d.stock[0]] <<= 8;
		d.m[d.stock[0]] = d.m[d.stock[0]] + padded_addr[4 * d.stock[0] + 2];
		d.m[d.stock[0]] <<= 8;
		d.m[d.stock[0]] = d.m[d.stock[0]] + padded_addr[4 * d.stock[0] + 1];
		d.m[d.stock[0]] <<= 8;
		d.m[d.stock[0]] = d.m[d.stock[0]] + padded_addr[4 * d.stock[0]];
		d.stock[0]++;
	}
	return (d);
}

static t_md5		ft_get_fonc0(t_md5 d)
{
	if (d.stock[4] <= 15)
		d.fonc[0] = (d.hset[1] & d.hset[2]) | ((~d.hset[1]) & d.hset[3]);
	else if (d.stock[4] <= 31)
		d.fonc[0] = (d.hset[3] & d.hset[1]) | ((~d.hset[3]) & d.hset[2]);
	else if (d.stock[4] <= 47)
		d.fonc[0] = d.hset[1] ^ d.hset[2] ^ d.hset[3];
	else if (d.stock[4] <= 63)
		d.fonc[0] = d.hset[2] ^ (d.hset[1] | (~d.hset[3]));
	return (d);
}

static t_md5		ft_exec_round(t_md5 d)
{
	d.stock[4] = 0;
	while (d.stock[4] < 64)
	{
		d = ft_get_fonc0(d);
		if (d.stock[4] <= 15)
			d.fonc[1] = d.stock[4];
		else if (d.stock[4] <= 31)
			d.fonc[1] = (5 * d.stock[4] + 1) % 16;
		else if (d.stock[4] <= 47)
			d.fonc[1] = (3 * d.stock[4] + 5) % 16;
		else if (d.stock[4] <= 63)
			d.fonc[1] = (7 * d.stock[4]) % 16;
		d.stock[3] = d.hset[3];
		d.hset[3] = d.hset[2];
		d.hset[2] = d.hset[1];
		d.hset[1] = d.hset[1] + rotl((d.hset[0] + d.fonc[0] + g_t[d.stock[4]]
					+ d.x[d.fonc[1]]), g_r[d.stock[4]]);
		d.hset[0] = d.stock[3];
		d.stock[4]++;
	}
	return (d);
}

static t_md5		ft_hash_md5(t_md5 d)
{
	d.h[0] = 0x67452301;
	d.h[1] = 0xefcdab89;
	d.h[2] = 0x98badcfe;
	d.h[3] = 0x10325476;
	d.stock[0] = 0;
	while (d.stock[0] < (d.stock[2] / 16))
	{
		d.stock[1] = 0;
		while (d.stock[1] < 16)
		{
			d.x[d.stock[1]] = d.m[16 * d.stock[0] + d.stock[1]];
			d.stock[1]++;
		}
		d.hset[0] = d.h[0];
		d.hset[1] = d.h[1];
		d.hset[2] = d.h[2];
		d.hset[3] = d.h[3];
		d = ft_exec_round(d);
		d.stock[0]++;
		d.h[0] = d.h[0] + d.hset[0];
		d.h[1] = d.h[1] + d.hset[1];
		d.h[2] = d.h[2] + d.hset[2];
		d.h[3] = d.h[3] + d.hset[3];
	}
	return (d);
}

int					ft_md5_resolve(char *str)
{
	unsigned char	*addr;
	unsigned char	*padded_addr;
	t_md5			d;

	addr = (unsigned char*)str;
	d.size = ft_strlen(str) * sizeof(unsigned char);
	padded_addr = ft_first_step(addr, d.size);
	if (!padded_addr)
		return (-1);
	d.stock[2] = (d.size * 8) + 1;
	while (d.stock[2] % 512 != 448)
		d.stock[2]++;
	d.stock[2] = (d.stock[2] + 64) / 32;
	d.m = (unsigned int*)malloc(sizeof(unsigned int) * d.stock[2]);
	if (!d.m)
		return (-1);
	d = ft_fill_m(d, padded_addr);
	d = ft_hash_md5(d);
	ft_memory_line(d.h, 16, "0123456789abcdef");
	free(d.m);
	free(padded_addr);
	return (0);
}
