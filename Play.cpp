#include <iostream>
#include <algorithm>
#include <time.h>
#include <random>
#include "Play.h"
#include "CmdHandler.h"
#include "Question.h"
#include "util.h"
#include "globals.h"

CmdHandler::Returns playHandler(std::wstring userInput);

PlayStage Play::getStage() {
	return _stage;
}

void Play::setStage(PlayStage stage)
{
	if (stage == PlayStage::QUESTION)
	{
		_stage = stage;

		_hasAnswered = false;

		if (_index >= _questions.size())
		{
			finishPlaying(std::vector<std::wstring>());
			return;
		}

		std::wcout << L"\n" << _questions[_index]->getQuestion() << L"\n";
		return;
	}

	if (stage == PlayStage::ANSWER)
	{
		_stage = stage;
		_index++;
	}
}

bool Play::updateAnswer(std::wstring answer)
{
	if (!_hasAnswered)
	{
		_isCorrect = _questions[_index]->isCorrect(answer);
		if (_isCorrect)
			_correct++;
		else
			_wrong++;
		_hasAnswered = true;
	}
	return _isCorrect;
}

DECLARE_CMD_FUNC(Play::cmdFuncPlay)
{
	std::wcout << L"Starting play with ";
	_questions = std::vector<Question*>();
	if (args.empty())
	{
		_questions = Question::getQuestionList();
		std::wcout << L"all ";
	}
	else
	{
		std::vector<Question*> questionList = Question::getQuestionList();
		for (unsigned int i = 0; i < questionList.size(); i++)
		{
			std::vector<std::wstring> thisQuestionTags = questionList[i]->getTags();
			std::transform(thisQuestionTags.begin(), thisQuestionTags.end(), thisQuestionTags.begin(), toUpper);
			if (shareAnyElems<std::wstring>(args, thisQuestionTags))
				_questions.push_back(questionList[i]);
		}
	}
	std::wcout << _questions.size()
		<< (_questions.size() > 1 ? L" cards" : L" card") << L"...\n\n"
		<< Globals::horizontalDoubleRule << L"\n\n"
		<< L"You'll get given questions, and you'll have to answer correctly. "
		<< L"If you think you've been marked down unfairly, type the command <"
		<< toLower(Command::getCommandInfo(CommandType::BOOST)->code)
		<< L"> before the next card rolls on. Good luck!\n\n"
		<< Globals::horizontalRule << L"\n\n";

	std::shuffle(_questions.begin(), _questions.end(), std::default_random_engine((unsigned int)time(NULL)));
	_index = 0;
	_correct = 0;
	_wrong = 0;

	setStage(PlayStage::QUESTION);

	CmdHandler::setHandler(playHandler);
	return CmdHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(Play::cmdFuncFinish)
{
	std::wcout << L"Are you sure you want to finish playing?";

	int numSkipped = getNumSkipped();
	if (numSkipped > 0)
	{
		std::wcout << L" You still have "
			<< numSkipped
			<< L" question"
			<< numSkipped > 1 ? L"s " : L" "
			<< L"to play.";
	}

	std::wcout << L" [Y/N]\n";

	if (getUserYesNo())
		Play::finishPlaying();
};

void Play::finishPlaying()
{
	std::wcout << L"\n" << Globals::horizontalRule << L"\n";
	std::wcout << L"\nEnd of play.\n"
		<< L"Final scores: "
		<< _correct << L" correct, "
		<< _wrong << L" incorrect";
	int numSkipped = getNumSkipped();
	if (numSkipped > 0)
		std::wcout << L" and " << numSkipped << L" skipped";
	std::wcout << L".\n" << Globals::horizontalDoubleRule << L"\n\n";

	CmdHandler::setHandlerDefault();
}

DECLARE_CMD_FUNC(Play::cmdFuncBoost)
{
	if (_hasAnswered && !_isCorrect)
	{
		_isCorrect = true;
		_correct++;
		_wrong--;
		return;
	}
	throw new std::exception("Invalid boost.");
};

int Play::getNumCorrect()
{
	return _correct;
}

int Play::getNumWrong()
{
	return _wrong;
}

int Play::getNumSkipped()
{
	return (int)_questions.size() - (_correct + _wrong);
}

std::wstring Play::getCurrentCorrectAnswer()
{
	return _questions[_index]->getAnswer();
}

PlayStage Play::_stage = PlayStage::QUESTION;
std::vector<Question*> Play::_questions = std::vector<Question*>();
unsigned int Play::_index = 0;
int Play::_correct = 0;
int Play::_wrong = 0;
bool Play::_hasAnswered = false;
bool Play::_isCorrect = true;

CmdHandler::Returns playHandler(std::wstring userInput)
{
	Command* command = Command::read(userInput);

	if (command != nullptr)
	{
		command->doCommandFunc();
	}

	if (Play::getStage() == PlayStage::QUESTION)
	{
		bool isCorrect = Play::updateAnswer(userInput);

		if (isCorrect)
		{
			std::wcout << L"Correct! Press enter to continue.\n\n";
			Play::setStage(PlayStage::ANSWER);
			return CmdHandler::Returns::SUCCESS;
		}
		else
		{
			std::wcout << L"Incorrect! The correct answer was:\n"
				<< indent(Play::getCurrentCorrectAnswer(), 1)
				<< "\nPress enter to continue, or enter the command <"
				<< toLower(Command::getCommandInfo(CommandType::BOOST)->code)
				<< "> if you have been marked down unfairly.\n";
			Play::setStage(PlayStage::ANSWER);
			return CmdHandler::Returns::SUCCESS;
		}
	}

	if (Play::getStage() == PlayStage::ANSWER)
	{
		if (command != nullptr && command->getCommandInfo().isType(CommandType::BOOST))
			Play::boost();

		Play::setStage(PlayStage::QUESTION);
		return CmdHandler::Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting play...\n";
	Play::finishPlay();
	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
