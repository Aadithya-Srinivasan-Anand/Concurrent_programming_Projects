#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <getopt.h>
#include "qsort.h"
#include "msort.h"

using namespace std; 

int main(int argc, char* argv[])
{
 int ct, el_end ,count =0,i, el_1=0;
 string answer_txt,str,triggr,temp;
 ofstream file_2; ifstream file_1;
 vector <int> vec;

while(1)
 {
    int id=0;
    static struct option ln_opt[] = {
    {"name", no_argument, NULL,'n'},{"o", required_argument, NULL,'o' },{"alg",  required_argument, NULL,'a'},{NULL,0,NULL,0 }
    };
    ct = getopt_long(argc, argv, "-:n:o:a:", ln_opt, &id); // specifying getopt_long arguments.
    if (ct == -1)
        break;
    switch (ct) 
    {
        case 1:
        triggr = string(optarg);
        file_1.open(triggr);
        while(getline(file_1,str))
        {
            vec.push_back(stoi(str));
        }
        break;

        case 'n':
        cout<<"Aadithya-Srinivasan-Anand";
        break;

        case 'o':
        temp = string(optarg);
        break;

        case 'a':
        //cout<<"case working"<<optarg;
        if(string(optarg) == "quick")
        {
            int n = vec.size();
            for(i=0; i< n; i++)
            {
                count++;
            }
            el_end = n-1;
            alg_qk_sort(vec,el_1,el_end);
            file_2.open(temp);
            for(i=0; i < vec.size(); i++)
            {
                    file_2<<vec.at(i)<<"\n";
            }

        }
        else if (string(optarg) == "merge" )
        {
            int m = vec.size();
            el_end = m-1;
            alg_mg_sort(vec,el_1,el_end);
            file_2.open(temp);
            for(i=0; i < vec.size(); i++)
            {
                    file_2<<vec.at(i)<<"\n";
            }

        }
    }
 }
}    