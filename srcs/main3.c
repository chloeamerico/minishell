/* ************************************************************************** */
/*                      PROBLÈMES IDENTIFIÉS CTRL+D                          */
/* ************************************************************************** */

/*
PROBLÈMES DANS TON CODE:

1. handle_eof() n'appelle pas cleanup_shell(env)
2. La boucle while(1) a un break mais pas de cleanup
3. Il y a du code mort à la fin de main() qui ne sera jamais exécuté
4. get_global()->want_exit et exit_code ne sont pas initialisés
5. Code dupliqué et incohérent dans la boucle principale

CORRECTIONS NÉCESSAIRES:
*/

/* ************************************************************************** */
/*                       VERSION CORRIGÉE DE MAIN2.C                         */
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

static int  handle_single_builtin(t_cmd *cmd, t_env **env)
{
    char    **args;
    char    **envp;
    int     status;
    int     need_env;

    args = tokens_to_array(cmd->args);
    envp = NULL;
    if (!args)
        return (1);

    /* on ne construit l'envp que si le builtin en a besoin */
    need_env = 0;
    if (args[0] && ( !ft_strcmp(args[0], "cd")
                  || !ft_strcmp(args[0], "unset")
                  || !ft_strcmp(args[0], "export")) )
        need_env = 1;
    if (need_env)
        envp = env_to_array(*env);

    status = 0;
    if (try_run_builtin(args, &envp, &status))
    {
        /* si l'env a pu être modifié, on le reconstruit */
        if (args[0] && ( !ft_strcmp(args[0], "cd")
                      || !ft_strcmp(args[0], "unset")
                      || !ft_strcmp(args[0], "export")) )
        {
            if (envp)
            {
                free_env(*env);
                *env = init_env_list(envp);
            }
        }
    }
    if (args)
        free_tab(args);
    if (envp)
        free_tab(envp);

    get_global()->last_status = status;
    return (status);
}

static int	is_single_builtin_cmd(t_cmd *cmd)
{
	char	**args;
	int		result;

	if (!cmd || cmd->next)
		return (0);
	
	args = tokens_to_array(cmd->args);
	if (!args || !args[0])
	{
		if (args) free_tab(args);
		return (0);
	}

	result = ( !ft_strcmp(args[0], "cd")
        || !ft_strcmp(args[0], "unset")
        || !ft_strcmp(args[0], "exit")
        || !ft_strcmp(args[0], "export")
        || !ft_strcmp(args[0], "pwd")
        || !ft_strcmp(args[0], "echo") );
	
	free_tab(args);
	return (result);
}

static void	process_line(char *line, t_env **env)
{
	char		**split;
	t_token		*tokens;
	t_cmd		*cmds;
	int			exit_status;

	split = split_minishell(line);
	if (!split)
		return ;

	tokens = tokenize(split);
	free_split(split);
	if (!tokens)
		return ;

	if (!validate_tokens(tokens))
	{
		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		get_global()->last_status = 2;
		free_token(tokens);
		return ;
	}

	expand_tokens(tokens, *env, get_global()->last_status);
	delete_quotes(tokens);

	cmds = parse_commands(tokens);
	free_token(tokens);
	if (!cmds)
		return ;

	if (is_single_builtin_cmd(cmds))
	{
		handle_single_builtin(cmds, env);
	}
	else
	{
		exit_status = exec_pipeline(cmds, *env);
		get_global()->last_status = exit_status;
	}

	free_cmd_list(cmds);
}

// VERSION CORRIGÉE de handle_eof - maintenant avec cleanup
static void	handle_eof(t_env *env)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	
	// AJOUT: Cleanup avant exit
	if (env)
		free_env(env);
	rl_clear_history();
	
	// Reset global state si nécessaire
	if (get_global())
	{
		get_global()->child_pid = 0;
		get_global()->hd_interrupted = 0;
		get_global()->sig = 0;
	}
	
	exit(get_global()->last_status);
}

static int	init_shell(char **envp, t_env **env)
{
	get_global()->sig = 0;
	get_global()->last_status = 0;
	get_global()->child_pid = 0;
	get_global()->hd_interrupted = 0;
	
	// AJOUT: Initialiser les champs manquants si ils existent
	// get_global()->want_exit = 0;
	// get_global()->exit_code = 0;

	*env = init_env_list(envp);
	if (!*env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", STDERR_FILENO);
		return (1);
	}

	setup_signals_interactive();
	return (0);
}

static void	cleanup_shell(t_env *env)
{
	if (env)
		free_env(env);
	rl_clear_history();
}

// VERSION CORRIGÉE du main avec gestion propre de EOF
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
		line = readline("minishell$ ");
		
		// CORRECTION: Gestion EOF (Ctrl+D) avec cleanup
		if (!line)
		{
			handle_eof(env);  // Passe env pour cleanup
			break ;  // Cette ligne ne sera jamais atteinte mais on la garde
		}

		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}

		add_history(line);
		process_line(line, &env);
		
		// CORRECTION: Gestion exit propre si tu utilises want_exit
		// if (get_global()->want_exit)
		// {
		// 	free(line);
		// 	cleanup_shell(env);
		// 	return (get_global()->exit_code);
		// }
		
		free(line);
	}

	// Cette partie ne devrait jamais être atteinte car handle_eof() fait exit()
	cleanup_shell(env);
	return (get_global()->last_status);
}