#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int>& vec, int el_1, int m, int el_end) 
{
	vector<int> temp;                                   // temp vector to hold and carry out the merge operation after comaprison 

	int i, j;
	i = el_1;
	j = m + 1;

	while (i <= m && j <= el_end) 
    {

		if (vec.at(i) <= vec.at(j))                     //compare and  merge the elements in the temp vector. 
        {
			temp.push_back(vec.at(i));
			++i;
		}
		else {
			temp.push_back(vec.at(j));
			++j;
		}

	}
	while (i <= m) 
    {
		temp.push_back(vec.at(i));
		++i;
	}

	while (j <= el_end) 
    {
		temp.push_back(vec.at(j));
		++j;
	}

	for (int i = el_1; i <= el_end; ++i)
		vec.at(i) = temp.at(i - el_1);
}

void alg_mg_sort(vector<int>& vec, int el_1, int el_end) 
{
	if (el_1 < el_end) 
    {
		int m = (el_1 + el_end) / 2;                            // m denotes the middle spilt, of the vector
		alg_mg_sort(vec, el_1, m);                             // recursivly calling the merge sort fuction, till sigle unit.
		alg_mg_sort(vec, m + 1, el_end);
		merge(vec, el_1, m, el_end);
	}
}
