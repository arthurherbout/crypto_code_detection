/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 08:05:11 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 09:47:48 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <unistd.h>

static	int		ft_pow(int nb, int pow)
{
	if (pow == 0)
		return (1);
	else
		return (nb * ft_pow(nb, pow - 1));
}

static	void	ft_print_fd(int taille, int c, int fd)
{
	while (taille >= 0)
	{
		ft_putchar_fd((char)(c / (ft_pow(10, taille)) + (int)'0'), fd);
		c %= ft_pow(10, taille);
		taille--;
	}
}

void			ft_putnbr_fd(int c, int fd)
{
	int	taille;
	int	tmp;

	if (c == 0)
		ft_putchar_fd('0', fd);
	if (c == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (c < 0)
	{
		ft_putchar_fd('-', fd);
		c = c * -1;
	}
	tmp = c;
	taille = 0;
	while (tmp != 0)
	{
		tmp /= 10;
		taille += 1;
	}
	taille--;
	ft_print_fd(taille, c, fd);
}
