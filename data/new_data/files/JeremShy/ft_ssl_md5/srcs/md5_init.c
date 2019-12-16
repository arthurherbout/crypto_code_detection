/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:39:50 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:39:53 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void	initialize_buffer(uint *buffer)
{
	buffer[0] = 0x67452301u;
	buffer[1] = 0xefcdab89u;
	buffer[2] = 0x98badcfeu;
	buffer[3] = 0x10325476u;
}

void	initialize_t(uint t[65])
{
	uint i;

	i = 1;
	while (i <= 64)
	{
		t[i] = 4294967296.0 * fabs(sin(i));
		i++;
	}
}
