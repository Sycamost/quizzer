#pragma once

void startWritingFlashcards();

class WriteFlashcard {
public:
	enum class InputStage {
		FRONT,
		BACK,
		CASE_SENSITIVE,
		TAGS
	};

	static void startWriting()
	{
		setStage(InputStage::FRONT);
	}

	static InputStage getValue() {
		return _value;
	}

	static void setStage(InputStage stage) {

		if (stage == InputStage::NEW_CARD)
		{
			if (newFlashcards[newFlashcards.size() - 1]->isCaseSensitive())
			{
				std::wcout << L"W";
			}
			else
			{
				std::wcout << L"If the previous card should be checked as case-sensitive, enter <"
					<< toLower(Globals::cmdCase) <<
					L">. Otherwise, w";
			}
			std::wcout << "ould you like to add another card ? [Y/N]\n";
			_value = stage;
			return;
		}

		if (stage == InputStage::FRONT)
		{
			front = L"";
			back = L"";
			tags = std::vector<std::wstring>();
			std::wcout << L"Front:\t";
			_value = stage;
			return;
		}

		if (stage == InputStage::BACK)
		{
			std::wcout << L"Back:\t";
			_value = stage;
			return;
		}

		if (stage == InputStage::TAGS)
		{
			std::wcout << L"Tag " << std::to_wstring(tags.size() + 1) << L":\t";
			_value = stage;
			return;
		}
	}

private:
	static WriteFlashcard::InputStage _value;
};