#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <filesystem>

#include "position.h"
#include "path.h"
#include "select.h"
#include "input.h"
#include "logs.h"
#include "scene.h"
#include <curses.h>

int main()
{
	initscr();
	setlocale(LC_ALL, "");
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	getmaxyx(stdscr, maxy, maxx);

	const char* options[] = {
		"Log in the vault",
		"Create vault",
		"Github",
		"Exit",
	};

	int length = sizeof(options) / sizeof(options[0]);
	int width = 0;

	for (int i = 0; i < length; i++)
	{
		if (strlen(options[i]) > width)
		{
			width = strlen(options[i]);
		}
	}

	vector<int> welcomeTextPosition = calculatePosition(positionsHorizontal::CENTERX, positionsVertical::CENTERY, 58, 12);
	int lines = 0;
	int symbols = 0;

	for (int i = 0; i < strlen(welcomeText); i++)
	{
		char ch = welcomeText[i];
		if (ch == '\n')
		{
			lines++;
			symbols = 0;
		}
		else
		{
			mvaddch(welcomeTextPosition[1] + lines - 1, welcomeTextPosition[0] + symbols, ch);
			symbols++;
		}
	}

	vector<int> versionInformationPosition = calculatePosition(positionsHorizontal::CENTERX, positionsVertical::BOTTOM, strlen(version), 1);
	attron(A_BOLD);
	mvprintw(versionInformationPosition[1] - 1, versionInformationPosition[0], version);
	attroff(A_BOLD);

	vector<int> position = calculatePosition(positionsHorizontal::CENTERX, positionsVertical::CENTERY, width, length);
	position[1] += 3;
	do
	{
		int highlight = selectMenu(options, length, position);
		const char* option = options[highlight];

		if (option == "Create vault")
		{
			clear();
			refresh();
			int width = 40;
			int heigth = 3;
			int maxLength = 10;
			char* vaultName = new char[maxLength + 2];
			memset(vaultName, 0, maxLength + 2);
			vector<int> position = calculatePosition(positionsHorizontal::CENTERX, positionsVertical::CENTERY, width, heigth);
			int pathLength = 100;
			char* path = new char[pathLength];
			string pathToVault;
			do
			{
				inputField(position, maxLength, 40, 3, "Enter a vault name: ", vaultName);
				if (filesystem::exists("rules.txt"))
				{
					string path = getPath();
					pathToVault = path + "/" + string(vaultName);
					if (filesystem::exists(pathToVault))
					{
						errorLog("A repository with this name already exists");
						continue;
					}
					break;
				}
				break;
			} while (1);
			if (!filesystem::exists("rules.txt")) {
				memset(path, 0, pathLength);
				do
				{
					inputField(position, pathLength, 40, 3, "Path to folder: ", path);
					if (!filesystem::exists(path))
					{
						errorLog("Enter a valid path to directory");
					}
					else
					{
						pathToVault = string(path) + "\\" + vaultName;
						if (filesystem::exists(pathToVault))
						{
							errorLog("A repository with this name already exists");
							continue;
						}
						break;
					}
				} while (1);
				ofstream file("rules.txt");
				if (file.is_open())
				{
					file << "path: " << path << endl;
				}
			}
			filesystem::create_directory(pathToVault);
			loadScene(vaultName);
		}

		if (option == "Exit")
		{
			endwin();
			exit(0);
		}

		if (option == "Github")
		{
			system(("start " + string(github)).c_str());
		}

		if (option == "Log in the vault")
		{
			if (filesystem::exists("rules.txt"))
			{
				clear();
				refresh();
				vector<string> vaultsVector;
				string path = getPath();

				for (const auto& entry : filesystem::directory_iterator(path))
				{
					if (!filesystem::is_directory(entry.path()))
					{
						continue;
					}
					string filename = entry.path().filename().string();
					const char* filenameToFiles = filename.c_str();
					vaultsVector.push_back(filenameToFiles);
				}

				int width = 0;
				const int vaultsLength = vaultsVector.size();
				const char* vaults[] = { "" };

				for (size_t i = 0; i < vaultsVector.size(); ++i) {
					vaults[i] = vaultsVector[i].c_str();
				}

				for (int i = 0; i < vaultsLength; i++)
				{
					if (strlen(vaults[i]) > width)
					{
						width = strlen(vaults[i]);
					}
				}

				vector<int> position = calculatePosition(positionsHorizontal::CENTERX, positionsVertical::CENTERY, width, vaultsLength);
				int highlight = selectMenu(vaults, vaultsLength, position);
				const char* option = vaults[highlight];
				loadScene(option);
			}
			else
			{
				errorLog("You have not created any storage");
			}
		}
	} while (1);

	return 0;
};