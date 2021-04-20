#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionType.h"

class QuestionWriter
{
public:
	QuestionWriter(
		QuestionType type,
		std::wstring startWritingMessage,
		void (*startInputData)(),
		bool (*inputData)(std::wstring userInput),
		void (*resetLastChildDataStep)(),
		Question* (*constructCurrent)(easy_list::list<std::wstring> tags)
	);
	const std::wstring getStartWritingMessage() const;
	void startInput();
	void resetLastStep();
	void processInput(std::wstring userInput);
	easy_list::list<Question*> writeToFile();

private:
	enum class Stage {
		CHILD_DATA,
		TAGS
	};

	static Stage _stage;
	static easy_list::list<std::wstring> _tags;

	QuestionType _type;
	std::wstring _startWritingMessage;
	easy_list::list<Question*> _newQuestions;

	void (*_startInputData)();
	bool (*_inputData)(std::wstring userInput);
	void (*_resetLastChildDataStep)();
	Question* (*_constructCurrent)(easy_list::list<std::wstring> tags);

	void setStage(Stage stage);
};