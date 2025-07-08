/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:43:23 by camerico          #+#    #+#             */
/*   Updated: 2025/07/08 17:57:23 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
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


int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	char	*input;
	char	**split;
	t_token	*tokens;

	(void)argc;
	(void)argv;

	setup_signals();
	env_list = init_env_list(envp);
	if (!env_list)
	{
		fprintf(stderr, "[ERREUR] échec init env\n");
		return (1);
	}

	while (1)
	{
		tokens = NULL;
		split = NULL;
		input = NULL;

		if (!read_line(&input))
		{
			printf("exit\n");
			break;
		}

		// Quitter avec la commande "exit"
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}

		if (check_close_quotes(input))
		{
			fprintf(stderr, "[ERREUR] Guillemets non fermés\n");
			free(input);
			continue;
		}

		split = split_minishell(input);
		if (!split)
		{
			fprintf(stderr, "[ERREUR] split_minishell a échoué\n");
			free(input);
			continue;
		}

		tokens = tokenize(split);
		if (!tokens)
		{
			fprintf(stderr, "[ERREUR] tokenize a échoué\n");
			free_split(split);
			free(input);
			continue;
		}

		expand_tokens(tokens, env_list, 0);

		// Affichage debug
		for (t_token *tmp = tokens; tmp; tmp = tmp->next)
			printf("Token: type=%d, str=%s\n", tmp->type, tmp->str);

		// Libération
		free_tokens(tokens);
		free_split(split);
		free(input);
	}

	// Libérer la liste env_list si tu as une fonction pour ça
	// free_env_list(env_list);

	return (0);
}
