/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:12:48 by lleichtn          #+#    #+#             */
/*   Updated: 2025/10/03 17:02:10 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

static int	is_blank_line(const char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (line[i] == '\0');
}

static int	need_env_update(char **args)
{
	if (!args || !args[0])
		return (0);
	return (!ft_strcmp(args[0], "cd") || !ft_strcmp(args[0], "unset")
		|| !ft_strcmp(args[0], "export"));
}

static void	update_env(char **envp, t_env **env)
{
	if (envp)
	{
		free_env(*env);
		*env = init_env_list(envp);
	}
}

static int	handle_single_builtin(t_cmd *cmd, t_env **env)
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

//AVANT DE REDUIRE
// static int	handle_single_builtin(t_cmd *cmd, t_env **env)
// {
// 	char	**args;
// 	char	**envp;
// 	int		status;
// 	int		need_env;

// 	args = tokens_to_array(cmd->args);
// 	envp = NULL;
// 	if (!args)
// 		return (1);
// 	need_env = 0;
// 	if (args[0] && (!ft_strcmp(args[0], "cd")
// 			|| !ft_strcmp(args[0], "unset")
// 			|| !ft_strcmp(args[0], "export")))
// 		need_env = 1;
// 	if (need_env)
// 		envp = env_to_array(*env);
// 	status = 0;
// 	if (try_run_builtin(args, &envp, &status))
// 	{
// 		if (args[0] && (!ft_strcmp(args[0], "cd")
// 				|| !ft_strcmp(args[0], "unset")
// 				|| !ft_strcmp(args[0], "export")))
// 		{
// 			if (envp)
// 			{
// 				free_env(*env);
// 				*env = init_env_list(envp);
// 			}
// 		}
// 	}
// 	if (args)
// 		free_tab(args);
// 	if (envp)
// 		free_tab(envp);
// 	get_global()->last_status = status;
// 	return (status);
// }

static int	is_single_builtin_cmd(t_cmd *cmd)
{
	char	**args;
	int		result;

	if (!cmd || cmd->next)
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

static t_cmd	*process_line2(char *line, t_env *env)
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

static void	process_line(char *line, t_env **env)
{
	t_cmd	*cmds;
	int		exit_status;

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

//AVANT DE DIVISER
// static void	process_line(char *line, t_env **env)
// {
// 	char	**split;
// 	t_token	*tokens;
// 	t_cmd	*cmds;
// 	int		exit_status;

// 	split = split_minishell(line);
// 	if (!split)
// 		return ;
// 	tokens = tokenize(split);
// 	free_split(split);
// 	if (!tokens)
// 		return ;
// 	if (!validate_tokens(tokens))
// 	{
// 		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
// 		get_global()->last_status = 2;
// 		free_token(tokens);
// 		return ;
// 	}
// 	expand_tokens(tokens, *env, get_global()->last_status);
// 	delete_quotes(tokens);
// 	cmds = parse_commands(tokens);
// 	free_token(tokens);
// 	if (!cmds)
// 		return ;
// 	if (is_single_builtin_cmd(cmds))
// 	{
// 		handle_single_builtin(cmds, env);
// 		free_cmd_list(cmds);
// 		return ;
// 	}
// 	else
// 	{
// 		exit_status = exec_pipeline(cmds, *env);
// 		get_global()->last_status = exit_status;
// 	}
// 	free_cmd_list(cmds);
// }

static void	cleanup_shell(t_env *env)
{
	if (env)
		free_env(env);
	rl_clear_history();
}

static void	handle_eof(t_env *env)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && isatty(STDERR_FILENO))
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	cleanup_shell(env);
	exit(get_global()->last_status);
}

static int	init_shell(char **envp, t_env **env)
{
	get_global()->sig = 0;
	get_global()->last_status = 0;
	get_global()->child_pid = 0;
	get_global()->hd_interrupted = 0;
	*env = init_env_list(envp);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && isatty(STDERR_FILENO))
		setup_signals_interactive();
	else
		setup_signals_child();
	setup_signals_interactive();
	return (0);
}

static char	*find_line(void)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)
		|| !isatty(STDERR_FILENO))
		return (get_next_line(STDIN_FILENO));
	else
		return (readline("minishell$ "));
}

static int	valid_line_for_main(char *line, t_env **env)
{
	add_history(line);
	process_line(line, env);
	if (get_global()->want_exit)
	{
		free(line);
		cleanup_shell(*env);
		exit(get_global()->exit_code);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

	(void)argc;
	(void)argv;
	if (init_shell(envp, &env))
		return (1);
	while (1)
	{
		line = find_line();
		if (!line)
		{
			handle_eof(env);
			break ;
		}
		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}
		valid_line_for_main(line, &env);
		free(line);
	}
	cleanup_shell(env);
	return (get_global()->last_status);
}

//AVANT DE REDUIRE
// int	main(int argc, char **argv, char **envp)
// {
// 	char	*line;
// 	t_env	*env;

// 	(void)argc;
// 	(void)argv;
// 	if (init_shell(envp, &env))
// 		return (1);
// 	while (1)
// 	{
// 		if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)
// 			|| !isatty(STDERR_FILENO))
// 			line = get_next_line(STDIN_FILENO);
// 		else
// 			line = readline("minishell$ ");
// 		if (!line)
// 		{
// 			handle_eof(env);
// 			break ;
// 		}
// 		if (is_blank_line(line))
// 		{
// 			free(line);
// 			continue ;
// 		}
// 		add_history(line);
// 		process_line(line, &env);
// 		if (get_global()->want_exit)
// 		{
// 			free(line);
// 			cleanup_shell(env);
// 			return (get_global()->exit_code);
// 		}
// 		free(line);
// 	}
// 	cleanup_shell(env);
// 	return (get_global()->last_status);
// }
