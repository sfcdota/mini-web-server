#include "parser.hpp"

int			strl(char *str)
{
  char	*s;

  if (!str)
	return (0);
  s = str;
  while (*s)
	s++;
  return (s - str);
}

char		*strj(char *s1, char s2)
{
  unsigned int	i;
  char			*out_str;

  if (!s1 || !s2)
	return (NULL);
  i = -1;
  if (!(out_str = (char *)malloc(sizeof(char) * (strl(s1) + 2))))
	return (NULL);
  while (s1[++i])
	out_str[i] = s1[i];
  free(s1);
  out_str[i++] = s2;
  out_str[i] = '\0';
  return (out_str);
}

int			get_next_line(int fd, char **line)
{
  int		ret;
  char	ch;
  char	*tmp;

  if (!(tmp = (char *)malloc(sizeof(char) * 1)))
	return (-1);
  *tmp = '\0';
  while ((ret = read(fd, &ch, 1)))
  {
	if (ch == '\n')
	  break ;
	if ((tmp = strj(tmp, ch)) == NULL)
	  return (-1);
  }
  *line = tmp;
  if (ret)
	return (1);
  return (0);
}