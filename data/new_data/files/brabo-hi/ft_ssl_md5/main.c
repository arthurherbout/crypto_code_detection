# include "sha512.h"
# include "libft/libft.h"
# include <stdio.h>


void    init_words(char *str);
char    *get_words(char *str);
char    *ft_concert_string_to_binary(char *input);
char    *ft_convert_string_to_hex(char *input);
void    get_hash(void);


int main()
{
//	char *words = get_words("abc");
	size_t i;

	char *words = get_words("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	i = ft_strlen(words) / 256;

	while (i-- > 0)
	{
		char *str;
		if (!(str = ft_memalloc(257)))
			return (-1);
		ft_strncpy(str, words, 256);
		str[256] = 0;
		init_words(str);
		get_hash();
		words += 256;
	}
	return (0);
}

void    get_hash(void)
{
	uint64_t    a, b, c, d, e, f, g, h;

	a = g_hash[0];
	b = g_hash[1];
	c = g_hash[2];
	d = g_hash[3];
	e = g_hash[4];
	f = g_hash[5];
	g = g_hash[6];
	h = g_hash[7];
//	printf("%llx-%llx-%llx-%llx-%llx-%llx-%llx-%llx\n", g_hash[0], g_hash[1], g_hash[2], g_hash[3], g_hash[4], g_hash[5], g_hash[6], g_hash[7]);

	int i = 0;
	while (i < 80)
	{

		uint64_t    ch = Ch(e, f, g);
		uint64_t    maj = Maj(a, b, c);
		uint64_t    sigma0 = SIGMA0(a);
		uint64_t    sigma1 = SIGMA1(e);
		uint64_t T1 = h + sigma1 + ch + g_constants[i] + (g_words[i]);
		uint64_t T2 = sigma0 + maj;

		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
		printf("[%d] %llx-%llx-%llx-%llx\n", i, a, b, c, d);
		printf("[%d] %llx-%llx-%llx-%llx\n\n", i, e ,f, g, h);
		i++;
	}
	g_hash[0] = a + g_hash[0];
	g_hash[1] = b + g_hash[1];
	g_hash[2] = c + g_hash[2];
	printf("H[%llx] e[%llx] [%llx]\n", g_hash[3], d, d + g_hash[3]);
	g_hash[3] = d + g_hash[3];
	g_hash[4] = e + g_hash[4];
	g_hash[5] = f + g_hash[5];
	g_hash[6] = g + g_hash[6];
	g_hash[7] = h + g_hash[7];
	printf("%llx %llx %llx %llx %llx %llx %llx %llx\n", g_hash[0], g_hash[1], g_hash[2], g_hash[3], g_hash[4], g_hash[5], g_hash[6], g_hash[7]);
}

char    *get_words(char *str)
{
	char    *bin_input;
	char    *bin_size;
	char    *bin;
	size_t  padding;
	size_t  len_input_size;
	size_t  n_block;

	padding = 0;
	n_block = 0;
	bin_input = ft_str_new_suffix(ft_convert_string_to_hex(str), '8', 1);
	bin_size = ft_itoa_base((int)(ft_strlen(str) * 8), 16);
//	printf("%s - %zu - %s\n", bin_input, ft_strlen(bin_input) , bin_size);
	len_input_size = ft_strlen(bin_input) + ft_strlen(bin_size);
	if (ft_strlen(bin_input) <= 224)
	{
	//	printf("%zu - %s\n", ft_strlen(bin_input), bin_input);
		n_block = 1;
		padding = 256 - ft_strlen(bin_input) - ft_strlen(bin_size);
	} else
	{
		size_t  total = 256;
		while (total < ft_strlen(bin_input) + 32)
		{
			total += 256;
			n_block++;
		}
		padding = total - ft_strlen(bin_input) - ft_strlen(bin_size) + ((n_block - 1) * 256);
	//	printf("n_block[%zu] total [%zu] padding [%zu]\n", n_block, total, padding);
	}


/*	if (len_input_size < 256)
		padding = 256 - len_input_size;
	else if (len_input_size > 256)
	{
		padding = len_input_size % 256;
		n_block = len_input_size / 256;
	}*/
	if (!(bin = ft_memalloc((n_block + 1) * 256 + 1)))
		return (NULL);
	//return (ft_strjoin(ft_str_new_suffix(ft_strcpy(bin, bin_input) , '0', padding * n_block), bin_size));
	return (ft_strjoin(ft_str_new_suffix(ft_strcpy(bin, bin_input) , '0', padding), bin_size));
}

void    init_words(char *str)
{
	int i;
	int j;
	char hex[17];
	j = 0;
	while (str && *str)
	{
		i = 0;
		while (i < 16)
			hex[i++] = *str++;
		hex[i] = 0;
		g_words[j++] = (uint64_t) strtol(hex, NULL, 16);
	}
	while (j < 80)
	{
		g_words[j] = SIGMA1_W(g_words[j - 2]) + g_words[j - 7] + SIGMA0_W(g_words[j - 15]) + g_words[j - 16];
		j++;
	}
}

char    *ft_concert_string_to_binary(char *input)
{

	char    *bin_input;
	char    *bin_char;
	char    *tmp;

	bin_input = NULL;
	while (input && *input)
	{
		bin_char = ft_itoa_base(*input++, 2);
		bin_char = ft_str_new_prefix(bin_char, '0', 8 - ft_strlen(bin_char));
		tmp = ft_str_new_concat(bin_input, bin_char);
		ft_strdel(&bin_char);
		ft_strdel(&bin_input);
		bin_input = tmp;
	}
	return (bin_input);
}

char    *ft_convert_string_to_hex(char *input)
{
	char    *bin_input;
	char    *bin_char;
	char    *tmp;

	bin_input = NULL;
	while (input && *input)
	{
		bin_char = ft_itoa_base(*input++, 16);
		tmp = ft_str_new_concat(bin_input, bin_char);
		ft_strdel(&bin_char);
		ft_strdel(&bin_input);
		bin_input = tmp;
	}
	return (bin_input);
}