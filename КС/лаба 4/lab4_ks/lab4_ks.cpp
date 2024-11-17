//#include <iostream>
//#include <string>
//#include <windows.h>
//#include <filesystem>
//#include <vector>
//#include <fstream>
//#include <iomanip>
//
//
//void process_file(std::string& filename) {
//   std::ifstream file(filename, std::ios::binary);
//   if (!file) {
//      std::cerr << "Не удалось открыть файл." << std::endl;
//      return;
//   }
//
//   char header[14];
//   file.read(header, 14);
//
//   int count_ipv4 = 0, count_arp = 0, count_eth_dix = 0, count_eth_snap = 0, count_novell_802_2 = 0, count_novell_802_3 = 0;
//   for (int i = 1; file && !file.eof(); i++, file.read(header, 14)) {
//      int size = 14;
//
//      std::cout << "Фрейм #" << std::dec << i << std::endl;
//
//      std::cout << "  MAC-адреса: ";
//      for (int j = 0; j < 6; j++) {
//         std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xFF & static_cast<unsigned int>(header[j]));
//         if (j != 5) std::cout << ":";
//      }
//      std::cout << " <= ";
//      for (int j = 0; j < 6; j++) {
//         std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xFF & static_cast<unsigned int>(header[6 + j]));
//         if (j != 5) std::cout << ":";
//      }
//      std::cout << std::endl;
//
//      unsigned short type = ((0xFF & header[12]) << 8) | (0xFF & header[13]);
//      if (type > 0x0600) {
//         if (type == 0x0800) {
//            std::cout << "  Тип: IPv4" << std::endl;
//
//            char ip_header[20];
//            file.read(ip_header, 20);
//            size += 20;
//
//            std::cout << "    IP-адреса: ";
//            for (int j = 0; j < 4; j++) {
//               std::cout << std::dec << (0xFF & static_cast<unsigned int>(ip_header[12 + j]));
//               if (j != 3) std::cout << ".";
//            }
//            std::cout << " => ";
//            for (int j = 0; j < 4; j++) {
//               std::cout << std::dec << (0xFF & static_cast<unsigned int>(ip_header[16 + j]));
//               if (j != 3) std::cout << ".";
//            }
//            std::cout << std::endl;
//
//            unsigned short total_length = ((0xFF & ip_header[2]) << 8) | (0xFF & ip_header[3]);
//            file.seekg(total_length - 20, std::ios::cur);
//            size += total_length - 20;
//            count_ipv4++;
//         }
//         else if (type == 0x0806) {
//            std::cout << "  Тип: ARP" << std::endl;
//
//            file.seekg(28, std::ios::cur);
//            size += 28;
//            count_arp++;
//         }
//         else {
//            std::cout << "  Тип: Ethernet DIX (Ethernet II)" << std::endl;
//            file.seekg(type, std::ios::cur);
//            size += type;
//            count_eth_dix++;
//         }
//      }
//      else {
//         char dsap_ssap[2];
//         file.read(dsap_ssap, 2);
//         size += 2;
//
//         if (dsap_ssap[0] == 0xAA && dsap_ssap[1] == 0xAA) {
//            std::cout << "  Тип: Ethernet SNAP" << std::endl;
//            file.seekg(type - 2, std::ios::cur);
//            size += type - 2;
//            count_eth_snap++;
//         }
//         else if (type <= 0x05DC) {
//            std::cout << "  Тип: Novell 802.2" << std::endl;
//            file.seekg(type - 2, std::ios::cur);
//            size += type - 2;
//            count_novell_802_2++;
//         }
//         else if (type == 0xFFFF) {
//            std::cout << "  Тип: Novell 802.3" << std::endl;
//            file.seekg(type - 2, std::ios::cur);
//            size += type - 2;
//            count_novell_802_3++;
//         }
//      }
//
//      std::cout << "  Размер: " << std::dec << size << std::endl;
//   }
//
//   std::cout << std::endl << "Обработано кадров: " << std::dec << count_ipv4 + count_arp + count_eth_dix + count_eth_snap + count_novell_802_2 + count_novell_802_3 << std::endl;
//   std::cout << "  IPv4: " << count_ipv4 << std::endl;
//   std::cout << "  ARP: " << count_arp << std::endl;
//   std::cout << "  Ethernet DIX (Ethernet II): " << count_eth_dix << std::endl;
//   std::cout << "  Ethernet SNAP: " << count_eth_snap << std::endl;
//   std::cout << "  Novell 802.2: " << count_novell_802_2 << std::endl;
//   std::cout << "  Novell 802.3: " << count_novell_802_3 << std::endl;
//
//   file.close();
//}
//
//int main() {
//   setlocale(LC_ALL, "");
//   SetConsoleCP(1251);
//   SetConsoleOutputCP(1251);
//
//   std::string filename;
//   std::cout << "Имя файла: ";
//   std::getline(std::cin, filename);
//
//   process_file(filename);
//
//   return 0;
//}
//






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
   filename = "ethers11.bin";
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

      output << "Frame " << n_frames << endl;
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
