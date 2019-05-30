#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

char* bitstringToByteArray(string bits, int size, char* paddingMask) {
  *paddingMask = (0xff<<(8-(bits.size()%8)));
  char * buf = new char[size];

  while(!(bits.size()%8==0)) {
    bits += "0";
  }

  for (int i = 0 ; i<size; ++i) {
    char byte = 0;
    for (int j = 7; j >= 0; --j) {
      cout << bits[7-j+8*i] << endl;
      byte = byte | (bits[7-j+8*i]-'0') << j;
    }
    buf[i] = byte;
  }
  return buf;
}

int main() {
  //string bitstring("0000000010");
  //char paddingMask;
  //int size = bitstring.size()%8 == 0 ? bitstring.size()/8 : bitstring.size()/8 + 1;
//
  //char * bytes = bitstringToByteArray(bitstring, size, &paddingMask);
//
  //printf("%d %d %d", (unsigned char)paddingMask, (unsigned char) bytes[0], (unsigned char) bytes[1]);
  //
  char* test = "abcdefg";
  stringstream strim;
  strim << test;
  strim << '\n';
  strim << 'b';

  string b;
  strim >> b;
  cout << b;
  strim >> b;
  cout << b;
  strim >> b;
}

