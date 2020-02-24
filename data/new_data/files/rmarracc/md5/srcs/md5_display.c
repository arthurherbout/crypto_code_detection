/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_display.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarracc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 11:16:20 by rmarracc          #+#    #+#             */
/*   Updated: 2019/02/10 11:16:26 by rmarracc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"

void	ft_memory_line(void *octet, unsigned int rest, char *base)
{
	unsigned char *ptr;

	ptr = octet;
	while (rest > 0)
	{
		ft_putchar(base[*ptr / 16]);
		ft_putchar(base[*ptr % 16]);
		ptr++;
		rest--;
	}
}
