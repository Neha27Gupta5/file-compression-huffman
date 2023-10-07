#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP
#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

class Node {
public:
    char data;
    int freq;
    Node* left;
    Node* right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}

    ~Node() {
        delete left;
        delete right;
    }
};

class CompareNodes {
public:
    bool operator()(Node* const& n1, Node* const& n2) {
        return n1->freq > n2->freq;
    }
};

class huffman{

private:

unordered_map<char, string> build_encoding_table(Node* root);

Node* build_huffman_tree(const unordered_map<char, int>& freq_table);

void encode(Node* root, string& encoded_str, const string& input);

void save_to_file(const string& filename, const string& data);

string read_from_file(const string& filename);

 void dfs(Node* curr, string& s);

 string serialize(Node* root);

 Node* decode(string& data);

 Node* deserialize(string data);

 string decode(Node* root, const string& encoded_str);

 int binToDec(string inStr);

 string decToBin(int inNum);

 public:
 void encoding();
 void decoding();

};

#endif




