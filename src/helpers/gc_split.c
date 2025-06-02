/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:07:55 by anachat           #+#    #+#             */
/*   Updated: 2025/06/02 15:04:20 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" // For t_gcnode, gc_malloc, gc_substr, ft_strlen

// New word counting function that respects quotes
static int	gc_word_count_quoted(const char *s)
{
	int		count;
	int		i;
	char	in_quote_char; // 0 for none, '\'' or '"' for active quote

	count = 0;
	i = 0;
	in_quote_char = 0;
	while (s && s[i]) // Added s check for safety
	{
		// Skip leading delimiters (spaces or tabs) if not currently inside a quote
		while (s[i] && (s[i] == ' ' || s[i] == '\t') && !in_quote_char)
		{
			i++;
		}
		if (s[i]) // If not end of string, potential start of a new word
		{
			count++;
			// Scan through the current word
			while (s[i])
			{
				if (s[i] == '\'' || s[i] == '"')
				{
					if (in_quote_char == 0)       // Entering a quote
						in_quote_char = s[i];
					else if (in_quote_char == s[i]) // Exiting the same type of quote
						in_quote_char = 0;
					// If s[i] is a quote but doesn't match in_quote_char (e.g., '" inside ''):
					// it's treated as a literal character within the current word/quote.
				}
				else if ((s[i] == ' ' || s[i] == '\t') && !in_quote_char)
				{
					break; // Delimiter found outside of quotes, so current word ends
				}
				i++; // Move to the next character of the current word
			}
		}
	}
	return (count);
}

// New splitting initialization function that respects quotes
static char	**gc_init_split_quoted(t_gcnode **gc, char *s, char **result_array)
{
	int		i;
	int		j;
	int		word_start;
	char	in_quote_char;

	i = 0;
	j = 0;
	in_quote_char = 0;
	while (s && s[i]) // Added s check
	{
		in_quote_char = 0; // Reset for each new word search
		// Skip leading delimiters
		while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		{
			i++;
		}
		if (s[i]) // Found the start of a word
		{
			word_start = i;
			while (s[i])
			{
				if (s[i] == '\'' || s[i] == '"')
				{
					if (in_quote_char == 0)
						in_quote_char = s[i];
					else if (in_quote_char == s[i])
						in_quote_char = 0;
				}
				else if ((s[i] == ' ' || s[i] == '\t') && !in_quote_char)
				{
					break; // End of word (delimiter found outside quote)
				}
				i++; // Advance within the current word
			}
			// The word is from s[word_start] to s[i-1]
			// gc_substr expects length, which is i - word_start
			result_array[j++] = gc_substr(gc, s, word_start, i - word_start);
		}
	}
	// The memory allocated by gc_malloc is zeroed, so result_array[j] will be NULL.
	// If not, explicitly set: result_array[j] = NULL;
	return (result_array);
}

// Modified main gc_split function
char	**gc_split(t_gcnode **gc, char *s)
{
	char	**res;
	int		num_words;

	if (!s)
		return (NULL);
	
	num_words = gc_word_count_quoted(s);
	// gc_malloc allocates (num_words + 1) for char pointers, and zeroes memory.
	// The last element will thus be NULL.
	res = gc_malloc(gc, (num_words + 1) * sizeof(char *));
	if (!res) // Should be handled by gc_malloc exiting on failure, but good practice
		return (NULL);

	return (gc_init_split_quoted(gc, s, res));
}

// The original static functions (word_count, fill_word, init_spl)
// that did not respect quotes can now be removed from this file
// if this new gc_split is the intended universal replacement.
// If they are used elsewhere, you might need to rename this new gc_split
// or the old one. Assuming you want to *change* this gc_split:
/*
static int	word_count(char *str) // Old version
{
	// ... original implementation ...
}

static char	*fill_word(t_gcnode **gc, char *str, int start, int end) // Old version
{
	// ... original implementation ...
}

static char	**init_spl(t_gcnode **gc, char *s, char **result) // Old version
{
	// ... original implementation ...
}
*/
