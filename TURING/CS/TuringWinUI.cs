// Core 
using System;
using System.Threading;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Turing;

namespace TuringWinUI
{
	/// <summary>
	/// Summary description for frmTuring.
	/// </summary>
	public class frmTuring : System.Windows.Forms.Form
	{
		private CTuring TM = null;
		Thread T;

		private System.Windows.Forms.RichTextBox rtbTuring;
		private System.ComponentModel.Container components = null; /// Required designer variable.
		private System.Windows.Forms.GroupBox grpOperation;
		private System.Windows.Forms.RadioButton rbtnBB1;
		private System.Windows.Forms.RadioButton rbtnBB2;
		private System.Windows.Forms.Button btnStep;
		private System.Windows.Forms.Button btnRun;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tpgBB;
		private System.Windows.Forms.RadioButton rbtnBB4;
		private System.Windows.Forms.Button btnPause;
		private System.Windows.Forms.RadioButton rbtnBB51;
		private System.Windows.Forms.RadioButton rbtnBB52;
		private System.Windows.Forms.Button btnKill;
		private System.Windows.Forms.RichTextBox rtbPgm;
		private System.Windows.Forms.RadioButton rbntBB3;


		public frmTuring() {
			InitializeComponent(); // Required for Windows Form Designer support
			// TODO: Add any constructor code after InitializeComponent call
		}

		protected void vStep() {
			try  { 
				bool bAlive = TM.bTrans();
				rtbPgm.Text = TM.TracePgm(false);
				rtbTuring.Text = TM.ToString();
				if (!bAlive) vState(5);
			} catch(CTuring.CTuringException e) 
			{ rtbTuring.Text = e.ToString(); vState(5); }
		}

		protected void vRun() {
			bool bAlive;
			try  { 
				do { bAlive = TM.bTrans();
					rtbPgm.Text = TM.TracePgm(false);
					rtbTuring.Text = TM.ToString(); rtbTuring.Update();	
				} while (bAlive);
			} catch(CTuring.CTuringException e) { rtbTuring.Text = e.ToString(); 
			} // finally {	vState(5); }							
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ){
				if (components != null) { components.Dispose(); }
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.btnStep = new System.Windows.Forms.Button();
			this.rtbTuring = new System.Windows.Forms.RichTextBox();
			this.grpOperation = new System.Windows.Forms.GroupBox();
			this.btnKill = new System.Windows.Forms.Button();
			this.rbtnBB52 = new System.Windows.Forms.RadioButton();
			this.rbtnBB51 = new System.Windows.Forms.RadioButton();
			this.rbtnBB4 = new System.Windows.Forms.RadioButton();
			this.rbntBB3 = new System.Windows.Forms.RadioButton();
			this.rbtnBB1 = new System.Windows.Forms.RadioButton();
			this.rbtnBB2 = new System.Windows.Forms.RadioButton();
			this.btnRun = new System.Windows.Forms.Button();
			this.btnPause = new System.Windows.Forms.Button();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tpgBB = new System.Windows.Forms.TabPage();
			this.rtbPgm = new System.Windows.Forms.RichTextBox();
			this.grpOperation.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tpgBB.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnStep
			// 
			this.btnStep.Location = new System.Drawing.Point(88, 24);
			this.btnStep.Name = "btnStep";
			this.btnStep.Size = new System.Drawing.Size(56, 24);
			this.btnStep.TabIndex = 1;
			this.btnStep.Text = "Step";
			this.btnStep.Click += new System.EventHandler(this.btnStep_Click);
			// 
			// rtbTuring
			// 
			this.rtbTuring.BackColor = System.Drawing.SystemColors.Menu;		/// AppWorkspace;
			this.rtbTuring.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, 
					System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.rtbTuring.ForeColor = System.Drawing.SystemColors.MenuText;	/// Info;
			this.rtbTuring.Location = new System.Drawing.Point(208, 96);
			this.rtbTuring.Name = "rtbTuring";
			this.rtbTuring.Size = new System.Drawing.Size(368, 256);
			this.rtbTuring.TabIndex = 2;
			this.rtbTuring.Text = "";
			this.rtbTuring.WordWrap = false;
			// 
			// grpOperation
			// 
			this.grpOperation.Controls.AddRange(new System.Windows.Forms.Control[] {
				this.btnKill,
				this.rbtnBB52,
				this.rbtnBB51,
				this.rbtnBB4,
				this.rbntBB3,
				this.rbtnBB1,
				this.rbtnBB2,
				this.btnStep,
			    this.btnRun,
			    this.btnPause});
			this.grpOperation.Location = new System.Drawing.Point(16, 24);
			this.grpOperation.Name = "grpOperation";
			this.grpOperation.Size = new System.Drawing.Size(152, 176);
			this.grpOperation.TabIndex = 3;
			this.grpOperation.TabStop = false;
			this.grpOperation.Text = "Operation";
			// 
			// btnKill
			// 
			this.btnKill.Location = new System.Drawing.Point(88, 136);
			this.btnKill.Name = "btnKill";
			this.btnKill.Size = new System.Drawing.Size(56, 24);
			this.btnKill.TabIndex = 9;
			this.btnKill.Text = "Kill";
			this.btnKill.Click += new System.EventHandler(this.btnKill_Click);
			// 
			// rbtnBB52
			// 
			this.rbtnBB52.Location = new System.Drawing.Point(16, 144);
			this.rbtnBB52.Name = "rbtnBB52";
			this.rbtnBB52.Size = new System.Drawing.Size(64, 24);
			this.rbtnBB52.TabIndex = 8;
			this.rbtnBB52.Text = "BB 5-2";
			this.rbtnBB52.CheckedChanged += new System.EventHandler(this.rbtnBB52_CheckedChanged);
			// 
			// rbtnBB51
			// 
			this.rbtnBB51.Location = new System.Drawing.Point(16, 120);
			this.rbtnBB51.Name = "rbtnBB51";
			this.rbtnBB51.Size = new System.Drawing.Size(64, 24);
			this.rbtnBB51.TabIndex = 7;
			this.rbtnBB51.Text = "BB 5-1";
			this.rbtnBB51.CheckedChanged += new System.EventHandler(this.rbtnBB51_CheckedChanged);
			// 
			// rbtnBB4
			// 
			this.rbtnBB4.Location = new System.Drawing.Point(16, 96);
			this.rbtnBB4.Name = "rbtnBB4";
			this.rbtnBB4.Size = new System.Drawing.Size(64, 24);
			this.rbtnBB4.TabIndex = 6;
			this.rbtnBB4.Text = "BB 4";
			this.rbtnBB4.CheckedChanged += new System.EventHandler(this.rbtnBB4_CheckedChanged);
			// 
			// rbntBB3
			// 
			this.rbntBB3.Location = new System.Drawing.Point(16, 72);
			this.rbntBB3.Name = "rbntBB3";
			this.rbntBB3.Size = new System.Drawing.Size(64, 24);
			this.rbntBB3.TabIndex = 5;
			this.rbntBB3.Text = "BB 3";
			this.rbntBB3.CheckedChanged += new System.EventHandler(this.rbtnBB3_CheckedChanged);
			// 
			// rbtnBB1
			// 
			this.rbtnBB1.Location = new System.Drawing.Point(16, 24);
			this.rbtnBB1.Name = "rbtnBB1";
			this.rbtnBB1.Size = new System.Drawing.Size(64, 24);
			this.rbtnBB1.TabIndex = 0;
			this.rbtnBB1.Text = "BB 1";
			this.rbtnBB1.CheckedChanged += new System.EventHandler(this.rbtnBB1_CheckedChanged);
			// 
			// rbtnBB2
			// 
			this.rbtnBB2.Location = new System.Drawing.Point(16, 48);
			this.rbtnBB2.Name = "rbtnBB2";
			this.rbtnBB2.Size = new System.Drawing.Size(64, 24);
			this.rbtnBB2.TabIndex = 4;
			this.rbtnBB2.Text = "BB 2";
			this.rbtnBB2.CheckedChanged += new System.EventHandler(this.rbtnBB2_CheckedChanged);
			// 
			// btnRun
			// 
			this.btnRun.Location = new System.Drawing.Point(88, 72);
			this.btnRun.Name = "btnRun";
			this.btnRun.Size = new System.Drawing.Size(56, 24);
			this.btnRun.TabIndex = 4;
			this.btnRun.Text = "Run";
			this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
			// 
			// btnPause
			// 
			this.btnPause.Location = new System.Drawing.Point(88, 104);
			this.btnPause.Name = "btnPause";
			this.btnPause.Size = new System.Drawing.Size(56, 24);
			this.btnPause.TabIndex = 5;
			this.btnPause.Text = "Pause";
			this.btnPause.Click += new System.EventHandler(this.btnPause_Click);
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					  this.tpgBB});
			this.tabControl1.Location = new System.Drawing.Point(16, 24);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(184, 344);
			this.tabControl1.TabIndex = 5;
			this.tabControl1.SelectedIndexChanged += 
				new System.EventHandler(this.tabControl1_SelectedIndexChanged);
			// 
			// tpgBB
			// 
			this.tpgBB.Controls.AddRange(new System.Windows.Forms.Control[] {
																				this.grpOperation});
			this.tpgBB.Location = new System.Drawing.Point(4, 22);
			this.tpgBB.Name = "tpgBB";
			this.tpgBB.Size = new System.Drawing.Size(176, 318);
			this.tpgBB.TabIndex = 0;
			this.tpgBB.Text = "BusyBeaver";
			this.tpgBB.Click += new System.EventHandler(this.tpgBB_Click);
			// 
			// rtbPgm
			// 
			this.rtbPgm.BackColor = System.Drawing.SystemColors.ScrollBar;
			this.rtbPgm.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, 
				System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.rtbPgm.Location = new System.Drawing.Point(208, 8);
			this.rtbPgm.Name = "rtbPgm";
			this.rtbPgm.Size = new System.Drawing.Size(200, 80);
			this.rtbPgm.TabIndex = 6;
			this.rtbPgm.Text = "";
			// 
			// frmTuring
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(680, 381);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
				this.rtbPgm,
				this.tabControl1,
				this.rtbTuring});
			this.Name = "frmTuring";
			this.Text = "UTM - Universal Turing Machine";
			this.Load += new System.EventHandler(this.frmTuring_Load);
			this.grpOperation.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.tpgBB.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new frmTuring());
		}

		private void frmTuring_Load(object sender, System.EventArgs e) 
		{	vState(0); }

		// TAB CONTROL
		// ----------------------------------------------------------------------------
		private void tabControl1_SelectedIndexChanged(object sender, System.EventArgs e) 
		{	}
		private void tpgBB_Click(object sender, System.EventArgs e) 
		{	}

		// TAB1: BUSY BEAVER
		// ----------------------------------------------------------------------------
		// BB Check Boxes
		private void rbtnBB1_CheckedChanged(object sender, System.EventArgs e) {	
			vState(5); /*KILL*/
			TM = new CTuring(1,1,"1001211012", 512,"0",2,"", 124,512, 0xE); 
			vState(1); /*CHCK*/ }
		private void rbtnBB2_CheckedChanged(object sender, System.EventArgs e) 
		{	vState(5); 
			TM = new CTuring(1,2,"10211112122011221012", 512,"0",2,"", 124,512, 0xE); 
			vState(1); }
		private void rbtnBB3_CheckedChanged(object sender, System.EventArgs e) 
		{	vState(5); 
			TM = new CTuring(1,3,"102121131120111212123021131012", 
				512,"0",2,"", 124,512, 0xE); 
			vState(1);	}
		private void rbtnBB4_CheckedChanged(object sender, System.EventArgs e) 
		{ 	vState(5); 
			TM = new CTuring(1,4,"1021211211201112130130011314114041241102", 
				512,"0",2,"", 124,512, 0xE); 
			vState(1); } 
		private void rbtnBB51_CheckedChanged(object sender, System.EventArgs e) 
		{	vState(5); 
			TM = new CTuring(1,5,"10212113012031221412301113120240502410125031151112", 
				30000,"0",2,"", 124,300000000, 0xE); 
			vState(1);	}
		private void rbtnBB52_CheckedChanged(object sender, System.EventArgs e) {		
		 	vState(5); 
			TM = new CTuring(1,5,"10211111112031221212301113141240111415125001251302", 
				30000,"0",2,"", 124,300000000, 0xE); 
			vState(1); }

		// BB Buttons
		private void btnStep_Click(object sender, System.EventArgs e) 
		{	vState(2); }
		private void btnRun_Click(object sender, System.EventArgs e) 
		{	vState(3); }
		private void btnPause_Click(object sender, System.EventArgs e) 
		{	vState(4); }
		private void btnKill_Click(object sender, System.EventArgs e)
		{	vState(5); }


		// UI State
		// obs:----------------------------------------------------------------------------
		// Suspend/Resume are deprecated (.NET 2.0), but safe to call on the current thread 
		private void vState(byte bS) {
			switch (bS) {
				case 0:	// INIT
					T = null; TM = null;
					btnStep.Enabled = btnRun.Enabled = btnPause.Enabled = btnKill.Enabled = false;
					break;
				case 1:	// CHCK
					rtbPgm.Text = rtbTuring.Text = "";
					rtbPgm.Text = TM.TracePgm(false);
					rtbTuring.Text = TM.ToString(); rtbTuring.Update();	
					btnStep.Enabled = btnRun.Enabled = true;
					break;
				case 2:	// STEP
					if (T!=null && (T.ThreadState & (ThreadState.Stopped | ThreadState.Unstarted))==0) T.Suspend();
					btnPause.Enabled = btnKill.Enabled = false;
					vStep(); 
					break;
				case 3:	// RUN
					if (T==null) { T= new Thread(new ThreadStart(vRun)); T.Start(); }
					if ((T.ThreadState & ThreadState.Suspended) > 0) T.Resume();
					btnPause.Enabled = btnKill.Enabled = true;
					break;
				case 4:	// PAUSE
					T.Suspend();
					btnPause.Enabled =  false;
					break;
				case 5:	// KILL
					if (T != null) { 
						if ((T.ThreadState & ThreadState.Suspended) > 0) T.Resume();
						T.Abort(); // Can't abort a suspended thread
					}; vState(0); 
					break;
				default:
					//
					break;
			}
		}

	}

		// TAB2: UTM <to-be-defined>
		// ----------------------------------------------------------------------------

}
