#pragma once
#include <vector>
#include <string>
#include "Flashcard.h"
#include "Command.h"

enum class PlayStage {
	QUESTION,
	ANSWER
};

class Play
{
public:
	static PlayStage getStage();
	static void setValue(PlayStage stage);
	static bool updateAnswer(std::wstring answer);
	static DEFINE_CMD_FUNC(startPlaying);
	static DEFINE_CMD_FUNC(finishPlaying);
	static DEFINE_CMD_FUNC(boost);
	static int getNumCorrect();
	static int getNumWrong();
	static int getNumSkipped();
	static std::wstring getCurrentCorrectAnswer();
private:
	static PlayStage _stage;
	static std::vector<Question*> _questions;
	static unsigned int _index;
	static int _correct;
	static int _wrong;
	static bool _hasAnswered;
	static bool _isCorrect;
};