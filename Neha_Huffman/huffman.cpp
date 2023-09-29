#include "huffman.hpp"

unordered_map<char, string> huffman::build_encoding_table(Node* root) {
    unordered_map<char, string> encoding_table;

    if (root == nullptr) return encoding_table;

    string code = "";
    queue<pair<Node*, string>> q;
    q.push({root, code});

    while (!q.empty()) {
        Node* node = q.front().first;
        code = q.front().second;
        q.pop();

        if (node->data != '\0') {
            encoding_table[node->data] = code;
        }

        if (node->left) {
            q.push({node->left, code + "0"});
        }

        if (node->right) {
            q.push({node->right, code + "1"});
        }
    }

    return encoding_table;
}

Node* huffman::build_huffman_tree(const unordered_map<char, int>& freq_table) {
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;

    for (const auto& entry : freq_table) {
        pq.push(new Node(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* combined = new Node('\0', left->freq + right->freq);
        combined->left = left;
        combined->right = right;

        pq.push(combined);
    }

    return pq.top();
}

void huffman:: encode(Node* root, string& encoded_str, const string& input) {
    unordered_map<char, string> encoding_table = build_encoding_table(root);

    for (char c : input) {
        encoded_str += encoding_table[c];
    }
}

void huffman::save_to_file(const string& filename, const string& data) {
    ofstream output_file(filename);
    if (!output_file.is_open()) {
        cerr << "Error opening output file." << endl;
        return;
    }

    output_file << data;
    output_file.close();
}

string huffman::read_from_file(const string& filename) {
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Error opening input file." << endl;
        return "";
    }

    string data;
    char c;
    while (input_file.get(c)) {
        data += c;
    }
    input_file.close();

    return data;
}

 void huffman::dfs(Node* curr, string& s) {
    if (!curr) {
        s.push_back('*');
        return;
    }
    s.append(to_string(curr->data) + ",");
    dfs(curr->left, s);
    dfs(curr->right, s);
}

string huffman::serialize(Node* root) {
    string s = "";
    dfs(root, s);
    return s;
}

Node* huffman::decode(string& data) {
    if (data.size() == 0) return NULL;
    if (data[0] == '*') {
        data = data.substr(1);
        return NULL;
    }
    int pos = 0;
    string no = "";
    while (data[pos] != ',')
        no += data[pos++];

    Node* curr = new Node(stoi(no), 0);
    data = data.substr(pos + 1);
    curr->left = decode(data);
    curr->right = decode(data);
    return curr;
}

Node* huffman::deserialize(string data) {
    if (data.size() <= 1) return NULL;
    return decode(data);
}


string huffman::decode(Node* root, const string& encoded_str) {
    string decoded_str = "";

    Node* current = root;
    for (char bit : encoded_str) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->data != '\0') {
            decoded_str += current->data;
            current = root;
        }
    }

    return decoded_str;
}

void huffman::encoding(){
string filename_input ;
    cout<<"enter file name"<<endl;
    cin>>filename_input;

 string filename_encoded_output = "encoded.txt";
 string serialize_output="serial.txt";

    ifstream input_file(filename_input);
    // if (!input_file.is_open()) {
    //     cerr << "Error opening input file." << endl;
    //   return ;
    // }

    string input;
    char c;
    while (input_file.get(c)) {
        input += c;
    }
    input_file.close();

    unordered_map<char, int> freq_table;

    for (char c : input) {
        freq_table[c]++;
    }

    Node* root = build_huffman_tree(freq_table);

    string encoded_str = "";
    encode(root, encoded_str, input);

    save_to_file(filename_encoded_output, encoded_str);
    cout << "Encoded string saved to : " << filename_encoded_output << endl;

   string serial = serialize(root);
   save_to_file(serialize_output,serial);
}


void huffman::decoding(){


string filename_encoded_output ;
cout<<"Enter Encoded file : "<<endl;
cin>>filename_encoded_output;
    string serialize_output="serial.txt";
    string original;
    cout<<"Original file name :"<<endl;
    cin>>original;

    string encoded_data = read_from_file(filename_encoded_output);

    string serial_data = read_from_file(serialize_output);
    Node*newroot=deserialize(serial_data);
    string decoded_str2 = decode(newroot, encoded_data);
    save_to_file(original,decoded_str2);
    // cout<<"decompressed !!! "<<endl; 
    // cout<<"deserialization generated string: "<<decoded_str2<<endl;

    delete newroot;
}