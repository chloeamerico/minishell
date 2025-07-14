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
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

/* ************************************************************************** */
/* DEFINES                                                                    */
/* ************************************************************************** */

#define STATE_NONE 0
#define STATE_SINGLE 1
#define STATE_DOUBLE 2

/* ************************************************************************** */
/* STRUCTURES                                                                 */
/* ************************************************************************** */

typedef enum e_type
{
	CMD,   // commande
	WRD,   // mot
	FD,    // mot après un metachar
	LIM,   // limitateur d'un here_doc
	RINT,  // <
	ROUT,  // >
	DRIN,  // <<
	DROUT, // >>
	PIPE //|
}	t_type;

typedef struct s_token
{
	char			*str;
	t_type			type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	int				input;
	int				output;
	t_token			*reds;		//liste chainee des redirections (tokens de type RIN, ROUT, etc..)
	t_token			*args;		//liste chainee des arguments de la commande (tokens de type CMD, WRD)
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}    t_env;


/* ************************************************************************** */
/* PROTO                                                                      */
/* ************************************************************************** */

void	setup_signals(void);
char	**get_env(char **env);
int	read_line(char **line);
t_env	*init_env_list(char **envp);
char	**word_to_array(char **array, char *word);
int	split_input(char ***array, char *line, int i);
char	**split_minishell(char *line);
t_token	*tokenize(char **split);
int	validate_tokens(t_token *tkn);
void	free_split(char **split);
void quote_state(char c, int *state);
void	expand_tokens(t_token *tokens, t_env *env, int exit_status);
char	*get_env_value(char *var, t_env *env, int exit_status);
int	check_close_quotes(char *line);
void	delete_quotes(t_token *tokens);
void	export(char **split, t_env **env);

/* ************************************************************************** */
/* DEBUG                                                                      */
/* ************************************************************************** */

#endif