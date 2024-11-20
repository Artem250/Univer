#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <map>
#include <iomanip>
using namespace std;

void printMacAddresses(ostream& output, const UCHAR* data, int startIndex) {
   for (UCHAR i = 0; i < 5; i++) {
      output << setw(2) << setfill('0') << (int)data[startIndex + i] << ':';
   }
   output << setw(2) << setfill('0') << (int)data[startIndex + 5] << endl;
}

void printIpAddresses(ostream& output, UCHAR* data, int startIndex) {
   for (int i = 0; i < 3; i++) {
      output << (int)data[startIndex + i] << '.';
   }
   output << (int)data[startIndex + 3] << endl;
}

int main()
{
   int n_dix = 0, n_novell = 0, n_snap = 0, n_llc = 0, n_ipv4 = 0, n_arp = 0;
   ofstream output("frames_info.txt");
   string filename;
   string filepath;
   cout << "Enter the filename: " << endl;
   //cin >> filename;
<<<<<<< Updated upstream
   filename = "ethers07.bin";
=======
   filename = "ethers06.bin";
>>>>>>> Stashed changes
   output << "File name: " << filename << endl;

   filepath = "./stud_ethers_var2_1/";

   ifstream input(filepath + filename, ios::binary);

   if (!input) {
      cerr << "File opening error" << endl;
      return 1;
   }

   vector<unsigned char> data(istreambuf_iterator<char>(input), {}); // Запись всего файла в вектор
   input.close();

   int byte = 0;
   int n_frames = 0;
   while (byte < data.size()) {
      n_frames++;

<<<<<<< Updated upstream
      output << "Frame " << n_frames << " byte: " << byte << endl;
=======
      output << "Frame " << n_frames << /*"byte:" << byte <<*/ endl;
>>>>>>> Stashed changes
      output << hex << uppercase;

      output << "Recipient's MAC address: ";
      printMacAddresses(output, data.data(), byte);

      output << "Sender's MAC address: ";
      printMacAddresses(output, data.data(), byte + 6);
      output << dec;

      unsigned short type_bytes = (data[byte + 12] << 8) + data[byte + 13];  // Определение типа фрейма
      int size;
      if (type_bytes > 0x05DC) {
         output << "Type of frame: Ethernet DIX (Ethernet II)" << endl;
         n_dix++;
         if (type_bytes == 0x0800) {
            output << "Protocol: IPv4" << endl;
            n_ipv4++;

            output << "Sender's IP address: ";
            printIpAddresses(output, data.data(), byte + 26);

            output << "Recipient's IP address: ";
            printIpAddresses(output, data.data(), byte + 30);

            size = (data[byte + 16] << 8) + data[byte + 17] + 14;
            output << "Size: " << size << " bytes" << endl;

            byte += size;
         }

         else if (type_bytes == 0x0806) {
            output << "Type of frame: ARP" << endl;
            n_arp++;

            output << hex << uppercase;
            output << "Sender's MAC address: ";
            printMacAddresses(output, data.data(), byte + 22);

            output << dec;
            output << "Sender's IP address: ";
            printIpAddresses(output, data.data(), byte + 28);

            output << hex << uppercase;
            output << "Recipient's MAC address: ";
            printMacAddresses(output, data.data(), byte + 32);

            output << dec;
            output << "Recipient's IP address: ";
            printIpAddresses(output, data.data(), byte + 38);

            size = 42;
            output << "Size: " << size << " bytes" << endl;


            byte += size;
         }

         else
            byte += type_bytes + 14;
      }
      else {
         unsigned short LLC = (data[byte + 14] << 8) + data[byte + 15];
         if (LLC == 0xFFFF) {
            n_novell++;
            output << "Type of frame: Ethernet Raw 802.3" << endl;
            size = type_bytes + 14;
         }
         else if (LLC == 0xAAAA) {
            n_snap++;
            output << "Type of frame: Ethernet SNAP" << endl;
            size = type_bytes + 14;

         }
         else {
            n_llc++;
            output << "Type of frame: Ethernet 802.2/LLC" << endl;
            size = type_bytes + 14;
            //size = LLC + 12;


         }
         output << "Size: " << size << " bytes" << endl;

         byte += size;
      }
      output << endl;
   }
   output << "Number of frames: " << n_frames << endl << endl;
   output << "Number of frames of each type:\n" << endl;
   output << "Ethernet DIX (Ethernet II): " << n_dix << endl;
   //if (n_dix > 0) {
   output << "   Protocols: " << endl;
   output << "      IPv4: " << n_ipv4 << endl;
   output << "      ARP: " << n_arp << endl;
   // }
   output << "Ethernet Raw 802.3 (Novell 802.3): " << n_novell << endl;
   output << "Ethernet SNAP: " << n_snap << endl;
   output << "Ethernet 802.2 / LLC: " << n_llc << endl;

   output.close();
   return 0;
}
