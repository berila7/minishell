/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:26:28 by anachat           #+#    #+#             */
/*   Updated: 2025/05/19 11:58:14 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(char **args, t_data *data)
{
	if (count_args(args) > 1)
		unset_env(&data->gc, &data->env, args[1]);
}
