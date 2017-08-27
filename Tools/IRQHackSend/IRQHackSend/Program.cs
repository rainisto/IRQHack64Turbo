using System;
using System.Threading;
using System.IO.Ports;
using System.IO;

namespace IRQHackSend
{
    class Program
    {

        static void Receive(SerialPort port)
        {
            Console.Out.WriteLine();
            Console.Out.Write(">>> Transmission from micro start");
            Console.Out.WriteLine();
            while (port.BytesToRead>0)
            {
                int character = port.ReadChar();
                Console.Out.Write((char)character);
            }
            Console.Out.WriteLine();
            Console.Out.Write("<<< Transmission from micro ends");
            Console.Out.WriteLine();
        }


        static void SendFile(string prgFile, string comPort)
        {
            SerialPort port = new SerialPort();
            port.BaudRate = 57600;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.RtsEnable = false;
            port.Parity = Parity.None;
            port.PortName = comPort;

            try
            {
                port.Open();
            }
            catch (Exception ex)
            {
                Console.Out.WriteLine("Port open failed!");
                throw ex;
            }

            byte[] fileContents;

            try
            {
                fileContents = File.ReadAllBytes(prgFile);
            }
            catch (Exception ex)
            {
                Console.Out.WriteLine("Failed reading file!");
                throw ex;
            }

            Receive(port);

            if (fileContents.Length > 65535) throw new Exception("File is too long!");

            Console.Out.WriteLine(String.Format("{0} is opened", comPort));
            Console.Out.WriteLine("Waiting arduino to initialize");
            Thread.Sleep(2000); //Wait arduino to come alive.
            port.Write(new char[] { '1' }, 0, 1); //Send 1 byte command '1'
            Console.Out.WriteLine("Send ReceiveFile command");
            //Wait as much as 2 seconds for the c64 to reset and the circuit to receive what we are
            //sending


            Receive(port);

            Console.Out.WriteLine("Waiting for C64 to reset");
            Thread.Sleep(2100);

            //Send length of prg file
            byte low = (byte)(fileContents.Length % 256);
            byte high = (byte)(fileContents.Length / 256);

            port.Write(new byte[] { low, high }, 0, 2);

            for (int i = 0; i < 2; i++)
            {
                port.Write(new byte[] { fileContents[i] }, 0, 1);
                if (i % 32 == 0)
                {
                    Thread.Sleep(10);
                }

            }

            for (int i = 2; i < fileContents.Length; i++)
            {
                port.Write(new byte[] { fileContents[i] }, 0, 1);
                if (i%32 == 0)
                {
                    Thread.Sleep(10);
                }
                
            }

            Receive(port);
            Thread.Sleep(100);
            Receive(port);
            Thread.Sleep(10000);
            port.Close();

            Console.ReadLine();
        }


        static void OpenMenu(string comPort)
        {
            SerialPort port = new SerialPort();
            port.BaudRate = 57600;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.RtsEnable = false;
            port.Parity = Parity.None;
            port.PortName = comPort;

            try
            {
                port.Open();
            }
            catch (Exception ex)
            {
                Console.Out.WriteLine("Port open failed!");
                throw ex;
            }

            Receive(port);

            Console.Out.WriteLine(String.Format("{0} is opened", comPort));

            port.Write(new char[] { '2' }, 0, 1); //Send 1 byte command '1'
            Console.Out.WriteLine("Send OpenMenu command");
  
            Receive(port);

            port.Close();

            Console.ReadLine();
        }

        static void Main(string[] args)
        {
            string prgFile = args[0];
            string comPort = args[1];

            SendFile(prgFile, comPort);
            //OpenMenu(comPort);
        }
    }
}
