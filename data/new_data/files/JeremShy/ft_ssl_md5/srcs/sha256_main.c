/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:36 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:42 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static int	ft_init_sha256(size_t *original_file_size, int *fd, char *filename)
{
	*original_file_size = 0;
	if (!filename)
		*fd = 0;
	else
		*fd = open(filename, O_RDONLY);
	if (*fd < 0)
	{
		ft_putstr_fd("Can't open file for reading\n", 2);
		return (0);
	}
	return (1);
}

void		hash_buffer_sha256(ssize_t r, t_params_sha256 *params, void *buffer)
{
	int	i;

	i = 0;
	while (i < r / 64)
	{
		sha256_compute_buffer(params, buffer + i * 16 * sizeof(uint32_t));
		i++;
	}
}

int			compute_from_string(char *str, t_params_sha256 *params, t_opt *opt)
{
	size_t	original_len;
	size_t	end_len;
	void	*buffer;

	original_len = ft_strlen(str);
	if (!(buffer = malloc((original_len + 64) * sizeof(char))))
		return (0);
	ft_strcpy(buffer, str);
	end_len = sha256_padd_buffer(0, original_len, buffer);
	hash_buffer_sha256(end_len, params, buffer);
	print_result_64((unsigned char *)params->h, opt);
	free(buffer);
	return (1);
}

int			read_file_sha256(char *filename, t_params_sha256 *params,
	t_opt *opt)
{
	char		buffer[8192 + 64];
	int			fd;
	ssize_t		r;
	size_t		original_file_size;

	if (!ft_init_sha256(&original_file_size, &fd, filename))
		return (0);
	while (fd != -1 && ((r = read(fd, buffer, 8192)) ||
		original_file_size == 0))
	{
		if (r < 0 && (fd == 0 && ((fd = -1) || 1)))
			r = 0;
		else if (r < 0 && (close(fd) || 1))
		{
			ft_putstr_fd("Read error\n", 2);
			return (0);
		}
		r > 0 && opt->flags & P_OPT ? write(1, buffer, r) : 0;
		r = r < 8192 ? sha256_padd_buffer(original_file_size, r, buffer) : r;
		hash_buffer_sha256(r, params, buffer);
		original_file_size += r;
	}
	print_result_64((unsigned char *)params->h, opt);
	close(fd);
	return (1);
}

int			main_256(t_opt *opt)
{
	uint32_t		k[64];
	uint32_t		h[8];
	uint32_t		schedule[64];
	uint32_t		working[8];
	t_params_sha256	params;

	init_constants(k, h, schedule, working);
	params.k = k;
	params.h = h;
	params.schedule = schedule;
	params.working = working;
	if (opt->flags & S_OPT)
		compute_from_string(opt->content, &params, opt);
	else
		read_file_sha256(opt->content, &params, opt);
	return (0);
}
