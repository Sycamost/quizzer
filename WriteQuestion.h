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
		std::wstring startWritingMessage,
		void (*startInputData)(),
		void (*inputData)(std::wstring),
		void (*cancel)(),
		void (*_resetLastInputStep)(),
		void (*pushCurrent)(std::vector<std::wstring>),
		std::vector<Question*> (*writeToFile)()
	);
	static const QuestionType getCurrentType();
	static void setCurrentType(const QuestionType qt);
	static const Stage getStage();
	static void setStage(Stage stage);
	static const std::vector<std::wstring> getTags();
	static void startWriting(const QuestionType qt);
	static void pushTag(std::wstring tag);
	static void inputData(std::wstring);
	static void finishWriting();
	static void cancel();
	static void resetLastStep();
	static void pushCurrent();
private:
	static WriteQuestion _currentInstance;
	static const std::vector<WriteQuestion> _instances;
	static std::vector<std::wstring> _tags;
	static Stage _stage;
	QuestionType _type;
	std::wstring _startWritingMessage;
	void (*_startInputData)();
	void (*_inputData)(std::wstring userInput);
	void (*_cancel)();
	void (*_resetLastInputStep)();
	void (*_pushCurrent)(std::vector<std::wstring> tags);
	std::vector<Question*> (*_writeToFile)();
};