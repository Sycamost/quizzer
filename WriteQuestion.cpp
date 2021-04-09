#include <algorithm>
#include <iostream>
#include "Question.h"
#include "WriteQuestion.h"
#include "Command.h"
#include "WriteFlashcard.h"
#include "CmdHandler.h"
#include "util.h"

const std::vector<WriteQuestion> WriteQuestion::_instances = std::vector<WriteQuestion>({
	WriteQuestion(
		QuestionType::FLASHCARD,
		WriteFlashcard::startWritingMessage,
		WriteFlashcard::startNextInputData,
		WriteFlashcard::inputData,
		WriteFlashcard::cancel,
		WriteFlashcard::resetLastInputStep,
		WriteFlashcard::pushCurrent,
		WriteFlashcard::writeToFile)
});

WriteQuestion::WriteQuestion(
	QuestionType type,
	std::wstring startWritingMessage,
	void(*startInputData)(),
	void(*inputData)(std::wstring),
	void(*cancel)(),
	void(*resetLastInputStep)(),
	void(*pushCurrent)(std::vector<std::wstring>),
	std::vector<Question*>(*writeToFile)())
{
	_type = type;
	_stage = Stage::NEXT_QUESTION;
	_tags = std::vector<std::wstring>();
	_startWritingMessage = startWritingMessage;
	_startInputData = startInputData;
	_inputData = inputData;
	_cancel = cancel;
	_resetLastInputStep = resetLastInputStep;
	_pushCurrent = pushCurrent;
	_writeToFile = writeToFile;
}

const QuestionType WriteQuestion::getCurrentType()
{
	return _currentInstance._type;
}

void WriteQuestion::inputData(std::wstring userInput)
{
	return _currentInstance._inputData(userInput);
}

void WriteQuestion::setCurrentType(const QuestionType qt)
{
	auto iter = std::find_if(
		_instances.begin(),
		_instances.end(),
		[qt](WriteQuestion wq) -> bool { return wq._type == qt; });
	if (iter == _instances.end())
		return;
	_currentInstance = *iter;
}

const WriteQuestion::Stage WriteQuestion::getStage()
{
	_currentInstance._stage;
}

void WriteQuestion::setStage(Stage stage)
{
	if (stage == Stage::NEXT_QUESTION)
	{
		std::wcout << L"Would you like to write another "
			<< questionTypeDisplay.at(getCurrentType())
			<< L"? [Y/N]\n";

		if (getUserYesNo())
		{
			setStage(Stage::INPUT_DATA);
			return;
		}

		finishWriting();
		return;
	}

	if (stage == Stage::INPUT_DATA)
	{
		inputData(L"");
		return;
	}

	if (stage == Stage::INPUT_TAGS)
	{
		std::wcout << L"Tag " << (getTags().size() + 1) << L":\t";
		return;
	}

	return;
}

const std::vector<std::wstring> WriteQuestion::getTags()
{
	return _currentInstance._tags;
}

void WriteQuestion::startWriting(const QuestionType qt)
{
	std::wstring qtDisp = questionTypeDisplay.at(qt);
	std::wcout << "Writing new " << qtDisp << L"s...\n\n";
	setCurrentType(qt);
	std::wcout << _currentInstance._startWritingMessage << "L\n";
	_currentInstance._startInputData();
}

void WriteQuestion::pushTag(std::wstring tag)
{
	_currentInstance._tags.push_back(tag);
}

void WriteQuestion::cancel()
{
	_currentInstance._cancel();
}

void WriteQuestion::resetLastStep()
{
	switch (getStage())
	{
	case Stage::NEXT_QUESTION:
		setStage(Stage::NEXT_QUESTION);
		break;

	case Stage::INPUT_DATA:
		_currentInstance._resetLastInputStep();
		break;

	case Stage::INPUT_TAGS:
		setStage(Stage::INPUT_TAGS);
		break;

	default:
		break;
	}
}

void WriteQuestion::pushCurrent()
{
	_currentInstance._pushCurrent(_currentInstance._tags);
	_currentInstance._tags = std::vector<std::wstring>();
}

void WriteQuestion::finishWriting()
{
	std::wcout << L"\nFinished writing new " << questionTypeDisplay.at(getCurrentType()) << L"s.\n";

	std::vector<Question*> newQuestions = _currentInstance._writeToFile();
	Question::appendQuestionsToList(newQuestions);

	CmdHandler::setHandlerDefault();
}
