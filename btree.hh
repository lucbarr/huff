#pragma once

#include <stdio.h>
#include <vector>
#include <sstream>

#ifndef MARKER
#define MARKER 0
#endif

struct Node {
	char key;
  int freq;
	struct Node* left, *right;

  Node(char key, int freq);
  Node() = default;
};

struct CompareNodes {
  bool operator()(Node* e1,Node* e2) { return e1->freq > e2->freq; }
};

Node* newNode(char key);
void Marshal(Node *root, std::vector<char>& fp);
void Unmarshal(Node *&root, std::stringstream& stream);
void printInorder(Node *root);
