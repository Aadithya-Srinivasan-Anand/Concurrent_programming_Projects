#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
using namespace std;

class KeyValuePair
{
    private:
        int key;
        string value;
    public:
        KeyValuePair();
        KeyValuePair(int key, string value);
        ~KeyValuePair();
        int get_key();
        string get_value();
};

class Node{
    public:
        
        KeyValuePair key_value_pair; // Stores the key and value for the Node
        vector<Node*> next;// Stores the reference of the next node until the top level for the node
        mutex node_lock;// Lock to lock the node when modifing it
        atomic<bool> marked = {false};// Atomic variable to be marked if this Node is being deleted
        atomic<bool> fully_linked = {false};// Atomic variable to indicate the Node is completely linked to predecessors and successors
        int top_level; // The Maximum level until which the node is available
        Node();
        Node(int key, int level);
        Node(int key, string value, int level);
        ~Node();
        int get_key();
        string get_value();
        void lock();
        void unlock();
};
