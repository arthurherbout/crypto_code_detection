/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_padding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarracc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 11:16:57 by rmarracc          #+#    #+#             */
/*   Updated: 2019/02/10 11:31:05 by rmarracc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"

unsigned char	*ft_first_step(unsigned char *addr, size_t size)
{
	size_t			new_size;
	size_t			b;
	unsigned char	*new_ptr;

	new_size = (size * 8) + 1;
	while (new_size % 512 != 448)
		new_size++;
	new_size = new_size / 8;
	new_ptr = (unsigned char*)ft_memalloc(
			sizeof(unsigned char) * (new_size + 8));
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, addr, size);
	new_ptr[size] = 128;
	b = size * 8;
	ft_memcpy(new_ptr + new_size, &b, 8);
	return (new_ptr);
}

unsigned int	rotl(unsigned int x, unsigned int y)
{
	return (((x) << (y)) | ((x) >> (32 - (y))));
}
