using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SMSTest1
{
    public partial class Form1 : Form
    {
        private const int port = 8000;

        // CLIENT_SendMsg 這Class 沒用到
        // 發送封包
        public class CLIENT_SendMsg
        {
            public CLIENT_SendMsg()
            {
                msg_set = new byte[100];
                msg_content = new byte[160];
            }
            public byte msg_type;
            public byte msg_coding;
            public byte msg_priority;
            public byte msg_country_code;
            public byte msg_set_len;
            public byte msg_content_len;
            public byte[] msg_set;
            public byte[] msg_content;
        }

        // 接收封包
        public class CLIENT_RetMsg {
            public CLIENT_RetMsg()
            {
                ret_set = new byte[80];
                ret_content = new byte[160];
            }
            public byte ret_code ;
            public byte ret_coding ;
            public byte ret_set_len;
            public byte ret_content_len;
            public byte[] ret_set;
            public byte[] ret_content;
        } ;

        private void Connect()
        {
            try
            {
                // IP 位址
                IPHostEntry ipHostInfo = Dns.Resolve("api.hiair.hinet.net");
                IPAddress ipAddress = ipHostInfo.AddressList[0];

                // IP 位置 + Port
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

                // 建立連線用Socket
                Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                // 連線
                client.Connect(remoteEP);


                // =========================================
                // 以下開始組需要的傳輸的字串
                // 相關內容請參照 https://sms.hinet.net/new/sent_software.htm

                // 建立傳輸暫存空間, 266個byte 是規定
                byte[] OutBuffer = new byte[266];

                // 帳號密碼 (要改!!!!)
                // 根本沒有這組 所以之後回傳一定會說Account 錯誤
                string User = "User";
                string PassWord = "Password";

                // 字串組成
                string acc_pwd_str = User.Trim() + "\0" + PassWord.Trim() + "\0" ;
                
                // 轉utf8 -> byte -> big 5 byte  
                byte[] acc_pwd_byte = Encoding.Convert(Encoding.Unicode, Encoding.GetEncoding(950), Encoding.Unicode.GetBytes(acc_pwd_str)); 
                byte acc_pwd_size = (byte)acc_pwd_byte.Length;

                // 開始放資料進傳輸暫存空間
                OutBuffer[0] = 0; //檢查密碼
                OutBuffer[1] = 1; //big編碼
                OutBuffer[2] = 0; //priority
                OutBuffer[3] = 0; //國碼 0:台灣
                OutBuffer[4] = acc_pwd_size; //msg_set_len
                OutBuffer[5] = 0; //msg_content_len, 驗證密碼時不需msg_content
                //設定msg_set 內容 "帳號"+"密碼"
                for (int i = 0; i < acc_pwd_size; i++)
                {
                    OutBuffer[i + 6] = acc_pwd_byte[i];
                }

                // 傳送整個傳輸暫存空間 必須要是266個byte
                int bytesSent = client.Send(OutBuffer);

                // 
                // 顯示出來 Debug
                StringBuilder Str = new StringBuilder(acc_pwd_byte.Length * 2);
                foreach (byte b in acc_pwd_byte)
                {
                    Str.AppendFormat("{0:x2}", b);
                }
                MessageBox.Show("Send: " + bytesSent.ToString() + "byte\n" + "Set 的值: " + Str + "\n");

                // ============================================================
                // 接收!!!
                // 傳送完後, 系統會回傳成功與否

                // 先宣告一個放接收資料的Buffer
                byte[] RecBuf = new byte[1024];
                
                // 將所有收到的資料都放進去
                int RecSize = client.Receive(RecBuf);

                // 
                // 以下只是為了顯示所有接收到的資料
                // CLIENT_RetMsg這個class 不用也沒關係
                // 有需要從這改
                CLIENT_RetMsg RecMsg = new CLIENT_RetMsg();
                RecMsg.ret_code = RecBuf[0];
                RecMsg.ret_coding = RecBuf[1];
                RecMsg.ret_set_len = RecBuf[2];
                RecMsg.ret_content_len = RecBuf[3];
                for (int i = 0; i < 80; i++)
                {
                    RecMsg.ret_set[i] = RecBuf[i + 4];
                }
                for (int i = 0; i < 160; i++)
                {
                    RecMsg.ret_content[i] = RecBuf[i + 4 + 80];
                }
                String RetStr = "RecMsg.ret_code: " + RecMsg.ret_code.ToString("x2") + "\n";
                RetStr += "RecMsg.ret_coding: " + RecMsg.ret_coding.ToString("x2") + "\n";
                RetStr += "RecMsg.ret_set_len: " + RecMsg.ret_set_len.ToString("x2") + "\n";
                RetStr += "RecMsg.ret_content_len: " + RecMsg.ret_content_len.ToString("x2") + "\n";

                //StringBuilder ret_setStr = new StringBuilder(RecMsg.ret_set.Length * 2);
                //foreach (byte b in RecMsg.ret_set)
                //{
                //    ret_setStr.AppendFormat("{0:x2}", b);
                //}
                //RetStr += "RecMsg.ret_set: " + ret_setStr + "\n";

                // 印出回傳Set Big5 編碼
                if (RecMsg.ret_set_len != 0)
                {
                    byte[] RecMsgRetSet = new byte[RecMsg.ret_set_len];
                    for (int i = 0; i < RecMsg.ret_set_len; i++)
                    {
                        RecMsgRetSet[i] = RecMsg.ret_set[i];
                    }

                    string RecMsgRetContentStr = Encoding.GetEncoding(950).GetString(RecMsgRetSet);
                    RetStr += "RecMsgRetSet :" + RecMsgRetSet + "\n";
                }
                else
                {
                    // 這不是錯誤 只是沒有資料而已
                    RetStr += "RecMsgRetSet :" + "All Data is zero" + "\n";
                }


                //// 印出回傳Content Hex 形式
                //StringBuilder ret_contentStr = new StringBuilder(RecMsg.ret_content.Length * 2);
                //foreach (byte b in RecMsg.ret_content)
                //{
                //    ret_contentStr.AppendFormat("{0:x2}", b);
                //}
                //RetStr += "RecMsg.ret_content: " + ret_contentStr + "\n";

                // 印出回傳Content Big5 編碼
                if (RecMsg.ret_content_len != 0)
                {
                    byte[] RecMsgRetContent = new byte[RecMsg.ret_content_len];
                    for (int i = 0; i < RecMsg.ret_content_len; i++)
                    {
                        RecMsgRetContent[i] = RecMsg.ret_content[i];
                    }

                    string RecMsgRetContentStr = Encoding.GetEncoding(950).GetString(RecMsgRetContent);
                    RetStr += "RecMsgRetContentStr :" + RecMsgRetContentStr + "\n";
                }
                else
                {
                    // 這不是錯誤 只是沒有資料而已
                    RetStr += "RecMsgRetContentStr :" + "All Data is zero" + "\n";
                }
                MessageBox.Show(RetStr);

                // 關掉連線
                client.Shutdown(SocketShutdown.Both);
                client.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

        }

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Connect();
        }
    }
}
