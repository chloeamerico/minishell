/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:43:23 by camerico          #+#    #+#             */
/*   Updated: 2025/09/04 12:02:08 by lleichtn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

// Libération complète de la liste chaînée de tokens
void	free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next;
        if (tokens->str)
            free(tokens->str);
        free(tokens);
        tokens = tmp;
    }
}

// int main(int argc, char **argv, char **envp)
// {
//     t_env *env_list = NULL;
//     t_token *tokens = NULL;
//     t_token *tmp = NULL;
//     char *input = NULL;
//     char **split = NULL;

//     (void)argc;
//     (void)argv;

//     // Initialisation des signaux
//     setup_signals();

//     // Initialisation des variables d'environnement
//     env_list = init_env_list(envp);
//     if (!env_list)
//     {
//         fprintf(stderr, "[ERREUR] Échec initialisation env\n");
//         return (1);
//     }

//     // Lire la ligne de commande
//     if (read_line(&input) == 0 || !input)
//     {
//         fprintf(stderr, "[ERREUR] Échec lecture ligne\n");
//         return (1);
//     }

//     // Vérifier les guillemets fermants avant split
//     if (check_close_quotes(input))
//     {
//         fprintf(stderr, "[ERREUR] Guillemets non fermés\n");
//         free(input);
//         return (1);
//     }

//     // Découper l'entrée en mots (split)
//     split = split_minishell(input);
//     if (!split)
//     {
//         fprintf(stderr, "[ERREUR] split_minishell a échoué\n");
//         free(input);
//         return (1);
//     }

//     // Tokenisation
//     tokens = tokenize(split);
//     if (!tokens)
//     {
//         fprintf(stderr, "[ERREUR] tokenize a échoué\n");
//         free_split(split);
//         free(input);
//         return (1);
//     }

//     // Expansion des variables
//     expand_tokens(tokens, env_list, 0);

//     // Affichage des tokens pour debug
//     tmp = tokens;
//     for (int i = 0; tmp; i++)
//     {
//         printf("Token[%d]: type=%d, str=%s\n", i, tmp->type, tmp->str);
//         tmp = tmp->next;
//     }

//     // Nettoyage
//     free_tokens(tokens);
//     free_split(split);
//     free(input);
//     return (0);
// }


// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	*env_list;
// 	char	*input;
// 	char	**split;
// 	t_token	*tokens;

// 	(void)argc;
// 	(void)argv;

// 	setup_signals();
// 	env_list = init_env_list(envp);
// 	if (!env_list)
// 	{
// 		fprintf(stderr, "[ERREUR] échec init env\n");
// 		return (1);
// 	}

// 	while (1)
// 	{
// 		tokens = NULL;
// 		split = NULL;
// 		input = NULL;

// 		if (!read_line(&input))
// 		{
// 			printf("exit\n");
// 			break;
// 		}

// 		// Quitter avec la commande "exit"
// 		if (strcmp(input, "exit") == 0)
// 		{
// 			free(input);
// 			break;
// 		}

// 		if (check_close_quotes(input))
// 		{
// 			fprintf(stderr, "[ERREUR] Guillemets non fermés\n");
// 			free(input);
// 			continue;
// 		}

// 		split = split_minishell(input);
// 		if (!split)
// 		{
// 			fprintf(stderr, "[ERREUR] split_minishell a échoué\n");
// 			free(input);
// 			continue;
// 		}
		
// 		export(split, &env_list);
		
// 		tokens = tokenize(split);
// 		if (!tokens)
// 		{
// 			fprintf(stderr, "[ERREUR] tokenize a échoué\n");
// 			free_split(split);
// 			free(input);
// 			continue;
// 		}

// 		expand_tokens(tokens, env_list, 0);

// 		delete_quotes(tokens);

// 		// Affichage debug
// 		for (t_token *tmp = tokens; tmp; tmp = tmp->next)
// 			printf("Token: type=%d, str=%s\n", tmp->type, tmp->str);

// 		// Libération
// 		free_tokens(tokens);
// 		free_split(split);
// 		free(input);
// 	}

// 	// Libérer la liste env_list si tu as une fonction pour ça
// 	// free_env_list(env_list);

// 	return (0);
// }

/* main.c */
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>

/* Fonctions attendues côté projet :
   - char    **split_line(char *line);
   - void     free_split(char **split);
   - t_token  *tokenize(char **split);
   - int      validate_tokens(t_token *tkn);
   - void     expand_tokens(t_token *lst, t_env *env, int last_status);
   - void     delete_quotes(t_token *lst);
   - void     free_tokens(t_token *lst);
   - t_cmd    *parse_commands(t_token *tokens, t_env *env);
   - void     free_cmd_list(t_cmd *cmds);
   - int      exec_pipeline(t_cmd *cmd_list, t_env *env);
   - t_env    *init_env_list(char **envp);
   - void     free_env_list(t_env *env);
   - void     setup_signals_interactive(void);
   - t_global *get_global(void);
*/

/* main.c : harness de test signaux + heredoc */
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

/* --- helpers --- */
static int	is_blank(const char *s)
{
	int i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s[i] == '\0');
}

/* heredoc minimal de test: <<DELIM   (écrit rien, teste juste les signaux) */
static int	handle_heredoc(const char *delim)
{
	char	*line;

	setup_signals_hd();
	get_global()->hd_interrupted = 0;
	while (1)
	{
		line = readline("> ");
		if (get_global()->hd_interrupted == 1)
		{
			get_global()->hd_interrupted = 0;
			if (line)
				free(line);
			break ;
		}
		if (!line)
			break ;
		if (strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
	setup_signals_interactive();
	return (0);
}

/* exécute via /bin/sh -c pour tester Ctrl-C et $? proprement */
static void	run_cmd_via_sh(const char *line)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		setup_signals_child();
		execl("/bin/sh", "sh", "-c", line, (char *)0);
		perror("exec sh");
		_exit(127);
	}
	get_global()->child_pid = pid;
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
	else
	{
		if (WIFEXITED(status))
			get_global()->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			get_global()->last_status = 128 + WTERMSIG(status);
	}
	get_global()->child_pid = 0;
}

/* parse très simple: "heredoc <<DELIM" pour tester handler_hd, sinon exécuter */
static void	handle_line(char *line)
{
	char *p;

	if (!line || is_blank(line))
		return ;
	add_history(line);
	p = strstr(line, "<<");
	if (p)
	{
		p += 2;
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p != '\0')
			handle_heredoc(p);
		return ;
	}
	if (strcmp(line, "exit") == 0)
	{
		int code;

		code = get_global()->last_status;
		free(line);
		exit(code);
	}
	run_cmd_via_sh(line);
}

/* --- boucle principale --- */
int	main(int ac, char **av, char **envp)
{
	char *line;

	(void)ac;
	(void)av;
	(void)envp;
	setup_signals_interactive();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			exit(get_global()->last_status);
		}
		handle_line(line);
		free(line);
	}
	return (0);
}
