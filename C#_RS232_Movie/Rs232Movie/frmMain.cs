using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Rs232Movie
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            InitializeComponent();
            this.Shown += Form1_Shown;

            this.com1.getDataEvent += com1_getDataEvent;
        }

        void com1_getDataEvent(object sender, EventArgs e)
        {
            string cmd = this.com1.cmdBuf[1].ToString("x");
            this.com1.cleanCmdBuf();
            if (this.frmFunc == null)
            {
                return;
            }
            switch (cmd)
            {
                case "10":
                    this.frmFunc.playSelectedVideo(0);
                    break;
                case "11":
                    this.frmFunc.playSelectedVideo(1);
                    break;
                case "12":
                    this.frmFunc.playSelectedVideo(2);
                    break;
                case "13":
                    this.frmFunc.playSelectedVideo(3);
                    break;
                case "14":
                    this.frmFunc.playSelectedVideo(4);
                    break;
                case "15":
                    this.frmFunc.playSelectedVideo(5);
                    break;
                case "16":
                    this.frmFunc.playSelectedVideo(6);
                    break;
                case "17":
                    this.frmFunc.playSelectedVideo(7);
                    break;
                case "18":
                    this.frmFunc.playSelectedVideo(8);
                    break;
                case "19":
                    this.frmFunc.playSelectedVideo(9);
                    break;
                case "1d":
                    this.frmFunc.playSelectedVideo(10);
                    break;
                case "1b":
                    this.frmFunc.playSelectedVideo(11);
                    break;
                case "1c":
                    this.frmFunc.playSelectedVideo(12);
                    break;
            }
        }

        void frmFunc_VisibleChanged(object sender, EventArgs e)
        {
            if (((DisplayFuncForm)sender).Visible == false)
            {
                this.Show();
            }
        }

        void Form1_Shown(object sender, EventArgs e)
        {
            com1.Open();
            if (com1.connectedFlag)
            {
                this.labelConnStatus.Text = "是";
            }
        }


        private void button2_Click(object sender, EventArgs e)
        {
            if (!com1.connectedFlag)
            {
                com1.Open();
                if (com1.connectedFlag)
                {
                    this.labelConnStatus.Text = "是";
                }
            }
            else
            {
                MessageBox.Show("已連線");

            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (!com1.connectedFlag)
            {
                MessageBox.Show("尚未連線, 請連線");
            }
            else
            {
                this.Hide();
                if (this.frmFunc == null)
                {
                    this.frmFunc = new DisplayFuncForm();
                    this.frmFunc.VisibleChanged += frmFunc_VisibleChanged;
                }
                this.frmFunc.Show();
            }
        }

        ArduinoCom com1 = new ArduinoCom();
        DisplayFuncForm frmFunc;

    }
}
