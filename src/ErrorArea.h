#pragma once
#include "SDL.h"
#include "FontAndColors.h"
#include <unordered_map>
#include <vector>
#include <string>

class ErrorArea
{
public:
	ErrorArea();
	~ErrorArea();

	struct errorStruct
	{
		errorStruct(size_t r, size_t c, const std::string& e) : row(r), column(c), errorMess(e) {}
		size_t row;
		size_t column;
		std::string errorMess;
	};

	size_t starting_X, starting_Y, ending_X, ending_Y;
	enum class errorKeywords { ERROR, WARNING, NOTE, ANALYZER};
	std::unordered_map<std::string, std::vector<errorStruct>> errorMap;

	void DisplayFileArea(SDL_Renderer* renderer, FontAndColors* color);
};
