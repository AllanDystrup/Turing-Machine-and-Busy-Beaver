//+1==========================================================================//
//  MODULE                      SORT.CPP                                      //
//============================================================================//
//  FUNCTION    Implementation of general array-sorting functions
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    SORT.HPP
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//


//----------------------------------------------------------------------------//
//                              Include                                       //
//----------------------------------------------------------------------------//
#include <iostream>				// <iostream.h>
#include <stdlib.h>
#include "sort.hpp"



//----------------------------------------------------------------------------//
//                              TestDriver                                    //
//----------------------------------------------------------------------------//
#define GENERAL_MAIN
//#undef GENERAL_MAIN



//+4 MODULE SORT.CPP ---------------------------------------------------------//
//   FUNCTION                       swap                                      //
//-- SYNOPSIS ----------------------------------------------------------------//
template <class Type>
void swap (Type *ia, int i, int j )
{ // swap two elements of an array
    Type tmp = ia[ i ];
    ia[ i ] = ia[ j ];
    ia[ j ] = tmp;
}


//+3 MODULE SORT.CPP ---------------------------------------------------------//
//   FUNCTION                       b_sort                                    //
//-- SYNOPSIS ----------------------------------------------------------------//
template <class Type>
void b_sort(Type *ia, int low, int high)
{ // bubble sort
    for ( int i = low; i <= high; i++ )
        for ( int j = i; j <= high; j++ )
            if ( ia[i] > ia[j] )
                swap( ia, i, j );
}


//+3 MODULE SORT.CPP ---------------------------------------------------------//
//   FUNCTION                       q_sort                                    //
//-- SYNOPSIS ----------------------------------------------------------------//
template <class Type>
void q_sort(Type *ia, int low, int high)
{ // quick sort
    if ( low < high ) {
    int lo = low;
    int hi = high + 1;
    Type elem = ia[ low ];

    for (;;) {
        while ( ia[ ++lo ] < elem ) ;
        while ( ia[ --hi ] > elem ) ;

        if ( lo < hi )
            swap( ia, lo, hi );
        else break;
    }

    swap( ia, low, hi );
    q_sort(ia, low, hi - 1);
    q_sort(ia, hi + 1, high);
    }
}



#ifdef GENERAL_MAIN
//============================================================================//
//                                  main                           TestDriver //
//============================================================================//
template <class Type>
void display
(   Type    *ia,
    int     size )
{
// Display format < 0 1 2 3 4 5 >
    std::cout << "<";
        for (int ix = 0; ix < size; ++ix)
            std::cout << ia[ix] << " ";
        std::cout << ">\n";
}

int	
main
(void)
{
    double da[] = { 26.7, 5.7, 37.7, 1.7, 61.7,
                    11.7, 59.7, 15.7, 48.7, 19.7, 14.9 };

    int ia[] = {    503, 87, 512, 61, 908, 170, 897, 275,
                    653, 426, 154, 509, 612, 677, 765, 703 };
    int size;

    size = sizeof(da)/sizeof(double);
    std::cout << "Quicksort of double array (size == " << size << ")" << std::endl;
    display(da, size);
    q_sort(da, 0, size-1);
    display(da, size);

    size = sizeof(ia)/sizeof(int);
    std::cout << "\nBoublesort of int array (size == " << size << ")" << std::endl;
    display(ia, size);
    b_sort(ia, 0, size-1);
    display(ia, size);
    
    return 1;
}
#endif // GENERAL_MAIN


// END MODULE SORT.CPP                                                        //
//============================================================================//