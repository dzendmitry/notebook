#include "StdAfx.h"
#include "NOTEBOOK.h"

NOTEBOOK::NOTEBOOK()
: FILE("notes.txt"), wasChanged(false)
{
	std::fstream file(FILE, std::ios_base::in);
	if(!file.is_open())
		throw std::ios_base::failure("NOTEBOOK::NOTEBOOK(): Ошибка. Невозможно открыть файл с записями. Cоздайте файл notes.txt рядом с .exe файлом программы и повторите попытку.");
	file >> notes;
	if(file.bad())
		throw std::ios_base::failure("NOTEBOOK::NOTEBOOK(): Ошибка. Чтение из файла завершилось неудачей.");
	file.close();
}

void NOTEBOOK::showAllCommands()
{
	std::cout << std::endl;
	std::cout << "---- Меню ----" << std::endl;
	std::cout << CLEAR_SCREEN << " - Очистить экран." << std::endl;
	std::cout << COUNT_NOTES << " - Показать количество записей." << std::endl;
	std::cout << LIST_OF_NOTES << " - Показать все записи." << std::endl;
	std::cout << SHOW_ONE_NOTE << " - Показать запись под номером №." << std::endl;
	std::cout << ADD_NOTE << " - Добавить запись." << std::endl;
	std::cout << REMOVE_NOTE << " - Удалить запись." << std::endl;
	std::cout << REMOVE_ALL_NOTES << " - Удалить все записи." << std::endl;
	std::cout << MOVE_NOTE << " - Переместить запись." << std::endl;
	std::cout << SAVE << " - Сохранить записи." << std::endl;
	std::cout << CLEAN_NOTE << " - Очистить запись." << std::endl;
	std::cout << FIND_IN_NOTES << " - Найти в записях." << std::endl;
	std::cout << REPLACE_IN_NOTE << " - Сделать замену в записи." << std::endl;
	std::cout << REMOVE_FROM_NOTE << " - Удалить из записи(вход - номер позиции)." << std::endl;
	std::cout << REMOVE_FROM_NOTE_BY_TEXT << " - Удалить из записи(вход - строка)." << std::endl;
	std::cout << ADD_TO_NOTE << " - Добавить текст в запись." << std::endl;
	std::cout << MERGE_NOTES << " - Объединить две записи." << std::endl;
	std::cout << EXIT << " - Выход." << std::endl;
}

bool NOTEBOOK::processCommand(int& command)
{
	switch(command)
	{
	case CLEAR_SCREEN:
		system("CLS");
		break;
	case COUNT_NOTES:
		std::cout << std::endl << "Всего записей: " << notes.length() << std::endl;
		break;
	case LIST_OF_NOTES:
		if(notes.length() > 0)
		{
			notes.setOutWithNums(true);
			std::cout << std::endl << notes;
			notes.setOutWithNums(false);
		}
		else
			std::cout << std::endl << "Записи отсутствуют" << std::endl;
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
		if(!getAnswer("Вы действительно желаете выйти?"))
			command = -1;
		else
			saveNotes();
		break;
	default:
		std::cout << std::endl << "Ошибка. Команда с таким номером не найдена." << std::endl;
		return false;
	}
	return true;
}

void NOTEBOOK::showOneNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "Записей нет. Смотреть нечего." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("Введите номер позиции, которую желаете просмотреть", notes.length());
	if(position > -1)
		std::cout << std::endl << notes[position] << std::endl;
}
 
void NOTEBOOK::addNote()
{
	int position = 0;
	dzString newNote;
	std::cout << "Введите запись, которую хотите добавить: ";
	std::cin >> newNote;
	if(newNote.length() == 1)
	{
		std::cout << std::endl << "Запись не содержит символов. Запись не будет добавлена." << std::endl;
		return;
	}
	if(notes.length() > 0)
		position = getPosition("Введите номер позиции в которую необходимо вставить запись", notes.length() + 1);
	if(position > -1)
	{
		notes.insert(newNote, position);
		wasChanged = true;
		std::cout << std::endl << "Добавление записи успешно произведено." << std::endl;
	}
}

void NOTEBOOK::removeNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "Записей нет. Удалять нечего." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("Введите номер позиции, которую необходимо удалить", notes.length());
	if( (position > -1) && getAnswer("Вы действительно хотите удалить запись?") )
	{
		notes.erase(position, 1);
		wasChanged = true;
		std::cout << std::endl << "Удаление записи успешно произведено." << std::endl;
	}
}

void NOTEBOOK::removeAllNotes()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "Записей нет. Удалять нечего." << std::endl;
		return;
	}
	if(getAnswer("Вы действительно хотите удалить все записи?"))
	{
		notes.erase();
		wasChanged = true;
		std::cout << std::endl << "Удаление записей успешно произведено." << std::endl;
	}
}

void NOTEBOOK::moveNote()
{
	if(notes.length() < 2)
	{
		std::cout << std::endl << "Менее двух записей. Перемещать нечего." << std::endl;
		return;
	}
	int position1 = getPosition("Введите номер позиции, которую необходимо переместить", notes.length());
	if(position1 > -1)
	{
		int position2 = getPosition("Введите номер позиции в которую необходимо поместить запись", notes.length());
		if(position1 == position2)
			return;
		if(position2 > -1)
		{
			dzString tmpStr = notes[position1];
			notes.erase(position1, 1);
			notes.insert(tmpStr, position2);
			wasChanged = true;
			std::cout << std::endl << "Перемещение записей успешно произведено." << std::endl;
		}
	}
}

void NOTEBOOK::saveNotes()
{
	if(wasChanged && getAnswer("Вы желаете сохранить записи?"))
	{
		std::fstream file(FILE, std::ios_base::out);
		if(!file.is_open())
		{
			std::cout << std::endl << "Ошибка. Невозможно открыть файл для записи." << std::endl;
			return;
		}
		file << notes;
		if(file.bad())
			std::cout << std::endl << "Ошибка. Произошла ошибка во время записи." << std::endl;
		else
		{
			std::cout << std::endl << "Запись произведена успешно." << std::endl;
			wasChanged = false;
		}
		file.close();
	}
}

void NOTEBOOK::cleanNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "Записей нет. Очищать нечего." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("Введите номер позиции, которую желаете очистить.", notes.length());
	if(position > -1 && getAnswer("Вы действительно хотите очистить запись?"))
	{
		notes[position].clear();
		wasChanged = true;
		std::cout << std::endl << "Очистка записи успешно произведена." << std::endl;
	}
}

void NOTEBOOK::findInNotes()
{
	if(notes.length() == 0)
	{
		std::cout << "Записей нет. Негде искать." << std::endl;
		return;
	}
	dzString strForSearch;
	std::cout << "Введите фразу, которую хотите найти: ";
	std::cin >> strForSearch;
	bool firstFound = true;
	for(int i = 0; i < notes.length(); i++)
	{
		if(notes[i].find(strForSearch) > -1)
		{
			if(firstFound)
			{
				std::cout << std::endl << "Фраза нашлась в записях:" << std::endl;
				firstFound = false;
			}
			notes.setOutWithNums(true);
			std::cout << (i+1) << " - " << notes[i] << std::endl;
			notes.setOutWithNums(false);
		}
	}
	if(firstFound)
		std::cout << std::endl << "Фраза не была найдена." << std::endl;
}

void NOTEBOOK::replaceInNote(bool delByTxt)
{
	if(notes.length() == 0)
	{
		std::cout << "Записей нет. Нечего заменять/удалять." << std::endl;
		return;
	}
	int position = 0;
	if(notes.length() > 1)
		position = getPosition("Введите номер записи в которой желаете произвести замену/удаление.", notes.length());
	if(position > -1)
	{
		dzString strForReplace;
		std::cout << "Введите фразу, которую хотите заменить/удалить: ";
		std::cin >> strForReplace;
		dzString strToReplace;
		if(!delByTxt)
		{
			std::cout << "Введите фразу на которую хотите заменить/удалить: ";
			std::cin >> strToReplace;
		}
		else
			strToReplace = "";
		bool all = getAnswer("Заменить/Удалить все вхождения фразы в запись?");
		int entry;
		if(!all)
		{
			do {
				std::cout << "Какое вхождение заменить/удалить?[1..]: ";
				std::cin >> entry;
				if (std::cin.fail())
					std::cin.clear();
				while (std::cin.get() != '\n');
				if(entry <= 0)
					std::cout << "Ошибка. Введите номер вхождения от 1 до ..." << std::endl;
			} while(entry <= 0);
		}
		if(getAnswer("Вы точно хотите произвести замену/удаление?"))
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
				std::cout << std::endl << "Замена/Удаление успешно произведены." << std::endl;
			else
				std::cout << std::endl << "Замена/Удаление не были произведены." << std::endl;
			wasChanged = true;
		}
	}
}

void NOTEBOOK::removeFromNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "Записей нет. Неоткуда удалять." << std::endl;
		return;
	}
	int position = getPosition("Введите номер записи, в которой желаете произвести удаление.", notes.length());
	if(position > -1)
	{
		if(notes[position].isEmpty())
		{
			std::cout << std::endl << "Эта запись пуста. Нечего удалять" << std::endl;
			return;
		}
		int posInNoteFrom = getPosition("Введите номер позиции, с которой необходимо произвести удаление.", notes[position].length() - 1);
		if(posInNoteFrom > -1)
		{
			int delLen = getPosition("Сколько символов удалить?", notes[position].length() - posInNoteFrom - 1);
			if(delLen > -1 && getAnswer("Вы точно хотите произвести удаление символов?"))
			{
				notes[position].erase(posInNoteFrom, delLen+1);
				wasChanged = true;
				std::cout << std::endl << "Удаление успешно произведено." << std::endl;
			}
		}
	}
}

void NOTEBOOK::addToNote()
{
	if(notes.length() == 0)
	{
		std::cout << std::endl << "Записей нет. Некуда добавлять." << std::endl;
		return;
	}
	int position = getPosition("Введите номер записи в которую нужно добавить текст.", notes.length());
	if(position > -1)
	{
		dzString txt;
		std::cout << "Введите текст, который хотите добавить: ";
		std::cin >> txt;
		int posInNote = getPosition("Введите позицию, в которую хотите добавить текст.", notes[position].length());
		if(posInNote > -1)
		{
			notes[position].insert(txt, posInNote);
			wasChanged = true;
			std::cout << std::endl << "Текст успешно добавлен." << std::endl;
		}
	}
}

void NOTEBOOK::mergeNotes()
{
	if(notes.length() < 2)
	{
		std::cout << std::endl << "В книге менее двух записей. Объединять нечего." << std::endl;
		return;
	}
	int position1 = getPosition("Введие номер записи, которую хотите объединить.", notes.length());
	if(position1 > -1)
	{
		int position2 = 0;
		do {
			position2 = getPosition("Введите номер записи с которой хотите объединить.", notes.length());
			if(position1 == position2)
				std::cout << std::endl << "Невозможно объединить позицию с самой собой." << std::endl << std::endl;
		} while( (position2 != -1) && position1 == position2);
		if(position2 > -1)
		{
			(notes[position2] += " ") += notes[position1];
			notes.erase(position1, 1);
			wasChanged = true;
			std::cout << std::endl << "Объединение успешно произведено." << std::endl;
		}
	}
}

int NOTEBOOK::getPosition(const char *message, int top, int bottom)
{
	int position = 0;
	bool ok;
	do {
		std::cout << message << " [" << bottom << ".." << top << "] или 0 для отмены: ";
		std::cin >> position;
		if (std::cin.fail())
			std::cin.clear();
		while (std::cin.get() != '\n');
		ok = (position >= bottom) && (position <= top);
		if(position && !ok)
			std::cout << "Ошибка. Неверно введен номер позиции. Пожалуйста, повторите попытку." << std::endl;
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
			std::cout << "Ошибка. Введите y или n." << std::endl;
	} while( !( (answer == 'y') || (answer == 'n') ) );
	if(answer == 'y')
		return true;
	return false;
}