#pragma once
#include <string>
#include <vector>
#include <fstream>

class Flashcard
{
private:
	std::wstring _front;
	std::wstring _back;
	std::vector<std::wstring> _tags;
	static std::vector<Flashcard> _flashcardList;
public:
	Flashcard(std::wstring front, std::wstring back, std::vector<std::wstring> tags = std::vector<std::wstring>());
	std::wstring getFront();
	std::wstring getBack();
	std::vector<std::wstring> getTags();
	void write(std::wofstream& stream);
	static std::vector<Flashcard> getFlashcardList();
	static void readFlashcardList();
	static void appendFlashcardsToList(std::vector<Flashcard> flashcards);
};