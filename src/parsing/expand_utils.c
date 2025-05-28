/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:59:50 by berila            #+#    #+#             */
/*   Updated: 2025/05/28 20:04:01 by berila           ###   ########.fr       */
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
    // If the token had quotes originally and the expansion doesn't contain spaces,
    // or if we're in a quoted context, we should remove shell quotes
	printf("should_remove_quotes: token->quote_type = %d, expanded = [%s]\n", token->quote_type, expanded);
    if (token->quote_type > 0)
    {
        // Check if the expanded value contains spaces
        int i = 0;
        while (expanded[i])
        {
            if ((expanded[i] == ' ' || expanded[i] == '\t') && ft_strchr(expanded, '$'))
                return (0); // Don't remove quotes if there are spaces
            i++;
        }
        return (1); // Remove quotes if no spaces
    }
    return (0);
}

char	*smart_quote_removal(t_gcnode **gc, char *str, t_token *token)
{
    if (should_remove_quotes(token, str))
        return (remove_quotes(gc, str));
    return (gc_strdup(gc, str));
}
