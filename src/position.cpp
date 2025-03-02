#include <vector>

#include "../include/globals.h"

using namespace std;

vector<int> calculatePosition(positionsHorizontal horizontal,
                              positionsVertical vertical, int width,
                              int height) {
  vector<int> position;
  switch (horizontal) {
  case positionsHorizontal::LEFT:
    position.push_back(0);
    break;
  case positionsHorizontal::CENTERX:
    position.push_back(maxx / 2 - width / 2);
    break;
  case positionsHorizontal::RIGHT:
    position.push_back(maxx - width);
    break;
  default:
    break;
  }

  switch (vertical) {
  case positionsVertical::TOP:
    position.push_back(0);
    break;
  case positionsVertical::CENTERY:
    position.push_back(maxy / 2 - height / 2);
    break;
  case positionsVertical::BOTTOM:
    position.push_back(maxy - height);
    break;
  default:
    break;
  }

  return position;
};
