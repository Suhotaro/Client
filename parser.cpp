#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "parser.h"
#include "util.h"

parser_error XMLParser::init()
{
	std::cout << "XML init" << std::endl;

	in.open (file_name, std::ifstream::in);
	EXITIFTRUE(in.fail(), "open \"%s\" file failed", file_name.c_str());

	return PARSER_ERROR_NONE;
}

parser_error XMLParser::process(int &low, int &high)
{
	std::string line;
	std::string value;
	std::string trash;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());

		/* read tag names */
		iss >> trash;
		/* check if a line starts with <low> tag word*/
		if (!trash.compare(0, 5, "<low>"))
		{
			/* read <low> tag value  */
			iss >> value;
			low = atoi(value.c_str());

			/* read next line which must be <high> tag */
			std::getline(in, line);
			iss.str(line.c_str());

			/* skipp <high> tag word*/
			iss >> trash;
			/* read <high> tag value */
			iss >> value;
			high = atoi(value.c_str());

			//std::cout << "range: " << low << " " << high << std::endl;

			return PARSER_ERROR_NONE;
		}
	}

	return PARSER_ERROR_EOF;
}

void XMLParser::deinit()
{
	std::cout << "XML deinit" << std::endl;

	if (in.is_open())
		in.close();
}
