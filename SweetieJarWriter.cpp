#include <iostream>
#include <string>
#include <easy_list.h>
#include <Windows.h>
#include <exception>
#include <fstream>
#include "Command.h"
#include "util.h"
#include "SweetieJar.h"
#include "SweetieJarWriter.h"
#include "globals.h"
#include "QuestionWriter.h"
#include "Write.h"

namespace SweetieJarWriter
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

	static_assert(false, "SweetieJarWriter must pre-declare its asking functions!");
	/*
	* The asking functions (see below) should be pre-declared here. E.g:
	* 
	*	void askMemberOne();
	*	void askMemberTwo();
	*	...
	* 
	*/

	static_assert(false, "SweetieJarWriter must define its input handlers!");
	/* 
	* The input handlers should be given here. Typically, there will be a
	* series of input handlers, one for each data member of SweetieJar, 
	* each one passing on to the next when finished. The last handler should
	* always hand back to the tag input handler by calling QuestionWriter::writeTags().
	* So, this section should look something like:
	* 
	*	DEFINE_INPUT_HANDLER_FUNC(memberOneInputHandler)
	*	{
	*		if (isInvalid(input))
	*		{
	*			askMemberOne();
	*			return InputHandlerReturns::SUCCESS;
	*		}
	* 
	*		memberOne = input;
	*		askMemberTwo();
	*		return InputHandlerReturns::SUCCESS;
	*	}
	* 
	*	...
	* 
	*	DEFINE_INPUT_HANDLER_FUNC(lastMemberInputHandler)
	*	{
	*		...
	* 
	*		lastMember = input;
	*		QuestionWriter::writeTags();
	*		return InputHandlerReturns::SUCCESS;
	*	}
	*/

	static_assert(false, "SweetieJarWriter must define its asking functions!");
	/*
	* This is where the asking functions should be defined. The asking functions
	* are implemented for each member data item, and simply make sure the input
	* handler is set up to ask for, and receive, the respective member data items
	* when called. E.g:
	* 
	*	void askMemberOne()
	*	{
	*		static const std::wstring msg = L"Member one:\t";
	*		setInputHandling(msg, &memberOneInputHandler);
	*	}
	* 
	*	void askMemberTwo()
	*	{
	*		...
	*	}
	* 
	*	...
	* 
	*/

	void writeSweetieJar()
	{
		static_assert(false, "SweetieJarWriter::writeSweetieJar() has not been implemented yet!");

		/* Clear stored member data, e.g:
		* 
		*	memberOne = 0;
		*	memberTwo = L"";
		*	memberThree = nullptr;
		*	...
		* 
		*/

		// Start message to user - make sure to personalise this message for SweetieJar!
		std::wcout <<
			L"Enter the values for new SweetieJar's data and tags.\n"
			<< L"Once you're finished adding tags, leave the next tag blank.\n"
			<< L"Make sure your data inputs are valid (what does this mean for SweetieJar?).\nUse <"
			<< toLower(CommandInfo::getFirstCode(CommandType::CANCEL))
			<< L"> to cancel adding the current SweetieJar.\n\n";

		/* Call first asking function, e.g:
		 * 
		 *	askMemberOne();
		 * 
		 */

		return;
	}

	bool resetLastStep()
	{
		static_assert(false, "SweetieJarWriter::resetLastStep() has not been implemented yet!");
		/*
		*	if (isInvalid(memberOne))
		*	{
		*		askMemberOne();
		*		return true;
		*	}
		*	else if (isInvalid(memberTwo))
		*	{
		*		askMemberTwo();
		*		return true;
		*	}
		*	...
		*	else if (isInvalid(lastMember))
		*	{
		*		askLastMember();
		*		return true;
		*	}
		*	return false;
		*/
	}

	Question* constructCurrent(easy_list::list<std::wstring> tags)
	{
		static_assert(false, "SweetieJarWriter::constructCurrent() has not been implemented yet!");
		/*
		 *	if (isInvalid(memberOne))
		 *		return nullptr;
		 *	if (isInvalid(memberTwo))
		 *		return nullptr;
		 *	...
		 *	if (isInvalid(lastMember))
		 *		return nullptr;
		 *	return new SweetieJar(memberOne, memberTwo, ..., lastMember);
		 * 
		 */
	}

	QuestionWriter& get()
	{
		static QuestionWriter writer = QuestionWriter(
			QuestionType::THIS_QUESTION_TYPE,
			&writeSweetieJar,
			&resetLastStep,
			&constructCurrent
		);
		return writer;
	}
}