#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

class TreeNode{
public:
    TreeNode():leftLink(NULL),rightLink(NULL),mark(-1),times(0){};
    friend class HMTree;
private:
    int mark;
    int times;
    TreeNode *leftLink;
    TreeNode *rightLink;
};

class HMTree{
public:
    HMTree():root(NULL){};
    void build_tree(char mark,string path) ;
    void depress_code(string inname);
    string dec_to_bin(int test);
    void output(string inname,string outname);

private:
    TreeNode* root;
};


int main(int argc,char* argv[]){
    string inname,outname;
    inname = argv[1];
    size_t found = inname.find(".compress");
    outname = inname.substr(0,found) + "_decompressed.txt";
    HMTree h;
    h.depress_code(inname);
    h.output(inname,outname);

    
    return 0;
}

void HMTree::depress_code(string inname){
    ifstream fin;
    fin.open(inname , ios::binary | ios::in);
    char temp;
    string HMcode;
    string decode;
    root = new TreeNode;
    fin.get(temp);
    for(int i=0;i<(int)temp;i++){
        char mark;
        char code[32];
        fin.get(mark);
        for(int k=0;k<32;k++){
            fin.get(code[k]);
            HMcode = HMcode + dec_to_bin((int)code[k]);
        }
        size_t found = HMcode.find("0");
        decode = HMcode.substr(found+1);
        build_tree(mark, decode);
        decode.clear();
        HMcode.clear();
    }
    fin.close();
    
}

void HMTree::build_tree(char mark, string path){
    TreeNode* now = root;
    for(int i=0;i<path.length();i++){
        if(path[i]=='1'){
            if(now->rightLink == NULL)now->rightLink = new TreeNode;
            now = now->rightLink;
        }
        else if(path[i]=='0'){
            if(now->leftLink==NULL)now->leftLink = new TreeNode;
            now = now ->leftLink;
        }
    }
    now->mark = mark;
    
}

string HMTree::dec_to_bin(int test){
    string temp;
    int check;
    check = (test + 256) % 256;
    while(check>0){
        if(check%2==1) temp = temp + '1';
        else if(check%2==0) temp = temp + '0';
        check = check / 2;
    }
    while(temp.size()!=8){
        temp = temp + '0';
    }
    int len = (int)temp.length();
    int n=len-1;
    for(int i=0;i<(len/2);i++){
      swap(temp[i],temp[n]);
      n = n-1;
    }
    return temp;
}

void HMTree::output(string inname, string outname){
    ifstream fin;
    ofstream fout;
    fin.open(inname , ios::in | ios::binary);
    fout.open(outname, ios::out);
    char temp;
    char count = '\0';
    fin.get(temp);

    fin.seekg(1+33*(int)temp ,fin.beg);
    int last_element=0,now_ptr=0;
    while(!fin.eof()){
        fin.get(count);
        last_element++;

    }
    fin.close();

    fin.open(inname, ios::in | ios::binary);
    
    fin.seekg(1+33*(int)temp,fin.beg);
 
    TreeNode* now=root;
    char word;
    bool ok=false;
    string path;
    fin.get(word);
    while(!fin.eof()){
        path = dec_to_bin((int)word);
        if(now_ptr==last_element-3) {
            path = path.substr(0,8-(int)count);ok=true;
        }
        for(int i=0;i<path.size();i++){
            if(path[i]=='1') now = now ->rightLink;
            else if(path[i] == '0') now = now->leftLink;
            if(now->leftLink == NULL && now->rightLink == NULL){
                fout << (char)now->mark;
                now = root;
            }
        }
        if(ok)break;
        now_ptr++;
        path.clear();
        fin.get(word);
        
    }
    
    fin.close();
    fout.close();
    
}
