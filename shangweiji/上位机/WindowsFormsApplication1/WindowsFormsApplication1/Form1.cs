using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
                button1.Text = "连接串口";
            }
            else
            {
                serialPort1.Open();
                button1.Text = "关闭串口";
            }
        }

        byte frameFlag=0,frameChecksum=0,frameLength,frameIndex;
        byte[] frameBuf = new byte[100];
        private void timer1_Tick(object sender, EventArgs e)
        {
            byte[] buf = new byte[65535];
            int i, cnt;
            

            if (serialPort1.IsOpen)
            {
                if (serialPort1.BytesToRead != 0)
                {
                    cnt = serialPort1.Read(buf, 0, serialPort1.BytesToRead);
                    for (i = 0; i < cnt; i++)
                    {
                        switch (frameFlag)
                        {
                            case 0:
                                if (buf[i] == 0xB5)
                                    frameFlag = 1;
                                break;
                            case 1:
                                if (buf[i] == 0x62)
                                    frameFlag = 2;
                                else
                                    frameFlag = 0;
                                break;
                            case 2:
                                frameLength = buf[i];
                                frameFlag = 3;
                                frameIndex = 0;
                                frameChecksum = buf[i];
                                break;
                            case 3:
                                frameBuf[frameIndex++] = buf[i];
                                frameChecksum ^= buf[i];
                                if (frameIndex >= frameLength)
                                    frameFlag = 4;
                                break;
                            case 4:
                                if (frameChecksum == buf[i])
                                {
                                    label1.Text = "BMP085--T:" + (System.BitConverter.ToInt16(frameBuf, 0) / 10.0).ToString() + "--P:" + (System.BitConverter.ToUInt32(frameBuf, 2)).ToString();
                                    label2.Text = "SHT1x--T:" + (System.BitConverter.ToSingle(frameBuf, 6)).ToString() + "--HR:" + (System.BitConverter.ToSingle(frameBuf, 10)).ToString("f1") + "%";
                                    label3.Text = "Light--" + (System.BitConverter.ToSingle(frameBuf, 14)).ToString() + "lux";
                                    label4.Text = "Rain--" + (System.BitConverter.ToSingle(frameBuf, 18)).ToString() + "%";
                                }
                                frameFlag = 0;
                                break;
                        }
                    }
                }
            }
        }
    }
}
