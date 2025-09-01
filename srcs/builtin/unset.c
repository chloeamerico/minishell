#include "minishell.h"

//trouver l’index où se trouve la var recherchée
static int	find_env_index(char **envp, char *key)
{
	int		i;
	size_t	len;

	if (!key)
		return (-1);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1); //si pas trouvee
}

//supp var
int	ft_unset(char **args, char ***envp)
{
	int	i;
	int	idx;

	i = 1;
	while (args[i])
	{
		idx = find_env_index(*envp, args[i]);
		if (idx != -1)
		{
			free((*envp)[idx]);
			while ((*envp)[idx])
			{
				(*envp)[idx] = (*envp)[idx + 1]; // decale elem pour combler la suppression
				idx++;
			}
		}
		i++;
	}
	return (0);
}
