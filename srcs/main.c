/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camerico <camerico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:18:48 by camerico          #+#    #+#             */
/*   Updated: 2025/08/15 17:41:18 by camerico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


// Fonction principale de traitement d'une ligne de commande
static int process_line(char *line, t_env **env, int *exit_status)
{
    char **split;
    t_token *tokens;
    t_cmd *cmds;
    int result;
    
    // Étape 1: Split de la ligne
    split = split_minishell(line);
    if (!split)
    {
        printf("minishell: syntax error: unclosed quotes\n");
        return (1);
    }
    
    // Étape 2: Gestion de export/env avant tokenisation
    export(split, env);
    
    // Si c'était juste export/env, on s'arrête là
    if (split[0] && (!ft_strcmp(split[0], "export") || !ft_strcmp(split[0], "env")))
    {
        free_split(split);
        return (0);
    }
    
    // Étape 3: Tokenisation
    tokens = tokenize(split);
    free_split(split);
    if (!tokens)
    {
        printf("minishell: tokenization error\n");
        return (1);
    }
    
    // Étape 4: Validation des tokens
    if (!validate_tokens(tokens))
    {
        printf("minishell: syntax error\n");
        free_token(tokens);
        return (1);
    }
    
    // Étape 5: Expansion des variables
    expand_tokens(tokens, *env, *exit_status);
    
    // Étape 6: Suppression des quotes
    delete_quotes(tokens);
    
    // Étape 7: Parsing des commandes
    cmds = parse_commands(tokens);
    free_token(tokens);
    if (!cmds)
    {
        printf("minishell: command parsing error\n");
        return (1);
    }
    
    // Étape 8: Exécution
    result = exec_pipeline(cmds, *env);
    free_commands(cmds);
    
    return (result);
}

// Fonction pour gérer les commandes built-in
static int handle_builtins(char *line, t_env **env)
{
    char **split;
    
    // Gestion de exit
    if (!ft_strncmp(line, "exit", 4) && (line[4] == '\0' || line[4] == ' '))
    {
        split = split_minishell(line);
        if (split && split[1])
        {
            int exit_code = ft_atoi(split[1]);
            free_split(split);
            free_env(*env);
            printf("exit\n");
            exit(exit_code);
        }
        free_split(split);
        free_env(*env);
        printf("exit\n");
        exit(0);
    }
    
    return (0); // Pas un builtin géré ici
}

int main(int argc, char **argv, char **envp)
{
    char *line;
    t_env *env;
    int exit_status;
    
    (void)argc;
    (void)argv;
    
    // Initialisation
    env = init_env_list(envp);
    if (!env)
    {
        printf("minishell: environment initialization failed\n");
        return (1);
    }
    
    setup_signals();
    exit_status = 0;
    
    printf("Welcome to minishell!\n");
    printf("Available commands: any shell command, export, env, exit\n");
    printf("Test examples:\n");
    printf("  - Simple: ls -la\n");
    printf("  - Pipes: ls | grep txt | wc -l\n");
    printf("  - Redirections: echo hello > file.txt\n");
    printf("  - Variables: export VAR=value; echo $VAR\n");
    printf("  - Exit status: echo $?\n\n");
    
    // Boucle principale
    while (1)
    {
        // Lecture de la ligne
        if (!read_line(&line))
        {
            printf("\nGoodbye!\n");
            break;
        }
        
        // Ligne vide
        if (!*line)
        {
            free(line);
            continue;
        }
        
        // Gestion des built-ins spéciaux
        if (handle_builtins(line, &env))
        {
            free(line);
            continue;
        }
        
        // Traitement de la ligne
        exit_status = process_line(line, &env, &exit_status);
        
        free(line);
    }
    
    // Nettoyage final
    free_env(env);
    clear_history();
    
    return (exit_status);
}