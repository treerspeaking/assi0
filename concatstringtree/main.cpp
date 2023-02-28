#include "ConcatStringTree.h"
#include "crtdbg.h"
#include "fstream"
void run() {

    // int indexOf(char c) const
    // 3 nodes, character at node 2 - index 29

    ConcatStringTree* s = new ConcatStringTree("Assignment 2");
    ConcatStringTree* s1 = new ConcatStringTree(" DSA, ");
    ConcatStringTree* s2 = new ConcatStringTree("testcase 23!");
    ConcatStringTree* c = new ConcatStringTree(s->concat(*s1));
    ConcatStringTree* clone = new ConcatStringTree(c->subString(0, c->length()));
    ConcatStringTree* c2 = new ConcatStringTree(clone->concat(*s2));

    std::cout << c2->indexOf('!') << std::endl;
}
int main() {
    run();
}