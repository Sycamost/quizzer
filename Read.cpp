#include <vector>
#include <fstream>
#include "Flashcard.h"
#include "Read.h"
#include "globals.h"
#include "util.h"

std::vector<Flashcard> read()
{
	std::vector<Flashcard> flashcards = std::vector<Flashcard>();

	try
	{
		std::wifstream file(Globals::flashcardsFileAddress);
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::wstring front = getUserInputLine(file);

				if (file.eof()) break;
				std::wstring back = getUserInputLine(file);

				std::vector<std::wstring> tags = std::vector<std::wstring>();
				while (!file.eof())
				{
					std::wstring tag = getUserInputLine(file);
					if (tag == L"")
						break;
					tags.push_back(tag);
				}

				flashcards.push_back(Flashcard(front, back, tags));
			}
		}
	}
	catch (std::exception) {}

	return flashcards;
}