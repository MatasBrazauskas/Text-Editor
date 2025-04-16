#pragma once

#include <vector>
#include <string>

class CommandLineArea
{
public:
	CommandLineArea();
	~CommandLineArea();

private:
	size_t strarting_X, starting_Y, ending_X, ending_Y;
	std::vector<std::string> commandLineMessage;
	std::string currentCommand;
};
