#pragma once
#include <string>
#include <vector>
#include <fstream>

class Flashcard
{
private:
	std::wstring _question;
	std::wstring _answer;
	std::vector<std::wstring> _tags;
	static std::vector<Flashcard> _flashcardList;
	bool _caseSensitive;
	static const std::wstring _optCaseSensitive;
public:
	Flashcard(std::wstring front, std::wstring back, std::vector<std::wstring> tags = std::vector<std::wstring>());
	std::wstring getFront();
	std::wstring getBack();
	std::vector<std::wstring> getTags();
	void write(std::wofstream& stream);
	bool isCaseSensitive();
	void setCaseSensitive();
	void setCaseInsensitive();
	static std::vector<Flashcard> readFlashcardList();
	static std::vector<Flashcard> getFlashcardList();
	static void appendFlashcardsToList(std::vector<Flashcard> flashcards);
};