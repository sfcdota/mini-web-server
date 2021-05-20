/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 18:55:20 by cbach             #+#    #+#             */
/*   Updated: 2020/05/30 20:09:07 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	unsigned char *t;

	if (s)
	{
		t = (unsigned char *)ft_calloc(ft_strlen(s) + 1, sizeof(char));
		if (t)
			return ((char *)ft_memmove(t, s, ft_strlen(s) + 1));
	}
	return (NULL);
}
