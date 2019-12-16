/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:04 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:07 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

int		padd_buffer(int original_file_size, int r, char *buffer)
{
	size_t	size;

	original_file_size += r;
	size = (r / 64 + 1) * 64;
	if (size - r <= 8)
		size += 64;
	((uint8_t*)buffer)[r] = 0x80;
	ft_bzero(buffer + r + 1, size - r - 8 - 1);
	*(uint64_t*)(buffer + size - 8) = original_file_size << 3;
	return (size);
}

int		compute_from_string_md5(char *str, t_opt *opt)
{
	size_t			original_len;
	size_t			end_len;
	void			*buffer;
	t_params_md5	params;

	initialize_buffer(params.buffer);
	initialize_t(params.t);
	original_len = ft_strlen(str);
	if (!(buffer = malloc((original_len + 64) * sizeof(char))))
		return (0);
	ft_strcpy(buffer, str);
	end_len = padd_buffer(0, original_len, buffer);
	hash_buffer_md5(end_len, &params, buffer);
	print_result_32(params.buffer, opt);
	free(buffer);
	return (1);
}

int		if_r_smaller_than_zero(ssize_t *r, int *fd)
{
	if (*fd == 0)
	{
		*r = 0;
		*fd = -1;
		return (1);
	}
	else
	{
		close(*fd);
		ft_putstr_fd("Read error\n", 2);
		return (0);
	}
}

int		read_file(char *filename, t_opt *opt)
{
	char			buffer[8192 + 64];
	int				fd;
	ssize_t			r;
	size_t			original_file_size;
	t_params_md5	params;

	r = 1;
	if (!ft_init(&params, &original_file_size, &fd, filename))
		return (0);
	while ((fd != -1) && ((r = read(fd, buffer, 8192))
		|| original_file_size == 0))
	{
		if (r < 0 && if_r_smaller_than_zero(&r, &fd) == 0)
			return (0);
		if (r > 0 && opt->flags & P_OPT)
			write(1, buffer, r);
		if (r < 8192)
			r = padd_buffer(original_file_size, r, buffer);
		hash_buffer_md5(r, &params, buffer);
		original_file_size += r;
	}
	print_result_32(params.buffer, opt);
	close(fd);
	return (1);
}
