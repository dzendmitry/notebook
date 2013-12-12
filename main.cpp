#include "stdafx.h"
#include <clocale>
#include <windows.h>
#include "NOTEBOOK.h"
 
int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	try
	{
		NOTEBOOK book;
		int command;
		std::cout << std::endl << "         ****** ����������� �������� ����� ******" << std::endl;
		do {
			book.showAllCommands();
			std::cout << "������� ����� ������� � ������� Enter: ";
			std::cin >> command;
			if (std::cin.fail())
				std::cin.clear();
			while (std::cin.get() != '\n');
			book.processCommand(command);
		} while(command != EXIT);
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		system("PAUSE");
		return -1;
	}

	std::cout << std::endl << "������ ���������� ������� ���������." << std::endl;
	system("PAUSE");
	return 0;
}