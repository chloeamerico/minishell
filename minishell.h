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
	DROUT  // >>
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
	t_token			*reds;
	t_token			*args;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;


/* ************************************************************************** */
/* PROTO                                                                      */
/* ************************************************************************** */

void	setup_signals(void);
char	**init_env(char **envp);
char	**get_env(char **env);
int	read_line(char **line);
char	**word_to_array(char **array, char *word);
int	split_input(char ***array, char *line, int i);
char	**split_minishell(char *line);

/* ************************************************************************** */
/* DEBUG                                                                      */
/* ************************************************************************** */

#endif