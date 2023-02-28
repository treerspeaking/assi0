#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"
class ADLNode {
public:
    int ADLid;
    ADLNode* left;
    ADLNode* right;
    int height;
    ADLNode(int ADLid = 0, ADLNode* left = nullptr, ADLNode* right = nullptr, int height = 1) {
        this->ADLid = ADLid;
        this->left = left;
        this->right = right;
        this->height = height;
    }
};
//class parenttree;
class ParentsTree {
public:
    ADLNode* root;
    int sizetree;
    ParentsTree();
    int size() const;
    string toStringPreOrder() const;
    int height(ADLNode* N);

    // A utility function to get maximum
    // of two integers
    int max(int a, int b);

    /* Helper function that allocates a
       new node with the given key and
       NULL left and right pointers. */
    ADLNode* newNode(int key);

    // A utility function to right
    // rotate subtree rooted with y
    // See the diagram given above.
    ADLNode* rightRotate(ADLNode* y);

    // A utility function to left
    // rotate subtree rooted with x
    // See the diagram given above.
    ADLNode* leftRotate(ADLNode* x);

    // Get Balance factor of node N
    int getBalance(ADLNode* N);

    // Recursive function to insert a key
    // in the subtree rooted with node and
    // returns the new root of the subtree.
    ADLNode* insert(ADLNode* node, int key);
    // Recursive function to delete a node 
// with given key from subtree with 
// given root. It returns root of the 
// modified subtree. 
    ADLNode* deleteADLNode(ADLNode* root, int key);
};
class Node {
public:
    static int newid;
    int leftlength;
    int length;
    string data;
    Node* left;
    Node* right;
    int id;
    ParentsTree partree;
    Node(int leftlength = 0, int length = 0, string data = "", Node* left = nullptr, Node* right = nullptr);
};
class ConcatStringTree {
public:
    Node* root;
    ConcatStringTree(const char* s);
    ConcatStringTree(ConcatStringTree& other);
    ConcatStringTree(ConcatStringTree&& other)noexcept;
    int length() const;
    char get(int index);
    int indexOf(char c);
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree& otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;
    ConcatStringTree& operator =(ConcatStringTree& other) noexcept;
    ConcatStringTree& operator =(ConcatStringTree&& other) noexcept;
    int getParTreeSize(const string& query) const;
    string getParTreeStringPreOrder(const string& query) const;
    ~ConcatStringTree();
};
//int ConcatStringTree::newid = 1;
//int Node::newid = 1;
/* The constructed AVL Tree would be
     9
    / \
    1 10
   / \ \
   0 5 11
  / / \
 -1 2 6
*/
class ReducedConcatStringTree; // forward declaration
class ReduceNode;
class HashConfig {
public:
    HashConfig(int p = 0, double c1 = 0, double c2 = 0, double lambda = 0, double alpha = 0, int initsize = 0);
    //HashConfig();
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;
    friend class ReducedConcatStringTree;
    friend class LitStringHash;
    //friend void free_litStringhash(LitStringHash* litStringHash);
};

struct litString {
    //bool hasdata;
public:
    long long indxpos;
    int numnode;
    string s;
    litString() {
        //hasdata = 0;
        numnode = 0;
    }
};
class LitStringHash {
public:
    HashConfig config;
    litString** hash_table;
    bool* is_tomb;//làm cái tomb
    int count;
    int size;
    int lastidx;
    LitStringHash(const HashConfig& hashConfig);
    void rehash();
    int getLastInsertedIndex() const;
    string toString() const;
    ~LitStringHash();
};
static int newidred = 1;
class reducenode {
public:
    //bool is_leaf;//nếu nó không là leaf thì data của nó sẽ chỉ đơn giản là null
    int leftlength;
    int length;
    litString* data;
    reducenode* left;
    reducenode* right;
    ParentsTree partree;
    int id;
    //int hs;// hàm băm của cái string dùng để search
    reducenode(int leftlength = 0, int length = 0, reducenode* left = nullptr, reducenode* right = nullptr);
};
class ReducedConcatStringTree /* */ {

public:
    LitStringHash* litStringHash;
    reducenode* root;
    ReducedConcatStringTree(ReducedConcatStringTree& other);
    ReducedConcatStringTree(ReducedConcatStringTree&& other);
    ReducedConcatStringTree& operator =(ReducedConcatStringTree& other) noexcept;
    ReducedConcatStringTree& operator =(ReducedConcatStringTree&& other) noexcept;
    ReducedConcatStringTree(const char* s, LitStringHash* litStringHash);
    ReducedConcatStringTree(string s);
    ReducedConcatStringTree concat(const ReducedConcatStringTree& otherS) const;
    int length() const;
    char get(int index);
    int indexOf(char c);
    string toStringPreOrder() const;
    string toString() const;
    int getParTreeSize(const string& query) const;
    string getParTreeStringPreOrder(const string& query) const;
    void deletenodered(reducenode*& root);
    void free_litStringhash(LitStringHash* litStringHash);
    ~ReducedConcatStringTree();
};

#endif // __CONCAT_STRING_TREE_H__