#include "minishell.h"

/*
	to calculate the size of the expanded string
*/
int	size_of_dollar(char *expanded)
{
	int		size;
	int		idx;

	size = 0;
	idx = 0;
	while (expanded[idx])
	{
		if (expanded[idx] == '$')
		{
			if (expanded[idx] == ' ' || expanded[idx] == '\0')
				break ;
			size++;
		}
		idx++;
	}
	return (size);
}

/*
	expander of the heredoc mode
	@params: cmd(read from readline), env
	It convert the command to the expanded string
	not concern about the quote type
*/
char	*expander_heredoc(char *cmd, t_env *env)
{
	int		cmd_len;
	char	*expanded;

	cmd_len = size_of_dollar(cmd, env);
	expanded = (char *)malloc(sizeof(char) * (cmd_len + 1));
	if (!expanded)
		return (NULL);
	copy_strings(cmd, expanded, env);
	return (expanded);
}
