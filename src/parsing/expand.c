/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:50:25 by berila            #+#    #+#             */
/*   Updated: 2025/05/19 17:36:17 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variables(t_gcnode **gc, char *str, t_data *data)
{
    int     i;
    char    *result;
    char    *status_str;
    char    *var_name;
    char    *var_value;
    int     in_single_quote;
    int     in_double_quote;
    int     start;

    result = gc_strdup(gc, "");
    if (!result)
        return (NULL);
    in_single_quote = 0;
    in_double_quote = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            result = ft_strjoin_char_free(gc, result, str[i]);
            i++;
        }
        else if (str[i] == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            result = ft_strjoin_char_free(gc, result, str[i]);
            i++;
        }
        else if (str[i] == '$' && str[i + 1] && (!in_single_quote || data->in_heredoc))
        {
            i++;
            if (str[i] == '?')
            {
                status_str = gc_itoa(gc ,exit_status(0, 0));
                result = ft_strjoin_free(gc, result, status_str);
                gc_free(gc, status_str);
                i++;
            }
			else if (ft_isdigit(str[i]))
			{
				char first_digit = str[i++];
                if (first_digit == '0')
                    result = ft_strjoin_free(gc, result, "minishell");
			}
            else if (str[i] && is_valid_var_char(str[i]))
            {
                start = i;
                while (str[i] && (is_valid_var_char(str[i])))
                    i++;
				var_name = gc_substr(gc, str, start, i - start);
				var_value = get_env(data->env, var_name);
				gc_free(gc, var_name);
				if (var_value)
				{
					if (in_double_quote)
						result = ft_strjoin_free(gc, result, var_value);
					else
						result = word_split_join(gc, result, var_value);
				}
            }
			else
				result = ft_strjoin_char_free(gc, result, '$');
        }
        else 
        {
            result = ft_strjoin_char_free(gc, result, str[i]);
            i++;
        }
    }
    return (result);
}
