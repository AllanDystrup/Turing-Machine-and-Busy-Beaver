// MAKE:
// VS.NET: RClick Turing Project -> Add Reference: Projects, Trace.dll
//

#region defines
#define DEBUG			// Equiv to /d:DEBUG on cmd.line
#define TRACE			// Equiv to /d:TRACE on cmd.line
#endregion

#region namespaces
// Core 
using DBG;
using System;
// Debug
using System.Diagnostics;
using System.IO;
// Data
using System.Text;
#endregion


namespace Turing
{
    #region classdoc
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
    #endregion


    public class CTuring
    {

        // TURING MEMBERS
        // ----------------------------------------------------------------------------
        #region Tmembers

        // The Turing Machine 
        protected byte bID;                 // ID unique for this TM	[0-255], eg 1,2,...

        // The Program (state trans def.)
        static public byte bPCells = 5;     // #Byte in one Trans.Def.	[0-255], const: 5
        public string stProgram;        // TM Program: (bPStates*bTSymbols) Trans.Def.s
        protected short sIProgram;      // Index to current Trans.Def. in stProgram
        protected byte bPStates;        // #ProgramStates	[0=HALT,1-255]

        // The Tape (input/output)
        public StringBuilder sbTape;        // TM I/O-Tape: sTCells byte (from bTSymbols)
        protected short sITape;     // Index to current byte.pos. in sbTape 
        protected byte bTSymbols;       // #TapeSymbols	[0-255], eg 2 [0,1]
        public short sTCells;       // #Byte on tape   ]3,32.000]

        // The History (transition log)
        protected StringBuilder sbTrans;    // Trace of StateTrans. (cyclic buffer)
        protected short sITrans;        // Index to current Trans.Log position in sbTrans
        protected short sHCells;        // #Byte in sbTrans   ]1,32.000],cyclic buf
        public int iTrans;          // #State.Trans current ]1,2*10^9]
        protected int iPLoop;           // #State.Trans max (cons. LoopOmega) ]1,2*10^9]

        // Debug
        public CD D;                // Debug class for trace
        #endregion

        // TURING LIFECYCLE
        //-----------------------------------------------------------------------------
        #region Tlife
        /// <summary>
        /// Full CTOR for the TURING type instantiating the UTM: program, tape and history
        /// </summary>
        /// <remarks>
        /// INITIALIZES
        ///	1: The Turing Machine
        ///	2: The Program (State Machine Definition)
        ///	3: The Tape    (Input/Output Record)
        ///	4: The History (Transition Log)
        ///	</remarks>
        ///	<param name="bID">1: [in] TM Unique identifier</param>
        /// <param name="bPStates">2: [in] TM #Program States</param>
        /// <param name="stProgram">3: [in] TM Program (trans seq.)</param>
        /// <param name="sTCells">4: [in] TM #Tape bytes</param>
        /// <param name="cSet">5: [in] TM Tape initial filler</param>
        /// <param name="bTSymbols">6: [in] TM #Tape Symbols</param>
        /// <param name="stTape">7: [in] TM Tape content (input)</param>
        /// <param name="sHCells">8: [in] TM Hist.Trace (cyclic)</param>
        /// <param name="iPLoop">9: [in] TM #Trans ~ LOOP OMEGA</param>
        public CTuring(byte bID, byte bPStates, string stProgram,           // PGM
            short sTCells, string cSet, byte bTSymbols, string stTape,      // TAPE
            short sHCells, int iPLoop, int iDBG)
        {                               // HIST

            // 1: The Turing Machine
            this.bID = bID;                                         // Unique ID for this TM object

            // 2: The Program (State Machine Definition)
            Debug.Assert(bPStates * bTSymbols * bPCells > 0, "CTuring() [2:TM-Pgm] ERR:PgmSize<=0");
            this.bPStates = bPStates;                                   // Number of states in TM 
            this.stProgram = stProgram;                             // Initialize pgm. storage, cf. param5
            sIProgram = 0;                                                  // Set Pgm.ptr on 1.transition

            // 3: The Tape (Input/Output Record)
            this.bTSymbols = bTSymbols;                             // Number of symbols on I/O Tape
            this.sTCells = (short)(sTCells);                        // Length (in chars) of I/O Tape
            sITape = (short)(sTCells / 2);                              // Set Tape.ptr on mid tape
            Debug.Assert(sITape > stTape.Length, "CTuring() [3:I/O-Tape] ERR:Input>Tape");
            this.sbTape = new StringBuilder(sTCells + 2);           // Allocate storage for the TM Tape
            sbTape.Insert(0, cSet, sTCells + 1);                        // Clear Tape storage, cf. param7
            sbTape.Remove(sITape, stTape.Length);                   // Init. Tape middle to input,
            sbTape.Insert(sITape, stTape, 1);                           //       cf. param6 (cut/paste)

            // 4: The History (Transition Log)
            Debug.Assert(sHCells >= 3, "CTuring() [4:Hist-Log] ERR:HistSize<3");
            this.sHCells = sHCells;                                     // Cyclic buffer tracing state trans.
            this.iPLoop = iPLoop;                                       // Number of Trans. considered LoopOmega!
            sbTrans = new StringBuilder(sHCells);                   // Allocate storage for the TM history
            sbTrans.Insert(0, "@", sHCells);                            // Clear Tape storage, cf. param7
            sITrans = 0;                                                    // Set "history pointer" on 1.position
            iTrans = 0;                                                 // Clear transition counter

            // Dump initial state of TM on stdout
            D = new CD((byte)iDBG);                                 // Create Debug class
            D.vOut(CD.bL1, "\nTM Constructing: {0}", this.bID);
            D.vOut(CD.bL3, "\nTM Constructing: {0} {1}", this.bID, this);
        }

        // Special CTOR & RESET for BBTM (TM for "Busy Beaver" simulation))
        public CTuring(byte bID, string stProgram, int iDBG)
            : this(bID, 5, stProgram, 30000, "0", 2, "", 1024, 30000000, iDBG)
        {
        }

        public void vReset(string stProgram)
        {
            this.stProgram = stProgram;                                     // ReInit Pgm. 
            bPStates = (byte)(stProgram.Length / (bPCells * bTSymbols));    // Reset States 
            sIProgram = 0;                                                          // ReSet Pgm.Ptr  (start)
            sbTape.Remove(0, sbTape.Length);
            sbTape.Insert(0, "0", sTCells + 1);                             // Clear I/O-Tape (all 0)
            sITape = (short)(sTCells / 2);                                      // ReSet TapePtr  (mid)
            sbTrans.Remove(0, sbTrans.Length);
            sbTrans.Insert(0, "@", sHCells);                                    // Clear History  (all @) 
            sITrans = 0;                                                            // ReSet HistPtr  (start)
            iTrans = 0;                                                         // Clear TransCtr.
            D.vOut(CD.bL1, "TM Resetting: {0}", this.bID);
            D.vOut(CD.bL3, "TM Resetting: {0} {1}", this.bID, this);
        }


        // TURING DESTRUCTOR
        // ----------------------------------------------------------------------------
        // GC frees resources instantiated in CTOR: stProgram, sbTape, sbTrans		
        ~CTuring()
        {
            D.vOut(CD.bL1, "TM Destructing: {0}", this.bID);
        }
        #endregion

        // TURING TRANSITION
        // ----------------------------------------------------------------------------
        #region Ttrans
        public class CTuringException : ApplicationException
        {
            public CTuringException(string sReason) :
                base("Unrecoverable error in Turing: " + sReason)
            { }
        }

        /// <summary>
        /// Perform ONE TM Transition: cf (CurrentState,Input)-> (NewState,Output,Move)
        /// </summary>
        /// <remarks>
        /// TRANSITION
        ///	1.1: FIND TRANS in program	(cf CurrentState:cPSta, Input:cTSym)
        ///	1.2: RECORD TRANS history	(cf cPSta and cTSym)
        ///	2:   WRITE OUTPUT and MOVE	(cf Output, Move)
        ///	3:   MAKE PGM.TRANS or HALT (cf NewState)
        ///	</remarks>
        ///	<exception cref="CTuringException">Loop Omega</exception>
        ///	<exception cref="CTuringException">Tape Underflow</exception>
        ///	<exception cref="CTuringException">Tape Overflow</exception>
        ///	<exception cref="ArgumentException">ERROR in TM Program</exception>
        ///
        public bool bTrans()
        {
            // 1.1: FIND TRANS IN TM PROGRAM (cf. current PgmState:cPSta & TapeSymbol:cTSym) 
            char cPMov;                                                         // TM TAPE move
            char cPSta = stProgram[sIProgram];                          // Current TM PGM state 
            char cTSym = sbTape[sITape];                                // Current TM INP symbol
            int End = (bPStates * bTSymbols * bPCells - 1);             //
                                                                        //
            while (sIProgram < End &&                   // LOOP in TM PGM:
                (cPSta == stProgram[sIProgram]) &&                      // WHILE current  TM State  
                (cTSym != stProgram[sIProgram + 1]))
            {                   //   AND !current TM Input
                sIProgram += bPCells;                                   // Bump PC to next Trans.Def
            }

            // 1.2: RECORD TRANS IN HISTORY (log: cPSta & cTSym)
            if (checked(++iTrans) > iPLoop) throw new CTuringException("Loop Omega");
            sbTrans[sITrans++] = cPSta;                                 // Record Trans. Pgm.State
            sbTrans[sITrans++] = cTSym;                                 //    "   Trans. TapeSymbol
            sbTrans[sITrans++] = '-';                                   // (Separator)
            if (sITrans + 3 > sHCells) sITrans = 0;                     // Wrap trace (cyclic buffer)
            sbTrans[sITrans] = '>';                                     // (Head of circ.buf)

            // 2: WRITE & MOVE I/O TAPE (cf Trans.Def.: OutSym, Move)
            cTSym = stProgram[sIProgram + 3];                               // Current TM OUT symbol
            cPMov = stProgram[sIProgram + 4];                               // Current TM MOV 
            sbTape[sITape] = cTSym;                                     // WRITE OUT to Tape
            switch (cPMov)
            {                                           // MOVE Tape [0=S|1=L|2=R]
                case '0':                                               // 1=STAY
                    break;                                              //
                case '1':
                    sITape--;                                   // 1=LEFT
                    if (sITape < 0) throw new CTuringException("Tape Underflow");
                    break;
                case '2':
                    sITape++;                                   // 2=RIGHT
                    if (sITape >= sTCells + 1) throw new CTuringException("Tape Overflow");
                    break;
                default:                                                // Else:ERROR
                    throw new ArgumentException("Move must be [0/S | 1/L | 2/R]", "cPMov");
            }

            // 3: MAKE PGM.TRANSITION or HALT (cf Trans.Def.: OutSta)
            cPSta = stProgram[sIProgram + 2];                               // Next TM PGM state
            if (cPSta == '0') return false;                             // HALT'ed (state 0)
            sIProgram = (short)((cPSta - '1') * bTSymbols * bPCells);           // Set "PC" on next TRANS
            return true;                                                // Still running
        }


        // TURING RUN
        // ----------------------------------------------------------------------------
        /// <summary>
        /// RUN TM Program on I/O-Tape with Trans.Trace until HALT or ERROR
        /// </summary>

        // Optional LOG event
        public delegate void RunHandler(string msg);                                // Run callbacks
        public event RunHandler eLog;                                                   // 1: Log event

        public void vRun(bool bStep, int iLog)
        {
            while (bTrans())
            {                                                           // Loop while Trans
                if (eLog != null && (iTrans % iLog == 0)) eLog(this.ToString());// LOG Source
                if (bStep) { Console.ReadLine(); }                                  // "GO" next Trans
                D.vOut(0x1, "--TRANS:{0}\n{1}", iTrans, this.TracePgm(false));
                D.vOut(0x8, "--TRANS:{0}{1}", iTrans, this);
            }
            D.vOut(CD.bL1, "--HALT:{0}{1}", iTrans, this.TracePgm(false));
            D.vOut(CD.bL3, "--HALT:{0}{1}", iTrans, this);
        }

        // LOG Event Sink (optional, - or write your own)
        public void vOnLog(string s)
        {
            try
            {
                FileInfo fi = new FileInfo("TMLog.txt");
                StreamWriter sw = fi.AppendText();
                sw.WriteLine(s); sw.Flush(); sw.Close();
            }
            catch (IOException e)
            {
                Console.WriteLine(e);
            }
        }
        #endregion

        // TURING DUMP 
        // ----------------------------------------------------------------------------
        #region Tdump
        /// <summary> DUMP the TM Program (State Machine) </summary>
        public string TracePgm(bool bHeader)
        {
            StringBuilder sbResult = new StringBuilder(500);
            if (bHeader)
            {
                sbResult.Append("PROGRAM:#States: " + this.bPStates + "\t#Symbols: " + this.bTSymbols + "\n");
                sbResult.Append("\t[InState,InSymbol] [OutState,OutSymbol] [Move: 1=LEFT,2=RIGHT]\n");
            }
            for (int iStart = 0, i = 0; i < bPStates; i++)
            {
                for (int j = 0; j < bTSymbols; j++, iStart += bPCells)
                {
                    sbResult.Append(sIProgram == iStart ? "\t>" : "\t ");
                    sbResult.Append(stProgram.Substring(iStart, 5) + " ");
                }
                sbResult.Append("\n");
            }
            return sbResult.ToString();
        }

        /// <summary> DUMP the TM Tape (I/O trace) </summary>
        public string TraceTape(int iLine, bool bCursor)
        {
            StringBuilder sbResult = new StringBuilder(500);
            string stTape = sbTape.ToString();
            int mid = (int)(sTCells / 2);
            for (int i = 0; i < sTCells; i += iLine)
            {
                sbResult.Append(i - mid + ":\t");
                int j = (i + iLine > sTCells ? sTCells - i : iLine);
                if (bCursor && (i <= sITape && sITape <= i + j))
                {
                    sbResult.Append(stTape.Substring(i, sITape - i) + ">");
                    sbResult.Append(stTape.Substring(sITape, i + j - sITape) + "\n");
                }
                else
                    sbResult.Append(stTape.Substring(i, j) + "\n");
            }
            return sbResult.ToString();
        }

        /// <summary> DUMP the TM History (Transition log in cyclic buffer) </summary>
        public string TraceHist(int iLine)
        {
            StringBuilder sbResult = new StringBuilder(500);
            string sTrans = sbTrans.ToString();
            for (int i = 0; i < sHCells; i += iLine)
            {
                sbResult.Append(i + ":\t");
                int j = (i + iLine > sHCells ? sHCells - i : iLine);
                sbResult.Append(sTrans.Substring(i, j) + "\n");
            }
            return sbResult.ToString();
        }

        /// <summary> DUMP the whole TM : ID, Program, I/O-Tape and History </summary>
        public override string ToString()
        {
            int iLine = 80;
            StringBuilder sbResult = new StringBuilder(500);

            // The TM ID
            sbResult.Append('\n'); sbResult.Append('-', iLine);
            sbResult.Append("\nTMID:" + this.bID + "\n");

            // The TM: Program, I/O-Tape & History
            sbResult.Append(TracePgm(true));

            sbResult.Append("\nTAPE..: ");
            sbResult.Append("#Cells: " + sTCells + ", TapePOS>" + (sITape - (int)(sTCells / 2)) + "\n");
            sbResult.Append(TraceTape(iLine, true));

            sbResult.Append("\nTRANS.: ");
            sbResult.Append("#Moves: " + iTrans + "\n");
            sbResult.Append(TraceHist(iLine));

            return sbResult.ToString();
        }
        #endregion
    }


    public class CTuringUnit
    {

        // UNIT MEMBERS & CTOR
        // ----------------------------------------------------------------------------
        #region members
        CTuring TM;
        string stProgram;

        public CTuringUnit()
        {
            stProgram = "1X2b21b0E22X1b22b0O2";
            // ===========================================
            //  Odd/Even (O/E) TM : Program/Trans.Sequence
            //  +----------------------------------------+
            //  |    INPUT      |           OUTPUT       |
            //  |State  Symbol  |   State   Symbol  Move*|
            //  +---------------+------------------------+
            //  |   1       X   |   2       b       2    |
            //  |___1_______b___|___0_______E______(2)___|
            //  |   2       X   |   1       b       2    |
            //  |___2_______b___|___0_______O______(2)___|
        }
        #endregion members

        // UNIT TEST
        // ----------------------------------------------------------------------------
        #region Ttest
        public void vTest()
        {
            Console.WriteLine("\n\nTESTCASE1:	INP-ODD: TAPE[bbb>Xbb]  EXPECT:HALT[bbbbO>b] (odd)");
            TM = new CTuring(1, 2, stProgram, 6, "b", 2, "X", 80, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);                   // Attach Log Hdlr.
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE2:	INP-EVEN:[bbb>XXbb]   EXPECT:HALT[bbbbbEb>] (even)");
            TM = new CTuring(2, 2, stProgram, 7, "b", 2, "XX", 80, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE3:	INP-EMPTY:[bb>bb]   EXPECT:HALT[bbEb] (even)");
            TM = new CTuring(3, 2, stProgram, 4, "b", 2, "", 80, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE4:	TAPE-LIM: TAPE[bb>XXb]  EXPECT:HALT[bbbbE]");
            TM = new CTuring(4, 2, stProgram, 5, "b", 2, "XX", 80, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            catch (Exception e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE5:	TAPE-OVFL: TAPE[bb>XX]  EXPECT:EX/TapeOverflow[bbbb]");
            TM = new CTuring(5, 2, stProgram, 4, "b", 2, "XX", 80, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            catch (Exception e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE6:	HIST-LIM:[@@@@@@@@@@@@]  EXPECT:HALT[1X-2X-1b->@@]");
            TM = new CTuring(6, 2, stProgram, 6, "b", 2, "XX", 12, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE7:	HIST-WRAP:[@@@@@@@@@@@]  EXPECT:HALT[>X-2X-1b-@@])");
            TM = new CTuring(7, 2, stProgram, 6, "b", 2, "XX", 11, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE8:	HIST-MIN:[@@@]   EXPECT:HALT[>b-]");
            TM = new CTuring(8, 2, stProgram, 6, "b", 2, "XX", 3, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            TM = null;

            Console.WriteLine("\n\nTESTCASE9:	HIST-ERR:[@@]   EXPECT:EX");
            TM = new CTuring(9, 2, stProgram, 6, "b", 2, "XX", 2, 50, 0xE);
            TM.eLog += new CTuring.RunHandler(TM.vOnLog);
            try { TM.vRun(false, 1); }
            catch (CTuring.CTuringException e) { Console.WriteLine(e); }
            catch (Exception e) { Console.WriteLine(e); }
            TM = null;
        }
        #endregion test
    }

}
