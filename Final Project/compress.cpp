#include<iostream>
#include<fstream>
#include<queue>
#include<string>
#include<vector>
#include<cmath>

using namespace std;


class TreeNode{
public:
    TreeNode():leftLink(NULL),rightLink(NULL),mark(-1),times(0){};
    friend class HMTree;
private:
    int mark;
    int times;
    string HMcode;
    TreeNode *leftLink;
    TreeNode *rightLink;
};

typedef TreeNode* node_ptr;

class HMTree{
public:
    HMTree();
    void add_to_array(int temp);
    void put_in_queue();
    void build_tree();
    void calculate_code(TreeNode* node,string code);
    void output_code(string inname,string outname);
    int bin_to_dec(const string& s) ;
    TreeNode* get_root();
    class compare{
    public:
        bool operator() (const node_ptr &cmp1, const node_ptr &cmp2) const{
            return (cmp1->times > cmp2->times);
        }
    };

private:
    TreeNode* root;
    TreeNode* arr[256];
    int count;
    priority_queue<TreeNode*,vector<TreeNode*>,compare> pq;
};



int main(int argc, char* argv[]){
    ifstream fin;
    string inname,outname,cmp;
    inname = argv[1] ;
    cmp = ".txt";
    size_t found = inname.find(cmp);
    outname =  inname.substr(0,found) + ".compress";
    int a=0,e=0,i=0,o=0,u=0,number=0;
    char temp;
    
    fin.open(argv[1]);
    
    HMTree h;
    
    fin.get(temp);
    while(!fin.eof()){
        switch(temp){
            case 'a': a++; number++; break;
            case 'e': e++; number++; break;
            case 'i': i++; number++; break;
            case 'o': o++; number++; break;
            case 'u': u++; number++; break;
            default: number++; break;
        }
        h.add_to_array((int)temp);
        fin.get(temp);
    }
    fin.close();
    h.put_in_queue();
    h.build_tree();
    h.calculate_code(h.get_root(),"");
    h.output_code(inname,outname);

    cout << number << endl << a << endl << e << endl << i << endl << o << endl << u << endl;
    return 0;
        
}

void HMTree::calculate_code(TreeNode* node, string code){
    if(node->leftLink == NULL && node->rightLink == NULL){
        node -> HMcode = code;
    }
    else {
        calculate_code(node->leftLink, code+"0");
        calculate_code(node->rightLink,code+"1");

    }
    
    
}

void HMTree::build_tree(){
    if(pq.size()==1){
        root = new TreeNode;
        root->leftLink = pq.top();
        root->rightLink = pq.top();
    }
    while(pq.size()>1){
        root = new TreeNode;
        root->leftLink = pq.top();
        root->times = pq.top()->times;
        pq.pop();
        root->rightLink = pq.top();
        root->times = root->times + pq.top()->times;
        pq.pop();
        pq.push(root);
    }
    
}

void HMTree::put_in_queue(){
    for(int i=0;i<256;i++){
        if(arr[i]->times > 0){
            pq.push(arr[i]);
            count ++;
        }
    }
    
}

void HMTree::add_to_array(int temp){
    
    arr[temp]->times ++;
    
}

HMTree::HMTree(){
    count = 0;
    for(int i=0;i<256;i++){
        arr[i] = new TreeNode;
        arr[i]->mark = i;
        arr[i]->times = 0;
    }
}

TreeNode* HMTree::get_root(){
    return root;
}


int HMTree::bin_to_dec(const string &s){
    int n = stoi(s);
    int output=0, i=0, remainder;
    while(n!=0){
        remainder = n%10;
        n = n/10;
        output = output + remainder*pow(2,i);
        i++;
    }
    return output;
    
}

void HMTree::output_code(string inname,string outname){
    ifstream fin;
    ofstream fout;
    string s ,last,code;
    fin.open(inname);
    fout.open(outname,ios::binary | ios::out);
    fout << (char)count;
    for(int i=0;i<256;i++){
        if(arr[i]->times > 0){
            fout << (char)i ;
            for(int k=0;k<(255-arr[i]->HMcode.size());k++) code = code + "1";
            code = code + "0";
            code = code + arr[i]->HMcode;
            last = (char)bin_to_dec(code.substr(0,8));
            for(int m=0;m<31;m++){
                code = code.substr(8);
                last = last + (char)bin_to_dec(code.substr(0,8));
            }
            fout << last;
            last.clear();
            code.clear();
        }
    }
    last.clear();
    char test;
    fin.get(test);
    while(!fin.eof()){
        s = s + arr[(int)test]->HMcode;
        while(s.size()>8){
            last = last + (char)bin_to_dec(s.substr(0,8));
            s = s.substr(8);
        }
        fin.get(test);
    }
    int count = 8 - (int) s.size();
    
    if(s.size()<8){
        s.append(count,'0');
    }
    last = last + (char)bin_to_dec(s);
    last = last + (char)count;
        
    fout << last;
    fin.close();
    fout.close();
}

