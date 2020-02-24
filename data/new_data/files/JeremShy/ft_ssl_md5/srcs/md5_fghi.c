/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_fghi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magouin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 16:39:44 by magouin           #+#    #+#             */
/*   Updated: 2019/01/24 16:39:47 by magouin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint	f(uint b, uint c, uint d)
{
	return ((b & c) | ((~b) & d));
}

uint	g(uint b, uint c, uint d)
{
	return ((b & d) | (c & (~d)));
}

uint	h(uint b, uint c, uint d)
{
	return (b ^ c ^ d);
}

uint	i(uint b, uint c, uint d)
{
	return (c ^ (b | (~d)));
}
