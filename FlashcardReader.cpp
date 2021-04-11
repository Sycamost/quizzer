#include "FlashcardReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"

void readFlashcardData(std::wstring line);
Question* constructFlashcard(std::vector<Option> options, std::vector<std::wstring> tags);

extern QuestionReader* const flashcardReader = new QuestionReader(
	QuestionType::FLASHCARD,
	&readFlashcardData,
	&constructFlashcard
);

std::wstring front;
std::wstring back;

void readFlashcardData(std::wstring line)
{

}