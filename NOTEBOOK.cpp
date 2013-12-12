#include "StdAfx.h"
#include "NOTEBOOK.h"

NOTEBOOK::NOTEBOOK()
: FILE("notes.txt"), wasChanged(false)
{
	std::fstream file(FILE, std::ios_base::in);
	if(!file.is_open())
		throw std::ios_base::failure("NOTEBOOK::NOTEBOOK(): ������. ���������� ������� ���� � ��������. C������� ���� notes.txt ����� � .exe ������ ��������� � ��������� �������.");
	file >> notes;
	if(file.bad())
		throw std::ios_base::failure("NOTEBOOK::NOTEBOOK(): ������. ������ �� ����� ����������� ��������.");
	file.close();
}

void NOTEBOOK::showAllCommands()
{
	std::cout << std::endl;
	std::cout << "---- ���� ----" << std::endl;
	std::cout << CLEAR_SCREEN << " - �������� �����." << std::endl;
	std::cout << COUNT_NOTES << " - �������� ���������� �������." << std::endl;
	std::cout << LIST_OF_NOTES << " - �������� ��� ������." << std::endl;
	std::cout << SHOW_ONE_NOTE << " - �������� ������ ��� ������� �." << std::endl;
	std::cout << ADD_NOTE << " - �������� ������." << std::endl;
	std::cout << REMOVE_NOTE << " - ������� ������." << std::endl;
	std::cout << REMOVE_ALL_NOTES << " - ������� ��� ������." << std::endl;
	std::cout << MOVE_NOTE << " - ����������� ������." << std::endl;
	std::cout << SAVE << " - ��������� ������." << std::endl;
	std::cout << CLEAN_NOTE << " - �������� ������." << std::endl;
	std::cout << FIND_IN_NOTES << " - ����� � �������." << std::endl;
	std::cout << REPLACE_IN_NOTE << " - ������� ������ � ������." << std::endl;
	std::cout << REMOVE_FROM_NOTE << " - ������� �� ������(���� - ����� �������)." << std::endl;
	std::cout << REMOVE_FROM_NOTE_BY_TEXT << " - ������� �� ������(���� - ������)." << std::endl;
	std::cout << ADD_TO_NOTE << " - �������� ����� � ������." << std::endl;
	std::cout << MERGE_NOTES << " - ���������� ��� ������." << std::endl;
	std::cout << EXIT << " - �����." << std::endl;
}

bool NOTEBOOK::processCommand(int& command)
{
	switch(command)
	{
	case CLEAR_SCREEN:
		system("CLS");
		break;
	case COUNT_NOTES:
		std::cout << std::endl << "����� �������: " << notes.length() << std::endl;
		break;
	case LIST_OF_NOTES:
		if(notes.length() > 0)
		{
			notes.setOutWithNums(true);
			std::cout << std::endl << notes;
			notes.setOutWithNums(false);
		}
		else
			std::cout << std::endl << "������ �����������" << std::endl;
		break;
	case SHOW_ONE_NOTE:
		showOneNote();
		break;
	case ADD_NOTE:
		addNote();
		break;
	case REMOVE_NOTE:
		removeNote();
		break;
	case REMOVE_ALL_NOTES:
		removeAllNotes();
		break;
	case MOVE_NOTE:
		moveNote();
		break;
	case SAVE:
		saveNotes();
		break;
	case CLEAN_NOTE:
		cleanNote();
		break;
	case FIND_IN_NOTES:
		findInNotes();
		break;
	case REPLACE_IN_NOTE:
		replaceInNote();
		break;
	case REMOVE_FROM_NOTE:
		removeFromNote();
		break;
	case REMOVE_FROM_NOTE_BY_TEXT:
		replaceInNote(true);
		break;
	case ADD_TO_NOTE:
		addToNote();
		break;
	case MERGE_NOTES:
		mergeNotes();
		break;
	case EXIT:
		if(!getAnswer("�� ������������� ������� �����?"))
			command = -1;
		else
			saveNotes();
		break;
	default:
		std::cout << std::endl << "������. ������� � ����� ������� �� �������." << std::endl;
		return false;
	}
	return true;
}

void NOTEBOOK::showOneNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "������� ���. �������� ������." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("������� ����� �������, ������� ������� �����������", notes.length());
	if(position > -1)
		std::cout << std::endl << notes[position] << std::endl;
}
 
void NOTEBOOK::addNote()
{
	int position = 0;
	dzString newNote;
	std::cout << "������� ������, ������� ������ ��������: ";
	std::cin >> newNote;
	if(newNote.length() == 1)
	{
		std::cout << std::endl << "������ �� �������� ��������. ������ �� ����� ���������." << std::endl;
		return;
	}
	if(notes.length() > 0)
		position = getPosition("������� ����� ������� � ������� ���������� �������� ������", notes.length() + 1);
	if(position > -1)
	{
		notes.insert(newNote, position);
		wasChanged = true;
		std::cout << std::endl << "���������� ������ ������� �����������." << std::endl;
	}
}

void NOTEBOOK::removeNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "������� ���. ������� ������." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("������� ����� �������, ������� ���������� �������", notes.length());
	if( (position > -1) && getAnswer("�� ������������� ������ ������� ������?") )
	{
		notes.erase(position, 1);
		wasChanged = true;
		std::cout << std::endl << "�������� ������ ������� �����������." << std::endl;
	}
}

void NOTEBOOK::removeAllNotes()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "������� ���. ������� ������." << std::endl;
		return;
	}
	if(getAnswer("�� ������������� ������ ������� ��� ������?"))
	{
		notes.erase();
		wasChanged = true;
		std::cout << std::endl << "�������� ������� ������� �����������." << std::endl;
	}
}

void NOTEBOOK::moveNote()
{
	if(notes.length() < 2)
	{
		std::cout << std::endl << "����� ���� �������. ���������� ������." << std::endl;
		return;
	}
	int position1 = getPosition("������� ����� �������, ������� ���������� �����������", notes.length());
	if(position1 > -1)
	{
		int position2 = getPosition("������� ����� ������� � ������� ���������� ��������� ������", notes.length());
		if(position1 == position2)
			return;
		if(position2 > -1)
		{
			dzString tmpStr = notes[position1];
			notes.erase(position1, 1);
			notes.insert(tmpStr, position2);
			wasChanged = true;
			std::cout << std::endl << "����������� ������� ������� �����������." << std::endl;
		}
	}
}

void NOTEBOOK::saveNotes()
{
	if(wasChanged && getAnswer("�� ������� ��������� ������?"))
	{
		std::fstream file(FILE, std::ios_base::out);
		if(!file.is_open())
		{
			std::cout << std::endl << "������. ���������� ������� ���� ��� ������." << std::endl;
			return;
		}
		file << notes;
		if(file.bad())
			std::cout << std::endl << "������. ��������� ������ �� ����� ������." << std::endl;
		else
		{
			std::cout << std::endl << "������ ����������� �������." << std::endl;
			wasChanged = false;
		}
		file.close();
	}
}

void NOTEBOOK::cleanNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "������� ���. ������� ������." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("������� ����� �������, ������� ������� ��������.", notes.length());
	if(position > -1 && getAnswer("�� ������������� ������ �������� ������?"))
	{
		notes[position].clear();
		wasChanged = true;
		std::cout << std::endl << "������� ������ ������� �����������." << std::endl;
	}
}

void NOTEBOOK::findInNotes()
{
	if(notes.length() == 0)
	{
		std::cout << "������� ���. ����� ������." << std::endl;
		return;
	}
	dzString strForSearch;
	std::cout << "������� �����, ������� ������ �����: ";
	std::cin >> strForSearch;
	bool firstFound = true;
	for(int i = 0; i < notes.length(); i++)
	{
		if(notes[i].find(strForSearch) > -1)
		{
			if(firstFound)
			{
				std::cout << std::endl << "����� ������� � �������:" << std::endl;
				firstFound = false;
			}
			notes.setOutWithNums(true);
			std::cout << (i+1) << " - " << notes[i] << std::endl;
			notes.setOutWithNums(false);
		}
	}
	if(firstFound)
		std::cout << std::endl << "����� �� ���� �������." << std::endl;
}

void NOTEBOOK::replaceInNote(bool delByTxt)
{
	if(notes.length() == 0)
	{
		std::cout << "������� ���. ������ ��������/�������." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("������� ����� ������ � ������� ������� ���������� ������/��������.", notes.length());
	if(position > -1)
	{
		dzString strForReplace;
		std::cout << "������� �����, ������� ������ ��������/�������: ";
		std::cin >> strForReplace;
		dzString strToReplace;
		if(!delByTxt)
		{
			std::cout << "������� ����� �� ������� ������ ��������/�������: ";
			std::cin >> strToReplace;
		}
		else
			strToReplace = "";
		bool all = getAnswer("��������/������� ��� ��������� ����� � ������?");
		int entry;
		if(!all)
		{
			do {
				std::cout << "����� ��������� ��������/�������?[1..]: ";
				std::cin >> entry;
				if (std::cin.fail())
					std::cin.clear();
				while (std::cin.get() != '\n');
				if(entry <= 0)
					std::cout << "������. ������� ����� ��������� �� 1 �� ..." << std::endl;
			} while(entry <= 0);
		}
		if(getAnswer("�� ����� ������ ���������� ������/��������?"))
		{
			int startPos = 0;
			if(!all)
				for(int i = 1; i < entry; i++)
				{
					startPos = notes[position].find(strForReplace, startPos);
					if(startPos > -1)
						startPos++;
				}
			if( (startPos > -1) && notes[position].replace(strForReplace, strToReplace, all, startPos))
				std::cout << std::endl << "������/�������� ������� �����������." << std::endl;
			else
				std::cout << std::endl << "������/�������� �� ���� �����������." << std::endl;
			wasChanged = true;
		}
	}
}

void NOTEBOOK::removeFromNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "������� ���. �������� �������." << std::endl;
		return;
	}
	int position = getPosition("������� ����� ������, � ������� ������� ���������� ��������.", notes.length());
	if(position > -1)
	{
		if(notes[position].isEmpty())
		{
			std::cout << std::endl << "��� ������ �����. ������ �������" << std::endl;
			return;
		}
		int posInNoteFrom = getPosition("������� ����� �������, � ������� ���������� ���������� ��������.", notes[position].length() - 1);
		if(posInNoteFrom > -1)
		{
			int delLen = getPosition("������� �������� �������?", notes[position].length() - posInNoteFrom - 1);
			if(delLen > -1 && getAnswer("�� ����� ������ ���������� �������� ��������?"))
			{
				notes[position].erase(posInNoteFrom, delLen+1);
				wasChanged = true;
				std::cout << std::endl << "�������� ������� �����������." << std::endl;
			}
		}
	}
}

void NOTEBOOK::addToNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "������� ���. ������ ���������." << std::endl;
		return;
	}
	int position = getPosition("������� ����� ������ � ������� ����� �������� �����.", notes.length());
	if(position > -1)
	{
		dzString txt;
		std::cout << "������� �����, ������� ������ ��������: ";
		std::cin >> txt;
		int posInNote = getPosition("������� �������, � ������� ������ �������� �����.", notes[position].length());
		if(posInNote > -1)
		{
			notes[position].insert(txt, posInNote);
			wasChanged = true;
			std::cout << std::endl << "����� ������� ��������." << std::endl;
		}
	}
}

void NOTEBOOK::mergeNotes()
{
	if(notes.length() < 2)
	{
		std::cout << std::endl << "� ����� ����� ���� �������. ���������� ������." << std::endl;
		return;
	}
	int position1 = getPosition("������ ����� ������, ������� ������ ����������.", notes.length());
	if(position1 > -1)
	{
		int position2 = 0;
		do {
			position2 = getPosition("������� ����� ������ � ������� ������ ����������.", notes.length());
			if(position1 == position2)
				std::cout << std::endl << "���������� ���������� ������� � ����� �����." << std::endl << std::endl;
		} while( (position2 != -1) && position1 == position2);
		if(position2 > -1)
		{
			(notes[position2] += " ") += notes[position1];
			notes.erase(position1, 1);
			wasChanged = true;
			std::cout << std::endl << "����������� ������� �����������." << std::endl;
		}
	}
}

int NOTEBOOK::getPosition(const char *message, int top, int bottom)
{
	int position = 0;
	bool ok;
	do {
		std::cout << message << " [" << bottom << ".." << top << "] ��� 0 ��� ������: ";
		std::cin >> position;
		if (std::cin.fail())
			std::cin.clear();
		while (std::cin.get() != '\n');
		ok = (position >= bottom) && (position <= top);
		if(position && !ok)
			std::cout << "������. ������� ������ ����� �������. ����������, ��������� �������." << std::endl;
	} while(position && !ok);
	--position;
	return position;
}

bool NOTEBOOK::getAnswer(const char *message)
{
	char answer;
	do {
		std::cout << message <<  " [y/n]: ";
		std::cin >> answer;
		while (std::cin.get() != '\n');
		answer = tolower(answer);
		if( !( (answer == 'y') || (answer == 'n') ) )
			std::cout << "������. ������� y ��� n." << std::endl;
	} while( !( (answer == 'y') || (answer == 'n') ) );
	if(answer == 'y')
		return true;
	return false;
}