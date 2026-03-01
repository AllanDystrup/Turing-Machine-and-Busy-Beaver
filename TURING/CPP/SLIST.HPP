//+1==========================================================================//
//  MODULE                      SLIST.HPP                                     //
//============================================================================//
//  FUNCTION    Define CLASS SLIST for maintaining a sorted list               
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module SLIST.CPP for definition of the class SList  
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//
#if !defined SLIST_HPP
#define SLIST_HPP 

#include <iostream>			// <iostream.h> 
#include <stdio.h>
#include <assert.h> 
#include <memory.h>
#include <string.h>




//=+2 MODULE SLIST.HPP =======================================================//
//    CLASS                     SList                                         //
//== SYNOPSIS ================================================================//
// Maintain Sorted List
class SList {

public : 
    // Data
    unsigned    uLSlots;                    // # Slots in sorted list
    unsigned    uLElems;                    // # Chars in list element
    char        *rpcList[50];               // The List DStruct
    char        rcLFile[13];                // The List file name
    FILE        *hLFile;                    // The List file handle
    unsigned    uLLast;                     // Last occupied slot in SList
    unsigned    uLMin;                      // Smallest element in list 
    char        rcLElem[5120];              // Buffer (>= uLElems) for reading SList Element
    

    // SL Constructor and Destructor
            SList (unsigned _uLSlots=5, unsigned _uLElems=201, char *_rcLFile=0);
            ~SList();
             
    // SL Accessors
    void    vInit   (char * rcLFile = (char*) "slistdef.txt");
    void    vFlush  ();
    void    vInsert (char *_pcNew, unsigned _fFlush);
    void    vDelMin (unsigned uCount = 1);

    // SL Print    
    friend std::ostream& operator<<(std::ostream&, const SList&);
    
};
// END CLASS SList



#endif // SLIST_HPP
// END MODULE SLIST.HPP                                                       //  
//============================================================================//
