# quizzer

A simple console app for practicing user-made quiz questions.

## Play

This is the syntax for the play command:

	play [tag1] [tag2] ...

If at least one tag is specified, the app plays all and only questions with at least one of the specified tags. If no tags are specified, all questions are played. Questions are always randomly shuffled before play.

If the user wants to end play early, they may use the <code>concede</code> command at any time. If they are marked down unfairly for a question, they may use the <code>boost</code> command to count their answer as correct.

At the end of play, the user gets a score tally. If they used <code>concede</code>, they are also told how many questions they skipped.

## Questions

The general functionality for questions is kept in the abstract class <code>Question</code>. Since <code>Question</code> is an abstract class, every actual question must be of another type that inherits from <code>Question</code>, not <code>Question</code> itself.

Every <code>Question</code> must be able to return its question string, be able to verify whether a given string counts as a correct answer or not, and a couple of other things, such as writing to file.

Child classes of <code>Question</code> include <code>Flashcard</code> and <code>MultipleChoice</code> (not yet implemented).

## Write

The syntax for the write command is as follows:

	write \<type\>

where <code>\<type\></code> is one of the following values:

	flashcards
	multiplechoice

The user is then led through the process of writing new questions of the respective type. If they want to cancel writing a question halfway through, they may use the <code>cancel</code> command at any time. The new questions are written to file, and loaded ready for play, once the user has finished adding new questions.

## Implementations of Question

### Flashcards

In this program, each flashcard has a string value for the front, a string value for the back, and a list of zero or more string tags.

Flashcards are stored in <code>flashcards.txt</code>. The front, back and tags are separated by line breaks, and flashcards are separated by a blank line. For example:

	This is the front of flashcard 1.
	This is the back of flashcard 1.
	tag1
	tag2
	tag3

	This is the front of flashcard 2.
	This is the back of flashcard 2.
	tag2
	tag4
