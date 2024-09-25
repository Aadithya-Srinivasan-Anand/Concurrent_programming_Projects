// headers
#include "headers.h"


KeyValuePair::KeyValuePair()
{}

KeyValuePair::KeyValuePair(int k, string v)
{
    key = k;
    value = v;
}

int KeyValuePair::get_key()
{
    return key;
}

string KeyValuePair::get_value()
{
    return value;
}

KeyValuePair::~KeyValuePair(){
}

// for node

Node::Node(){}

Node::Node(int key, int level)
{
    key_value_pair = KeyValuePair(key, "");
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    top_level = level;
}

Node::Node(int key, string value, int level){
    key_value_pair = KeyValuePair(key, value);
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    top_level = level;
}

int Node::get_key(){
    return key_value_pair.get_key();
}

string Node::get_value(){
    return key_value_pair.get_value();
}


void Node::lock(){
    node_lock.lock();
}

void Node::unlock(){
    node_lock.unlock();
}

Node::~Node(){
}