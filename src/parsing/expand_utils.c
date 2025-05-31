/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:59:50 by berila            #+#    #+#             */
/*   Updated: 2025/05/31 16:59:33 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_quote(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	exp->in_single_quote = !(exp->in_single_quote);
	exp->result = ft_strjoin_char_free(gc, exp->result, str[i]);
	return (i + 1);
}

int	handle_double_quote(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	exp->in_double_quote = !(exp->in_double_quote);
	exp->result = ft_strjoin_char_free(gc, exp->result, str[i]);
	return (i + 1);
}

int	handle_exit_status(t_gcnode **gc, int i, t_expand *exp)
{
	char	*status_str;

	status_str = gc_itoa(gc, exit_status(0, 0));
	exp->result = ft_strjoin_free(gc, exp->result, status_str);
	gc_free(gc, status_str);
	return (i + 1);
}

int	handle_digit_var(t_gcnode **gc, char *str, int i, t_expand *exp)
{
	char	first_digit;

	first_digit = str[i++];
	if (first_digit == '0')
		exp->result = ft_strjoin_free(gc, exp->result, "minishell");
	return (i);
}

int	should_remove_quotes(t_token *token, char *expanded)
{
   
    if (token->quote_type > 0)
    {
        int i = 0;
        while (expanded[i])
        {
            if (((expanded[i] == ' ' || expanded[i] == '\t') 
				&& ft_strchr(expanded, '$')) 
				|| (expanded[i] == '\'' && expanded[i + 1] == '\'')
				|| (expanded[i] == '\"' && expanded[i + 1] == '\"'))
                return (0);
            i++;
        }
        return (1);
    }
    return (0);
}

char	*smart_quote_removal(t_gcnode **gc, char *str, t_token *token)
{
    if (should_remove_quotes(token, str))
    	return (strip_anchored_quotes(gc, str, token->quote_type));
    return (gc_strdup(gc, str));
}


char *strip_anchored_quotes(t_gcnode **gc, char *str, int token_quote_type) {
    size_t len;
	(void)token_quote_type;

    if (!str)
    {    return (NULL);}
	len = ft_strlen(str);
    if (len < 2) { // Cannot be an anchored pair if shorter than 2
        return gc_strdup(gc, str);
    }

    if (str[0] == '\'') { // Original token indicated single-quote preference
        if (str[0] == '\'' && str[len - 1] == '\'') {
            return gc_substr(gc, str, 1, len - 2);
        }
    } else if (str[0] == '\"') { // Original token indicated double-quote preference
        if (str[0] == '\"' && str[len - 1] == '\"') {
            return gc_substr(gc, str, 1, len - 2);
        }
    }
    // If no anchored quotes of the specified type match, or no type specified for removal
    return gc_strdup(gc, str);
}
