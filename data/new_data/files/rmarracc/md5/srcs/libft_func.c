#include "md5.h"

void ft_putchar(char c)
{
	write(1, &c, 1);
}

int ft_strlen(char *s)
{
	return(*s ? ft_strlen(++s) + 1 : 0);
}

void ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *d;
	const unsigned char *s;

	d = dest;
	s = src;
		while (n > 0)
		{
			*d = *s;
			s++;
			d++;
			n--;
		}
	return (dest);
}

void	*ft_memset(void *str, int c, size_t n)

{
	unsigned char *ptr;

	ptr = str;
	n++;
	while (--n > 0)
		*ptr++ = c;
	return (str);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}

void	*ft_memalloc(size_t size)
{
	void *addr;

	addr = malloc(size);
	if (!addr)
		return (NULL);
	ft_bzero(addr, size);
	return (addr);
}
