# quizzer

A command-line console app for practicing user-made quiz questions.

## Commands

Commands all follow the syntax, <code>\<command> [arg1] [arg2] ...</code>, where <code>[arg1], [arg2], ...</code> are any arguments, and <code><command></code> is the command code. The backslash <code>\</code> is to distinguish commands from other inputs, such as writing new questions or answering questions. If the user provides more arguments than the command handler knows what to do with, the handler discards the excess arguments and carries on.

See [Play](#Play) for more on the <code>play</code> command, and see [Write](#Write) for more on the <code>write</code> command.

At any time, the user may call the quit command with <code>\quit</code> or <code>\exit</code>. No prizes for guessing what it does!

When in play, the following commands may be valid:

	\concede
	\boost

The concede command ends play early, skipping any remaining questions. The boost command should be used just after giving a wrong answer, and re-marks your answer as correct - you should use this if you think you have been marked down unfairly, for example, if you made a minor spelling mistake or typo.

When writing new questions, the <code>\cancel</code> command may be valid. This scraps the question you're currently writing, and asks if you would like to carry on afterwards.

***!!! NOT IMPLEMENTED YET !!!*** When writing new questions, you may also be able to use the <code>\save</code> command, which saves your writing progress, and then returns you to where you were with your writing. You can even call this in the middle of writing a question!

## Play

This is the syntax for the play command:

	\play [tag1] [tag2] ...

If at least one tag is specified, the app plays all and only questions with at least one of the specified tags. If no tags are specified, all questions are played. Questions are always randomly shuffled before play.

If the user wants to end play early, they may use the <code>concede</code> command at any time. If they are marked down unfairly for a question, they may use the <code>boost</code> command to count their answer as correct.

At the end of play, the user gets a score tally. If they used <code>concede</code>, they are also told how many questions they skipped.

## Questions

The generic functionality for questions is kept in the abstract class <code>Question</code>. Since <code>Question</code> is an abstract class, every actual question must be of another type that inherits from <code>Question</code>, not <code>Question</code> itself.

Every <code>Question</code> must be able to return its question string, be able to verify whether a given string counts as a correct answer or not, and a couple of other things, such as writing to an output stream.

What children are there of <code>Question</code>?

| Name           | Description                                                              | Implemented?       |
| :------------- | :----------------------------------------------------------------------- | :----------------- |
| Flashcard      | Shows the user the front, they have to guess what's on the back.         | :heavy_check_mark: |
| MultipleChoice | Asks the user a multiple-choice question, they have to guess the option. | :x:                |
| SweetieJar     | The user must guess a number to some specified accuracy.                 | :x:                |
| Birthday       | The user must guess a date to some given accuracy.                       | :x:                |
| Partition      | The user must correctly partition given items into given categories.     | :x:                |

## Write

The syntax for the write command is as follows:

	\write <type>

where <code>\<type\></code> is the name of one of the implemented question types. (No commands are ever case-sensitive.)

The user is then led step-by-step through the process of writing new questions of the respective type. If they want to cancel writing a question halfway through, they may use the <code>cancel</code> command at any time. The new questions are written to file, and loaded ready for play, once the user has finished adding new questions.

There is no way to delete questions in-app. The user may do so manually. Questions are stored in text format, in files with the name "userdata_xxx.txt", where "xxx" is the name of the type of question, lower-case, with underscores between words.

## Implementations of Question

### Flashcards

In this program, each flashcard has a string value for the front, a string value for the back, and a list of zero or more string tags.

Flashcards are stored in <code>userdata_flashcard.txt</code>. The front, back and tags are separated by line breaks, and flashcards are separated by a blank line. They may or not be marked with a case-sensitivity option. For example, if the file contains two flashcards, the latter of which is case-sensitive, <code>userdata_flashcard.txt</code> may look like this:

	This is the front of flashcard 1.
	This is the back of flashcard 1.
	tag1
	tag2
	tag3

	%%case_sensitive
	This is the front of flashcard 2.
	This is the back of flashcard 2.
	tag2
	tag4
