/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleichtn <lleichtn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:43:23 by camerico          #+#    #+#             */
/*   Updated: 2025/09/02 17:04:41 by lleichtn         ###   ########.fr       */
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

static int	process_line(char *line, t_env **env)
{
	char	**split;
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	if (!line || line[0] == '\0')
		return (0);
	add_history(line);
	split = split_line(line);
	if (!split)
		return (write(2, "minishell: split failed\n", 24), 1);
	tokens = tokenize(split);
	free_split(split);
	if (!tokens || !validate_tokens(tokens))
	{
		if (tokens)
			free_tokens(tokens);
		get_global()->last_status = 2;
		return (write(2, "minishell: syntax error\n", 24), 1);
	}
	expand_tokens(tokens, *env, get_global()->last_status);
	delete_quotes(tokens);
	cmds = parse_commands(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		get_global()->last_status = 2;
		return (write(2, "minishell: parse error\n", 23), 1);
	}
	status = exec_pipeline(cmds, *env);
	free_cmd_list(cmds);
	get_global()->last_status = status;
	return (0);
}

static void	shell_loop(t_env **env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (process_line(line, env))
		{
			free(line);
			continue ;
		}
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;

	(void)ac;
	(void)av;
	setup_signals_interactive();
	env = init_env_list(envp);
	if (!env)
		return (write(2, "minishell: env init failed\n", 27), 1);
	get_global()->last_status = 0;
	shell_loop(&env);
	clear_history();
	free_env_list(env);
	return (0);
}
