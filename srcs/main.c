/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:43:23 by camerico          #+#    #+#             */
/*   Updated: 2025/09/15 14:26:30 by camerico         ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:00:00 by your_login       #+#    #+#             */
/*   Updated: 2025/09/15 00:00:00 by your_login      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

/* Vérifie si la ligne est vide ou contient que des espaces */
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

/* Gère les commandes builtin qui doivent s'exécuter dans le processus parent */
static int	handle_single_builtin(t_cmd *cmd, t_env **env)
{
	char	**args;
	char	**envp;
	int		status;
	int		saved_stdin;
	int		saved_stdout;

	/* Sauvegarde des fd originaux */
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	
	/* Application des redirections si nécessaire */
	if (apply_redirections(cmd, *env))
	{
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}

	/* Conversion en format execve */
	args = tokens_to_array(cmd->args);
	envp = env_to_array(*env);
	
	if (!args || !envp)
	{
		free_tab(args);
		free_tab(envp);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}

	/* Exécution du builtin */
	status = 0;
	if (try_run_builtin(args, &envp, &status))
	{
		/* Si c'est export, on met à jour notre env */
		if (args[0] && !ft_strcmp(args[0], "export"))
		{
			free_env(*env);
			*env = init_env_list(envp);
		}
	}

	/* Restauration des fd */
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);

	/* Nettoyage */
	free_tab(args);
	free_tab(envp);
	
	get_global()->last_status = status;
	return (0);
}

/* Vérifie si c'est une commande builtin simple (sans pipe) */
static int	is_single_builtin_cmd(t_cmd *cmd)
{
	char	**args;
	int		result;

	if (!cmd || cmd->next)
		return (0);
	
	args = tokens_to_array(cmd->args);
	if (!args || !args[0])
	{
		free_tab(args);
		return (0);
	}

	result = (!ft_strcmp(args[0], "cd") || !ft_strcmp(args[0], "export") ||
			  !ft_strcmp(args[0], "unset") || !ft_strcmp(args[0], "exit"));
	
	free_tab(args);
	return (result);
}

/* Traite une ligne de commande complète */
static void	process_line(char *line, t_env **env)
{
	char		**split;
	t_token		*tokens;
	t_cmd		*cmds;
	int			exit_status;

	/* Étape 1: Split de la ligne */
	split = split_line(line);
	if (!split)
		return ;


	export(split, env);
	
	/* Étape 2: Tokenisation */
	tokens = tokenize(split);
	free_split(split);
	if (!tokens)
		return ;

	
	/* Étape 3: Validation syntaxique */
	if (!validate_tokens(tokens))
	{
		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		get_global()->last_status = 2;
		free_token(tokens);
		return ;
	}

	/* Étape 4: Expansion des variables */
	expand_tokens(tokens, *env, get_global()->last_status);

	/* Étape 5: Suppression des quotes */
	delete_quotes(tokens);

	/* Étape 6: Parse en commandes */
	cmds = parse_commands(tokens);
	free_token(tokens);
	if (!cmds)
		return ;

	/* Étape 7: Exécution */
	if (is_single_builtin_cmd(cmds))
	{
		/* Builtin simple - exécution dans le parent */
		handle_single_builtin(cmds, env);
	}
	else
	{
		/* Pipeline ou commande externe */
		exit_status = exec_pipeline(cmds, *env);
		get_global()->last_status = exit_status;
	}

	/* Nettoyage */
	free_cmd_list(cmds);
}

/* Gestion de l'EOF (Ctrl+D) */
static void	handle_eof(void)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(get_global()->last_status);
}

/* Initialisation de l'environnement global */
static int	init_shell(char **envp, t_env **env)
{
	/* Initialisation de la structure globale */
	get_global()->sig = 0;
	get_global()->last_status = 0;
	get_global()->child_pid = 0;
	get_global()->hd_interrupted = 0;

	/* Initialisation de l'environnement */
	*env = init_env_list(envp);
	if (!*env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", STDERR_FILENO);
		return (1);
	}

	/* Configuration des signaux */
	setup_signals_interactive();

	return (0);
}

/* Nettoyage final */
static void	cleanup_shell(t_env *env)
{
	free_env(env);
	rl_clear_history();
}

/* Fonction principale */
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

	/* Vérification des arguments (optionnel) */
	(void)argc;
	(void)argv;

	/* Initialisation */
	if (init_shell(envp, &env))
		return (1);

	/* Boucle principale */
	while (1)
	{
		/* Lecture de la ligne */
		line = readline("minishell$ ");
		
		/* Gestion de l'EOF (Ctrl+D) */
		if (!line)
		{
			handle_eof();
			break ;
		}

		/* Ignore les lignes vides */
		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}

		/* Ajout à l'historique */
		add_history(line);

		/* Traitement de la ligne */
		process_line(line, &env);

		/* Libération de la ligne */
		free(line);
	}

	/* Nettoyage final */
	cleanup_shell(env);
	return (get_global()->last_status);
}
