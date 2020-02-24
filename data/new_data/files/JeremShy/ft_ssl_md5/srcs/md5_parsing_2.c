/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_parsing_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:09 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:13 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void	hash_buffer_md5(ssize_t r, t_params_md5 *params, char *buffer)
{
	int			i;
	static char	nbr_du_milieu[4][4] = {{7, 12, 17, 22},
		{5, 9, 14, 20}, {4, 11, 16, 23}, {6, 10, 15, 21}};

	i = 0;
	while (i <= r / 64 - 1)
	{
		compute_buffer(params, nbr_du_milieu, buffer + i * 16 * sizeof(uint));
		i++;
	}
}

int		ft_init(t_params_md5 *params, size_t *original_file_size,
		int *fd, char *filename)
{
	initialize_buffer(params->buffer);
	initialize_t(params->t);
	*original_file_size = 0;
	if (filename)
	{
		*fd = open(filename, O_RDONLY);
		if (*fd < 0)
		{
			ft_putstr_fd("Can't open file for reading\n", 2);
			return (0);
		}
	}
	else
		*fd = 0;
	return (1);
}
