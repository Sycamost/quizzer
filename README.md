# quizzer

A command-line console app for practicing user-made quiz questions.

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
| MultipleChoice | Asks the user a multiple-choice question, they have to guess the option. | :heavy_check_mark: |
| SweetieJar     | Asks the user a question, they have to guess a number to some accuracy.  | :x:                |
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

### Multiple Choice

Multiple choice questions come with a question, a correct answer, and a collection of incorrect answers. The user is presented with the question and all the alternative answers and asked which answer is the correct one.