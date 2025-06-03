/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:00:00 by mberila           #+#    #+#             */
/*   Updated: 2025/06/02 20:22:11 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool has_mixed_format(char *str)
{
    if (!str || strlen(str) == 0)
        return false;
    
    int len = strlen(str);
    bool has_quoted = false;
    bool has_unquoted = false;
    int pos = 0;
    
    while (pos < len)
    {
        if (str[pos] == '"')
        {
            // Found start of quoted segment
            pos++; // Skip opening quote
            
            // Find closing quote
            while (pos < len && str[pos] != '"')
                pos++;
            
            if (pos < len) // Found closing quote
            {
                has_quoted = true;
                pos++; // Skip closing quote
            }
            else
            {
                // Unclosed quote, treat as unquoted
                has_unquoted = true;
                break;
            }
        }
        else
        {
            // Found unquoted character
            has_unquoted = true;
            
            // Skip to next quote or end
            while (pos < len && str[pos] != '"')
                pos++;
        }
    }
    
    return has_quoted && has_unquoted;
}

// Main processing function
char *process_mixed_quoted(t_gcnode **gc, char *input, t_data *data)
{
    if (!input)
        return NULL;
    
    // Check if it has mixed format (both quoted and unquoted parts)
    // if (!has_mixed_format(input) 
	// 	|| data->is_export
	// )
    //     return gc_strdup(gc, input); // Return as-is if not mixed format
    
    int len = strlen(input);
    int pos = 0;
    char *result = gc_strdup(gc, "");
    
    if (!result)
        return NULL;
    
    while (pos < len)
    {
        if (input[pos] == '"')
        {
            // Found start of quoted string
            int quote_start = pos;
            pos++; // Skip opening quote
            
            // Find closing quote
            while (pos < len && input[pos] != '"')
                pos++;
            
            if (pos >= len)
            {
                // No closing quote found - shouldn't happen due to has_mixed_format check
                // But handle it anyway by treating as unquoted
                char *remaining = gc_malloc(gc, len - quote_start + 1);
                if (!remaining)
                    return result;
                
                strcpy(remaining, input + quote_start);
                char *expanded = expand_variables(gc, remaining, data);
                if (!expanded)
                    return result;
                
                char *new_result = gc_strjoin(gc, result, expanded);
                return new_result ? new_result : result;
            }
            
            // Extract quoted part including quotes
            int quoted_len = pos - quote_start + 1; // +1 to include closing quote
            char *quoted_part = gc_malloc(gc, quoted_len + 1);
            if (!quoted_part)
                return result;
            
            strncpy(quoted_part, input + quote_start, quoted_len);
            quoted_part[quoted_len] = '\0';
            
            // Process with remove_quotes
            char *unquoted = remove_quotes(gc, quoted_part);
            if (!unquoted)
                return result;
            
            // Join to result
            char *new_result = gc_strjoin(gc, result, unquoted);
            if (!new_result)
                return result;
            result = new_result;
            
            pos++; // Skip closing quote
        }
        else
        {
            // Found unquoted text
            int start = pos;
            
            // Find next quote or end of string
            while (pos < len && input[pos] != '"')
                pos++;
            
            // Extract unquoted part
            int unquoted_len = pos - start;
            if (unquoted_len > 0)
            {
                char *unquoted_part = gc_malloc(gc, unquoted_len + 1);
                if (!unquoted_part)
                    return result;
                
                strncpy(unquoted_part, input + start, unquoted_len);
                unquoted_part[unquoted_len] = '\0';
                
                // Process with expand_variables
                char *expanded = expand_variables(gc, unquoted_part, data);
                if (!expanded)
                    return result;
                
                // Join to result
                char *new_result = gc_strjoin(gc, result, expanded);
                if (!new_result)
                    return result;
                result = new_result;
            }
        }
    }
    
    return result;
}