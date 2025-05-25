#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


char	*ptr_to_hex_str(void *ptr)
{
	unsigned long	addr;
	char			*str;
	const char		*hex;
	int				i;

	addr = (unsigned long)ptr;
	str = malloc(17);
	if (!str)
		return (NULL);
	str[16] = '\0';
	hex = "0123456789abcdef";
	i = 15;
	while (addr)
	{
		str[i--] = hex[addr % 16];
		addr /= 16;
	}
	while (i >= 0)
		str[i--] = '0';
	return (str);
}

char	*generate_unique_ptr_string(void)
{
	static void	*used[1024];
	static int	count = 0;
	void		*ptr;

	if (count >= 1024)
		return (NULL);
	ptr = malloc(1);
	if (!ptr)
		return (NULL);
	used[count++] = ptr;
	return (ptr_to_hex_str(ptr));
}

int	main(void)
{
	char	*s;
	int		i = 0;

	while (i < 50)
	{
		s = generate_unique_ptr_string();
		if (s)
		{
			printf("%s\n", s);
			free(s);
		}
		i++;
	}
	return (0);
}
