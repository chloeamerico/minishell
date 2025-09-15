
#include "minishell.h"


/* libère une liste chaînée de t_token (str + maillon) */
static void	free_token_list(t_token *t)
{
	t_token	*n;

	while (t)
	{
		n = t->next;
		if (t->str)
			free(t->str);
		free(t);
		t = n;
	}
}

/* libère la liste d'env (key/value + maillon) */
void	free_env_list(t_env *env)
{
	t_env	*n;

	while (env)
	{
		n = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = n;
	}
}

/* libère la liste de commandes :
   - ferme les fd valides
   - libère args (t_token*) et reds (t_token*)
   - libère le maillon t_cmd */
void	free_cmd_list(t_cmd *cmds)
{
	t_cmd	*n;

	while (cmds)
	{
		n = cmds->next;
		if (cmds->input >= 0)
			close(cmds->input);
		if (cmds->output >= 0)
			close(cmds->output);
		if (cmds->args)
			free_token_list(cmds->args);
		if (cmds->reds)
			free_token_list(cmds->reds);
		free(cmds);
		cmds = n;
	}
}
