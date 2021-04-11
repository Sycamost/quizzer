#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionList.h"

class QuestionReader
{
private:
	enum class Stage {
		CHILD_DATA,
		TAGS
	};
	Stage _stage;
	std::vector<std::wstring> _tags;
	void setStage(Stage stage);
public:
	Question* read(std::wifstream& stream);
};