#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "QuestionTypeInfo.h"
#include "Option.h"
#include "Command.h"
#include "Write.h"
#include "CmdHandler.h"
#include "util.h"



QuestionWriter::QuestionWriter(
	QuestionType type,
	std::string fileAddress,
	std::wstring startWritingMessage,
	void(*startInputData)(),
	void(*inputData)(std::wstring userInput),
	void(*resetLastChildDataStep)(),
	Question*(*constructCurrent)())
	:
	_type(type),
	_fileAddress(fileAddress),
	_startWritingMessage(startWritingMessage),
	_startInputData(startInputData),
	_inputData(inputData),
	_resetLastChildDataStep(resetLastChildDataStep),
	_constructCurrent(constructCurrent)
{}

const std::wstring QuestionWriter::getStartWritingMessage() const
{
	return _startWritingMessage;
}

void QuestionWriter::startInput()
{
	_stage = Stage::CHILD_DATA;
	_tags = std::vector<std::wstring>();
	_startInputData();
}

void QuestionWriter::resetLastStep()
{
	if (_stage == Stage::CHILD_DATA)
	{
		_resetLastChildDataStep();
		return;
	}

	else if (_stage == Stage::TAGS)
	{
		setStage(Stage::TAGS);
		return;
	}
}

void QuestionWriter::processInput(std::wstring userInput)
{
	if (_stage == Stage::CHILD_DATA)
	{
		_inputData(userInput);
		return;
	}

	else if (_stage == Stage::TAGS)
	{
		if (userInput == L"")
		{
			Question* newQuestion = _constructCurrent();
			if (newQuestion == nullptr)
			{
				std::wcout << L"Something went wrong constructing the "
					<< getQuestionTypeInfo(_type)->displaySingular
					<< " from the input given. Please try again, and ensure all inputs are valid.\n";
				return;
			}
			_newQuestions.push_back(newQuestion);
			Write::nextQuestion();
			return;
		}

		else if (userInput.find(L' ') != std::wstring::npos)
		{
			std::wcout << L"Tags must not contain spaces.\n";
			resetLastStep();
			return;
		}

		_tags.push_back(userInput);
		return;
	}
}

std::vector<Question*> QuestionWriter::writeToFile()
{
	std::wofstream file;
	try
	{
		file.open(_fileAddress, std::ios::app);
		if (!file.is_open())
			throw new std::exception("File did not open correctly.");
	}
	catch (std::exception e)
	{
		std::wcout << L"Oops! Something went wrong when trying to access the destination file. We got the following error message:\n"
			<< e.what() << L"\n";
		return std::vector<Question*>();
	}

	int numWritten;
	for (numWritten = 0; numWritten < _newQuestions.size(); numWritten++)
		_newQuestions[numWritten]->write(file);

	file.close();

	return std::vector<Question*>(_newQuestions.begin(), _newQuestions.begin() + numWritten, std::allocator<Question*>());
}

void QuestionWriter::setStage(Stage stage)
{
	if (stage == Stage::CHILD_DATA)
	{
		_startInputData();
		return;
	}

	else if (stage == Stage::TAGS)
	{
		_stage = stage;
		std::wcout << L"Tag " << (_tags.size() + 1) << L":\t";
		return;
	}

	return;
}
