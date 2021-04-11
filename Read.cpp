#include <iostream>
#include "Read.h"
#include "QuestionTypeInfo.h"
#include "QuestionList.h"

void read()
{
	clearQuestionList();

	for (QuestionTypeInfo qti : questionTypeInfos)
	{
		std::wifstream file;
		int countRead = 0;
		try
		{
			file.open(qti.fileAddress);
			if (!file.is_open())
				throw new std::exception("File didn't open correctly.");
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
			Question* question = qti.reader->read(file);
			if (question == nullptr)
				break;
			appendQuestionToList(question);
			countRead++;
		}
		std::wcout << countRead << L" " << qti.displayPlural << L"...\n";
	}
	std::wcout << L"Read a total of " << getQuestionList().size() << L" questions from file.\n\n";
}