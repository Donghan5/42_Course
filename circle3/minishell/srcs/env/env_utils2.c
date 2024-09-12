/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 23:12:58 by donghank          #+#    #+#             */
/*   Updated: 2024/09/12 16:28:07 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// create_new_env_var --> goal : to create new env_var
void	create_new_env_var(char *tok_str, t_env_node **env_node)
{
	t_env_node	*new_var;
	char		*new_var_str;

	new_var_str = ft_strdup(tok_str);
	if (!new_var_str)
		exit_error(ALLOC_ERROR);
	new_var = new_env_node((void *)new_var_str);
	if (!new_var)
		exit_error(ALLOC_ERROR);
	env_node_add_front(env_node, new_var);
}

// swap_new_env_var --> goal : to replace env_var
// to using the cur_env_ptr --> to de-allocate current env value
char	*swap_new_env_var(char **cur_e_ptr, char *var_name, char *new_var)
{
	char	*tmp_env;
	char	*fin_env;

	if (!cur_e_ptr || !*cur_e_ptr)
		return (NULL);
	tmp_env = ft_strjoin(var_name, "=");
	if (!tmp_env)
		exit_error(ALLOC_ERROR);
	fin_env = ft_strjoin(tmp_env, new_var);
	if (!fin_env)
		exit_error(ALLOC_ERROR);
	free(tmp_env);
	tmp_env = NULL;
	free(*cur_e_ptr);
	*cur_e_ptr = NULL;
	return (fin_env);
}

// update_new_env_var --> goal : to apply replaced env_var to current_var
// example var=value - update_new_env_var -> val=new_value
void	update_new_env_var(char *var, char *new_val, t_env_node *env_node)
{
	char	*cur_env;
	int		idx;

	while (env_node && env_node->next)
	{
		idx = 0;
		cur_env = (char *)env_node->content;
		if (!cur_env)
		{
			env_node = env_node->next;
			continue ;
		}
		while (cur_env[idx] && var[idx] && (cur_env[idx] == var[idx]))
			idx++;
		if ((cur_env[idx] == '\0' || cur_env[idx] == '=') && !var[idx])
			env_node->content = swap_new_env_var(&cur_env, var, new_val);
		env_node = env_node->next;
	}
}

// add the environ node at front
void	env_node_add_front(t_env_node **env_node, t_env_node *new_node)
{
	new_node->next = *env_node;
	*env_node = new_node;
}

// make a new node(data) to environ
t_env_node	*new_env_node(char *content)
{
	t_env_node	*new_node;

	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
