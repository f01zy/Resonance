#include <fstream>;
#include <filesystem>
#include <vector>

#include <curses.h>
#include "globals.h"
#include "path.h"
#include "select.h";
#include "position.h"
#include "input.h"
#include "textarea.h"

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
		wborder(filesWindow, ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ');
		wborder(editor, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

		std::ifstream file("rules.txt");
		string line;
		string pathToFolder;
		vector<string> fileNames;

		while (getline(file, line)) {
			if (line.find("path:") != std::string::npos) {
				pathToFolder = line.substr(line.find(":") + 1) + "\\" + scene;
				pathToFolder.erase(0, pathToFolder.find_first_not_of(" \t"));
				break;
			}
		}

		file.close();

		fileNames.push_back("Create a note");

		for (const auto& entry : filesystem::directory_iterator(pathToFolder))
		{
			string filename = entry.path().filename().string();
			const char* filenameToFiles = filename.c_str();
			fileNames.push_back(filenameToFiles);
		}

		const char* options[] = { "" };

		for (size_t i = 0; i < fileNames.size(); ++i) {
			options[i] = fileNames[i].c_str();
		}

		wrefresh(filesWindow);
		wrefresh(editor);

		int highlight = selectMenu(options, fileNames.size(), {1, 1});
		string option = fileNames[highlight];

		if (option == "Create a note")
		{
			delwin(filesWindow);
			delwin(editor);
			clear();
			refresh();
			
			int maxLength = 15;
			char* filename = new char[maxLength];
			memset(filename, 0, maxLength);

			vector<int> position = calculatePosition(positionsHorizontal::CENTERX, positionsVertical::CENTERY, 40, 3);
			inputField(position, 15, 40, 3, "Enter a file name: ", filename);

			string path = getPath() + "\\" + scene + "\\" + filename;
			printw(path.c_str());
			getch();

			ofstream file(path);

			loadScene(scene);
		}
		else
		{
			string path = getPath() + "\\" + scene + "\\" + option;
			textarea(editor, path);
			loadScene(scene);
		}
	} while (1);
};