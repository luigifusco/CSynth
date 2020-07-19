#pragma once

#include <string>
#include <vector>
#include <map>

namespace gui {

void init(std::map<char, void (*)()>);
void showError(std::string err);
int choose(std::string prompt, std::vector<std::string> choices);
char getCharacter();

}