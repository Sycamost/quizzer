#include <iostream>
#include <random>
#include <easy_list.h>
#include "Play.h"
#include "Handlers.h"
#include "Question.h"
#include "util.h"
#include "QuestionList.h"
#include "globals.h"

DECLARE_INPUT_HANDLER_FUNC(receiveAnswerInputHandler);
DECLARE_INPUT_HANDLER_FUNC(nextQuestionInputHandler);

/// <summary>
/// If there's another question to ask, asks it and waits for an answer. If not, ends play.
/// </summary>
void Play::askQuestion()
{
	// Hang on - have we finished all the questions yet?
	if (_index >= _questions.size())
	{
		finishPlaying();
		return;
	}

	// Ask the question and wait for an answer.
	const std::wstring msg = L"\nFront:\t" + _questions[_index]->getQuestion() + L"\nBack:\t";
	setHandling(msg, &receiveAnswerInputHandler, CommandType::CONCEDE);
	return;
}

/// <summary>
/// Moves the counter on to the next question, and waits to see if the user wants to boost the last question before moving on.
/// </summary>
void Play::nextQuestion()
{
	static const std::wstring correctMsg = L"Press enter to continue.\n";
	static const std::wstring wrongMsg =
		L"Press enter to continue, or enter the command <"
		+ toLower(CommandInfo::get(CommandType::BOOST)->getCode())
		+ L"> if you have been marked down unfairly.\n";
	_index++;
	setHandling(
		_isCorrect ? correctMsg : wrongMsg,
		&nextQuestionInputHandler,
		CommandType::CONCEDE, CommandType::BOOST
	);
}

bool Play::updateAnswer(std::wstring answer)
{
	_isCorrect = _questions[_index]->isCorrect(answer);
	if (_isCorrect)
		_correct++;
	else
		_wrong++;
	return _isCorrect;
}

DEFINE_CMD_FUNC(Play::cmdFuncPlay)
{
	_questions = QuestionList::get();
	if (_questions.empty())
	{
		std::wcout << L"Can't play, because there are no questions loaded.\n";
		return CommandHandlerReturns::SUCCESS;
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
			return CommandHandlerReturns::SUCCESS;
		}
	}

	std::wcout << L"Starting play with ";
	if (args.empty() && _questions.size() > 1)
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

	askQuestion();
	return CommandHandlerReturns::SUCCESS;
};

DEFINE_CMD_FUNC(Play::cmdFuncConcede)
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

	return CommandHandlerReturns::RESET_INPUT;
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

	setHandlingDefault();
}

DEFINE_CMD_FUNC(Play::cmdFuncBoost)
{
	if (!_isCorrect)
	{
		_isCorrect = true;
		_correct++;
		_wrong--;
		std::wcout << L"Boosted!\n";
		askQuestion();
		return CommandHandlerReturns::SUCCESS;
	}
	return CommandHandlerReturns::INVALID_STATE;
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

//PlayStage Play::_stage = PlayStage::QUESTION;
easy_list::list<Question*> Play::_questions = easy_list::list<Question*>();
unsigned int Play::_index = 0;
int Play::_correct = 0;
int Play::_wrong = 0;
bool Play::_isCorrect = true;

DEFINE_INPUT_HANDLER_FUNC(receiveAnswerInputHandler)
{
	bool isCorrect = Play::updateAnswer(input);

	if (isCorrect)
	{
		std::wcout << L"Correct!\n";
		Play::nextQuestion();
		return InputHandlerReturns::SUCCESS;
	}
	else
	{
		std::wcout << L"Incorrect! The correct answer was:\n"
			<< indent(Play::getCurrentCorrectAnswer(), 1)
			<< "\n";
		Play::nextQuestion();
		return InputHandlerReturns::SUCCESS;
	}
}

DEFINE_INPUT_HANDLER_FUNC(nextQuestionInputHandler)
{
	Play::askQuestion();
	return InputHandlerReturns::SUCCESS;
}