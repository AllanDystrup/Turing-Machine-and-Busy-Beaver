# Turing-Machine-and-Busy-Beaver
Programs in C++ and C# to simulate a Universal Turing Machine and run a Busy Beaver search on this.
```
  /// #region classdoc
  /// <summary>
  /// CLASS TURING, a type for simulation of a Universal Turing Machine (UTM).
  /// </summary>
  ///  ----------------------------------------------------------------------------
  /// <remarks>
  /// The structure of the UTM is defined as follows (by example): 
  /// PROGRAM
  /// --------------------
  /// bPStates  = 5   :   Turing machine states .... [(0)|1|2|3|4|5]
  /// bTSymbols = 2   :   Tape I/O symbol alfabet... [0|1]       
  /// bPCells = 5byte:    #byte in one TM transition defined as :
  ///						Turing Machine transition definition =
  ///                     [InState,InSymbol,OutState,OutSymbol,Move]
  ///
  /// stProgram           Sequence of TM transition definitions 
  ///						(bPCells) describing the current TM program
  /// sIProgram           Index = the current TM transition def.
  /// 
  ///                     +----------------------------------------+
  ///                     |Program ~ Seq. of Transition Definitions|
  ///                     +----------------------------------------+
  ///                     |    Input      |           Output       |
  ///                     |State  Symbol  |   State   Symbol  Move*|
  ///                     |										 |
  ///                     |                                bPCells |
  ///                     |                                   |    |
  ///                     +---------------+-------------------5----+
  /// stProgram----->pos 0|   1       0   |   2       1       2    |
  ///                     |___1_______1___|___3_______0_______1____2-bTSymbols
  ///                     |   2       0   |   3       1       2    |
  ///                     |___2_______1___|___4_______1_______2____|
  /// sIProgram, eg:---->20   3       0   |   1       1       1    |
  ///						|___3_______1___|___2_______0_______2____|
  ///                     |   4       0   |   5       0       2    |
  ///                     |___4_______1___|__(0)______1______(0)___|
  ///                     |   5       0   |   3       1       1    |
  ///            bPStates-5   5       1   |   1       1       2    |
  ///                     +---------------+------------------50----+
  ///                                                         ^
  ///                                                         | *) 0=Halt
  ///                                                         |    1=Left
  ///                                                         |    2=Right
  ///                                                         |
  ///								 (bPStates * bTSymbols * bPCells)-1
  ///							     (    5     *    2      *   5   )-1
  ///						                                   49
  /// DATA
  /// --------------------
  /// sTCells				#Byte total on TM I/O Tape
  /// sbTape              Sequence of cells with I/O symbols
  ///                     describing the current TM tape
  /// sITape              Index = the current TM tape position
  ///                      
  ///                     
  ///             +-+-+-+--/\/------------+-+----------------/\/--+-+
  /// sbTape----->|0|1|1|  ...            |0|     ...             |1|
  ///             +-+-+-+--/\/------------+-+----------------/\/--+-+
  ///              ^                       ^                       ^
  ///              |                       |                       |
  ///         pos -sTCells/2            sITape                 +sTCells/2
  ///      
  ///						                                  
  /// HISTORY
  /// --------------------
  /// sbTrans;	// Trace of state transitions (cyclic buffer)
  ///	sITrans;	// Index to current position in sbTrans
  /// lTrans;		// #SstateTransitions total
  ///                     
  ///             +-+-+-+--/\/----+-+------------------------/\/--+-+
  /// sbTrans---->| | | |  ...    | |             ...             | | WRAP
  ///             +-+-+-+--/\/----+-+------------------------/\/--+-+
  ///                              ^                               ^
  ///                              |                               |
  ///                           sITrans                        sHCells
  ///            
  /// ----------------------------------------------------------------------------
  /// SYSTEM      IBM PC >= i386 cpu
  ///
  /// SEE ALSO    Module TuringConUI.cs and TuringWinUi.cs for test driver applications.
  ///
  /// PROGRAMMER  Allan Dystrup, June 2002; Feb.2026
  ///
  /// VERSION     0.2		Allan Dystrup: Alpha/Test version. 
  ///						Using VS 2002 (v.7.0) and C# v.1
  ///						
  ///				0.3		Allan Dystrup: Port of Turing 0.2 
  ///						to VS 2026 (v.18.3) with C# v.14 
  ///
  /// REFERENCES  
  /// 	    	Turing, A.M. (1937). "On Computable Numbers, with an Application 
  ///					to the Entscheidungsproblem". Proceedings of the London 
  ///					Mathematical Society. 2. 42 (1): 230–65.
  ///
  ///				Rado, T: "On Non-Computable Functions",
  ///					 The Bell System Technical Journal,
  ///					( Volume: 41, Issue: 3, pp.877 - 884, May 1962)
  ///
	///  USAGE 		Test drivers,
	///					Console UI:		./bin/TuringConUI.exe
	///					Windows UI: 	./bin/TuringWinUI.exe
	///					Application:	./bib/Beaver.exe
	///
 	///  DOC			Incorporated in the code comments. 
	///
	///	 BUGS:
	///		Design:	The main design principle has been comprehensibility and fast 
	///             execution, - NOT maximum reusability (eg. Class Turing could be
	///             decomposed into Class TMProgram, Class TMTape etc.
	///  	Code:	This is an alpha/pre-release, and as such it has not been 
	///				developed for robustness or strict code standard compliance.
	///
	///  COPYRIGHT   none!
  /// </remarks>
  ///  #endregion

	#region classdoc
	/// <summary>
	/// CLASS BEAVER, a type for simulation of a "Busy Beaver" TM (BBTM);
	/// (A wrapper of class Turing for: BB State, Pesistence, TM generation & Dump)
	/// </summary>
  ///  ----------------------------------------------------------------------------
	/// <remarks>
	///  The purpose of the "Busy Beaver" simulation is to find n-state "Busy" TM's; 
	///  An n-state "Beasy Beaver" Turing Machine: BBTM(n) is defined by:
	///     - n+1 Program states: [(0)|1|...|n], where 0=HALT
	///     - 2   Tape symbols..: [0|1]
	///     - 2+1 Moves.........: [L|R|(H)], where L=LEFT, R=RIGHT; (H=HALT)
	///     - takes as input an unlimited Tape filled with 0's
	///     - writes at least as many 1's on the Tape as any other n-BTM
	///     - eventually HALTs !!
	///
	///  Below are listed BBTM(n) for n = [1...4] plus a conjectured BBTM(5) :
	///       +------------ PROGRAM -------------------+
	///       |    Input      |           Output       |
	///   +---+-------+-------|-------+--------+-------+-----+---------+-------------------+
	///   | n | State | Symbol| State | Symbol | Move  |#1's | #Trans  |       #BTM(n)     | 
	///   +---+---------------+------------------------+-----+---------+-------------------+
	///   
	//		  stProgram = "1001211012";
	///       +---------------+------------------------+
	///    1  |   1       0   |  (0)      1       H    | 001   001        64
	///       |___1_______1___|__(0)______1_______H____| 
	///       
	///       stProgram = "10211112122011221012";
	///       +---------------+------------------------+
	///    2  |   1       0   |   2       1       L    | 004   006		  20.736
	///       |___1_______1___|___2_______1_______R____| 
	///       |   2       0   |   1       1       R    |
	///       |___2_______1___|___0_______1_______H____|
	///       
	///	      stProgram = "102121131120111212123021131012";
	///	      +---------------+------------------------+
	///    3  |   1       0   |   2       1       R    | 006   013		  268.435.456  
	///       |___1_______1___|___3_______1_______L____|    
	///       |   2       0   |   1       1       L    |
	///       |___2_______1___|___2_______1_______R____|
	///       |   3       0   |   2       1       L    |
	///       |___3_______1___|__(0)______1______(H)___|
	///	   
	///	      stProgram = "1021211211201112130130011314114041241102";
	///       +---------------+------------------------+
	///	   4  |   1       0   |   2       1       2    | 012  107         25.600.000.000
	///		  |___1_______1___|___2_______1_______1____| 
	///		  |   2       0   |   1       1       1    |
	///		  |___2_______1___|___3_______0______(1)___|
	///		  |   3       0   |   0       1       2    |
	///		  |___3_______1___|___4_______1_______1____|
	///		  |   4       0   |   4       1       2    |
	///		  |___4_______1___|___1_______0______(2)___| 
	///                    
	///        stProgram = "10212113012031221412301113120240502410125031151112";
	///       +---------------+------------------------+
	///    5? |   1       0   |   2       1       1    | 4.098 11.798.826  63.403.380.965.374
	///       |___1_______1___|___1_______1_______1____| 
	///       |   2       0   |   3       1       2    |
	///       |___2_______1___|___2_______1_______2____|
	///       |   3       0   |   1       1       1    |
	///       |___3_______1___|___4_______1_______2____|
	///       |   4       0   |   1       1       1    |
	///       |___4_______1___|___5_______1_______2____|
	///       |   5       0   |   0       1      (2)   |
	///       |___5_______1___|___3_______0_______2____| 
	///
	///       *) Located by this program (using random probing w. genetic mutation) 
	///               
	/// There are two obvious ways of attacking the BBTM(n) problem :
	///    [1] Find an analytical method of identifying BBTM(n) 
	///    [2] Conduct an exhaustive search of all BTM(n)'s
	///
	/// Both these approaches however have major flaws :
	///   [1] Let BBTM(n) = max #1's producible by a BTM(n) before HALTing; Then :
	///       BBTM(n)= 1,4,6,12,4098? for n=[1,2,3,4,5]
	///       BBTM(n) is however NONCOMPUTABLE, since it is undecidable
	///       how long to wait for a BTM(n) to halt (the "Halting Problem").
	///   [2] Let n= #Program-states, a= #Tape-symbols(2), m= #Moves(2: L|R); 
	///       We can then calculate the #different n-BTM as follows :
	///          #transitions in BTM Program/Graph : T = (n*a) = 2n
	///          #unique graphs....: (n+1) ** T = (n+1) ** (n*2)
	///          each decorated by.: (a**T) * (m**T) Output/Move comb.
	///                             = 2**T * 2**T = 2**2T = 2 ** 4n 
	///          which gives the total # different n-state BTM's. :
	///          ((n+1)**2n) * 2**4n   =   ((n+1) * 4) ** 2n 
	///          #BTM(n) for n=[1,2,3,4,5] are listed in the diagram above.
	///          The combinatorial complexity of BTM(n) graphs increases 
	///          exponentially as a function of n, which makes an exhaustive
	///          search INFEASABLE. (and furthermore the Halting Problem
	///          also crops up in this context)
	///
	///          So by both theoretical (graph analysis) & practical (simulation)
	///          methods the BBTM(n) problem is in general UNSOLVABLE. Why then
	///          waste your time on the problen? The BBTM(n) problem is a simple
	///          canonical representation of a basic problem in Computer Science,
	///          namely the computational undecidability of the complexity class
	///          NP (NonPolynomial systems). Many problems in NP are of great 
	///          theoretical and practical interest, ranging from simple areas
	///          as graph analysis (TSP) to artificial intelligence.
	///
	///          I believe that one way to deal with NP-problems is by imposing 
	///          a conceptional hierachy on the problem solving method by using :
	///             A: a basic "dumb" algorithm to crank away on problem instances
	///             B: higher level "oracles" for :
	///                - deciding the feasability of specific problem instances
	///                  based on heuristics of program structure, ...
	///                - evaluate the simulation with respect to anomalies such 
	///                  as nontermination, resource exhaustion ...
	///             C: a top level control logic to :
	///                - "spawn" instances of A, using program mutation (in the
	///                  sence of genetic programming) related to some specific
	///                  fitness criteria, - eg. #1's produced by BTM(n)
	///                - for each A, call on B to supervise the problem solving.
	///
	///           The BBTM(n) problem seems ideal to try out the validity of this
	///           concept.
	/// ----------------------------------------------------------------------------
	///  SYSTEM      IBM PC >= i386 cpu
	///  SEE ALSO    Class Turing for implementation of the basic TM
	///  PROGRAMMER  Allan Dystrup
	///  VERSION     0.1
	///  REFERENCES  
	///  USAGE       
	///  DOC
	///  BUGS
	///  COPYRIGHT   
	/// ----------------------------------------------------------------------------
	/// </remarks>
	#endregion
```
  
    
