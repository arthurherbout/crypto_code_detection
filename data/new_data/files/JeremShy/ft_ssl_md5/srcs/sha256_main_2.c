/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_main_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:52 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:41:01 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

int		sha256_padd_buffer(int original_file_size, int r, char *buffer)
{
	size_t	size;

	original_file_size += r;
	size = (r / 64 + 1) * 64;
	if (size - r <= 8)
		size += 64;
	((uint8_t*)buffer)[r] = 0x80;
	ft_bzero(buffer + r + 1, size - r - 8 - 1);
	*(uint64_t*)(buffer + size - 8) = end_conv_64(original_file_size << 3);
	return (size);
}

void	init_constants(uint32_t k[64], uint32_t h[8], uint32_t schedule[64],
	uint32_t working[8])
{
	ft_memcpy(k, (uint32_t[64]){0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
		0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa,
		0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138,
		0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624,
		0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f,
		0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2}
		, 64 * sizeof(uint32_t));
	ft_memcpy(h, (uint32_t[8]){0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19}, 8 * sizeof(uint32_t));
	ft_bzero(schedule, 64 * sizeof(uint32_t));
	ft_bzero(working, 8 * sizeof(uint32_t));
}
