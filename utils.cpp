#include "utils.h"

// read in books from given file, store book status in an unoredered_map and return it
std::unordered_map<std::string, int> read_in_books(std::string filename)
{
	std::ifstream file(filename); // Replace 'example.txt' with your file name
	std::string line;
	std::unordered_map<std::string, int> books;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::string word;
			std::stringstream ss(line);
			std::vector<std::string> res;
			while (std::getline(ss, word, ' '))
			{
				res.push_back(word);
			}
			std::string book_name = res[0].substr(0, res[0].size() - 1); // ignore the ending ','
			books[book_name] = std::stoi(res[1]);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file";
	}

	return books;
}

void status_to_buffer(char *buffer, int size, std::unordered_map<std::string, int> &books)
{
	// build the send buffer
	int curIndex = 0;
	size = buffer_size;
	// seprate each entry with ',', seperate book id and quantity with ':'
	for (auto &p : books)
	{
		if (curIndex >= size)
		{
			std::cout << "sending buffer size is too small" << std::endl;
			exit(-1);
		}
		strcpy(buffer + curIndex, p.first.c_str());
		curIndex += p.first.size();
		buffer[curIndex] = ':';
		curIndex++;
		std::string number_str = std::to_string(p.second);
		strcpy(buffer + curIndex, number_str.c_str());
		curIndex += number_str.size();
		buffer[curIndex] = ',';
		curIndex++;
	}
	buffer[--curIndex] = '\0'; // do not use the last ','
}

void buffer_to_status(char *buffer, int size, std::unordered_map<std::string, int> &books)
{
	std::string str(buffer);
	std::stringstream ss(str);
	std::string abook;
	// set the books into the map
	while (getline(ss, abook, ','))
	{
		const long unsigned int pos = abook.find(':');
		if (pos == std::string::npos)
		{
			std::cout << "incorrect message from UDP datagram!" << std::endl;
			exit(-1);
		}
		books[abook.substr(0, pos)] += std::stoi(abook.substr(pos + 1));
	}
}

void printBooks(std::unordered_map<std::string, int> &books)
{
	for (auto &p : books)
	{
		std::cout << p.first << ":" << p.second << std::endl;
	}
}

void printMembers(std::unordered_map<std::string, std::string> &members)
{
	for (auto &p : members)
	{
		std::cout << p.first << ":" << p.second << std::endl;
	}
}

std::unordered_map<std::string, std::string> read_in_members(std::string filename)
{
	std::ifstream file(filename); // Replace 'example.txt' with your file name
	std::string line;
	std::unordered_map<std::string, std::string> members;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::string word;
			std::stringstream ss(line);
			std::vector<std::string> res;
			while (std::getline(ss, word, ' '))
			{
				res.push_back(word);
			}
			std::string member_name = res[0].substr(0, res[0].size() - 1); // ignore the ending ','
			// mysterious unseen character at the end....????
			// sometimes end with \r but sometimes not
			if (res[1][res[1].size() - 1] == '\r')
				res[1] = res[1].substr(0, res[1].size() - 1);
			members[member_name] = res[1];
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file";
	}

	return members;
}

std::string encryptedStr(std::string str)
{
	std::string encrypted = str;
	for (char &c : encrypted)
	{
		if (c >= '0' && c <= '9')
		{
			if (c + 5 > '9')
			{
				c = '0' + c + 4 - '9';
			}
			else
			{
				c += 5;
			}
		}
		if (c >= 'a' && c <= 'z')
		{
			if (c + 5 > 'z')
			{
				c = 'a' + c + 4 - 'z';
			}
			else
			{
				c += 5;
			}
		}
		if (c >= 'A' && c <= 'Z')
		{
			if (c + 5 > 'Z')
			{
				c = 'A' + c + 4 - 'Z';
			}
			else
			{
				c += 5;
			}
		}
	}
	return encrypted;
}

std::string unencrypStr(std::string &str)
{
	std::string unencryped_str = str;
	for (char &c : unencryped_str)
	{
		if (c >= '0' && c <= '9')
		{
			if (c - 5 >= '0')
			{
				c -= 5;
			}
			else
			{
				c = '9' + c - 4;
			}
		}
		if (c >= 'A' && c <= 'Z')
		{
			if (c - 5 >= 'A')
				c -= 5;
			else
			{
				c = 'Z' + c - 4;
			}
		}
		if (c >= 'a' && c <= 'z')
		{
			if (c - 5 >= 'a')
			{
				c -= 5;
			}
			else
			{
				c = 'z' + c - 4;
			}
		}
	}
	return unencryped_str;
}