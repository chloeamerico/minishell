#include "minishell.h"

// Signal envoyer lors de ctrl +C
static void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

char	**init_env(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = strdup(envp[i]);
		if (!copy[i])
			return (NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	**get_env(char **env)
{
	static char	**env_storage = NULL; // la sigleton donc

	if (env)
		env_storage = env;
	return (env_storage);
}

int	read_line(char **line)
{
	*line = readline("minishell$ "); //met minishell$ sur le bash
	if (!*line)
		return (0);
	if (**line)
		add_history(*line); //ajout la ligne de commence
	return (1);
}