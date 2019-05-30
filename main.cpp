#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include <map>
#include <stdio.h>

#include "btree.hh"

using namespace std;

void codecMode(int argc, char** argv);
void decodeMode(int argc, char** argv);

map<char,int> getFrequencies(char* buffer, int length);
Node* buildHuffmanTree(map<char,int>& freq);
char* bitstringToByteArray(string bits, int size, char* paddingMask);
void writeHeader(vector<char>& header, char paddingMask, Node* root);
string getDecodedString(string bitstring, map<string,char> encodeMap);
string getTextBitString(string encoded, int paddingMask);

const char nullchar = 0xff;
const bool useFile = false;

void fillCodeMap(Node* root, map<char,string>& codeMap, string prefix = "") {
  if (root == NULL) return;

  if (root->key != nullchar) {
    codeMap[root->key] = prefix;
  }
  else {
    fillCodeMap(root->left, codeMap, prefix+"0");
    fillCodeMap(root->right, codeMap, prefix+"1");
  }
}

int main(int argc, char** argv) {
  codecMode(argc,argv);
}

string getDecodedString(string bitstring, map<string,char>encodeMap) {
  string decodedString;
  for (size_t i = 0 ; i < bitstring.size();) {
    string aux;
    while(i < bitstring.size()){
      aux+=bitstring[i];
      i++;
      if (encodeMap.count(aux)==1) break;
    }
    decodedString += encodeMap[aux];
  }
  return decodedString;
}

string getTextBitString(string encoded, int paddingMask) {
  string result;
  for (int j = 0 ; j < encoded.size(); ++j) {
    char c = encoded[j];
    for (int i = 7 ; i >= 0 ; --i) {
      result += ((c>>i & 1) + '0');
    }
  }
  return result;
}

void codecMode(int argc, char** argv) {
  char * buffer;
  int length;
  if (useFile) {
    ifstream textFile("text.txt", ios::in | ios::binary);
    if (textFile) {
      textFile.seekg(0,textFile.end);
      length = textFile.tellg();
      textFile.seekg(0,textFile.beg);
    }
    buffer = new char[length];
    textFile.read(buffer,length);
    printf("File length: %d Text:\n", length);
    textFile.close();
  } else {
    string input;
    cout << "Insira o texto:\n" ;
    getline(cin,input);
    length = input.size();
    buffer = new char[length];
    for (int i = 0 ; i < length; ++i) buffer[i] = input[i];
  }

  auto frequencies = getFrequencies(buffer,length);
  auto root = buildHuffmanTree(frequencies);

  map<char,string> codeMap;
  fillCodeMap(root,codeMap);

  map<string,char> encodeMap;

  for (auto p : codeMap) {
    printf("\nchar %c, code: %s, codeLength: %lu", p.first, p.second.c_str(),p.second.size());
    encodeMap[p.second] = p.first;
  }

  string bitstring;
  for (int i = 0; i < length; ++i) {
    bitstring += codeMap[buffer[i]];
  }

  string userString, decoded;
  cout << "\nString codificada:\n" << bitstring;
  cout << "\nInsira a string a ser decodificada:" ;
  cin >> userString;

  for (size_t i = 0; i < userString.size();) {
    string aux;
    if (userString[i] != '1' && userString[i] != '0') continue;
    while (encodeMap.count(aux)==0) {
      aux+=userString[i];
      i++;
    }
    decoded += encodeMap[aux];
  }

  cout << decoded;

  char paddingMask;
  int size = bitstring.size()%8 == 0 ? bitstring.size()/8 : bitstring.size()/8 + 1;
  char* bytes = bitstringToByteArray(bitstring, size, &paddingMask);

  // first byte will be padding mask. Remaining ones will be the huffman tree.

  ofstream out("out", ios::out | ios::binary);
  vector<char> header;
  writeHeader(header, paddingMask,root);

  char * headerBytes = new char[header.size()];
  copy(header.begin(), header.end(), headerBytes);

  out.write(headerBytes, header.size());
  out.write(bytes, size);

  delete[] buffer;
  delete[] headerBytes;
}

void writeHeader(vector<char>& header, char paddingMask, Node* root) {
  header.push_back(paddingMask);
  Marshal(root, header);
  header.push_back('\n');
}

char* bitstringToByteArray(string bits, int size, char* paddingMask) {
  *paddingMask = (0xff<<(8-(bits.size()%8)));
  char * buf = new char[size];

  while(!(bits.size()%8==0)) {
    bits += "0";
  }

  for (int i = 0 ; i<size; ++i) {
    char byte = 0;
    for (int j = 7; j >= 0; --j) {
      byte = byte | (bits[7-j+8*i]-'0') << j;
    }
    buf[i] = byte;
  }
  return buf;
}

map<char,int> getFrequencies(char* buffer, int length) {
  map<char,int> frequencies;
  char c;
  for (int i = 0 ; i < length; ++i) {
    c = buffer[i];
    if (c=='\n') continue;
    frequencies[c]++;
  }
  return frequencies;
}

Node* buildHuffmanTree(map<char,int>& freq) {
  priority_queue<Node*, vector<Node*>, CompareNodes> pq;

  for (auto p : freq) {
    auto node = new Node(p.first, p.second);
    pq.push(node);
  }

  while(pq.size() != 1) {
    auto first = pq.top();
    pq.pop();
    auto second = pq.top();
    pq.pop();

    auto newNode = new Node(nullchar, first->freq+second->freq);
    newNode->left = first;
    newNode->right = second;
    pq.push(newNode);
  }

  return pq.top();
}

void decodeMode(int argc, char** argv) {
  ifstream binFile("out", ios::in | ios::binary);

  string line;
  getline(binFile,line);

  char paddingMask = line[0];
  string tree = line.substr(1,line.size()-1);

  stringstream stream(tree);
  Node* root;
  Unmarshal(root, stream);

  map<char,string> codeMap;
  map<string,char> encodeMap;
  fillCodeMap(root,codeMap);

  for (auto p : codeMap) {
    encodeMap[p.second] = p.first;
  }

  getline(binFile,line);
  string textString;

  string bitstring = getTextBitString(line,paddingMask);

  string decodedString = getDecodedString(bitstring, encodeMap);
  cout << endl << endl << decodedString << endl << endl ;
}

