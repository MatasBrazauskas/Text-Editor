#pragma once

#include <unordered_map>
#include <vector>
#include <string>

class ErrorArea
{
public:
	ErrorArea();
	~ErrorArea();

private:
	struct errorStruct
	{
		errorStruct(size_t r, size_t c, const std::string& e) : row(r), column(c), errorMess(e) {}
		size_t row;
		size_t column;
		std::string errorMess;
	};

	size_t strarting_X, starting_Y, ending_X, ending_Y;
	enum class errorKeywords { ERROR, WARNING, NOTE, ANALYZER};
	std::unordered_map<std::string, std::vector<errorStruct>> errorMap;
};
