#include "parser.hpp"

void split_words(std::string str, std::vector<std::string> &arr)
{
	std::string word = "";
	for (int i = 0; i < str.size(); i++)
		if (str[i] == ' ' || str[i] == '\t')
		{
			if (word.size())
				arr.push_back(word);
			word = "";
		}
		else
			word += str[i];
	if (word.size())
		arr.push_back(word);
}

int main() {
	int fd;
	int find;
	std::string str;
	int index;
	bool server_status;
	bool location_status;


	std::vector<std::string> string_arr;

	fd = open("./server.conf", O_RDONLY);

	while (get_next_line(fd, str)) {
		split_words(str, string_arr);

		for(int i = 0; i < string_arr.size(); i++)
			std::cout << string_arr[i] << " ";
		if (string_arr[0][0] == '#')
			continue;
		std::cout << std::endl;
//		find  = str.find("server");
//		std::cout << str << std::endl;
//		std::cout << find << std::endl;
//		std::cout << "continue" << std::endl;
		string_arr.clear();
		str = "";
	}
//	std::cout << str << std::endl;

	return 0;
}