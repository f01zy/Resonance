#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

#include "globals.h"
#include "input.h"
#include "path.h"
#include "position.h"
#include "select.h"
#include "textarea.h"
#include <ncurses.h>

using namespace std;

string getFileNameWithoutExtension(const string &fileName) {
  size_t pos = fileName.find_last_of(".");
  if (pos != std::string::npos) {
    return fileName.substr(0, pos);
  }
  return fileName;
}

void loadScene(const char *scene) {
  do {
    clear();
    refresh();
    int x = maxx / 3.5;
    WINDOW *filesWindow = newwin(maxy, x, 0, 0);
    WINDOW *editor = newwin(maxy, x * 2, 0, x + 1);
    wborder(filesWindow, ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ');
    wborder(editor, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    curs_set(0);

    std::ifstream file("rules.txt");
    string line;
    string pathToFolder;
    vector<string> fileNames;

    while (getline(file, line)) {
      if (line.find("path:") != std::string::npos) {
        pathToFolder = line.substr(line.find(":") + 1) + "/" + scene;
        pathToFolder.erase(0, pathToFolder.find_first_not_of(" \t"));
        break;
      }
    }

    file.close();
    if (string(scene).find("/") != string::npos)
      fileNames.push_back("Back");
    else
      fileNames.push_back("Exit");
    fileNames.push_back("Create a note");
    fileNames.push_back("Create a directory");

    for (const auto &entry : filesystem::directory_iterator(pathToFolder)) {
      string filename = entry.path().filename().string();
      const char *filenameToFiles = filename.c_str();
      fileNames.push_back(filenameToFiles);
    }

    const char *options[fileNames.size()];

    for (int i = 0; i < fileNames.size(); i++) {
      options[i] = fileNames[i].c_str();
    }

    wrefresh(filesWindow);
    wrefresh(editor);

    int highlight = selectMenu(options, fileNames.size(), {1, 1});
    string option = fileNames[highlight];

    if (option == "Exit") {
      endwin();
      exit(0);
      continue;
    }

    if (option == "Back") {
      size_t pos = string(scene).find_last_of("/");
      string newScene = string(scene).erase(pos, strlen(scene));
      loadScene(newScene.c_str());
      continue;
    }

    if (option == "Create a note") {
      delwin(filesWindow);
      delwin(editor);
      clear();
      refresh();

      int maxLength = 15;
      char *filename = new char[maxLength];
      memset(filename, 0, maxLength);

      vector<int> position = calculatePosition(
          positionsHorizontal::CENTERX, positionsVertical::CENTERY, 40, 3);
      inputField(position, 15, 40, 3, "Enter a file name: ", filename);

      string path = getPath() + "/" + scene + "/" + filename + ".md";
      ofstream file(path);

      loadScene(scene);
      continue;
    }

    if (option == "Create a directory") {
      delwin(filesWindow);
      delwin(editor);
      clear();
      refresh();

      int maxLength = 15;
      char *directory = new char[maxLength];
      memset(directory, 0, maxLength);

      vector<int> position = calculatePosition(
          positionsHorizontal::CENTERX, positionsVertical::CENTERY, 40, 3);
      inputField(position, 15, 40, 3, "Enter a directory name: ", directory);

      string path = getPath() + "/" + scene + "/" + directory;
      filesystem::create_directory(path);

      loadScene(scene);
      continue;
    }

    string path = getPath() + "/" + scene + "/" + option;
    if (filesystem::is_directory(path)) {
      string newScene = string(scene) + "/" + option;
      loadScene(newScene.c_str());
    } else {
      textarea(editor, path);
      loadScene(scene);
    }
  } while (1);
};
