#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "QuestionTypeInfo.h"
#include "Option.h"
#include "Command.h"
#include "Write.h"
#include "InputHandler.h"
#include "util.h"

QuestionWriter::Stage QuestionWriter::_stage = QuestionWriter::Stage::CHILD_DATA;
std::vector<std::wstring> QuestionWriter::_tags = std::vector<std::wstring>();

QuestionWriter::QuestionWriter(
	QuestionType type,
	std::wstring startWritingMessage,
	void(*startInputData)(),
	bool(*inputData)(std::wstring userInput),
	void(*resetLastChildDataStep)(),
	Question*(*constructCurrent)(std::vector<std::wstring> tags))
	:
	_type(type),
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
		if (!_inputData(userInput))
			setStage(Stage::TAGS);
		return;
	}

	else if (_stage == Stage::TAGS)
	{
		if (userInput == L"")
		{
			Question* newQuestion = _constructCurrent(_tags);
			if (newQuestion == nullptr)
			{
				std::wcout << L"Something went wrong constructing the "
					<< QuestionTypeInfo::get(_type)->getDisplaySingular()
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
		setStage(Stage::TAGS);
		return;
	}
}

std::vector<Question*> QuestionWriter::writeToFile()
{
	std::wofstream file;
	try
	{
		file.open(QuestionTypeInfo::get(_type)->getFileAddress(), std::ios::app);
		if (!file.is_open())
			throw std::exception("File did not open correctly.");
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
