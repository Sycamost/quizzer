#include "Flashcard.h"
#include "FlashcardReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"
#include "globals.h"

void clearFlashcardData();
void readFlashcardData(std::wstring line);
Question* constructFlashcard(std::vector<Option> options, std::vector<std::wstring> tags);

extern QuestionReader* const flashcardReader = new QuestionReader(
	&clearFlashcardData,
	&readFlashcardData,
	&constructFlashcard
);

std::wstring front = L"";
std::wstring back = L"";

void clearFlashcardData()
{
	front = L"";
	back = L"";
}

void readFlashcardData(std::wstring line)
{
	if (front == L"")
		front = line;
	else if (back == L"")
		back = line;
	// if we get any extra inputs after front and back are filled, they are ignored
}

Question* constructFlashcard(std::vector<Option> options, std::vector<std::wstring> tags)
{
	if (front == L"")
		return nullptr;
	if (back == L"")
		return nullptr;
	bool caseSensitive = options.end() != std::find_if(options.begin(), options.end(), [](Option opt) -> bool { opt.getOption() == Globals::optionCaseSensitive; });
	return new Flashcard(front, back, caseSensitive, tags);
}