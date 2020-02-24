/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_stages.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:15 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:18 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void	functions(t_params_md5 *params, t_fghi function, uint n)
{
	uint	*a;
	uint	*b;
	uint	*c;
	uint	*d;

	a = params->buffer + (4 - n) % 4;
	b = params->buffer + (5 - n) % 4;
	c = params->buffer + (6 - n) % 4;
	d = params->buffer + (7 - n) % 4;
	*a = *b + cshift((*a + function(*b, *c, *d)
		+ (params->x)[params->k] + params->t[params->i]), params->s);
}

void	stage1(t_params_md5 *params, char nbr_du_milieu[4][4])
{
	uint	i;

	i = 0;
	while (i < 16)
	{
		params->k = i;
		params->s = nbr_du_milieu[0][i % 4];
		params->i = i + 1;
		functions(params, f, i % 4);
		i++;
	}
}

void	stage2(t_params_md5 *params, char nbr_du_milieu[4][4])
{
	uint	i;

	i = 0;
	params->k = 12;
	while (i < 16)
	{
		params->k = (params->k + 5) % 16;
		params->s = nbr_du_milieu[1][i % 4];
		(params->i)++;
		functions(params, g, i % 4);
		i++;
	}
}

void	stage3(t_params_md5 *params, char nbr_du_milieu[4][4])
{
	uint	i;

	i = 0;
	params->k = 2;
	while (i < 16)
	{
		params->k = (params->k + 3) % 16;
		params->s = nbr_du_milieu[2][i % 4];
		(params->i)++;
		functions(params, h, i % 4);
		i++;
	}
}

void	stage4(t_params_md5 *params, char nbr_du_milieu[4][4])
{
	uint	j;

	j = 0;
	params->k = 9;
	while (j < 16)
	{
		params->k = (params->k + 7) % 16;
		params->s = nbr_du_milieu[3][j % 4];
		(params->i)++;
		functions(params, i, j % 4);
		j++;
	}
}
