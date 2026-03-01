// MAKE:
// VS.NET: RClick Beaver Project -> Add Reference: Projects, Turing.dll
//


#region defines
// I/O & Streaming
#define SOAP			
//#define BINARY
// Debug
#define DEBUG			// Equiv to /d:DEBUG on cmd.line
#define TRACE			// Equiv to /d:TRACE on cmd.line
#endregion defines


#region namespaces
// Core 
using System;
using System.Threading;
//using System.Reflection;	// assembly attributes

// I/O & Streaming
using System.IO;
#if BINARY
using System.Runtime.Serialization.Formatters.Binary;
#elif SOAP
using System.Runtime.Serialization.Formatters.Soap;
#endif

// Data
using System.Collections;
using System.Text;
using System.Text.RegularExpressions;

// Debug
using System.Diagnostics;
using DBG;

// Custom Namespaces
using Turing;		// Project properties -> Output Type = Class Lib. (dll)

#endregion namespaces

#region assembly	
//[assembly:AssemblyKeyFile(@".\beaverkey.snk")]
//[assembly:AssemblyVersion("1.0.0.0")]	// // in AssemblyInfo.cs
#endregion assembly


namespace Beaver {

	#region beaver
	#region classdoc
	/// <summary>
	/// CLASS BEAVER, a type for simulation of a "Busy Beaver" TM (BBTM);
	/// (A wrapper of class Turing for: BB State, Pesistence, TM generation & Dump)
	/// </summary>
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

	[Serializable]
	public class CBeaver {
		
		// BEAVER MEMBERS
		// ----------------------------------------------------------------------------
		#region Bmembers
		public int		iOnes;
		public int		iTrans;
		public string	stProgram;
		// CBeaver must be serializable but we don't want to stream out the TM, so
		// instead of making CBeaver a Turing specialization, we embed it in CBeaver.
		[NonSerialized] public byte bPStates;
		[NonSerialized] public CTuring TM;
		[NonSerialized] public CD	D;					
		#endregion Bmembers
		
		// BEAVER LIFECYCLE
		//-----------------------------------------------------------------------------
		#region Blife
		
		// CTOR 1
		public CBeaver(byte bID, byte bPStates, string stProgram,			// PGM
			short sTCells, string cSet, byte bTSymbols, string stTape,		// TAPE
			short sHCells, int iPLoop,													// HIST
			int iDBG) {																		// Trace
			this.iOnes = this.iTrans = 0;
			this.stProgram = stProgram; 
			this.bPStates = bPStates;
			TM = new CTuring( bID, bPStates, stProgram, sTCells, cSet, bTSymbols, stTape,
				sHCells, iPLoop, CD.bL0 );
			this.D = new CD((byte)iDBG);											// Create DBGclass
		}

		// CTOR 2
		public CBeaver(byte bID, string stBeaver) { 
			this.iOnes = this.iTrans = 0;
			this.stProgram = stBeaver;
			bPStates = (byte)(stProgram.Length/(CTuring.bPCells*2));
			TM = new CTuring(bID, stBeaver, CD.bL0); 
			this.D = new CD((byte)0x7);											// Create Debug class
		} 

		// RESET Beaver & associated TM
		public void vReset(string stProgram) {
			this.iOnes = this.iTrans = 0;												// Reset CBeaver
			this.stProgram = stProgram;												
			this.bPStates = (byte)(stProgram.Length/(CTuring.bPCells*2));
			this.TM.vReset(stProgram);													// Reste TM
		}
	
		//  CREATE a (pseudo)random - but syntactically valid - BTM program
		public void vCreate(ref StringBuilder sbProgram) {
			for (int iStart=0, i=0; i < bPStates; iStart+=CTuring.bPCells*2, i++) {
				// IN State & Symbols
				sbProgram[iStart] = 
				sbProgram[iStart+CTuring.bPCells] = Convert.ToChar(49+i);	// Current State
				sbProgram[iStart+1]					  = '0';							// IN symbol [0]
				sbProgram[iStart+CTuring.bPCells+1] = '1';						// IN symbol [1]
				D.vOut(CD.bL1, "vCreate:\t{0}", stDump(sbProgram.ToString(),0,-1));
				// OUT Transition
				vMutate(ref sbProgram, iStart);
				vMutate(ref sbProgram, iStart+CTuring.bPCells);
			}
			// CHECK - Parch the generated program for structural deficiencies
			vPatch(ref sbProgram);
			D.vOut(CD.bL1, "vCreate:\t{0}", stDump(sbProgram.ToString(),0,-1));
		}                   

		//  MUTATE one BTM transition : OUT-State, OUT-Symbol and Tape-MOVE
		public void vMutate(ref StringBuilder sbProgram, int iPos) {
			// OUT  State, Symbol & Move
			Random r = new Random();
			D.vOut(CD.bL4, "vMutate:\t\t{0}", stDump(sbProgram.ToString(),iPos,-1));
			sbProgram[iPos+2] = Convert.ToChar(48+r.Next(0,bPStates+1));	// GoTo State	[0...iStates]
			sbProgram[iPos+3] = Convert.ToChar(48+r.Next(0,2));				// OUT Symbol	[0|1]
			sbProgram[iPos+4] = Convert.ToChar(48+r.Next(1,3));				// Move			[1/L|2/R]
			D.vOut(CD.bL1, "vMutate:\t{0}", stDump(sbProgram.ToString(),iPos,-1));
		} 
		public void vMutate(ref string sProgram, int iPos) {
			StringBuilder sb = new StringBuilder(sProgram);
			vMutate(ref sb, iPos);
			sProgram = sb.ToString();
		}

		// SEX (ie X-over) BTM-program sP1 with sP2
		public void vSex(ref string sP1, string sP2) {
			Random r = new Random();
			int iT=CTuring.bPCells;							
			int iX = r.Next(0,bPStates*2);				// XrossOver Trans
			int iL = r.Next(1,bPStates*2-iX+1);			// XO Length (#Trans, min:1)
			int iStart = iX*iT, iLen = iL*iT, iEnd = iStart+iLen;
			D.vOut(CD.bL3, "sP1\t\t{0}", stDump(sP1,iStart,iEnd));
			D.vOut(CD.bL3, "sP2\t\t{0}", stDump(sP2,iStart,iEnd));	
			StringBuilder sbTmp = new StringBuilder(sP1.Substring(0,iStart));
			sbTmp.Append(sP2.Substring(iStart,iLen));
			sbTmp.Append(sP1.Substring(iEnd,bPStates*2*iT-iEnd));
			sP1 = sbTmp.ToString();
			D.vOut(CD.bL1, "vSex:\t\t{0}", stDump(sP1,iStart,iEnd));
		}

		// RECORD BTM success (count #1's on tape)
		public void vRecord()	{
			iOnes=0;
			for (int i=0, j=TM.sTCells; i < j; i++) if (TM.sbTape[i] == '1') iOnes++;
			iTrans = TM.iTrans;
		}		
		#endregion Blife

		#region Bpatch

		// 1: HALT -- Assert exactly one HALT ((NOT in 1.trans)
		public void vPatchHalt(ref StringBuilder sbProgram)	{
			Random r = new Random();
			Stack s = new Stack();
			for (int iStart=0, iEnd=bPStates*2*CTuring.bPCells; iStart<iEnd; iStart+=CTuring.bPCells)
				if (sbProgram[iStart+2] == '0') { s.Push(iStart); }
			if (s.Count > 1) {				// >1 HALT state? - Reset ALL [1...bPStates]
				D.vOut(CD.bL4, "PatchHALT: {0}\t{1}", s.Count, stDump(s));
				while(s.Count>0)  {
					int i = (int)s.Pop();
					sbProgram[i+2] = Convert.ToChar(48+r.Next(1,bPStates+1));
					D.vOut(CD.bL4, "PatchHALT:  {0}\t{1}", i, stDump(sbProgram.ToString(),i,-1));
				}
			}
			if (s.Count == 0)	{				// 0 HALT state? - Insert ONE (random)
				int iPos = r.Next(0,bPStates*2)*CTuring.bPCells; 
				sbProgram[iPos+2] = '0';
				D.vOut(CD.bL1, "PatchHALT:  {0}\t{1}", iPos, stDump(sbProgram.ToString(),iPos,-1));
			}
		}

		// 2: TRAP -- Reject BLIND state (direct recursion state: x<->x => n-1 Beaver)
		public void vPatchTrap(ref StringBuilder sbProgram)	{
			Random r = new Random();
			for (int iStart=0, iEnd=bPStates*2*CTuring.bPCells, i=0; iStart<iEnd; iStart+=2*CTuring.bPCells) 
				while (sbProgram[iStart] == sbProgram[iStart+2] &&
					sbProgram[iStart] == sbProgram[iStart+CTuring.bPCells+2] ) {
					i = iStart + r.Next(0,2)*CTuring.bPCells;
					sbProgram[i+2] = Convert.ToChar(48+r.Next(1,bPStates+1));
					D.vOut(CD.bL1, "PatchTRAP:  {0}\t{1}", i, stDump(sbProgram.ToString(),i,-1));
				}
		}

		// 3: DISC -- Reject SPLIT graph ("forest" w. disconnecxed state: x => n-1 Beaver)
		public void vPatchDisc(ref StringBuilder sbProgram)	{
			int i, j, k, m;
			Stack[] asSta = new Stack[bPStates+1];
			for (i=0; i < bPStates+1; i++) asSta[i] = new Stack();

			for (int iStart=0, iEnd=bPStates*2*CTuring.bPCells; iStart<iEnd; iStart+=2*CTuring.bPCells) {
				asSta[(int)(sbProgram[iStart+2]-'0')].Push(iStart);
				asSta[(int)(sbProgram[iStart+CTuring.bPCells+2]-'0')].Push(iStart+CTuring.bPCells);
			}
			D.vOut(CD.bL4, "PatchDISC:\n{0}", stDump(asSta));

			for (i=0; i<bPStates+1; i++)
				if (asSta[i].Count == 0) {				// A disconnected state...
					for (j=k=m=0; j<bPStates+1; j++) if (asSta[j].Count>m) { m=asSta[j].Count; k=j; }
					asSta[i].Push(asSta[k].Pop());	// Patch trans. from most connected state
					m = (int) asSta[k].Peek();
					sbProgram[m+2] = Convert.ToChar(48+i);
					D.vOut(CD.bL1, "PatchDISC:  {0}\t{1}", m, stDump(sbProgram.ToString(),m,-1));
				}
			D.vOut(CD.bL4, "PatchDISC:\n{0}", stDump(asSta));
		}

		// PATCH -- Perform all patches on BTM program
		public void vPatch(ref StringBuilder sbProgram)	{
			Random r = new Random();

			vPatchHalt(ref sbProgram);					// 1: One HALT
			vPatchTrap(ref sbProgram);					// 2: No  TRAP (Blind)
			vPatchDisc(ref sbProgram);					// 3: No  DISCconnected

			// 4: Check LOOPs
			while (sbProgram.ToString().Substring(0,3) == "101") {
				sbProgram[2] = Convert.ToChar(48+r.Next(0,bPStates+1));
			}

		}
		public void vPatch(ref string sProgram) {
			StringBuilder sb = new StringBuilder(sProgram);
			vPatch(ref sb);
			sProgram = sb.ToString();
		}
		#endregion Bpatch

		// BEAVER DUMP 
		// ----------------------------------------------------------------------------
		#region Bdump
		
		// (Override) Formatted dump of a TM Program
		public string stDump(string stProgram, int iPos1, int iPos2) {
			StringBuilder sbResult = new StringBuilder(500);
			for (int i=0; i < bPStates*10; i+=5) {
				sbResult.AppendFormat(i==iPos1 ? ">" : (i==iPos2 ? "<" : " "));
				sbResult.AppendFormat("{0}", stProgram.Substring(i,5));
			}
			return sbResult.ToString();
		}

		// (Override) Formatted dump of Array of Stacks 
		public string stDump(Stack[] sa) {
			StringBuilder sbResult = new StringBuilder(500); int i=0;
			foreach (Stack s in sa) {
				sbResult.AppendFormat("\t{0}\t{1}\t", i++, s.Count); 
				sbResult.Append(stDump(s)); sbResult.Append("\n");
			}
			return sbResult.ToString();
		}

		// (Override) Formatted dump of one Stack 
		public string stDump(Stack s) {
			StringBuilder sbResult = new StringBuilder(500);
			foreach (object o in s)	sbResult.AppendFormat("{0} ", (int)o );
			return sbResult.ToString();
		}

		public override string ToString() {
			StringBuilder sbResult = new StringBuilder(500);
			sbResult.AppendFormat("   {0} {1}\t{2}", iOnes, iTrans, stProgram);
			return sbResult.ToString();
		}
		#endregion Bdump

	} // END Class CBeaver
	#endregion beaver

	#region beaverlist
	#region classdoc
	/// <summary>
	/// CLASS BEAVERLIST 
	/// </summary>
	#endregion

	[Serializable]
	public class CBeaverList : IDisposable  {

		// BEAVERLIST MEMBERS
		// ----------------------------------------------------------------------------
		#region BLmembers
		public SortedList slBB;
		[NonSerialized] protected byte bPStates;				// #Program states
		[NonSerialized] protected string sFname;				// File name  (persisting BL)
		[NonSerialized] protected FileStream fsBB;			// File Stream
		#if BINARY														// Stream formatter
			[NonSerialized] protected BinaryFormatter fmBB;	//    BINARY
		#elif SOAP														//		or
		[NonSerialized] protected SoapFormatter fmBB;	//		SOAP
		#endif															//
		[NonSerialized] protected bool bDisposed = false;	//
		[NonSerialized] public CD	D;							// Debug class for trace
		[NonSerialized] static short sTCells = 30000;		// Type of Beaver to spawn
		[NonSerialized] static short sHCells = 1024;
		[NonSerialized] static int   iPLoop  = 300000;
		#endregion BLmembers

		// BEAVERLIST LIFECYCLE
		//-----------------------------------------------------------------------------
		#region BLlife

		// CTOR
		public CBeaverList(string sFname) {
			this.sFname = sFname;
			slBB = new SortedList();
			#if BINARY	
				fmBB = new BinaryFormatter();	
			#elif SOAP
			fmBB = new SoapFormatter();	
			#endif
			D = new CD((byte) 0x7);							// Create Debug class
			D.vOut(CD.bL1, "BL Constructing {0}", sFname);
		}

		// DTOR
		protected virtual void Dispose(bool disposing) {	// CBeaverList Dispose
			if (! this.bDisposed && disposing ) 
				using (fsBB=File.Create(sFname)) { fmBB.Serialize(fsBB, slBB); }
			D.vOut(CD.bL1, "BL Disposing {0}", sFname);
		}
		public virtual void Dispose() {							// Override IDisposable:Dispose
			Dispose(true);						
			GC.SuppressFinalize(this);								// Uncomment to allow DTOR
		}
		~CBeaverList () {												// DTOR/Finalizer
			Dispose(false);
			D.vOut(CD.bL1, "BL Destructing {0}", sFname);
		}

		// PERSIST
		public void vPersist(bool bOut) {
			//lock(this)
			try {
				switch ( bOut ) {
					case true:	// OUT: Persist this CBeaverList to sFname
						using (fsBB=File.Create(sFname)) { fmBB.Serialize(fsBB, slBB); }
						break;
					case false:	// IN: Restore this CBeaverList from sFname (if any)
						if (! File.Exists(sFname)) break;
						using (fsBB=File.OpenRead(sFname)) { slBB=(SortedList)fmBB.Deserialize(fsBB); }
						break;
				}
			} catch( Exception e ) { Console.WriteLine(e); }
		}
		#endregion BLlife
			
		// BEAVERLIST TRANSITION
		// ----------------------------------------------------------------------------
		#region BLtrans

		// SPAWN Beavers -- Method for worker threads
		public void vSpawnBeavers() {
			// 1.1: CREATE frame for a bState Beaver
			int i = bPStates*CTuring.bPCells*2;
			StringBuilder sbPgm = new StringBuilder(i+1); sbPgm.Insert(0, "0", i);			
			CBeaver B = new CBeaver(1,bPStates,sbPgm.ToString(),sTCells,"0",2,"",sHCells,iPLoop,0x7);
			//CBeaver B = new CBeaver(1,10,sbPgm.ToString(),30000,"0",2,"",1024,30000000, 0x7);
			B.bPStates = bPStates;

			// 1.2: LOOP: MUTATE & RUN beavers (till completion or exception)
			while (true) {						
				B.vCreate(ref sbPgm);					// Create new Pgm
				B.vReset(sbPgm.ToString());			// Reset the TM
				vRunBeaver(B, 0);							// Run Beaver (Keep all BB)
			}
		}
	
		// RUN Beaver 
		public void vRunBeaver(CBeaver B, int iMax) {		
			try  {	
				B.TM.vReset(B.stProgram);				// Reset TM			
				B.TM.vRun(false, 1);						// Run  --
				B.vRecord();								// OK:  Count 1's for BB 
				if (B.iOnes >= iMax) vInsert(B);		//      Keep in BL if > iMax
				D.vOut(CD.bL1, "BB!:\t{0} {1}\t{2}\n{3}", B.iOnes, B.iTrans, B.stDump(B.stProgram,0,-1), this);
			}													// ERR:
			catch( CTuring.CTuringException e ) { 
				D.vOut(CD.bL1, "EXCEPTION:\t{0}\n", e.Message);
				//if (e.Message.IndexOf("Loop Omega") > 0) Console.WriteLine("POST MORTEM: {0}\n", B.TM.ToString()); 
			}
			finally { D.vOut(CD.bL4, "POST MORTEM: {0}\n", B.TM.ToString()); }
		}

		// PERSISR Beaver
		public void vInsert(CBeaver B) {
			lock(this) {
				vPersist(false);							// Strean IN
				slBB[B.iOnes] = B;						// Insert in BL
				vPersist(true);							// Stream OUT
			}
		}
		#endregion BLtrans

		// BEAVERLIST DUMP 
		// ----------------------------------------------------------------------------
		#region BLdump
		/// <summary> DUMP the TM Program (State Machine) </summary>
		public override string ToString() {
			StringBuilder sbResult = new StringBuilder(500);
			for (int i=0; i < slBB.Count; i++)  {
				sbResult.AppendFormat(" {0}:{1}\n", slBB.GetKey(i), slBB.GetByIndex(i));
			}
			return sbResult.ToString();
		}
		#endregion BLdump

	
		#region console	
		/// <summary>
		/// Console Interface: The main entry point for the app.
		/// </summary>
		class CConsole {

			[MTAThread]
			static void Main(string[] args) {
				int i, j, c;	// Loop counters
				CBeaver B;

				// 1: GETARGS: Beaver <#States> <#Threads> (PROMPT if no args[])
				// 1.1: First arg <#States> for CBeaverList (and BB<#States>.XML file)
				byte bStates = 100;												// No default
				if (args.Length > 0) 
					try  { bStates = byte.Parse(args[0]); }				// <#States>?
					catch( Exception e ) { Console.WriteLine(e); }
				while( ! (0 < bStates && bStates < 10))					// GET:[1...9]
					try {	Console.Write("Enter #States for BB:[1-9]-> ");
						bStates = Convert.ToByte(Console.ReadLine()); 
					} catch (Exception e) { Console.WriteLine(e); } 
	
				// 1.2: Second arg <#Threads> for spawning initial beavers
				byte bThreads = (byte) (args.Length > 1 ? 100 : 0);	// Default:0
				if (args.Length > 1) {
					try  { bThreads = byte.Parse(args[1]); }				// <#Threads>?
					catch( Exception e ) { Console.WriteLine(e); }	
	        		while( ! (0 <= bThreads && bThreads < 21))			// GET:[0...10]
						try {	Console.Write("Enter #Threads for BB:[0-20]-> ");
							bThreads = System.Convert.ToByte(Console.ReadLine()); 
						} catch (Exception e) { Console.WriteLine(e); } 
				}
				
				// 2: RESTORE list of BB-candidates from file: "BB<#States>.XML"
				CBeaverList BL; 
				using (BL = new CBeaverList("BB" + bStates + ".xml")) {
					BL.bPStates = bStates;
					BL.vPersist(false);											// Stream IN 
					Console.WriteLine(BL);
				}
	
				// 3: LOOP 
				// 3.1: Generate a StartPopulation (<#Threads>, each spawning beavers for 5 min)
				if (BL.slBB.Count == 0) bThreads = 10;
				if (bThreads > 0) {
					Thread[] aT = new Thread[bThreads];						// ThreadPool
					for (i=0; i <bThreads; i++) {						// Init & start threads
					aT[i] = new Thread(new ThreadStart(BL.vSpawnBeavers)); aT[i].Start(); }
					Thread.Sleep(3600000);										// Put Main to sleep for 1H
					for (i=0; i <bThreads; i++) { aT[i].Abort(); }	// CleanUp ThreadPool
				}
	
				// 3.2: Eternally evolve the StartPopulation
				BL.vPersist(false);												// Update pool of BB
				Console.WriteLine(BL);
				while (true) {
					// 321: Loop through existing Beavers in BB-list: Mutate & sex ("Stir the pot")
					IList ilKeys = BL.slBB.GetKeyList();
					int iMax = ((CBeaver)BL.slBB[ilKeys[ilKeys.Count-1]]).iOnes;
					for (j=CTuring.bPCells, i=0; i < ilKeys.Count; i++) {
						try { 
							B = new CBeaver((byte)i, BL.bPStates, ((CBeaver)BL.slBB[ilKeys[i]]).stProgram,
								sTCells, "0", 2, "", sHCells, iPLoop, 0x7);
							Console.WriteLine("Revive{0}:{1}", i, B);
							Random r = new Random();
							switch (r.Next(0,3)) {
								case 0:		
								case 1:		// Mutate
									B.vMutate(ref B.stProgram, r.Next(0,B.bPStates-1)*j*2 + r.Next(0,2)*j);
									B.vPatch(ref B.stProgram);
									Console.WriteLine("Mutate{0}:{1}", i, B);
									BL.vRunBeaver(B, iMax);
									break;
								case 2:		// Sex (CrossOver)
									for (c=ilKeys.Count-1; c > 0 ; c--) {
										B.vReset(B.stProgram);
										B.vSex(ref B.stProgram, ((CBeaver)BL.slBB[ilKeys[c]]).stProgram);
										//B.vSex(ref B.stProgram, ((CBeaver)BL.slBB[ilKeys[r.Next(0,c)]]).stProgram);
										B.vPatch(ref B.stProgram);
										Console.WriteLine("Sex{0}:{1}", c, B);
										BL.vRunBeaver(B, iMax);
									} break;
									default:
									throw new ArgumentException("TEST: Switch expr. out-of-bounds");
							}
						} catch( Exception e ) { Console.WriteLine(e);  }
					} 
	
					// 322: Make room & mix new blood ("Add spice")
					i = bStates*CTuring.bPCells*2;
					StringBuilder sbPgm = new StringBuilder(i+1); sbPgm.Insert(0, "0", i);			
					B = new CBeaver(1,bStates,sbPgm.ToString(),sTCells,"0",2,"",sHCells,iPLoop,0x7);
					B.bPStates = bStates;
					for (j=0; j<10; j++) {						// LOOP j times
						B.vCreate(ref sbPgm);					// Create new Pgm
						B.vReset(sbPgm.ToString());			// Reset the TM
						BL.vRunBeaver(B, 0);						// Run Beaver (Overwrite BB)
					}
				}
			}			
		}
	}
	#endregion console

	#endregion beaverlist
}