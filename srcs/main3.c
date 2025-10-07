/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:06:17 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/07 16:06:42 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_single_builtin_cmd(t_cmd *cmd)
{
	char	**args;
	int		result;

	if (!cmd || cmd->next)
		return (0);
	if (cmd->reds)
		return (0);
	args = tokens_to_array(cmd->args);
	if (!args || !args[0])
	{
		if (args)
			free_tab(args);
		return (0);
	}
	result = (!ft_strcmp(args[0], "cd")
			|| !ft_strcmp(args[0], "unset")
			|| !ft_strcmp(args[0], "exit")
			|| !ft_strcmp(args[0], "export"));
	free_tab(args);
	return (result);
}

t_cmd	*process_line2(char *line, t_env *env)
{
	char	**split;
	t_token	*tokens;
	t_cmd	*cmds;

	split = split_minishell(line);
	if (!split)
		return (NULL);
	tokens = tokenize(split);
	free_split(split);
	if (!tokens)
		return (NULL);
	if (!validate_tokens(tokens))
	{
		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		get_global()->last_status = 2;
		free_token(tokens);
		return (NULL);
	}
	expand_tokens(tokens, env, get_global()->last_status);
	delete_quotes(tokens);
	cmds = parse_commands(tokens);
	free_token(tokens);
	return (cmds);
}

void	process_line(char *line, t_env **env)
{
	t_cmd	*cmds;
	int		exit_status;

	get_global()->hd_interrupted = 0;
	cmds = process_line2(line, *env);
	if (!cmds)
		return ;
	if (is_single_builtin_cmd(cmds))
	{
		handle_single_builtin(cmds, env);
		free_cmd_list(cmds);
		return ;
	}
	exit_status = exec_pipeline(cmds, *env);
	get_global()->last_status = exit_status;
	free_cmd_list(cmds);
}

void	cleanup_shell(t_env *env)
{
	if (env)
		free_env(env);
	rl_clear_history();
}
