/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:43:23 by camerico          #+#    #+#             */
/*   Updated: 2025/06/09 18:57:00 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


// int	main(void)
// {
// 	char	*line;
// 	char	**split;
// 	int		i;

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line)
// 			break; // CTRL+D
// 		if (line[0])
// 			add_history(line);
// 		split = split_minishell(line);
// 		if (!split)
// 		{
// 			free(line);
// 			continue;
// 		}
// 		i = 0;
// 		while (split[i])
// 		{
// 			printf("Token %d: [%s]\n", i, split[i]);
// 			free(split[i]);
// 			i++;
// 		}
// 		free(split);
// 		free(line);
// 	}
// 	return (0);
// }


static void	print_tokens(t_token *token)
{
	while (token)
	{
		printf("Token: %-12s | Type: %d\n", token->str, token->type);
		token = token->next;
	}
}

// 💡 tu peux ajouter une fonction free_token_list si tu veux éviter les leaks

int	main(void)
{
	char	*line;
	char	**split;
	t_token	*tokens;
	int		i;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break; // CTRL+D
		if (line[0])
			add_history(line);
		split = split_minishell(line);
		if (!split)
		{
			free(line);
			continue;
		}

		// (facultatif) afficher le tableau brut découpé
		i = 0;
		while (split[i])
		{
			printf("Split %d: [%s]\n", i, split[i]);
			i++;
		}

		tokens = tokenize(split);
		if (!tokens || !validate_tokens(tokens))
			printf("❌ Ligne invalide : erreur de syntaxe\n");
		else
			print_tokens(tokens);

		// libération mémoire
		i = 0;
		while (split[i])
			free(split[i++]);
		free(split);
		free(line);
		// 💡 free_tokens(tokens); à ajouter si dispo
	}
	return (0);
}
