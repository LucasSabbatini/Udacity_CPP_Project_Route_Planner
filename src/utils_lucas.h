#include <iostram>
#include <vector>
#include "route_model.h"

using std::cout;
using std::vector;

void PrintOpenList(vector<RouteModel::Node*> open_list) {
  int counter = 0;
  for (auto node : open_list) {
    double value = node->h_value + node->g_value;
    cout << "value for node: " << value << "\n";
    if (counter >= 20) {
      break;
    }
    counter++;
  }
}

void PrintNode(RouteModel::Node *node) {
  float value = node->h_value + node->g_value;
  cout << "Node: x: " << node->x << ", y: " << node->y << ", h:" << node->h_value << ", g: " << node->g_value << ", total value: " << value << "\n";
}