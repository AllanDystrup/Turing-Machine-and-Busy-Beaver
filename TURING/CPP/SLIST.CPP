//+1==========================================================================//
//  MODULE                      SLIST.CPP                                     //
//============================================================================//
//  FUNCTION    Implement CLASS SLIST for maintaining a sorted list               
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module SLIST.HPP for definition of the class SList  
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//


#include "SList.hpp"
#include "general.hpp"



//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            SList                           constructor   //
//-- SYNOPSIS ----------------------------------------------------------------//
                                        // SideEff: Instantiated SList object
SList::SList                            //
(   unsigned _uLSlots,                  //
    unsigned _uLElems,                  //
    char    *_rcLFile )                 //
{                                       //------------------------------------//

    assert(uLSlots <= 50 && uLElems < 5120);
    uLSlots = _uLSlots;
    uLElems = _uLElems;
    uLMin   = 1u;
    uLLast  = 0u;
    for (unsigned U = 0; U < uLSlots; U++) {
        rpcList[U] = new char[uLElems+1];
        assert(rpcList[U] != 0);
        memset(rpcList[U], '\0', uLElems);
    }
    if (_rcLFile != 0) vInit(_rcLFile);
}



//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            ~SList                           destructor   //
//-- SYNOPSIS ----------------------------------------------------------------//
                                        // SideEff: Free'd SList object
SList::~SList                           //
()                                      //
{                                       //------------------------------------//
    vFlush();
    for (unsigned U = 0; U < uLSlots; U++) {
        if (rpcList[U] != 0) {
            delete [] rpcList[U];
        }
    }
}



//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            vInit                                         //
//-- SYNOPSIS ----------------------------------------------------------------//
void
SList::vInit
( char *_rcLFile )
{
 
    (void) strncpy(rcLFile, _rcLFile, 12);
    if (rcLFile != 0 && (hLFile = fopen(rcLFile, "r")) != NULL) {
        while (fgets(rcLElem, uLElems, hLFile) != NULL) {
             vInsert(rcLElem, 0);
        }
        assert(fclose(hLFile) == 0);
    }
}



//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            vFlush                                        //
//-- SYNOPSIS ----------------------------------------------------------------//
void
SList::vFlush()
{
     if ((hLFile = fopen(rcLFile, "w+")) != NULL) {
          for (unsigned U = 0; U < uLSlots && *rpcList[U] != '\0'; U++) {
                fputs(rpcList[U], hLFile);
            }
          assert(fclose(hLFile) == 0);
     }
}


//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            vInsert                                       //
//-- SYNOPSIS ----------------------------------------------------------------//
void
SList::vInsert
( char   *_pcNew, unsigned fFlush)
{
     unsigned    uNCnt = 0;
     unsigned    uBCnt = 0;
     unsigned    U = 0, V = 0;
     int         I;


     // IF..: SList full and new Beaver less busy than minimum, return immediately
     sscanf(_pcNew+1, "%u", &uNCnt);
     if (uLLast == uLSlots-1 && uNCnt < uLMin) return;

     // ELSE: Advance U until found a BB less busy than <_pcNew>, or empty slot ('\0')
     for (  U = 0;
            U <= uLSlots-1 && *rpcList[U] != '\0' &&
            sscanf(rpcList[U]+1, "%u", &uBCnt) && (uNCnt < uBCnt);
            U++ ) {
                /* Advance to next slot */
     }

     // Overwrite equivalent or insert new (shift down) BB candidate
     if (*rpcList[U] == '\0' || uNCnt == uBCnt) {
          // If <_pcNew> equally busy as located BB (or 1.st empty slot), overwrite!
          strncpy(rpcList[U], _pcNew, uLElems-1);
     } else {
          // If <_pcNew> more busy than located BB, insert in SList!
          char *pcTmp = rpcList[uLSlots-1];
          for (V = uLSlots-1; V > U; V--) {rpcList[V] = rpcList[V-1];}  // Shift down
          rpcList[U] = pcTmp;                                           // Insert new
          strncpy(rpcList[U], _pcNew, uLElems-1);
          // If flag fFlush ON, write updated SList to disk file
          if (fFlush) vFlush();
     }

     // Set new min
     for (I = uLSlots-1; I >= 0; I--)
          if (*rpcList[I] != '\0') {
                uLLast = I;
                sscanf(rpcList[I]+1, "%u", &uLMin);
                D(DLev4, std::cout << "CURRENT MIN SLIST[" << uLLast << "]:=" << uLMin << std::endl);
                return;
          }
}



//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            vDelMin                                       //
//-- SYNOPSIS ----------------------------------------------------------------//
void
SList::vDelMin
( unsigned  uCount)
{
//   DESCRIPTION
//   Delete last uCount elements from this SLIST 
//-2
    while (uLLast > 3 && uCount--) {  
         *rpcList[uLLast--] = '\0';
         sscanf(rpcList[uLLast]+1, "%u", &uLMin);
    }        
}




//+2 MODULE : SLIST.CPP ------------------------------------------------------//
//   FUNCTION public            operator<<                                    //
//-- SYNOPSIS ----------------------------------------------------------------//
std::ostream&
operator<<                              // Function
(   std::ostream&   os,                 // Param 1:
    const SList&    SL )                //       2:
{                                       //------------------------------------//
     unsigned U;

     // DUMP content of result stack on stdout
     for (U = 0; U < SL.uLSlots && SL.rpcList[U] != 0 && *(SL.rpcList[U]) != '\0'; U++) {
        std::cout << U << '>' << SL.rpcList[U];
     }
     std::cout << std::endl;

     return os;

} // END function SList::operator<<



// END MODULE SLIST.CP                                                       //
//============================================================================//
