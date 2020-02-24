/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:39:56 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:39:58 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

int	main_md5(t_opt *opt)
{
	if (opt->flags & S_OPT)
		compute_from_string_md5(opt->content, opt);
	else
		read_file(opt->content, opt);
	return (1);
}
