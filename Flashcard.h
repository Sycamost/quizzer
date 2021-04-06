#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Question.h"

class Flashcard : Question
{
private:
	std::wstring _question;
	std::wstring _answer;
public:
	Flashcard(std::wstring question, std::wstring answer, std::vector<std::wstring> tags = std::vector<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
	void write(std::wofstream& stream);
	static std::vector<Question&> readFlashcardList();
};