//+1==========================================================================//
//  MODULE                      XASSERT.HPP                                   //
//============================================================================//
//  FUNCTION    Implementation of quantifiers in assertions for C++         
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module RANDOM.HPP
//  PROGRAMMER  Harald M. Mueller, mueller@garwein.hai.siemens.co.at
//              Allan Dystrup: restructured, commented
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.        "Powerful Assertions for C++ Without Language Extensions".
//              Harald M. Mueller, c/C++ Users Journal, Oct.1994
//  BUGS        ?
//  COPYRIGHT   none!
//-1==========================================================================//

#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <iostream>



static int asstInvert, asstResult, asstEval, asstShortCut;

static struct AssertCt {
    // empty
} asstCt;

inline AssertCt& anAssertCt() {
    asstInvert = 0;
    asstEval = 1;
    return asstCt;
}

inline int asstDoEval(int& asstShortCut) {
    int eval = asstEval;
    asstShortCut = ! asstEval && asstResult;
    asstEval = 0;
    return eval;
}

inline const AssertCt& operator!(const AssertCt& a) {
    asstInvert = !asstInvert;
    return a;
}

inline int operator&&(int left, const AssertCt&) {
    asstEval = left;
    return left;
}

inline int operator||(int left, const AssertCt&) {
    asstEval = !left;
    return left;
}

static std::ostream& assertout(std::ostream& os, unsigned line, char* file)
{
    return os << "Assertion in line " << line << ", file "
              << file << " failed for ";
}

// ----------------------------------------------------------------------

#define asstLoopTest(asstFor,asstAll,asstCond)                  \
        anAssertCt();                                           \
        { int asstShortCut;                                     \
          if (asstDoEval(asstShortCut)) {                       \
              int asstInvert = ::asstInvert;                    \
              asstResult = asstAll;                             \
              for asstFor {                                     \
                  asstResult = 0 || asstCond;                   \
                  if (asstResult != asstAll) break;             \
              }                                                 \
              if (asstInvert) asstResult = !asstResult;         \
          }                                                     \
          ::asstShortCut = asstShortCut;                        \
        }                                                       \
        asstResult = ::asstShortCut ? asstResult : asstResult

#define assertp(asstCond,asstOutput)                            \
        asstResult = 0 || asstCond, asstResult ||               \
        (assertout(ASSERTSTREAM,__LINE__,(char*)__FILE__) << asstOutput\
        << std::endl, ASSERTACTION, 0)

//#define assert(asstCond) assertp(asstCond,"condition \`" #asstCond "\'")
#define assert(asstCond) assertp(asstCond,"condition " #asstCond)

#define forall(asstFor,asstCond)    asstLoopTest(asstFor,1,asstCond)
#define exists(asstFor,asstCond)    asstLoopTest(asstFor,0,asstCond)

#define old(asstname)                   old_##asstname
#define saveold(assttype,asstname)      assttype old_##asstname = asstname

#define ASSERTSTREAM    std::cerr
#define ASSERTACTION    exit(125)


#endif //_ASSERT_H_
// END MODULE XASSERT.HPP                                                     //
//============================================================================//
