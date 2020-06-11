#include <iostream>
#include <vector>
#include "route_model.h"
#include <string>
using std::cout;
using std::vector;

void PrintOpenList(vector<RouteModel::Node*> &open_list) {
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
#include <string>
void PrintNode(RouteModel::Node *node, std::string message="Node: ") {
  float value = node->h_value + node->g_value;
  cout << message <<" x: " << node->x << ", y: " << node->y << ", h:" << node->h_value << ", g: " << node->g_value << ", total value: " << value << "\n";
}

void PrintFinalPath(vector<RouteModel::Node> &final_path) {
  cout << "Printing Final Path: " << "\n";
  for (int i=0; i<=final_path.size(); i++) {
    std::string message = "Node " + std::to_string(i) + ": ";
    PrintNode(&final_path[i], message);
  }
}