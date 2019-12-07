/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitwise_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:39:09 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:39:13 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint32_t	rotl(uint32_t n, uint32_t x)
{
	return ((x << n) | (x >> (32 - n)));
}

uint32_t	rotr(uint32_t n, uint32_t x)
{
	return (rotl(32 - n, x));
}

uint		end_conv_32(uint nbr)
{
	return (uint)((nbr >> 24) | ((nbr & 0xFF0000) >> 8) |
		((nbr & 0xFF00) << 8) | (nbr << 24));
}

uint64_t	end_conv_64(uint64_t nbr)
{
	return ((uint64_t)end_conv_32(nbr & -1) << 32 | end_conv_32(nbr >> 32));
}
