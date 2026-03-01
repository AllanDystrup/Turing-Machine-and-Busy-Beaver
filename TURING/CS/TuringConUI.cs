using System;

namespace Turing
{
    /// <summary>
    /// CTuring Console UI
    /// </summary>
    public class CTuringConUI
    {
        [STAThread]
        static void Main(string[] args)
        {

            CTuringUnit TU;
            CTuring TM;
            string stProgram;
            byte bTask;
            bool fCont = true;

            while (fCont)
            {

                // MENU
                bTask = 100;
                while (!(0 <= bTask && bTask <= 8))
                {
                    bTask = 100;
                    Console.WriteLine("\t__________________");
                    Console.WriteLine("\t0: Run Turing Unit");
                    Console.WriteLine("\t1: Run 1-state BB");
                    Console.WriteLine("\t2: Run 2-state BB");
                    Console.WriteLine("\t3: Run 3-state BB");
                    Console.WriteLine("\t4: Run 4-state BB");
                    Console.WriteLine("\t5: Run 5-state BB");
                    Console.WriteLine("\t6: Run Unary Add");
                    Console.WriteLine("\t7: Run Unary Mul");
                    Console.WriteLine("\t8: EXIT");

                    Console.Write("\n\tEnter task [0-8]-> ");
                    try { bTask = byte.Parse(Console.ReadLine()); }
                    catch (System.Exception e) { Console.WriteLine(e.Message); }
                    Console.WriteLine();
                }

                // TASK
                switch (bTask)
                {
                    case 0:
                        try
                        {
                            TU = new CTuringUnit();
                            TU.vTest();
                        }
                        catch (Exception e) { Console.WriteLine(e); }
                        finally { TU = null; }
                        break;
                    case 1:
                        // ============================================================
                        //  BusyBeaver TM : Program/Trans.Sequence
                        //  +---------------+------------------------+
                        //  |    INPUT      |           OUTPUT       |
                        //  |State  Symbol  |   State   Symbol  Move*|
                        //  +---------------+------------------------+
                        //					    
                        //  __BB-1STATE_________1/1/64________________ #1s/#Trans/#TMs
                        //  |   1       0   |   0       1       (2)  |         
                        //  |___1_______1___|___0_______1_______(2)__| 
                        stProgram = "1001211012";
                        TM = new CTuring(1, 1, stProgram, 128, "0", 2, "", 128, 50, 0xE);
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        finally { TM = null; }
                        break;

                    case 2:
                        //  __BB-2STATE_________4/6/20.736____________ 
                        // |    1       0   |   2       1       1    |        
                        // |____1_______1___|___2_______1_______2____| 
                        // |    2       0   |   1       1       2    |
                        // |____2_______1___|___0_______1______(2)___| 
                        stProgram = "10211112122011221012";
                        TM = new CTuring(11, 2, stProgram, 128, "0", 2, "", 128, 50, 0xE);
                        //TM.eLog += new CTuring.RunHandler(TM.vOnLog); // Attach Log Hdlr.
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        //TM.vReset(stProgram);						 // Reset & single-step
                        //TM.vRun(true, 1);
                        finally { TM = null; }
                        break;

                    case 3:
                        //  __BB-3STATE_________6/13/268.435.456______ 
                        //  |   1       0   |   2       1       2    |
                        //  |___1_______1___|___3_______1_______1____|
                        //  |   2       0   |   1       1       1    |
                        //  |___2_______1___|___2_______1_______2____|
                        //  |   3       0   |   2       1       1    |
                        //  |___3_______1___|___0_______1______(2)___| 
                        stProgram = "102121131120111212123021131012";
                        TM = new CTuring(12, 3, stProgram, 128, "0", 2, "", 128, 500, 0xE);
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        finally { TM = null; }
                        break;

                    case 4:
                        //  __BB-4STATE________12/107/25.600.000.000__ 
                        //  |   1       0   |   2       1       2    |
                        //  |___1_______1___|___2_______1_______1____|
                        //  |   2       0   |   1       1       1    |
                        //  |___2_______1___|___3_______0______(1)___|
                        //  |   3       0   |   0       1       2    |
                        //  |___3_______1___|___4_______1_______1____|
                        //  |   4       0   |   4       1       2    |
                        //  |___4_______1___|___1_______0______(2)___| 
                        stProgram = "1021211211201112130130011314114041241102";
                        TM = new CTuring(14, 4, stProgram, 512, "0", 2, "", 124, 500, 0xE);
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        finally { TM = null; }
                        break;

                    case 5:
                        // ============================================================
                        //  _BB-5STATE_501/134.467/63.403.380.965.374_ 
                        //  |   1       0   |   2       1       2    |
                        //  |___1_______1___|___3_______0_______1____|
                        //  |   2       0   |   3       1       2    |
                        //  |___2_______1___|___4_______1_______2____|
                        //  |   3       0   |   1       1       1    |
                        //  |___3_______1___|___2_______0_______2____|
                        //  |   4       0   |   5       0       2    |
                        //  |___4_______1___|___0_______1______(2)___|
                        //  |   5       0   |   3       1       1    |
                        //  |___5_______1___|___1_______1_______2____| 
                        // stProgram = "10212113012031221412301113120240502410125031151112";
                        // TM = new CTuring(15, stProgram, 0xE); 
                        // try  { TM.vRun(false, 1); } 
                        // catch( CTuring.CTuringException e ) { Console.WriteLine(e); } 	

                        //  BB-5STATE_4.098/11.798.826/63.403.380.965.374
                        //  |   1       0   |   2       1       1    |
                        //  |___1_______1___|___1_______1_______1____|
                        //  |   2       0   |   3       1       2    |
                        //  |___2_______1___|___2_______1_______2____|
                        //  |   3       0   |   1       1       1    |
                        //  |___3_______1___|___4_______1_______2____|
                        //  |   4       0   |   1       1       1    |
                        //  |___4_______1___|___5_______1_______2____|
                        //  |   5       0   |   0       1      (2)   |
                        //  |___5_______1___|___3_______0_______2____| 
                        stProgram = "10211111112031221212301113141240111415125001251302";
                        TM = new CTuring(15, stProgram, 0xE);
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        finally { TM = null; }
                        break;

                    case 6:
                        // ============================================================
                        //  Unary Arithmetic
                        //
                        //  Unary Addition: UA TM
                        //  For TAPE[> 11110111], EXPECT:HALT[> 1111111]
                        stProgram = "1 2 210---11---" + // Move R to first '1'
                            "2 ---2031021212" +             // Move R to '0', erase w. '1'
                            "3 4 130---31312" +             // Move to R-most 1
                            "4 ---40---415 0" +             // Erase it 
                            "5 6 150---51---" +             // Move L to first ' '
                            "6 0 060---61611";
                        TM = new CTuring(100, 6, stProgram, 256, " ", 3, " 11110111", 256, 1000, 0xE);
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        finally { TM = null; }
                        break;

                    case 7:
                        //  Unary Multiplication: UM TM
                        //  For TAPE[>1111X111], EXPECT:HALT[111111111111       >]
                        stProgram = "1 ---112111X---1*---1A---" +   // Move L to first ' '
                            "2 3*221---2X---2*---2A---" +   // & write '*' here.
                            "3 4 1313123X3X23*3*23A3A2" +   // Move R to first ' '
                            "4 ---415 14X5X14*---4A---" +   // Erase '1' L of ' ' 
                            "5 ---515115X6X15*: 25A---" +   // & move to L of X
                            "6 ---617A16X---6*9*26A6A1" +   // LOOP: Chg. '1' to 'A'  
                            "7 812717117X---7*7*17A---" +   // & write '1' L of '*'
                            "8 ---818128X6X18*8*28A8A2" +   // 
                            "9 4 1919129X9X29*---9A912" +   // Chg 'A' to '1',GOTO:4
                            ": 0 2:1: 2;X: 2:*---:A: 2";    // Erase X and R until ' '
                        TM = new CTuring(100, 10, stProgram, 256, " ", 5, "1111X111", 256, 1000, 0xE);
                        try { TM.vRun(false, 1); }
                        catch (CTuring.CTuringException e) { Console.WriteLine(e); }
                        finally { TM = null; }
                        break;

                    case 8:
                        fCont = false;
                        break;

                    default:
                        Console.WriteLine("ERROR: Input --please enter a digit between 0 and 7");
                        break;
                }
            }
        }
    }
}
