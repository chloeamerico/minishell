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

typedef	struct s_pipeline
{
	int	pipefd1[2];	//premier pipe, avec lecture et ecriture
	int	pipefd2[2];	//duxieme pipe, avec lecture et ecriture
	int	current_pipe;	//quel pipe on utilise actuellement (1 ou 0)
	int prev_pipe;		//quel pipe a ete utilise precedemment (-1 au deb), pr savoir quel pipe fermer
	int	nb_cmd;
}	t_pipeline;

typedef struct s_global
{
	int sig;          /* dernier signal reçu (ex: SIGINT) */
	int last_status;  /* dernier code de sortie pour $?   */
	pid_t child_pid;       /* pid de l'enfant en cours (0 si aucun) */
	int   hd_interrupted;  /* flag: 1 si heredoc annulé par Ctrl-C */
}	t_global;


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
t_cmd *parse_commands(t_token *tokens);

//PIPE
int	exec_pipeline(t_cmd *cmd_list, t_env *env);
char **env_to_array(t_env *env);
char **tokens_to_array(t_token *args);
void	free_tab(char **tab);
int	child_process(int cmd_index, t_pipeline *pipeline, t_cmd *cmd, t_env *env);
void close_all_pipes(t_pipeline *pipeline);
int	exec_simple_cmd(t_cmd *cmd, t_env *env);
void	parent_process(t_pipeline *pipeline, int cmd_index);
int	wait_children_pid(t_pipeline *pipeline, pid_t *pid);
int	loop_pipe(t_pipeline *pipeline, int	cmd_index, t_cmd *current_cmd, pid_t *pids, t_env *env);
int	one_cmd_without_pipe(t_cmd *cmd_list, t_env *env);

//FREE
void	free_token(t_token *token);
void	free_commands(t_cmd *cmd);
void	free_env(t_env *env);

//builtin
int	ft_cd(char **args, char ***envp);
int	ft_echo(char **args);
int	ft_exit(char **args);
int	ft_pwd(void);
int	ft_unset(char **args, char ***envp);
int	try_run_builtin(char **args, char ***envp, int *status);
int   is_single_builtin(t_cmd *cmd);
int   run_builtin_parent(t_cmd *cmd, t_env **env, int *status);
int	print_env(char **env);
int	ft_export(char **args, char ***env);
char *find_key(char *arg);
int	find_index(char *key, char **env);
void	export_one_arg(char *arg, char ***env);


//signaux
void	setup_signals_interactive(void);
void	setup_signals_child(void);
void	setup_signals_hd(void);
t_global *get_global(void);
int	 ms_heredoc(char *delim, int expand, t_env *env);
int	 apply_redirections(t_cmd *cmd, t_env *env);
int	mark_limiter_if_quoted(t_token *tok);
int	apply_redirs(t_cmd *cmd, t_env *env);

//main
char    **split_line(char *line);
void	free_cmd_list(t_cmd *cmds);

/* ************************************************************************** */
/* DEBUG                                                                      */
/* ************************************************************************** */

#endif