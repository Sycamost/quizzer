# flashcards

A simple console app for practicing user-made flashcards. The following is currently a specification, and may not be fully implemented yet.

In this program, each flashcard has a string value for the front, a string value for the back, and a list of zero or more string tags.

Flashcards are stored in a user-generated text file called <code>flashcards.txt</code>. The front, back and tags are separated by line breaks, and flashcards are separated by a blank line. For example:

	This is the front of flashcard 1.
	This is the back of flashcard 1.
	tag1
	tag2
	tag3

	This is the front of flashcard 2.
	This is the back of flashcard 2.
	tag2
	tag4

The user may write the flashcard file themselves manually, or may do so by using the <code>write</code> command in-app. To do the latter, the user simply enters <code>write</code> and is then guided through, step-by-step, in adding more flashcards. There is no way to remove flashcards in-app.

If the user is not currently playing any flashcards, the user may begin playing by simply entering the code <code>play [tag1] [tag2] ...</code>, where <code>[tag1], [tag2], ...</code> are optional tags. If tags are supplied, only flashcards with the supplied tag are played. If no tags are supplied, all flashcards are played. To quit play early, the user may enter <code>concede</code>. ("concede" was preferred to "quit" or "exit", since either of the latter suggest exiting the program rather than just exiting play.) When the user finishes play, either by use of the <code>concede</code> command or by doing every flashcard once, the user is given a tally of how many they got right, how many they got wrong and how many they skipped (if they used <code>concede<code>).