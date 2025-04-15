/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:50:01 by anachat           #+#    #+#             */
/*   Updated: 2025/04/14 21:03:44 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// Makes a deep copy of the environment linked list
t_env *copy_env_list(t_env *env)
{
    t_env *head;
    t_env *new_node;
    t_env *current;
    t_env *prev;

    if (!env)
        return (NULL);
    
    head = NULL;
    prev = NULL;
    current = env;
    
    while (current)
    {
        new_node = (t_env *)malloc(sizeof(t_env));
        if (!new_node)
        {
            // Cleanup if malloc fails
            while (head)
            {
                t_env *temp = head;
                head = head->next;
                free(temp->key);
                if (temp->value)
                    free(temp->value);
                free(temp);
            }
            return (NULL);
        }
        
        new_node->key = ft_strdup(current->key);
        new_node->value = current->value ? ft_strdup(current->value) : NULL;
        new_node->next = NULL;
        
        if (prev)
            prev->next = new_node;
        else
            head = new_node;
            
        prev = new_node;
        current = current->next;
    }
    
    return (head);
}

// Comparison function for sorting environment variables by key
int compare_env_keys(t_env *a, t_env *b)
{
    return (ft_strcmp(a->key, b->key));
}

// Sorts the environment linked list alphabetically by key
void sort_env_list(t_env **env)
{
    t_env *result;
    t_env *current;
    t_env *next;
    t_env **merged;
    int list_size;
    int sublist_size;
    int i;
    
    if (!env || !(*env) || !(*env)->next)
        return;
    
    // Count the size of the list
    list_size = 0;
    current = *env;
    while (current)
    {
        list_size++;
        current = current->next;
    }
    
    // Merge sort
    result = NULL;
    for (sublist_size = 1; sublist_size < list_size; sublist_size *= 2)
    {
        current = *env;
        merged = &result;
        
        while (current)
        {
            // First sublist
            t_env *left = current;
            t_env *left_end = left;
            for (i = 1; i < sublist_size && left_end->next; i++)
                left_end = left_end->next;
                
            // Second sublist
            t_env *right = left_end->next;
            left_end->next = NULL;  // Detach first sublist
            
            t_env *right_end = right;
            for (i = 1; i < sublist_size && right_end && right_end->next; i++)
                right_end = right_end->next;
                
            // Prepare for next iteration
            if (right_end)
            {
                next = right_end->next;
                right_end->next = NULL;  // Detach second sublist
            }
            else
            {
                next = NULL;
            }
            
            // Merge the two sublists
            while (left || right)
            {
                if (!right || (left && compare_env_keys(left, right) <= 0))
                {
                    *merged = left;
                    left = left->next;
                }
                else
                {
                    *merged = right;
                    right = right->next;
                }
                merged = &((*merged)->next);
            }
            
            current = next;
        }
        
        *env = result;
        result = NULL;
    }
}









int	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}

void	ft_export(char **args, t_data *data)
{
	char	*key;
	char	*val;
	char	*equals;
	int		i;

	if (count_args(args) == 1)
	{
		t_env *new = copy_env_list(data->env);
		sort_env_list(&new);
		print_env(new);
	}
	else if (count_args(args) > 1)
	{
		i = 1;
		while (args[i])
		{
			equals = ft_strchr(args[i], '=');
			if (equals)
			{
				key = ft_substr(args[i], 0, (equals - args[i]));
				val = ft_strdup(equals + 1);
				set_env(&data->env, key, val);
			}
			i++;
		}
	}
}
