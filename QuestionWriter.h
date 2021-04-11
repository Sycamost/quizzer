#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionType.h"

class QuestionWriter
{
public:
	QuestionWriter(
		QuestionType type,
		std::string fileAddress,
		std::wstring startWritingMessage,
		void (*startInputData)(),
		void (*inputData)(std::wstring userInput),
		void (*resetLastChildDataStep)(),
		Question* (*constructCurrent)()
	);
	const std::wstring getStartWritingMessage() const;
	void startInput();
	void resetLastStep();
	void processInput(std::wstring userInput);
	std::vector<Question*> writeToFile();

private:
	enum class Stage {
		CHILD_DATA,
		TAGS
	};

	static Stage _stage;
	static std::vector<std::wstring> _tags;

	QuestionType _type;
	std::string _fileAddress;
	std::wstring _startWritingMessage;
	std::vector<Question*> _newQuestions;

	void (*_startInputData)();
	void (*_inputData)(std::wstring userInput);
	void (*_resetLastChildDataStep)();
	Question* (*_constructCurrent)();
	std::vector<Question*> (*_writeToFile)();

	void setStage(Stage stage);
};