#include <map>
#include "headers.h"

class Skip_List{
    private:
        // Head and Tail of the Skiplist
        Node *head_node;
        Node *tail_node;
    public:
        Skip_List();
        Skip_List(int max_elements, float probabilityability);
        ~Skip_List();
        int get_random_level();

        // Supported operations
        int find(int key, vector<Node*> &predecessors, vector<Node*> &successors);
        bool add(int key, string value);
        string search(int key);
        bool remove(int key);
        map<int, string> range(int start_key, int end_key);
        void display();
};