# Lab_0 Report

## code structure

In `main.cpp`, initially we use the `getopt_long` bash built in UNIX shell command for parsing command line. we use the `option` struct in this case, and inside the structure, `ln_opt` is a pointer to the first element of an array of struct option declared in <getopt.h> in the structre. The struct contains the option of parsing regular arguments, optional and no arguments. In our case `--name` does not require any post arguments. the last option from the struct gives the short arguments.

Here ` ct = getopt_long(argc, argv, "-:n:o:a:", ln_opt, &id); `. `ct` is the control option for the swtich cases, and argc, arv are the arguments from the command line. Further, `-:n:o:a:` represents the mechanism in which the arguments are expected and are parsed with the help of the getopt.h

According to the user input option, at `--alg<=merge,quick` the header files containing one of the sorting algorithm is executed (files `msort.h`
and `qsort.h`)

The input from the argument is given in the form of a `.txt` file, the given is file consisting of numbers is stored is stored inside a vector `vec`. The contents are then converted into integer from string to perform sorting operations. The type of sort to be performed is determined from the user arguments `--alg<=merge,quick`, controlled by the variable 'ct'.

`qsort.h` code organisation:
 `alg_qk_sort()` gets the arguments `vec, el_1 and el_end` vec represents the vector to be operated on which is passed through reference and el_1 and el_end are the starting and ending elements of the vector. the quick sort algorithm checks for el_1<elend. To call the `breakdown()` fuction which essentailly is an instance of divide and conquor . The `breakown()` function the loop traverses and finds elements and compares them to the shifter element value and onc ethe condition check is stisfied the two ierative pointer elements gets swapped, the process is continued till 'j' <'i' and then the element from shifter gets swapped in, creating two lists on the left and right sides of the swapped in element, which is now in the sorted position. This process is continued in both the halves recursively by calling the `alg_qk_sort()`.
 
 `msort.h` :
 `alg_mg_sort()` calls the merge sort function which also uses divide and conquor to sort the elements. Here the function `merge()` the list which is presented to it from the bottom up. the `alg_mg_sort()` splits the entire list into single elemental list and then each of the elments are merged by comparing the elements in each lists. This is done recursivly until we get all the elements in the list are sorted.


`Makefile` is an executable file, which constists of our target named output with executable dependencies`main.o` into an executable named `mysort`.






