#pragma once

#include <string>
#include <vector>

namespace gui {

void init();
void clear();
void showError(std::string err);
int choose(std::string prompt, std::vector<std::string> choices);

}