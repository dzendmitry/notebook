#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <fstream>
#include "ARRAYOFSTR.h"

enum COMMANDS {
	CLEAR_SCREEN,
	COUNT_NOTES,
	LIST_OF_NOTES,
	SHOW_ONE_NOTE,
	ADD_NOTE,
	REMOVE_NOTE,
	REMOVE_ALL_NOTES,
	MOVE_NOTE,
	SAVE,
	CLEAN_NOTE,
	FIND_IN_NOTES,
	REPLACE_IN_NOTE,
	REMOVE_FROM_NOTE,
	REMOVE_FROM_NOTE_BY_TEXT,
	ADD_TO_NOTE,
	MERGE_NOTES,
	EXIT
};

class NOTEBOOK
{
public:
	NOTEBOOK();
	void showAllCommands();
	bool processCommand(int& command);
private:
	dzArray notes;
	const char *FILE;
	bool wasChanged;

	void showOneNote();
	void addNote();
	void removeNote();
	void removeAllNotes();
	void moveNote();
	void saveNotes();
	void cleanNote();
	void findInNotes();
	void replaceInNote(bool delByTxt = false);
	void removeFromNote();
	void addToNote();
	void mergeNotes();

	int getPosition(const char *, int top, int bottom = 1);
	bool getAnswer(const char *);
};

#endif // NOTEBOOK_H