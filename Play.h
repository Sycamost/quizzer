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
	static void setStage(PlayStage stage);
	static bool updateAnswer(std::wstring answer);
	static void finishPlaying();
	static DEFINE_CMD_FUNC(cmdFuncPlay);
	static DEFINE_CMD_FUNC(cmdFuncConcede);
	static DEFINE_CMD_FUNC(cmdFuncBoost);
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