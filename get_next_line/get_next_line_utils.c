/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 22:02:22 by cbach             #+#    #+#             */
/*   Updated: 2020/12/27 14:55:56 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memset_gnl(void *s, int c, size_t n)
{
	unsigned char	cc;
	unsigned char	*t;

	cc = (unsigned char)c;
	t = (unsigned char *)s;
	while (n--)
		*t++ = cc;
	return (s);
}

void	*ft_calloc_gnl(size_t nmemb, size_t size)
{
	unsigned char *t;

	t = (unsigned char *)malloc(nmemb * size);
	if (t)
		t = (unsigned char *)ft_memset_gnl(t, 0, size * nmemb);
	return (t);
}

int		str_len(const char *s)
{
	int len;

	len = 0;
	while (s && *s++)
		len++;
	return (len);
}

int		str_line_len(const char *s)
{
	int len;
	int is_found;

	len = 0;
	is_found = 0;
	while (s && *s != '\0' && !is_found)
	{
		if (*s == '\n')
		{
			is_found = 1;
			len--;
		}
		len++;
		s++;
	}
	return (is_found ? -len : len);
}

void	gnl_eof(char *buf, int status)
{
	if (status && buf && *buf && buf[str_len(buf) - 1] != '\n'
		&& str_len(buf) != BUFFER_SIZE)
	{
		write(STDIN_FILENO, "  \b\b", 4);
	}
}
