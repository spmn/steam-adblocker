#include <iostream>
#include <fstream>
#include <vector>

#include "ad-block/ad_block_client.h"

const char *ADBLOCKPLUS_FILTER = "ABPFilterParserData.dat";


void print_help()
{
	std::cout << "Usage: urltest [url] [referrer (optional)]" << std::endl;
}

bool read_file(const std::string& file, std::vector<char>& content)
{
	std::ifstream f(file, std::ios::binary);
	if (!f)
		return 0;

	std::vector<char> temp((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	content = temp;
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		print_help();
		return 1;
	}
	char *url = argv[1];
	char *referrer = nullptr;

	if (argc > 2) 
		referrer = argv[2];

	std::vector<char> content;
	if (!read_file(ADBLOCKPLUS_FILTER, content))
	{
		std::cout << "Can't read file: " << ADBLOCKPLUS_FILTER << std::endl;
		return 1;
	}

	AdBlockClient client;
	client.deserialize(&content[0]);

	if (client.matches(url, FONoFilterOption, referrer))
		std::cout << "Request blocked" << std::endl;
	else
		std::cout << "Request not blocked" << std::endl;

	return 0;
}