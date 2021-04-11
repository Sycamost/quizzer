#pragma once
#include "QuestionTypeInfo.h"
#include "Command.h"

class Write
{
public:
	static InputHandler::Returns InputHandler(std::wstring userInput);
	static DEFINE_CMD_FUNC(cmdFuncWrite);
	static DEFINE_CMD_FUNC(cmdFuncCancel);
	static void nextQuestion();
private:
	enum class Stage {
		NEXT_QUESTION,
		INPUT
	};
	static QuestionTypeInfo _typeInfo;
	static Stage _stage;
	static void setStage(Stage stage);
	static void startWriting(const QuestionTypeInfo qti);
	static void finishWriting();
	static void resetLastStep();
};