using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Rs232Movie
{
    public class ArduinoCom
    {
        public SerialPort comport;
        private string portName = "COM";
        private int portNum = 0;
        private int maxPortNum = 50;
        private int retryNum = 3;
        private bool _connectedFlag;
        public bool connectedFlag
        {
            get
            {
                return _connectedFlag;
            }
            set
            {
                _connectedFlag = value;
                if (comport != null & _connectedFlag)
                {
                    comport.DiscardInBuffer();
                }
            }
        }
        public Byte[] cmdBuf = new Byte[3];

        static private Byte[] bufNak = new Byte[3] { 0x55, 0x00, 0x33 };
        static private Byte[] bufArk = new Byte[3] { 0x55, 0x01, 0x33 };

        public delegate void getDataEventHandler(object sender, EventArgs e);
        public event getDataEventHandler getDataEvent;

        protected virtual void onGetEvent()
        {
            // Raise the event by using the () operator.
            getDataEvent(this, new EventArgs());
        }

        public ArduinoCom()
        {
            initilize();
        }

        public void Open()
        {
            reset();
            findArduinoPort();
        }

        public void close()
        {
            if (comport.IsOpen)
            {
                comport.Close();
            }
        }

        private void initilize()
        {
            comport = new SerialPort();
            comport.PortName = portName + portNum.ToString();
            comport.BaudRate = 115200;
            comport.DataBits = 8;
            comport.Parity = Parity.None;
            comport.StopBits = StopBits.One;
            comport.Handshake = Handshake.None;
            comport.Encoding = Encoding.ASCII;
            comport.DiscardNull = false;
            comport.DtrEnable = false;
            comport.RtsEnable = false;
            comport.ReceivedBytesThreshold = 3;
            comport.ReadTimeout = 100;
            comport.WriteTimeout = 100;
            comport.DataReceived += comport_DataReceived;
        }

        private void reset()
        {
            portNum = 0;
            connectedFlag = false;
        }

        //找尋Arduino
        public bool findArduinoPort()
        {
            bool foundFlag = false;
            while (!foundFlag)
            {
                //最大搜尋com數
                if (portNum > maxPortNum)
                {
                    MessageBox.Show("No Comports");
                    return false;
                }
                try
                {
                    foundFlag = firstHandShake();
                }
                catch (Exception ex)
                {
                    this.close();
                }
                if (!foundFlag)
                {
                    //
                    portNum++;
                    comport.PortName = portName + portNum.ToString();
                    continue;
                }
            }
            return foundFlag;
        }

        //判斷是否連接
        private bool firstHandShake()
        {
            comport.Open();

            for (int i = 0; i < retryNum; i++)
            {
                //先送Ark
                sendArk();
                System.Threading.Thread.Sleep(100);

                //送完後如果收到Ark 就代表連接上
                if (arrayCompare(cmdBuf, bufArk))
                {
                    //旗標設真
                    connectedFlag = true;
                    return true;
                }
                System.Threading.Thread.Sleep(100);
            }
            this.close();
            return false;
        }

        //Send Ark
        public void sendArk()
        {
            if (comport.IsOpen)
            {
                comport.Write(bufArk, 0, bufArk.Length);
            }
        }

        //Send Nak
        public void sendNak()
        {
            if (comport.IsOpen)
            {
                comport.Write(bufNak, 0, bufNak.Length);
            }
        }

        //送資料
        public bool sendData(byte[] tmpBuf)
        {
            if ((tmpBuf != null) & comport.IsOpen)
            {
                for (int i = 0; i < retryNum; i++)
                {
                    comport.Write(tmpBuf, 0, tmpBuf.Length);
                    System.Threading.Thread.Sleep(10);
                    if (arrayCompare(cmdBuf, bufArk))
                    {
                        return true;
                    }

                    System.Threading.Thread.Sleep(100);
                }
            }
            return false;
        }

        //debug
        public string combineToString(byte[] tmpData)
        {
            if (tmpData == null)
            {
                return null;
            }

            string res = "";
            for (int i = 0; i < tmpData.Count(); i++)
            {
                res += "Data[" + i.ToString() + "]: ";
                res += tmpData[i].ToString("x2");
                res += "\n";
            }
            return res;
        }

        //比對Array
        private bool arrayCompare(Byte[] tmpBuf1, Byte[] tmpBuf2)
        {
            if (count != 0)
            {
                count = 0;
                cleanCmdBuf();
                return false;
            }

            if (BitConverter.ToString(tmpBuf1) == BitConverter.ToString(tmpBuf2))
            {
                count = 0;
                cleanCmdBuf();
                return true;
            }

            cleanCmdBuf();
            return false;
        }

        int count = 0;
        //收資料 Event
        private void comport_DataReceived(Object sender, SerialDataReceivedEventArgs e)
        {
            if (!comport.IsOpen)
            {
                return;
            }

            Byte[] tmp = new Byte[comport.ReceivedBytesThreshold];
            comport.Read(tmp, 0, comport.ReceivedBytesThreshold);
            for (int i = 0; i < comport.ReceivedBytesThreshold; i++)
            {
                //如果讀到開始, 就從開始
                if (tmp[i] == bufArk[0])
                {
                    count = 0;
                }
                else
                {
                    if (count == 0)
                    {
                        continue;
                    }
                }
                cmdBuf[count] = tmp[i];

                //如果讀到結束, 就結束, 剩下的就不用了
                if (cmdBuf[bufArk.Count() - 1] == bufArk[bufArk.Count() - 1])
                {
                    count = 0;
                    comport.DiscardInBuffer();
                    if (connectedFlag)
                    {
                        onGetEvent();
                    }
                    return;
                }
                count++;

                if (count == bufArk.Count())
                {
                    count = 0;
                }
            }
        }

        public void cleanCmdBuf()
        {
            this.cmdBuf = new Byte[bufArk.Count()];
        }

    }
}
