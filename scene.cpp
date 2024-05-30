#include <fstream>;
#include <filesystem>
#include <vector>

#include <curses.h>
#include "globals.h"

using namespace std;

void loadScene(const char* scene)
{
	do
	{
		clear();
		refresh();
		int x = maxx / 3.5;
		WINDOW* filesWindow = newwin(maxy, x, 0, 0);
		WINDOW* editor = newwin(maxy, x * 2, 0, x);
		wborder(filesWindow, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		wborder(editor, '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

		std::ifstream file("rules.txt");
		string line;
		string pathToFolder;
		vector<const char*> fileNames;

		while (getline(file, line)) {
			if (line.find("path:") != std::string::npos) {
				pathToFolder = line.substr(line.find(":") + 1);
				pathToFolder.erase(0, pathToFolder.find_first_not_of(" \t"));
				break;
			}
		}

		file.close();

		for (const auto& entry : filesystem::directory_iterator(pathToFolder))
		{
			string filename = entry.path().filename().string();
			const char* filenameToFiles = filename.c_str();
			fileNames.push_back(filenameToFiles);
		}

		for (int i = 0; i < fileNames.size(); i++)
		{
			mprintw(filesWindow, fileNames[i]);
		}

		wrefresh(filesWindow);
		wrefresh(editor);
		getch();
	} while (1);
};