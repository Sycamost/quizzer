#include <algorithm>
#include <iostream>
#include "Question.h"
#include "WriteQuestion.h"
#include "CmdHandler.h"
#include "util.h"

const std::vector<const WriteQuestion> WriteQuestion::_instances = std::vector<const WriteQuestion>({
	WriteQuestion(/* flashcard */)
});

WriteQuestion::WriteQuestion(
	QuestionType type,
	void(*inputData)(std::wstring),
	void(*cancel)(),
	void(*resetLastStep)(),
	void(*pushCurrent)(),
	std::vector<Question*>(*writeToFile)())
{
	_type = type;
	_stage = Stage::NEXT_QUESTION;
	_tags = std::vector<std::wstring>();
	_inputData = inputData;
	_cancel = cancel;
	_resetLastStep = resetLastStep;
	_pushCurrent = pushCurrent;
	_writeToFile = writeToFile;
}

const QuestionType WriteQuestion::getCurrentType()
{
	return _currentInstance._type;
}

const void WriteQuestion::inputData(std::wstring userInput)
{
	_currentInstance._inputData(userInput);
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

void WriteQuestion::pushTag(std::wstring tag)
{
	_currentInstance._tags.push_back(tag);
}

const void WriteQuestion::cancel()
{
	_currentInstance._cancel();
}

const void WriteQuestion::resetLastStep()
{
	_currentInstance._resetLastStep();
}

const void WriteQuestion::pushCurrent()
{
	_currentInstance._pushCurrent();
}

const void WriteQuestion::finishWriting()
{
	std::wcout << L"\nFinished writing new " << questionTypeDisplay.at(getCurrentType()) << L"s.\n";

	std::vector<Question*> newQuestions = _currentInstance._writeToFile();
	Question::appendQuestionsToList(newQuestions);

	CmdHandler::setHandlerDefault();
}
