using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WMPLib;

namespace Rs232Movie
{
    public partial class DisplayFuncForm : Form
    {
        public DisplayFuncForm()
        {
            InitializeComponent();

            this.Width = Screen.PrimaryScreen.Bounds.Width;//取得目前解析度Width
            this.Height = Screen.PrimaryScreen.Bounds.Height;//取得目前解析度Height
            this.Load += Form1_Load;

            this.WMP1.PlayStateChange += WMP1_PlayStateChange;
            this.WMP1.uiMode = "none";
            this.WMP1.stretchToFit = true;

        }

        void WMP1_PlayStateChange(object sender, AxWMPLib._WMPOCXEvents_PlayStateChangeEvent e)
        {
            if (e.newState == 1 & playingFlag)
            {
                playingFlag = false;
            }
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            //跨執行緒作業無效: 存取控制項 'panel1' 時所使用的執行緒與建立控制項的執行緒不同。
            //所以要加這行, 參考http://wachaolala.blogspot.com/2011/07/vb-aspnet.html
            Control.CheckForIllegalCrossThreadCalls = false;
            Cursor.Hide();
            WMP1.Width = this.Width ;//取得目前解析度Width
            WMP1.Height = this.Height;//取得目前解析度Height
        }

        //加入至播放列表
        private void changeFile(int orderNum)
        {
            switch(orderNum)
            {
                case 0:
                    WMP1.URL = Application.StartupPath + @"\movie\00.wmv";
                    break;
                case 1:
                    WMP1.URL = Application.StartupPath + @"\movie\01.wmv";
                    break;
                case 2:
                    WMP1.URL = Application.StartupPath + @"\movie\02.wmv";
                    break;
                case 3:
                    WMP1.URL = Application.StartupPath + @"\movie\03.wmv";
                    break;
                case 4:
                    WMP1.URL = Application.StartupPath + @"\movie\04.wmv";
                    break;
                case 5:
                    WMP1.URL = Application.StartupPath + @"\movie\05.wmv";
                    break;
                case 6:
                    WMP1.URL = Application.StartupPath + @"\movie\06.wmv";
                    break;
                case 7:
                    WMP1.URL = Application.StartupPath + @"\movie\07.wmv";
                    break;
                case 8:
                    WMP1.URL = Application.StartupPath + @"\movie\08.wmv";
                    break;
                case 9:
                    WMP1.URL = Application.StartupPath + @"\movie\09.wmv";
                    break;
                case 10:
                    WMP1.URL = Application.StartupPath + @"\movie\10.wmv";
                    break;
                case 11:
                    WMP1.URL = Application.StartupPath + @"\movie\11.wmv";
                    break;
                case 12:
                    WMP1.URL = Application.StartupPath + @"\movie\12.wmv";
                    break;
            }
        }

        public void playSelectedVideo(int orderNum)
        {
            if (!playingFlag)
            {
                WMP1.Ctlcontrols.stop();
                changeFile(orderNum);
                WMP1.Ctlcontrols.play();
                playingFlag = true;
            }
        }

        bool playingFlag = false;

        private void DisplayFuncForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                Cursor.Show();
                this.Hide();
            }
        }

    }
}
