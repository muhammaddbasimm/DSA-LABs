// Lab 9 - Task 3: Weighted M-ary Tree - Galaxy Representation Problem
// Muhammad Basim | CMS 547333 | BSDS 3A | CS-250 DSA
//
// Each StarSystem is a node with up to M outgoing TravelRoutes.
// A TravelRoute is a weighted edge: it stores the travel cost and the
// child StarSystem it leads to. Galaxy owns the root and offers
// insertion, printing, is_balanced() and count_leaves().
//
// Balance rule (from the handout): for every node, the sizes of all
// child subtrees differ by at most 1. An empty child slot is a
// subtree of size 0, so all M slots take part in the check.

#include <iostream>
#include <string>
using namespace std;

class StarSystem;

class TravelRoute {
public:
  float travel_cost;
  StarSystem* destination;  // child system this route leads to
  TravelRoute() : travel_cost(0.0f), destination(nullptr) {}
  TravelRoute(float cost, StarSystem* dest) : travel_cost(cost), destination(dest) {}
};

class StarSystem {
public:
  int system_id;
  string name;
  int num_planets;
  double x, y, z;
  bool is_habitable;
  int M;
  TravelRoute* routes;  // array of M child slots

  StarSystem(int id, const string& nm, int planets,
             double px, double py, double pz, bool habitable, int m)
    : system_id(id), name(nm), num_planets(planets),
      x(px), y(py), z(pz), is_habitable(habitable), M(m) {
    routes = new TravelRoute[M];
  }

  ~StarSystem() { delete[] routes; }

  int child_count() const {
    int c = 0;
    for (int i = 0; i < M; i++)
      if (routes[i].destination != nullptr) c++;
    return c;
  }

  // Place a child in the first free slot. Returns false if all M are used.
  bool add_route(StarSystem* child, float cost) {
    for (int i = 0; i < M; i++) {
      if (routes[i].destination == nullptr) {
        routes[i] = TravelRoute(cost, child);
        return true;
      }
    }
    return false;
  }
};

class Galaxy {
  int M;
  StarSystem* root;

  StarSystem* find(StarSystem* node, int id) const {
    if (node == nullptr) return nullptr;
    if (node->system_id == id) return node;
    for (int i = 0; i < node->M; i++) {
      StarSystem* f = find(node->routes[i].destination, id);
      if (f != nullptr) return f;
    }
    return nullptr;
  }

  // Post-order helper: returns the size of the subtree at x and
  // clears 'ok' as soon as any node breaks the balance rule.
  // Every node is visited once, so the whole check is O(n * M).
  int check_balance(StarSystem* x, bool& ok) const {
    if (x == nullptr) return 0;
    int smallest = -1, largest = -1, total = 1;
    for (int i = 0; i < x->M; i++) {
      int s = check_balance(x->routes[i].destination, ok);
      total += s;
      if (smallest == -1 || s < smallest) smallest = s;
      if (largest == -1 || s > largest) largest = s;
    }
    if (largest - smallest > 1) ok = false;
    return total;
  }

  void print(StarSystem* node, int depth, float cost) const {
    if (node == nullptr) return;
    for (int i = 0; i < depth; i++) cout << "    ";
    if (depth > 0) cout << "+-(" << cost << ")-> ";
    cout << "[" << node->system_id << "] " << node->name
         << " | planets: " << node->num_planets
         << " | (" << node->x << ", " << node->y << ", " << node->z << ")"
         << " | " << (node->is_habitable ? "habitable" : "not habitable") << endl;
    for (int i = 0; i < node->M; i++)
      print(node->routes[i].destination, depth + 1, node->routes[i].travel_cost);
  }

  void destroy(StarSystem* node) {
    if (node == nullptr) return;
    for (int i = 0; i < node->M; i++) destroy(node->routes[i].destination);
    delete node;
  }

public:
  // Overloaded constructors: an empty galaxy of order M,
  // or a galaxy of order M that starts with a given root system.
  Galaxy(int m) : M(m), root(nullptr) {}
  Galaxy(int m, StarSystem* r) : M(m), root(r) {}
  ~Galaxy() { destroy(root); }

  int get_M() const { return M; }
  StarSystem* get_root() const { return root; }
  StarSystem* find(int id) const { return find(root, id); }

  StarSystem* make_system(int id, const string& nm, int planets,
                          double px, double py, double pz, bool habitable) const {
    return new StarSystem(id, nm, planets, px, py, pz, habitable, M);
  }

  // Attach a new system under the parent with id parent_id.
  bool add_system(int parent_id, StarSystem* child, float cost) {
    StarSystem* parent = find(parent_id);
    if (parent == nullptr || !parent->add_route(child, cost)) {
      cout << "  Could not add " << child->name << endl;
      delete child;
      return false;
    }
    return true;
  }

  bool is_balanced(StarSystem* x) const {
    bool ok = true;
    check_balance(x, ok);
    return ok;
  }

  // A leaf is a star system with no outgoing routes.
  int count_leaves(StarSystem* x) const {
    if (x == nullptr) return 0;
    if (x->child_count() == 0) return 1;
    int leaves = 0;
    for (int i = 0; i < x->M; i++) leaves += count_leaves(x->routes[i].destination);
    return leaves;
  }

  int subtree_size(StarSystem* x) const {
    bool ok = true;
    return check_balance(x, ok);
  }

  void print() const {
    if (root == nullptr) cout << "  (empty galaxy)" << endl;
    else print(root, 0, 0.0f);
  }
};

void report(const Galaxy& g, const string& title) {
  cout << "--- " << title << " ---" << endl;
  g.print();
  StarSystem* r = g.get_root();
  cout << "Total star systems: " << g.subtree_size(r) << endl;
  cout << "is_balanced(root): " << (g.is_balanced(r) ? "true" : "false") << endl;
  cout << "count_leaves(root): " << g.count_leaves(r) << endl << endl;
}

int main() {
  cout << "=== Task 3: Weighted M-ary Tree - Galaxy Representation ===" << endl;

  // Constructor 1: M only, empty galaxy
  Galaxy empty(3);
  cout << "Galaxy(3) created with no root:" << endl;
  empty.print();
  cout << "is_balanced: " << (empty.is_balanced(empty.get_root()) ? "true" : "false")
       << " | count_leaves: " << empty.count_leaves(empty.get_root()) << endl << endl;

  // Constructor 2: M plus a root system
  const int M = 3;
  StarSystem* sol = new StarSystem(1, "Sol", 8, 0, 0, 0, true, M);
  Galaxy g(M, sol);
  cout << "Galaxy(" << g.get_M() << ", Sol) created with Sol as root" << endl << endl;

  g.add_system(1, g.make_system(2, "Alpha Centauri", 3, 4.37, 1.2, -0.5, false), 4.5f);
  g.add_system(1, g.make_system(3, "Sirius", 2, 8.6, -2.1, 1.0, false), 8.6f);
  g.add_system(1, g.make_system(4, "Tau Ceti", 4, 11.9, 3.3, -2.4, true), 11.9f);
  g.add_system(2, g.make_system(5, "Proxima", 1, 4.24, 1.1, -0.4, true), 0.2f);
  g.add_system(3, g.make_system(6, "Sirius B", 0, 8.7, -2.0, 1.1, false), 0.5f);
  report(g, "Galaxy A (subtree sizes at Sol: 2, 2, 1)");

  // Grow one branch so it gets two levels deeper than the others
  g.add_system(5, g.make_system(7, "Kepler-442", 5, 1206.0, 40.2, 15.7, true), 12.3f);
  report(g, "Galaxy B (added Kepler-442 under Proxima)");

  StarSystem* alpha = g.find(2);
  StarSystem* sirius = g.find(3);
  cout << "Subtree checks:" << endl;
  cout << "  Alpha Centauri -> size " << g.subtree_size(alpha)
       << ", balanced: " << (g.is_balanced(alpha) ? "true" : "false")
       << ", leaves: " << g.count_leaves(alpha) << endl;
  cout << "  Sirius         -> size " << g.subtree_size(sirius)
       << ", balanced: " << (g.is_balanced(sirius) ? "true" : "false")
       << ", leaves: " << g.count_leaves(sirius) << endl << endl;

  // Fill remaining slots to show more leaves and test the M limit
  g.add_system(4, g.make_system(8, "Trappist-1", 7, 40.7, -10.0, 5.5, true), 28.8f);
  g.add_system(4, g.make_system(9, "Gliese 581", 3, 20.4, 6.2, -1.9, false), 9.1f);
  g.add_system(1, g.make_system(10, "Vega", 0, 25.0, 12.0, 8.0, false), 25.0f);
  cout << "(Sol already has M = 3 routes, so Vega was rejected)" << endl << endl;
  report(g, "Galaxy C (Trappist-1 and Gliese 581 added under Tau Ceti)");

  return 0;
}
