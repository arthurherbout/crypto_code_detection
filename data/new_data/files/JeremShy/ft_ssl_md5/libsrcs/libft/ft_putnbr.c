/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 09:10:19 by magouin           #+#    #+#             */
/*   Updated: 2015/11/26 09:46:46 by magouin          ###   ########.fr       */
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

static	void	ft_printing(int taille, int c)
{
	while (taille >= 0)
	{
		ft_putchar((char)(c / (ft_pow(10, taille)) + (int)'0'));
		c %= ft_pow(10, taille);
		taille--;
	}
}

void			ft_putnbr(int c)
{
	int	taille;
	int	tmp;

	if (c == 0)
		ft_putchar('0');
	if (c == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (c < 0)
	{
		ft_putchar('-');
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
	ft_printing(taille, c);
}
