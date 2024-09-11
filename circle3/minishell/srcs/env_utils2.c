/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 23:12:58 by donghank          #+#    #+#             */
/*   Updated: 2024/09/12 00:01:04 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// create_new_env_var --> goal : to create new env_var
void	create_new_env_var(char *tok_str, t_list **env)
{
	t_list	*new_var;
	char	*new_var_str;

	new_var_str = ft_strdup(tok_str);
	if (!new_var_str)
		exit_error(ALLOC_ERROR);
	new_var = ft_lstnew((void *)new_var_str);
	if (!new_var)
		exit_error(ALLOC_ERROR);
	ft_lstadd_front(env, new_var_str);
}

// swap_new_env_var --> goal : to replace env_var
// to using the cur_env_ptr --> to de-allocate current env value
char	*swap_new_env_var(char **cur_env_ptr, char *var_name, char *new_var)
{
	char	*tmp_env;
	char	*fin_env;

	tmp_env = ft_strjoin(var_name, "=");
	if (!tmp_env)
		exit_error(ALLOC_ERROR);
	fin_env = ft_strjoin(tmp_env, new_var);
	if (!fin_env)
		exit_error(ALLOC_ERROR);
	free(tmp_env);
	tmp_env = NULL;
	free(*cur_env_ptr);
	*cur_env_ptr = NULL;
	return (fin_env);
}

// update_new_env_var --> goal : to apply replaced env_var to current_var
// example var=value - update_new_env_var -> val=new_value
void	update_new_env_var(char *var, char *new_value, t_list *env)
{
	char	*cur_env;
	int		idx;

	while (env)
	{
		idx = 0;
		cur_env = (char *)env->content;
		while (cur_env[idx] && var[idx] && (cur_env[idx] == var[idx]))
			idx++;
		if ((cur_env[idx] == '\0' || cur_env[idx] == '=') && !var[idx])
			env->content = swap_new_env_var(&cur_env, var, new_value);
		env = env->next;
	}
}
// shlvl_increment --> goal : implement $SHLVL
