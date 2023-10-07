#include "huffman.hpp"

unordered_map<char, string> huffman::build_encoding_table(Node *root)
{
    unordered_map<char, string> encoding_table;

    if (root == nullptr)
        return encoding_table;

    string code = "";
    queue<pair<Node *, string>> q;
    q.push({root, code});

    while (!q.empty())
    {
        Node *node = q.front().first;
        code = q.front().second;
        q.pop();

        if (node->data != '\0')
        {
            encoding_table[node->data] = code;
        }

        if (node->left)
        {
            q.push({node->left, code + "0"});
        }

        if (node->right)
        {
            q.push({node->right, code + "1"});
        }
    }

    return encoding_table;
}

Node *huffman::build_huffman_tree(const unordered_map<char, int> &freq_table)
{
    priority_queue<Node *, vector<Node *>, CompareNodes> pq;

    for (const auto &entry : freq_table)
    {
        pq.push(new Node(entry.first, entry.second));
    }

    while (pq.size() > 1)
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *combined = new Node('\0', left->freq + right->freq);
        combined->left = left;
        combined->right = right;

        pq.push(combined);
    }

    return pq.top();
}

// void huffman:: encode(Node* root, string& encoded_str, const string& input) {
//     unordered_map<char, string> encoding_table = build_encoding_table(root);

//     for (char c : input) {
//         encoded_str += encoding_table[c];
//     }
// }

void huffman::encode(Node *root, string &encoded_str, const string &input)
{
    unordered_map<char, string> encoding_table = build_encoding_table(root);

    string in = "";
    string s = "";

    for (char c : input)
    {
        s += encoding_table[c];

        while (s.length() > 8)
        {
            in += (char)binToDec(s.substr(0, 8));
            s = s.substr(8);
        }
       
    }
    int count = 8 - s.length();
    if (s.length() < 8)
    {
        s.append(count, '0');
    }
    in += (char)binToDec(s);
    
    in += (char)count;
    string filename_output = "compressed.huf";

 
    ofstream output_file(filename_output, ios::out | ios::binary);
    if (!output_file.is_open())
    {
        cerr << "Error opening output file." << endl;
        return;
    }

    
    output_file.write(in.c_str(), in.size());

    
    output_file.close();

    cout << "Data saved to 'compressed.huf'." << endl;

    //  string filename_input1 = "compressed.huf";
}

void huffman::save_to_file(const string &filename, const string &data)
{
    ofstream output_file(filename);
    if (!output_file.is_open())
    {
        cerr << "Error opening output file." << endl;
        return;
    }

    output_file << data;
    output_file.close();
}

string huffman::read_from_file(const string &filename)
{
    ifstream input_file(filename);
    if (!input_file.is_open())
    {
        cerr << "Error opening input file." << endl;
        return "";
    }

    string data;
    char c;
    while (input_file.get(c))
    {
        data += c;
    }
    input_file.close();

    return data;
}

void huffman::dfs(Node *curr, string &s)
{
    if (!curr)
    {
        s.push_back('*');
        return;
    }
    s.append(to_string(curr->data) + ",");
    dfs(curr->left, s);
    dfs(curr->right, s);
}

string huffman::serialize(Node *root)
{
    string s = "";
    dfs(root, s);
    return s;
}

Node *huffman::decode(string &data)
{
    if (data.size() == 0)
        return NULL;
    if (data[0] == '*')
    {
        data = data.substr(1);
        return NULL;
    }
    int pos = 0;
    string no = "";
    while (data[pos] != ',')
        no += data[pos++];

    Node *curr = new Node(stoi(no), 0);
    data = data.substr(pos + 1);
    curr->left = decode(data);
    curr->right = decode(data);
    return curr;
}

Node *huffman::deserialize(string data)
{
    if (data.size() <= 1)
        return NULL;
    return decode(data);
}

string huffman::decode(Node *root, const string &encoded_str)
{
    string decoded_str = "";

    Node *current = root;
    for (char bit : encoded_str)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        if (current->data != '\0')
        {
            decoded_str += current->data;
            current = root;
        }
    }

    return decoded_str;
}

int huffman::binToDec(string inStr)
{
    int res = 0;
    for (auto c : inStr)
    {
        res = res * 2 + c - '0';
    }
    return res;
}

string huffman::decToBin(int inNum)
{
    string temp = "", res = "";
    while (inNum > 0)
    {
        temp += (inNum % 2 + '0');
        inNum /= 2;
    }
    res.append(8 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--)
    {
        res += temp[i];
    }
    return res;
}
void huffman::encoding()
{

    string filename_input;
    cout << "Enter File Name :" << endl;
    cin >> filename_input;

    string serialize_output = "serial.txt";

    ifstream input_file(filename_input);
    if (!input_file.is_open())
    {
        cerr << "Error opening input file." << endl;
        return;
    }

    string input;
    char c;
    while (input_file.get(c))
    {
        input += c;
    }
    input_file.close();

    unordered_map<char, int> freq_table;

    for (char c : input)
    {
        freq_table[c]++;
    }

    Node *root = build_huffman_tree(freq_table);

    string encoded_str = "";
    encode(root, encoded_str, input);

    string serial = serialize(root);
    save_to_file(serialize_output, serial);
}

void huffman::decoding()
{

    string filename_input1;
    cout << "Enter huf file : " << endl;
    cin >> filename_input1;
    string serialize_output = "serial.txt";
    string original;
    cout <<"Output file name :" << endl;
    cin >> original;

    
    ifstream input_file1(filename_input1, ios::in | ios::binary);
    if (!input_file1.is_open())
    {
        cerr << "Error opening input file." << endl;
        return;
    }


    string compressed_data((istreambuf_iterator<char>(input_file1)), (istreambuf_iterator<char>()));

  
    input_file1.close();

    cout << "Data read from 'compressed.huf'." << endl;

    string in = compressed_data;

    int countback = (int)in[in.size() - 1] ;
    if (countback < 0)
    {
        countback = 0; 
    }

    string out = "";
   
    for (int i = 0; i < in.size() - 2; i++)
    {
        int check1 = (int)in[i];
        if (check1 < 0)
        {
            out += decToBin(256 + (int)in[i]);
        }
        else
        {
            out += decToBin((int)in[i]);
        }
    }
   
    string last = "";
    
    int check = (int)in[in.size() - 2];
    //    cout<<check<<endl;
    if (check < 0)
    {

        last = decToBin(256 + (int)in[in.size() - 2]);
    }
    else
    {
        last = decToBin(((int)in[in.size() - 2]));
    }
    // cout<<last<<endl;

    last = last.substr(0, 8 - countback);
    out += last;
    //   cout<<out<<endl;
    string serial_data = read_from_file(serialize_output);
    Node *newroot = deserialize(serial_data);
    string decoded_str2 = decode(newroot, out);
    save_to_file(original, decoded_str2);

    delete newroot;
}
