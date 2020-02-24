/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:40:20 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:40:23 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static int	print_str_and_ret(char *str1, char c)
{
	ft_putstr_fd(str1, 2);
	write(2, &c, 1);
	ft_putstr_fd("\n", 2);
	return (0);
}

static int	get_option(char *option, t_opt *opt, int (*fun) (t_opt*))
{
	int	i;

	i = 1;
	while (option[i])
	{
		if (option[i] >= 'p' && option[i] <= 's')
			opt->flags |= 1 << (option[i] - 'p');
		else
			return (print_str_and_ret("Illegal option: ", option[i]));
		if (opt->flags & S_OPT)
		{
			opt->content = option[i + 1] == '\0' ? NULL : option + i + 1;
			return (1);
		}
		else if (opt->flags & P_OPT)
		{
			opt->content = NULL;
			fun(opt);
			opt->flags &= ~P_OPT;
		}
		i++;
	}
	return (1);
}

static int	handle_s_opt(t_opt *opt, int (*fun)(t_opt*), char **av, int *i)
{
	if (!opt->content)
	{
		if (!av[*i + 1])
			return (0);
		opt->content = av[*i + 1];
		(*i)++;
	}
	fun(opt);
	opt->flags &= ~S_OPT;
	return (1);
}

static int	do_parsing(char **av, t_opt *opt, int (*fun) (t_opt*))
{
	int	i;
	int	ignore;

	i = 2;
	ignore = 0;
	while (av[i])
	{
		if (av[i][0] == '-' && !ignore)
		{
			if (!get_option(av[i], opt, fun))
				return (0);
			if (opt->flags & S_OPT && handle_s_opt(opt, fun, av, &i) == 0)
				return (0);
		}
		else
		{
			opt->content = av[i];
			fun(opt);
			ignore = 1;
		}
		i++;
	}
	if (opt->content == NULL && !(av[2] && !av[3] && ft_strequ("-p", av[2])))
		fun(opt);
	return (1);
}

int			parse_options(int ac, char **av, t_opt *opt)
{
	int (*fun) (t_opt*);

	if (ac < 2)
		return (0);
	ft_bzero(opt, sizeof(t_opt));
	if (ft_strequ(av[1], "md5"))
		fun = main_md5;
	else if (ft_strequ(av[1], "sha256"))
		fun = main_256;
	else
	{
		ft_putstr_fd("ft_ssl: Error: '", 2);
		ft_putstr_fd(av[1], 2);
		ft_putstr_fd("' is an invalid command.\n\nStandard commands:\n\n\
Message Digest commands:\nmd5\nsha256\n", 2);
		return (0);
	}
	if (!do_parsing(av, opt, fun))
		return (0);
	return (1);
}
