#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/* INCLUDES                                                                   */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>
# include "./libft/includes/libft.h"
# include <fcntl.h> 
# include <sys/wait.h>

/* ************************************************************************** */
/* DEFINES                                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* STRUCTURES                                                                 */
/* ************************************************************************** */

//struct c'est pour stocker plusieurs variables
//enum (pour enumeration) est une liste de nom associes a des entiers, par exemple les differents types de token. ce sont tous des int.

//le type du token (a completer)
typedef enum e_token_type {
	WORD,			//= 0	//ex : "ls", "fichier.txt"
	PIPE,			//= 1	//ex : |
	REDIR_IN,		//= 2	//ex : <
	REDIR_OUT,		//= 3	//ex : >
	HEREDOC,				//ex : <<
	APPEND					//ex : >>
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next; // obligatoire pour chaîner les tokens
}	t_token;


//structure principale de minishell
typedef struct s_minishell {
	t_token	*tokens;
	char	**envp;
}	t_minishell;

typedef struct s_redir // stock les fichier a reddiriger
{
	char			*file;
	t_token_type	type; // REDIR_IN, REDIR_OUT, etc.
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char		**argv;         // arguments de la commande
	t_redir		*in;            // liste des redirs d’entrée
	t_redir		*out;           // liste des redirs de sortie
	struct s_cmd *next;         // commande suivante (si pipe)
}	t_cmd;


/* ************************************************************************** */
/* PROTO                                                                      */
/* ************************************************************************** */

void	create_token(t_token **lst, char *value, t_token_type type);
void	recup_word(char *input, int *i, t_token **token);



/* ************************************************************************** */
/* DEBUG                                                                      */
/* ************************************************************************** */

#endif