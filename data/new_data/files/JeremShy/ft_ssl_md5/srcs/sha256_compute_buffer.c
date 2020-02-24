/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_compute_buffer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:24 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:27 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void	prepare_schedule(t_params_sha256 *params, void *buffer)
{
	int	i;

	i = 0;
	while (i <= 15)
	{
		params->schedule[i] = end_conv_32(((uint32_t*)buffer)[i]);
		i++;
	}
	while (i <= 63)
	{
		params->schedule[i] = ps1(params->schedule[i - 2]) +
					params->schedule[i - 7] +
					ps0(params->schedule[i - 15]) + params->schedule[i - 16];
		i++;
	}
}

void	compute_next_hash(t_params_sha256 *params)
{
	uint32_t	*h;
	uint32_t	*working;

	h = params->h;
	working = params->working;
	h[0] += working[0];
	h[1] += working[1];
	h[2] += working[2];
	h[3] += working[3];
	h[4] += working[4];
	h[5] += working[5];
	h[6] += working[6];
	h[7] += working[7];
}

void	sha256_compute_buffer(t_params_sha256 *params, void *buffer)
{
	uint32_t	t;
	uint32_t	t1;
	uint32_t	t2;
	uint32_t	*working;

	working = params->working;
	prepare_schedule(params, buffer);
	ft_memcpy(working, params->h, 8 * sizeof(uint32_t));
	t = 0;
	while (t < 64)
	{
		t1 = working[7] + gs1(working[4]) + ch(working[4],
			working[5], working[6]) + params->k[t] + params->schedule[t];
		t2 = gs0(working[0]) + maj(working[0], working[1], working[2]);
		working[7] = working[6];
		working[6] = working[5];
		working[5] = working[4];
		working[4] = working[3] + t1;
		working[3] = working[2];
		working[2] = working[1];
		working[1] = working[0];
		working[0] = t1 + t2;
		t++;
	}
	compute_next_hash(params);
}
