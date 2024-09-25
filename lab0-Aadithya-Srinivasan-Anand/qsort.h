#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std; 

int breakdown(vector <int>& vec, int el_1, int el_end) // breaking the vector into 2
{	
	int shifter = el_end;                   // shifter element is taken as the last element.
	int j = el_1;
	for(int i=el_1;i<el_end;++i)
   {                                      //To check if the the traversal has crossed, the point gives the sorted location
		if(vec.at(i)<vec.at(shifter))
      {
			swap(vec.at(i),vec.at(j));
			++j;
		}
	}
	swap(vec.at(j),vec.at(shifter));
	return j;
	
}

void alg_qk_sort(vector <int>& vec, int el_1, int el_end) // passing vec by reference, negating return the vector
{
 if (el_1 < el_end)
 {
 int x = breakdown(vec,el_1,el_end);
 alg_qk_sort(vec, el_1, x-1);                                        // recursivly sorting 
 alg_qk_sort(vec, x+1, el_end);
 }

}

