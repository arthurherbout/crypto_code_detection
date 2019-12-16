/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/04 19:19:55 by magouin           #+#    #+#             */
/*   Updated: 2016/02/01 15:40:29 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static int	compte(unsigned long nbr, int base)
{
	int ret;

	ret = 0;
	while (nbr != 0)
	{
		nbr /= base;
		ret++;
	}
	return (ret);
}

char		*ft_itoa_base(unsigned int nbr, int base)
{
	char		*retour;
	int			nb;
	int			i;
	char		tab[17];

	if (nbr == 0)
		return (ft_alloc_str("0"));
	ft_strcpy(tab, "0123456789abcdef");
	nb = compte(nbr, base);
	retour = mallocp(nb + 1);
	i = 1;
	while (nbr != 0)
	{
		retour[nb - i] = tab[nbr % base];
		nbr /= base;
		i++;
	}
	retour[nb] = '\0';
	return (retour);
}
