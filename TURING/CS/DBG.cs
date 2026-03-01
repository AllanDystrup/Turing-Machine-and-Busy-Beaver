using System;
//using System.Windows.Forms;

namespace DBG
{
    /// <summary>
    /// Summary description for Class1.
    /// </summary>
    public class CD
    {
        // Members
        public static byte bL0 = 0x0;   // OFF
        public static byte bL1 = 0x1;   // Object Life
        public static byte bL2 = 0x2;   // Object Major Trans
        public static byte bL3 = 0x4;   // Object Minor Trans
        public static byte bL4 = 0x8;   // Object HotSpot Debug
        public static byte bL5 = 0xF;   // ALL
        private byte bDLevel;

        // Ctor
        public CD(byte bDL) { bDLevel = bDL; }

        // Output
        public void vOut(byte bL, string sFormat, object o1)
        { if ((bDLevel & bL) > 0) Console.WriteLine(sFormat, o1); }
        // { if ((bDLevel & bL)>0) MessageBox.Show(o1.ToString()); }
        public void vOut(byte bL, string sFormat, object o1, object o2)
        { if ((bDLevel & bL) > 0) Console.WriteLine(sFormat, o1, o2); }
        // { {if ((bDLevel & bL)>0) MessageBox.Show(o1.ToString()+" " +o2.ToString()); } 
        public void vOut(byte bL, string sFormat, object o1, object o2, object o3)
        { if ((bDLevel & bL) > 0) Console.WriteLine(sFormat, o1, o2, o3); }
        public void vOut(byte bL, string sFormat, object o1, object o2, object o3, object o4)
        { if ((bDLevel & bL) > 0) Console.WriteLine(sFormat, o1, o2, o3, o4); }

        public void vOut(byte bL, params object[] os)
        {
            if ((bDLevel & bL) > 0) { foreach (object o in os) Console.Write(o); Console.WriteLine(); }
        }

        /*
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            byte bDLevel = 0x2;
            CD D = new CD(bDLevel);
            D.vOut(0x3, "Hej {0}", bDLevel);
            D.vOut(0x3, "Hej {0}", bDLevel, 5);
            Console.ReadLine();
        }
        */
    }
}