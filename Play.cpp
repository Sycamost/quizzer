#include <iostream>
#include <random>
#include <easy_list.h>
#include "Play.h"
#include "Handlers.h"
#include "Question.h"
#include "util.h"
#include "QuestionList.h"
#include "globals.h"

InputHandler::Handler getPlayHandler();

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
			finishPlaying();
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
	_questions = QuestionList::get();
	if (_questions.empty())
	{
		std::wcout << L"Can't play, because there are no questions loaded.\n";
		return InputHandler::Returns::SUCCESS;
	}

	if (!args.empty())
	{
		auto questionHasAnyTag = [args](Question* q) -> bool { return q->getTags().shares(args); };
		_questions = _questions.select(questionHasAnyTag);

		if (_questions.empty())
		{
			if (args.size() == 1)
				std::wcout << L"Can't play with that tag, because there are no questions loaded with that tag.\n";
			else
				std::wcout << L"Can't play with those tags, because there are no questions loaded with any of those tags.\n";
			return InputHandler::Returns::SUCCESS;
		}
	}

	std::wcout << L"Starting play with ";
	if (args.empty())
	{
		std::wcout << L"all ";
	}
	std::wcout << _questions.size()
		<< (_questions.size() == 1 ? L" card" : L" cards") << L"...\n\n"
		<< Globals::horizontalDoubleRule << L"\n\n"
		<< L"You'll get given questions, and you'll have to answer correctly. "
		<< L"If you think you've been marked down unfairly, type the command <"
		<< toLower(CommandInfo::get(CommandType::BOOST)->getCode())
		<< L"> before the next question rolls in. Good luck!\n\n"
		<< Globals::horizontalRule << L"\n\n";

	_questions.shuffle();
	_index = 0;
	_correct = 0;
	_wrong = 0;

	setStage(PlayStage::QUESTION);

	InputHandler::set(getPlayHandler());
	return InputHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(Play::cmdFuncConcede)
{
	std::wstring message = L"Are you sure you want to finish playing?";

	int numSkipped = getNumSkipped();
	if (numSkipped > 0)
	{
		message += L" You still have "
			+ std::to_wstring(numSkipped)
			+ L" question"
			+ (numSkipped > 1 ? L"s " : L" ")
			+ L"to play.";
	}

	if (inputYesNo(message))
		Play::finishPlaying();

	return InputHandler::Returns::SUCCESS;
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

	InputHandler::setDefault();
}

DECLARE_CMD_FUNC(Play::cmdFuncBoost)
{
	if (_hasAnswered && !_isCorrect)
	{
		_isCorrect = true;
		_correct++;
		_wrong--;
		std::wcout << L"Boosted!\n";
		Play::setStage(PlayStage::QUESTION);
		return InputHandler::Returns::SUCCESS;
	}
	return InputHandler::Returns::INVALID_STATE;
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
easy_list::list<Question*> Play::_questions = easy_list::list<Question*>();
unsigned int Play::_index = 0;
int Play::_correct = 0;
int Play::_wrong = 0;
bool Play::_hasAnswered = false;
bool Play::_isCorrect = true;

InputHandler::Handler getPlayHandler()
{
	static InputHandler::Handler playHandler = [](std::wstring input) -> InputHandler::Returns
	{
		Command* command = Command::read(input);
		if (command != nullptr)
		{
			CommandInfo cmdInfo = command->getCommandInfo();

			if (cmdInfo.getType() == CommandType::CONCEDE)
				return command->doCommandFunc();

			else if (cmdInfo.getType() == CommandType::BOOST && Play::getStage() == PlayStage::ANSWER)
				return command->doCommandFunc();

			return InputHandler::Returns::INVALID_STATE;
		}

		if (Play::getStage() == PlayStage::QUESTION)
		{
			bool isCorrect = Play::updateAnswer(input);

			if (isCorrect)
			{
				std::wcout << L"Correct! Press enter to continue.\n\n";
				Play::setStage(PlayStage::ANSWER);
				return InputHandler::Returns::SUCCESS;
			}
			else
			{
				std::wcout << L"Incorrect! The correct answer was:\n"
					<< indent(Play::getCurrentCorrectAnswer(), 1)
					<< "\nPress enter to continue, or enter the command <"
					<< toLower(CommandInfo::get(CommandType::BOOST)->getCode())
					<< "> if you have been marked down unfairly.\n";
				Play::setStage(PlayStage::ANSWER);
				return InputHandler::Returns::SUCCESS;
			}
		}

		if (Play::getStage() == PlayStage::ANSWER)
		{
			Play::setStage(PlayStage::QUESTION);
			return InputHandler::Returns::SUCCESS;
		}

		std::wcout << L"\nSomething went wrong interpreting that input. Exiting play...\n";
		Play::finishPlaying();
		return InputHandler::Returns::CMD_NOT_RECOGNISED;
	};
	return playHandler;
}