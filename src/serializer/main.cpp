#include <iostream>
#include <fstream>
#include <string>

#include "ad-block/ad_block_client.h"

const char *DUMP_TO = "ABPFilterParserData.dat";


void print_help()
{
	std::cout << "Usage: serializer [ABP FILTER LIST]" << std::endl;
	std::cout << "Generate \"" << DUMP_TO << "\" from an ABP filter list" << std::endl;
}

bool read_file(const std::string& file, std::string& content)
{
	std::ifstream f(file);
	if (!f)
		return 0;
	
	std::string temp((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	content = temp;
	
	return 1;
}

bool write_file(const std::string& file, const char *data, int size)
{
	std::ofstream f(file, std::ios::binary);
	if (!f)
		return 0;

	f.write(data, size);

	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) 
	{
		print_help();
		return 1;
	}

	std::string content;
	if (!read_file(argv[1], content)) 
	{
		std::cout << "Can't read file: " << argv[1] << std::endl;
		return 1;
	}

	std::cout << "Parsing data... It might take a while" << std::endl;
	AdBlockClient client;
	client.parse(content.c_str());

	std::cout << "Serializing..." << std::endl;
	int size;
	char *data = client.serialize(&size);
	if (!write_file(DUMP_TO, data, size))
	{
		std::cout << "Can't write file: " << DUMP_TO << std::endl;
		return 1;
	}
	delete[] data;

	std::cout << "Done." << std::endl;
	return 0;
}