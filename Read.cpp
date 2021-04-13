#include <iostream>
#include "Read.h"
#include "QuestionTypeInfo.h"
#include "QuestionList.h"

void read()
{
	QuestionList::clear();

	for (QuestionTypeInfo qti : questionTypeInfos)
	{
		std::wifstream file;
		int countRead = 0;
		try
		{
			file.open(qti.getFileAddress());
			if (!file.is_open())
				throw std::exception("File didn't open correctly.");
		}
		catch (std::exception e)
		{
			std::wcout << L"Error reading "
				<< qti.displayPlural
				<< L" from file. Error message:\n\t"
				<< e.what()
				<< L"\n";
			continue;
		}
		while (!file.eof())
		{
			Question* question = qti.reader.read(file);
			if (question == nullptr)
				break;
			QuestionList::append(question);
			countRead++;
		}
		std::wcout << countRead << L" " << qti.displayPlural << L"...\n";
	}
	std::wcout << L"Read a total of " << QuestionList::get().size() << L" questions from file.\n\n";
}