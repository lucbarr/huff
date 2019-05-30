// A C++ program to demonstrate serialization and deserialiation of
// Binary Tree
#include "btree.hh"


/* A binary tree Node has key, pointer to left and right children */
struct Node;

Node::Node(char key, int freq) : key(key), freq(freq) {}

/* Helper function that allocates a new Node with the
given key and NULL left and right pointers. */
Node* newNode(char key) {
	Node* temp = new Node;
	temp->key = key;
	temp->left = temp->right = NULL;
	return (temp);
}

// This function stores a tree in a file pointed by fp
void Marshal(Node *root, std::vector<char>& fp) {
	// If current node is NULL, store marker
	if (root == NULL)
	{
		//sprintf(fp, "%c", MARKER);
    fp.push_back(MARKER);
		return;
	}

	// Else, store current node and recur for its children
	//sprintf(fp, "%c", root->key);
  fp.push_back(root->key);
	Marshal(root->left, fp);
	Marshal(root->right, fp);
}

void Unmarshal(Node *&root, std::stringstream& stream) {
	char val;
  if (stream.eof()) return;
  stream >> val;
	if (val == MARKER) return;

	root = newNode(val);
	Unmarshal(root->left, stream);
	Unmarshal(root->right, stream);
}

void printInorder(Node *root) {
	if (root != NULL) {
		printInorder(root->left);
		printf("%c ", root->key);
		printInorder(root->right);
	}
}
