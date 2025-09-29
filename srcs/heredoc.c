#include "minishell.h"

static char	*hd_expand(char *s, t_env *env, int last)
{
	t_token	fake;
	t_token	one;
	char	*res;

	fake.next = &one;
	fake.prev = NULL;
	one.next = NULL;
	one.prev = &fake;
	one.type = WRD;
	one.str = ft_strdup(s);
	if (!one.str)
		return (ft_strdup(""));
	expand_tokens(&one, env, last);
	res = ft_strdup(one.str);
	free(one.str);
	return (res);
}

static int	hd_loop(int wfd, char *delim, int expand, t_env *env)
{
	char	*l;
	char	*e;

	while (1)
	{
		if (get_global()->hd_interrupted)
            return (1); // Interrompu par signal
		l = readline("> ");
		if (!l)
        {
            printf("heredoc delimited by end-of-file (wanted `%s')\n", delim);
			return (0);
        }	            
        if (get_global()->hd_interrupted) // Vérifier après readline aussi
        {
            free(l);
            return (1);
        }
		if (!ft_strcmp(l, delim))
			return (free(l), 0);
		if (expand)
		{
			e = hd_expand(l, env, get_global()->last_status);
			if (!e)
				return (free(l), 1);
			free(l);
			l = e;
		}
		if (write(wfd, l, ft_strlen(l)) < 0 || write(wfd, "\n", 1) < 0)
			return (free(l), 1);
		free(l);
	}
}

int ms_heredoc(char *delim, int expand, t_env *env)
{
    int     p[2];
    pid_t   pid;
    int     st;
    void    (*old_int)(int);
    void    (*old_quit)(int);

    if (pipe(p) < 0)
    {
        perror("pipe");
        return (-1);
    }
    get_global()->hd_interrupted = 0;

    old_int = signal(SIGINT, SIG_IGN);
    old_quit = signal(SIGQUIT, SIG_IGN);

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        close(p[0]);
        close(p[1]);
        /* restaurer */
        signal(SIGINT, old_int);
        signal(SIGQUIT, old_quit);
        return (-1);
    }

    if (pid == 0)
    {
        /* Enfant heredoc : gère SIGINT, sort direct */
        setup_signals_hd();
        close(p[0]);
        if (hd_loop(p[1], delim, expand, env))
            _exit(130);
        close(p[1]);
        _exit(0);
    }

    /* Parent */
    get_global()->child_pid = pid;
    close(p[1]);

    if (waitpid(pid, &st, 0) < 0)
    {
        perror("waitpid");
        close(p[0]);
        signal(SIGINT, old_int);
        signal(SIGQUIT, old_quit);
        return (-1);
    }
    get_global()->child_pid = 0;

    /* Restaurer les signaux interactifs */
    signal(SIGINT, old_int);
    signal(SIGQUIT, old_quit);

    if (WIFEXITED(st))
    {
        int exit_status = WEXITSTATUS(st);
        if (exit_status == 0)
            return (p[0]);
        if (exit_status == 130)
        {
            get_global()->hd_interrupted = 1;
            get_global()->last_status = 130;
            write(STDOUT_FILENO, "\n", 1);
            close(p[0]);
            return (-1);
        }
    }
    else if (WIFSIGNALED(st))
    {
        get_global()->hd_interrupted = 1;
        get_global()->last_status = 130;
        write(STDOUT_FILENO, "\n", 1);
    }

    close(p[0]);
    return (-1);
}
