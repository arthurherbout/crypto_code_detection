/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarracc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/10 10:41:12 by rmarracc          #+#    #+#             */
/*   Updated: 2019/02/10 10:41:25 by rmarracc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"

int	main(int argc, char **argv)
{
	int	ret;

	if (argc != 2)
	{
		ft_putstr("invalid number of arguments\n");
		return (0);
	}
	ret = ft_md5_resolve(argv[1]);
	if (ret == -1)
		ft_putstr("malloc() error\n");
	else
		ft_putchar('\n');
	return (0);
}
