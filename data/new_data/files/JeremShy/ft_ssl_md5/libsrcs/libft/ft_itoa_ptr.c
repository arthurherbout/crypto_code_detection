/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/04 19:24:51 by magouin           #+#    #+#             */
/*   Updated: 2016/02/01 15:37:10 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static int		compte(unsigned long nbr)
{
	int ret;

	ret = 0;
	while (nbr != 0)
	{
		nbr /= 16;
		ret++;
	}
	return (ret);
}

char			*ft_itoa_ptr(void *ptr)
{
	char			*retour;
	int				nb;
	int				i;
	char			tab[17];
	unsigned long	nbr;

	nbr = (unsigned long)ptr;
	if (nbr == 0)
		return (ft_alloc_str("0x0"));
	ft_strcpy(tab, "0123456789abcdef");
	nb = compte(nbr);
	retour = mallocp(nb + 1 + 2);
	ft_strcpy(retour, "0x");
	i = 1;
	while (nbr != 0)
	{
		retour[nb - i + 2] = tab[nbr % 16];
		nbr /= 16;
		i++;
	}
	retour[nb + 2] = '\0';
	return (retour);
}
