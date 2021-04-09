#pragma once
#include "Flashcard.h"

class WriteFlashcard {
public:
	static const std::wstring startWritingMessage;
	static void startInputData();
	static void inputData(std::wstring userInput);
	static void cancel();
	static void resetLastInputStep();
	static void pushCurrent(std::vector<std::wstring> tags);
	static std::vector<Question*> writeToFile();
private:
	enum class InputDataStage {
		FRONT,
		BACK,
		CASE_SENSITIVE,
		SLEEP
	};
	static void setInputDataStage(const InputDataStage inputDataStage);
	static WriteFlashcard::InputDataStage _inputDataStage;
	static std::wstring _currentFront;
	static std::wstring _currentBack;
	static bool _currentCaseSensitive;
	static std::vector<Flashcard*> _newFlashcards;
};