/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:50:08 by berila            #+#    #+#             */
/*   Updated: 2025/05/30 16:56:29 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <dirent.h>
# include <signal.h>
# include <fcntl.h>
# include <termios.h>
# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h> 
# include <string.h>
# include <errno.h>

static int	word_count(char *str)
{
	int		count;
	int		flag_pnt;
	size_t	i;

	count = 0;
	flag_pnt = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] != ' ' || str[i] != '\t') && flag_pnt == 0)
		{
			flag_pnt = 1;
			count++;
		}
		else if (str[i] == ' ' || str[i] == '\t')
			flag_pnt = 0;
		i++;
	}
	return (count);
}

static char	*fill_word(char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((end - start + 1) * sizeof(char));
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	return (word);
}

static char	**init_spl(char *s, char **result)
{
	size_t	i;
	int		j;
	int		s_word;

	i = 0;
	j = 0;
	s_word = -1;
	while (i <= strlen(s))
	{
		if ((s[i] != ' ' && s[i] != '\t') && s_word < 0)
			s_word = i;
		else if (((s[i] == ' ' || s[i] == '\t') || i == strlen(s)) && s_word >= 0)
		{
			result[j] = fill_word(s, s_word, i);
			s_word = -1;
			j++;
		}
		i++;
	}
	return (result);
}

char	**gc_split(char *s)
{
	char	**res;

	if (!s)
		return (NULL);
	res = malloc((word_count(s) + 1) * sizeof(char *));
	return (init_spl(s, res));
}

int main(void)
{
	char *test_str = "Hello	world!	This is    		 a test    	   string.";
	char **result = gc_split(test_str);
	
	if (result)
	{
		for (int i = 0; result[i]; i++)
		{
			printf("Word %d: '%s'\n", i, result[i]);
			free(result[i]); // Free each word
		}
		free(result); // Free the array itself
	}
	
	return 0;
}