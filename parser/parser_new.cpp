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

//  init_parser(pars);
//	init_serv(serv);
  pars.fd = open("./server.conf", O_RDONLY);
  while ((pars.res = get_next_line(pars.fd, &line)) > -1) {
	line = whitespace(line);
	std::cout << line << std::endl;
	if (!pars.res)
	  break;
  }
  return 0;
}