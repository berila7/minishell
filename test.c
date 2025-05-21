// Online C compiler to run C program online
#include <stdio.h>
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9' )
	{
		return (1);
	}
	return (0);
}
int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (c);
	return (0);
}
int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (c);
	return (0);
}
int is_valid_env_key(char *str)
{
	size_t	i;
	
	if (!str || (str[0] != '_' && !ft_isalpha(str[0])))
		return (0);

	// Can contain only letters, digits, or underscores ([a-zA-Z0-9_])
	i = -1;
	while (str[++i])
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (0);
	}
	return (1);	
}


int main(int ac, char **av) {
	(void)ac;
    // Write C code here
    printf(is_valid_env_key(av[1]) ? "✅ Valid ENV Key [%s]\n" : "❌ Invalid ENV Key [%s]\n", av[1]) ;

    return 0;
}