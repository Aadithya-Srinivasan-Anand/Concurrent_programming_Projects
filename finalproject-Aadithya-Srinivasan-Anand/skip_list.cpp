 //   Implementation of Concurrent Skip list data structure with insert, delete, search and range operations


#include <iostream>
#include <math.h>
#include <limits>
#include <map> 
#include <stdio.h> 
#include <stdlib.h>
#include "skip_list.h"

#define INT_MINI numeric_limits<int>::min() 
#define INT_MAXI numeric_limits<int>::max()

static int max_lvl;


Skip_List::Skip_List(int max_elements, float probability) // the skiplist constructor
{
    max_lvl = (int) round(log(max_elements) / log(1/probability)) - 1;
    head_node = new Node(INT_MINI, max_lvl);
    tail_node = new Node(INT_MAXI, max_lvl);

    for (size_t i = 0; i < head_node->next.size(); i++) {
        head_node->next[i] = tail_node;
    }
}


int Skip_List::find(int key, vector<Node*> &predecessors, vector<Node*> &successors) // Finds the predecessors and successors at each level
 {
    int found = -1;
    Node *prev_node = head_node; 

    for (int level = max_lvl; level >= 0; level--){
        Node *curr_node = prev_node->next[level];

        while (key > curr_node->get_key()){
            prev_node = curr_node;
            curr_node = prev_node->next[level];
        }
        
        if(found == -1 && key == curr_node->get_key()){
            found = level;
        }

        predecessors[level] = prev_node;
        successors[level] = curr_node;
    }
    return found;
}


int Skip_List::get_random_level()  //randomly generating a number and incrementing level if number is less than equal to 0.3 
{
    int l = 0;
    while(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= 0.3){
        l++;
    }
    return l > max_lvl ? max_lvl : l;
}


bool Skip_List::add(int key, string value) // implementation of skiplist using locks
 {
    int top_level = get_random_level();

    vector<Node*> preds(max_lvl + 1); 
    vector<Node*> succs(max_lvl + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }

    while(true)// Keep trying to insert the element into the list. In case predecessors and successors are changed, this loop helps to try the insert again
    {
        
        int found = find(key, preds, succs);
        if(found != -1){
            Node* node_found = succs[found];
            
            if(!node_found->marked){
                while(! node_found->fully_linked){
                }
                return false;
            }
            continue;
        }

        map<Node*, int> locked_nodes; // Store all the Nodes which lock we acquire in a map

        try // exp handling since loops terminate
        {
            Node* pred;
            Node* succ;

            // Used to check if the predecessor and successors are same from when we tried to read them before
            bool valid = true;

            for (int level = 0; valid && (level <= top_level); level++){
                pred = preds[level];
                succ = succs[level];

                // If not already acquired lock, then acquire the lock 
                if(!(locked_nodes.count( pred ))){
                    pred->lock();
                    locked_nodes.insert(make_pair(pred, 1));
                }

                // If predecessor marked or if the predecessor and successors change, then abort and try again
                valid = !(pred->marked.load(std::memory_order_seq_cst)) && !(succ->marked.load(std::memory_order_seq_cst)) && pred->next[level]==succ;                
            }

            // Conditons are not met, release locks, abort and try again.
            if(!valid){
                for (auto const& x : locked_nodes){
                    x.first->unlock();
                }
                continue;
            }

            // All conditions satisfied, create the Node and insert it as we have all the required locks
            Node* new_node = new Node(key, value, top_level);

           
            for (int level = 0; level <= top_level; level++){
                new_node->next[level] = succs[level];
            }

            for (int level = 0; level <= top_level; level++){
                preds[level]->next[level] = new_node;
            }

            new_node->fully_linked = true;
        
            for (auto const& x : locked_nodes){
                x.first->unlock();
            }
            
            return true;
        }catch(const std::exception& e){
            // If any exception occurs during the above insert, release locks of the held nodes and try again.
            std::cerr << e.what() << '\n';
            for (auto const& x : locked_nodes){
                    x.first->unlock();
            }
        }
    }
}


string Skip_List::search(int key){

    vector<Node*> preds(max_lvl + 1); 
    vector<Node*> succs(max_lvl + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }
    int found = find(key, preds, succs);
    if(found == -1){
        return "";
    }

    Node *curr_node = head_node; 

    for (int level = max_lvl; level >= 0; level--){
        while (curr_node->next[level] != NULL && key > curr_node->next[level]->get_key()){
            curr_node = curr_node->next[level];
        }
    }

    curr_node = curr_node->next[0];

    if ((curr_node != NULL) && (curr_node->get_key() == key) && succs[found]->fully_linked && !succs[found]->marked){
        return curr_node->get_value();
    }else {
        return "";
    }
}


bool Skip_List::remove(int key)
{
    
    Node* victim = NULL;
    bool is_marked = false;
    int top_level = -1;

    vector<Node*> preds(max_lvl + 1); 
    vector<Node*> succs(max_lvl + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }

    
    while(true){
        
        
        int found = find(key, preds, succs);
        if(found != -1){
            victim = succs[found];
        }

        // If node not found and the node to be deleted is fully linked and not marked return
        if(is_marked | 
                (found != -1 &&
                (victim->fully_linked && victim->top_level == found && !(victim->marked))
                )
            ){
                // If not marked, the we lock the node and mark the node to delete
                if(!is_marked){
                    top_level = victim->top_level;
                    victim->lock();
                    if(victim->marked){
                        victim->unlock();
                        return false;
                    }
                    victim->marked = true;
                    is_marked = true;
                }

                map<Node*, int> locked_nodes;

                try{
                    Node* pred;
                    bool valid = true;

                    for(int level = 0; valid && (level <= top_level); level++){
                        pred = preds[level];
                        if(!(locked_nodes.count( pred ))){
                            pred->lock();
                            locked_nodes.insert(make_pair(pred, 1));
                        }
                        
                       
                        valid = !(pred->marked) && pred->next[level] == victim;
                    }
                    if(!valid){
                        for (auto const& x : locked_nodes){
                            x.first->unlock();
                        }
                        continue;
                    }

                    for(int level = top_level; level >= 0; level--){
                        preds[level]->next[level] = victim->next[level];
                    }

                    victim->unlock();
                    for (auto const& x : locked_nodes){
                        x.first->unlock();
                    }

                    return true;
                }catch(const std::exception& e){
                    for (auto const& x : locked_nodes){
                        x.first->unlock();
                    }
                }

            }else{
                return false;
            }
    }
}


map<int, string> Skip_List::range(int start_key, int end_key)
{

    map<int, string> range_output;

    if(start_key > end_key){
        return range_output;
    }

    Node *curr_node = head_node;

    for (int level = max_lvl; level >= 0; level--){
        while (curr_node->next[level] != NULL && start_key > curr_node->next[level]->get_key()){
            if(curr_node->get_key() >= start_key && curr_node->get_key() <= end_key){
                range_output.insert(make_pair(curr_node->get_key(), curr_node->get_value()));
            }
            curr_node = curr_node->next[level];
        }
    }

    while(curr_node != NULL && end_key >= curr_node->get_key()){
        if(curr_node->get_key() >= start_key && curr_node->get_key() <= end_key){
            range_output.insert(make_pair(curr_node->get_key(), curr_node->get_value()));
        }
        curr_node = curr_node->next[0];
    }

    return range_output;

}

void Skip_List::display(){
    for (int i = 0; i <= max_lvl; i++) {
        Node *temp = head_node;
        int count = 0;
        if(!(temp->get_key() == INT_MINI && temp->next[i]->get_key() == INT_MAXI)){
            printf("Level_Position %d  ", i);
            while (temp != NULL){
                printf("%d -> ", temp->get_key());
                temp = temp->next[i];
                count++;
            }
            cout<<endl;
        }
        if(count == 3) break;
    }
    printf("End of display :\n\n");
}

Skip_List::Skip_List(){   
}

Skip_List::~Skip_List(){
}
