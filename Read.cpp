#include <iostream>
#include "Read.h"
#include "QuestionTypeInfo.h"
#include "QuestionList.h"

void read()
{
	QuestionList::clear();

	for (QuestionTypeInfo qti : *QuestionTypeInfo::getList())
	{
		std::wifstream file;
		auto fileAddress = qti.getFileAddress();
		auto wFileAddress = std::wstring(fileAddress.begin(), fileAddress.end());
		int countRead = 0;
		try
		{
			file.open(fileAddress);
			if (!file.is_open())
				throw std::exception((fileAddress + " didn't open correctly.").c_str());
		}
		catch (std::exception e)
		{
			std::wcout << L"Error reading "
				<< qti.getDisplayPlural()
				<< L" from "
				<< wFileAddress
				<< L". Error message:\n\t"
				<< e.what()
				<< L"\n";
			continue;
		}
		while (!file.eof())
		{
			Question* question = qti.getReader()->read(file);
			if (question == nullptr)
				break;
			QuestionList::append(question);
			countRead++;
		}
		std::wcout
			<< L"\t" << countRead
			<< L" " << qti.getDisplayPlural()
			<< L" read from "
			<< wFileAddress
			<< L"...\n";
	}
	std::wcout << L"Read a total of " << QuestionList::get().size() << L" questions from file.\n\n";
}