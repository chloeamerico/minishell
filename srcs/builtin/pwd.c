#include "minishell.h"

int	ft_pwd(void)
{
	char	*buffer;

	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (1);
	ft_printf("%s\n", buffer);
	free(buffer);
	return (0);
}
