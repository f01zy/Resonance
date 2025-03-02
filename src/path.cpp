#include <string>
#include <fstream>

using namespace std;

string getPath()
{
	std::ifstream file("rules.txt");

	string line;
	string path;

	while (getline(file, line)) {
		if (line.find("path:") != std::string::npos) {
			path = line.substr(line.find(":") + 1);
			path.erase(0, path.find_first_not_of(" \t"));
			break;
		}
	}

	return path;
};