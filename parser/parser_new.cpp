#include "parser.hpp"

char *whitespace(char *ch){
  while(isspace(*ch) && ++ch);
    return ch;
}

int main() {
  parser pars;
  server serv;
  location loc;
  char *line;
  pars.fd = open("../server.conf", O_RDONLY);
  while ((pars.res = get_next_line(pars.fd, &line)) > -1) {
	line = whitespace(line);
	if (*line == '#')
		continue;
	if (strncmp(line, "server", 6) == 0)
		line = line + 6;
	std::cout << line << std::endl;
	if (!pars.res)
	  break;
  }
  return 0;
}