#include "minishell.h"

static t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->input = -1;
	cmd->output = -1;
	cmd->reds = NULL;
	cmd->args = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	add_token(t_token **dest, t_token *src)
{
	t_token	*copy;
	t_token	*tmp;

	copy = malloc(sizeof(t_token));
	if (!copy)
		return (1);
	copy->str = ft_strdup(src->str);
	copy->type = src->type;
	copy->next = NULL;
	copy->prev = NULL;
	if (!*dest)
		*dest = copy;
	else
	{
		tmp = *dest;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = copy;
		copy->prev = tmp;
	}
	return (0);
}

static int	assign_token(t_cmd *cmd, t_token *tkn)
{
	if (tkn->type == CMD || tkn->type == WRD)
		return (add_token(&cmd->args, tkn));
	if (tkn->type == RINT || tkn->type == ROUT
		|| tkn->type == DRIN || tkn->type == DROUT
		|| tkn->type == FD || tkn->type == LIM)
		return (add_token(&cmd->reds, tkn));
	return (0);
}

static int	fill_cmds(t_cmd **head, t_token *tkn)
{
	t_cmd	*curr;

	curr = create_cmd();
	if (!curr)
		return (1);
	*head = curr;
	while (tkn)
	{
		if (tkn->type == PIPE)
		{
			curr->next = create_cmd();
			if (!curr->next)
				return (1);
			curr->next->prev = curr;
			curr = curr->next;
		}
		else if (assign_token(curr, tkn))
			return (1);
		tkn = tkn->next;
	}
	return (0);
}

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*cmds;

	cmds = NULL;
	if (!tokens)
		return (NULL);
	if (fill_cmds(&cmds, tokens))
		return (NULL);
	return (cmds);
}
