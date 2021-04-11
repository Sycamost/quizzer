#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionType.h"
#include "QuestionList.h"

class QuestionReader
{
public:
	QuestionReader(
		QuestionType type,
		void(*readChildData)(std::wstring line),
		void(*constructCurrent)(std::vector<std::wstring> tags));
	Question* read(std::wifstream& stream);
private:
	enum class Stage {
		CHILD_DATA,
		TAGS
	};
	QuestionType _type;
	Stage _stage;
	std::vector<std::wstring> _tags;
	void (*_readChildData)(std::wstring line);
	void (*_constructCurrent)(std::vector<std::wstring> tags);

	void setStage(Stage stage);
};