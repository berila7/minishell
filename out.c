t_command *parse_tokens(t_token *tokens)
{
    if (!tokens)
        return NULL;
    
    t_command *cmd_list = NULL;     // Final command list to return
    t_command *current_cmd = NULL;  // Current command being built
    
    // Create first command
    current_cmd = new_command();
    if (!current_cmd)
        return NULL;
    
    t_token *token = tokens;
    while (token)
    {
        if (token->type == TOKEN_WORD)
        {
            // Add word as argument to current command
            add_argument(current_cmd, token->value);
            token = token->next;
        }
        else if (token->type == TOKEN_PIPE)
        {
            // Finish current command, add to list
            add_command(&cmd_list, current_cmd);
            
            // Create new command for after pipe
            current_cmd = new_command();
            if (!current_cmd) {
                free_commands(cmd_list);
                return NULL;
            }
            
            token = token->next;
        }
        else if (token->type == TOKEN_REDIR_IN)
        {
            // Handle input redirection
            token = token->next;
            if (token && token->type == TOKEN_WORD)
            {
                current_cmd->input_file = ft_strdup(token->value);
                token = token->next;
            }
            else
            {
                // Syntax error: missing filename after <
                free_commands(cmd_list);
                free_command(current_cmd);
                return NULL;
            }
        }
        else if (token->type == TOKEN_REDIR_OUT)
        {
            // Handle output redirection
            token = token->next;
            if (token && token->type == TOKEN_WORD)
            {
                current_cmd->output_file = ft_strdup(token->value);
                current_cmd->append_mode = 0;  // Not appending
                token = token->next;
            }
            else
            {
                // Syntax error: missing filename after >
                free_commands(cmd_list);
                free_command(current_cmd);
                return NULL;
            }
        }
        else if (token->type == TOKEN_REDIR_APPEND)
        {
            // Handle append redirection
            token = token->next;
            if (token && token->type == TOKEN_WORD)
            {
                current_cmd->output_file = ft_strdup(token->value);
                current_cmd->append_mode = 1;  // Appending
                token = token->next;
            }
            else
            {
                // Syntax error: missing filename after >>
                free_commands(cmd_list);
                free_command(current_cmd);
                return NULL;
            }
        }
        else if (token->type == TOKEN_HEREDOC)
        {
            // Handle heredoc
            token = token->next;
            if (token && token->type == TOKEN_WORD)
            {
                current_cmd->heredoc_delim = ft_strdup(token->value);
                token = token->next;
            }
            else
            {
                // Syntax error: missing delimiter after <<
                free_commands(cmd_list);
                free_command(current_cmd);
                return NULL;
            }
        }
    }
    
    // Add the last command to the list
    add_command(&cmd_list, current_cmd);
    
    return cmd_list;
}