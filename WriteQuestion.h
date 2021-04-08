#pragma once
#include <fstream>
#include "Question.h"

class WriteQuestion
{
public:
	enum class Stage {
		NEXT_QUESTION,
		INPUT_DATA,
		INPUT_TAGS
	};
	WriteQuestion(
		QuestionType type,
		void (*inputData)(std::wstring),
		void (*cancel)(),
		void (*resetLastStep)(),
		void (*pushCurrent)(),
		std::vector<Question*> (*writeToFile)()
	);
	static const QuestionType getCurrentType();
	static void setCurrentType(const QuestionType qt);
	static const Stage getStage();
	static void setStage(Stage stage);
	static const std::vector<std::wstring> getTags();
	static void pushTag(std::wstring tag);
	static const void inputData(std::wstring);
	static const void finishWriting();
	static const void cancel();
	static const void resetLastStep();
	static const void pushCurrent();
private:
	static WriteQuestion _currentInstance;
	static const std::vector<const WriteQuestion> _instances;
	QuestionType _type;
	std::vector<std::wstring> _tags;
	Stage _stage;
	void (*_inputData)(std::wstring);
	void (*_cancel)();
	void (*_resetLastStep)();
	void (*_pushCurrent)();
	std::vector<Question*> (*_writeToFile)();
};