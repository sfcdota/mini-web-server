#include "parser.hpp"

int			get_next_line(int fd, std::string &str)
{
	int		ret;
	char	ch;
	while ((ret = read(fd, &ch, 1)))
	{
		if(ret == -1)
			return -1;
		if (ch == '\n')
			break ;
		str += ch;
	}
	if (ret)
		return (1);
	return (0);
}
