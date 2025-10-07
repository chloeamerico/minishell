/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:04:31 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 16:05:21 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_blank_line(const char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (line[i] == '\0');
}

int	need_env_update(char **args)
{
	if (!args || !args[0])
		return (0);
	return (!ft_strcmp(args[0], "cd") || !ft_strcmp(args[0], "unset")
		|| !ft_strcmp(args[0], "export"));
}

void	update_env(char **envp, t_env **env)
{
	if (envp)
	{
		free_env(*env);
		*env = init_env_list(envp);
	}
}

int	handle_single_builtin(t_cmd *cmd, t_env **env)
{
	char	**args;
	char	**envp;
	int		status;

	args = tokens_to_array(cmd->args);
	envp = NULL;
	if (!args)
		return (1);
	if (need_env_update(args))
		envp = env_to_array(*env);
	status = 0;
	if (try_run_builtin(args, &envp, &status))
	{
		if (need_env_update(args))
			update_env(envp, env);
	}
	if (args)
		free_tab(args);
	if (envp)
		free_tab(envp);
	get_global()->last_status = status;
	return (status);
}
