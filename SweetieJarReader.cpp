#include "safeprojectname.h"
#include "SweetieJarReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"
#include "globals.h"
#include "util.h"

namespace SweetieJarReader
{
	static_assert(false, "SweetieJarWriter must have initialised global variables!");
	/*
	* The required inputs from the user should be stored in file-scope globals,
	* which should be declared here. E.g.:
	*
	*	type memberOne{ value };
	*	type memberTwo{ value };
	*	...
	*
	*/

	void clear()
	{
		static_assert(false, "SweetieJarReader::clear() hasn't been implemented yet!");
		/* 
		* 
		* memberOne = L"";
		* memberTwo = 0;
		* memberThree = nullptr;
		* ...
		* 
		*/
	}

	void read(std::wstring line)
	{
		static_assert(false, "SweetieJarReader::read() hasn't been implemented yet!");
		/*
		*	if (isInvalid(memberOne))
		*		memberOne = line;
		*	else if (isInvalid(memberTwo))
		*		memberTwo = line;
		*	...
		* 
		*/
	}

	Question* construct(easy_list::list<Option> options, easy_list::list<std::wstring> tags)
	{
		static_assert(false, "SweetieJarReader::construct() hasn't been implemented yet!");
		/*
		*	if (isInvalid(memberOne))
		*		return nullptr;
		*	if (isInvalid(memberTwo))
		*		return nullptr;
		*	...
		* 
		*	return new SweetieJar( ... );
		* 
		*/
	}

	QuestionReader& get()
	{
		static QuestionReader reader = QuestionReader(
			&clear,
			&read,
			&construct
		);
		return reader;
	}
}