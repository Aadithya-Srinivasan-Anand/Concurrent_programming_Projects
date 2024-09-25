/*
    Concurrent programming project 
    Author - Aadithya Srinivasan Anand
	Implementation of the concurr_nodeent skip list data structure
	Sorted output written into another file
	Usage: ./skiplist [--name] -m <iterations> -t <num_threads> 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h> 
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "skip_list.h"

using namespace std;

size_t num_threads = 1;
Skip_List skiplist;


vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int,int>> numbers_range;



void generate_input(int max_number) // Generates input data in vectors to perform the operations.
{
    
    for(int i = 1; i <= max_number; i++) // insertion
    {
        numbers_insert.push_back(i);
    }
    for(size_t i = 0; i < numbers_insert.size(); i++) // deletion
    {
        if( rand() % 4 == 0 ){
            numbers_delete.push_back(numbers_insert[i]);
        }
    }
    for(size_t i = 0; i < numbers_insert.size(); i++) // searching
    {
        if( rand() % 4 == 0 ){
            numbers_get.push_back(numbers_insert[i]);
        }
    }
    for(size_t i = 0; i < num_threads; i++) // range
    {
        int a = (rand() % static_cast<int>(max_number + 1));
        int b = a + (rand() % static_cast<int>(max_number - a + 1));
        numbers_range.push_back(make_pair(a,b));
    }
}


void skiplist_add(size_t start, size_t end){
    if(end >= numbers_insert.size()) end = numbers_insert.size();
    if(start == end) skiplist.add(numbers_insert[start], to_string(numbers_insert[start]));
    for(size_t i = start; i < end; i++){
        skiplist.add(numbers_insert[i], to_string(numbers_insert[i]));
    }
}


void skiplist_remove(size_t start, size_t end){
    if(end >= numbers_delete.size()) end = numbers_delete.size();
    if(start == end) skiplist.remove(numbers_delete[start]);
    for(size_t i = start; i < end; i++){
        skiplist.remove(numbers_delete[i]);
    }
}

void skiplist_search(size_t start, size_t end){
    if(end >= numbers_get.size()) end = numbers_get.size();
    if(start == end) end++;
    for(size_t i = start; i < end; i++){
        string s = skiplist.search(numbers_get[i]);
        if(s.empty()) s = "Not Found";
        cout << "Searching for " << numbers_get[i] << " Search value: " << s << endl;
    }
}


void skiplist_range(int start, int end){
    map<int, string> range_output = skiplist.range(start, end);

    string s = "";
    for (auto const& x : range_output){
        s += x.second + " ";
    }
    cout << "Nodes between " << start << " and " << end <<" -> " << s << endl;
}


void Concurrent_skiplist(){
    vector<thread> threads;

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    cout << "\n Insertion complete: skip_list status: " << endl;

    skiplist.display();

    threads.clear();

    cout << "\n Deleting elements :" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
    std::cout << *i << ' ';

    // delete
    chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_remove, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    cout << "\n\n deletion complete: skip list status " << endl;

    skiplist.display();

    threads.clear();

    cout << "\n searching random numbers " << endl;
    // search
    chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();

    cout << "\n Range between random numbers" << endl;
    // range
    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
}

int main(int argc, char *argv[])
{

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {0, 0, 0, 0}
    };

    //Required option variables to run the program
    bool name = false;
    int max_number = -1;
    string operation = "";

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "m:t:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 't':
                num_threads = stoi(optarg);
                break;
            case 'm':
                max_number = stoi(optarg);
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if(name){
        cout << "\nAadithya Srinivasan Anand\n";
    }

	else if(argc > 2){
	    if(max_number <= 0 || num_threads < 1 ){
	        return 0;
	    }
        else{

            generate_input(max_number);
            skiplist = Skip_List(numbers_insert.size(), 0.3);

            Concurrent_skiplist();
	    }
    }
    else
    {
        cout<<"Invalid Usecase";
    }
    return 0;
}