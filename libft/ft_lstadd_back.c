/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/25 21:44:41 by cbach             #+#    #+#             */
/*   Updated: 2020/05/27 14:48:22 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list *s;

	if (lst && new_node)
	{
      new_node->next = NULL;
		if (!*lst)
			*lst = new_node;
		else
		{
			s = *lst;
			while (s->next)
				s = s->next;
			s->next = new_node;
		}
	}
}
