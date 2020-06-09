#include "route_planner.h"
#include <algorithm>
#include <vector>
using std::vector;
using std::cout;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x, start_y); // Should I dereference it?
    end_node = &m_Model.FindClosestNode(end_x, end_y);
  	start_node->g_value = 0;
  	start_node->h_value = CalculateHValue(start_node);
  	m_Model = model;
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
  //find neighbors for current node using FindNeighbors
    current_node->FindNeighbors(); // populate current_node.neighbors. This will also check if node has been visited
    for (auto node : current_node->neighbors) {
        node->parent = current_node; // node.parent is a pointer, and so is current_node
        (*node).h_value = this->CalculateHValue(node);
        (*node).g_value = current_node->g_value + current_node->distance(*node); // 
        this->open_list.push_back(node);
        (*node).visited = true;
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.
#include <algorithm>
using std::sort;
bool Compare(const RouteModel::Node *n1, const RouteModel::Node *n2) {
  // Compare the two nodes using the sum of their h ang g values
  int v1 = n1->h_value + n1->g_value;
  int v2 = n2->h_value + n2->g_value;
  return v1 > v2;
}
/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void NodeSort(std::vector<RouteModel::Node*> &open) {
  sort(open.begin(), open.end(), Compare);
}

void PrintOpenList(vector<RouteModel::Node*> open_list) {
  if (open_list.size() > 30) {
    for (int i=0; i<20; i++) {
      auto node = open_list[i];
      double value = node->h_value + node->g_value;
      cout << "value for node: " << value << "\n";
    }
  }
}
  
RouteModel::Node *RoutePlanner::NextNode() { // This means it will return a pointer to a node
  // Sorting the open list using auxiliar functions
  cout << "------------\n" << "Before NodeSort" << "\n";
//   if (open_list.size() > 30)//   	for (int i=0; i<20; i {
//       auto node = open_t[i];
//       double value = node->h_value + nodg_value;
//       cout << "value for node: " << ue "\n";
//     }
//   }
//   cout << "Size of open list: " << open_list.size() << "\n";
  PrintOpenList(open_list);
  NodeSort(open_list);
  cout << "After NodeSort" << "\n";
  PrintOpenList(open_list);
  //Pointing to the first node from the list: the next node to open
  RouteModel::Node *next_node = this->open_list.back();
  this->open_list.pop_back();
  return next_node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
  	path_found.push_back(*current_node); // insert final node
  	while (current_node != this->start_node) {
      distance += current_node->distance(*(current_node->parent));
      current_node = current_node->parent;
      path_found.push_back(*current_node);
    }
	// TODO reverse path_found list
  	std::reverse(path_found.begin(), path_found.end());
  
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
  RouteModel::Node *current_node = nullptr;

  // TODO: Implement your solution here.
  start_node->visited = true;
  current_node = start_node;
  open_list.push_back(current_node);
  AddNeighbors(current_node);

  while (!open_list.empty()) {
    current_node = NextNode();
    if (current_node == end_node) {
      std::vector<RouteModel::Node> final_path = ConstructFinalPath(current_node);
      this->m_Model.path = final_path;
      std::cout << "The final path has " << final_path.size() << " nodes" << "\n" ;
      return;
    }
    AddNeighbors(current_node);
  }


}