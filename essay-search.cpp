#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include <set>

using namespace std;

//*************************************************************
const int ALPHABET_SIZE = 26;

// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode =  new TrieNode;

    pNode->isEndOfWord = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else
// false
bool search_exact(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl->isEndOfWord);
}

bool search_prefix(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (true);
}

//*************************************************************

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){  //把所有單字轉成英文字母跟小寫
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
			{
                if(ch>=65 && ch<=90) //轉小寫
                    ch=ch+32;
				new_str.push_back(ch);
			}
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> query_parse(vector<string> tmp_string){//把query 轉成小寫，但不轉符號
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
            if(ch>=65 && ch<=90) //轉小寫
                ch=ch+32;
            new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {  //依照delim的字元分割str，遇到delim則改成\n
	vector<string> res;                                        //把所有句子照空格分成單字
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}


int main(int argc, char *argv[])
{

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route


    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

    fstream output_txt(output+".txt" ,ios::out);//輸出的txt檔
	// Read File & Parser Example


	string file, title_name, tmp;
	string query_line;
	fstream fi;
	vector<string> tmp_string,tmp_query;

    vector<TrieNode *> head_prefix;
    vector<TrieNode *> head_suffix;

    vector<string> original_title;
    set<int> ans_id;
	// from data_dir get file ....
	// eg : use 0.txt in data directory
	//int i=2;
	//************************************************************建prefix_tree for prefix search和 exact search
	int count_=0;//記錄所有有幾個data

	for(int i=0;i<=150000;i++)
    {

        fi.open(data_dir+to_string(i)+".txt", ios::in);
        if(!fi)  //如果檔案到結尾了
            break;
        count_++;
        struct TrieNode *root = getNode();

        head_prefix.push_back(root);

        // GET TITLENAME
        getline(fi, title_name);
       //cout<<title_name<<endl;
        original_title.push_back(title_name);

        // GET TITLENAME WORD
        tmp_string = split(title_name, " ");

        vector<string> title = word_parse(tmp_string);


         for(auto &word : title){
            //cout << word << endl;
            insert(root, word);
         }


        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){

            // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");

            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);


            for(auto &word : content){
               //cout << word << endl;
               insert(root, word);
            }


            //......
        }
        //cout<<original_title[0]<<endl;

        // CLOSE FILE
        fi.close();
    }
    //************************************************************建suffix tree for suffix search
    //cout<<count_<<endl;
	for(int i=0;i<=count_-1;i++)
    {
        fi.open(data_dir+to_string(i)+".txt", ios::in);
        if(!fi)  //如果檔案到結尾了
            break;

        struct TrieNode *root = getNode();

        head_suffix.push_back(root);

        // GET TITLENAME
        getline(fi, title_name);

        // GET TITLENAME WORD
        tmp_string = split(title_name, " ");

        vector<string> title = word_parse(tmp_string);


         for(auto &word : title){
            //cout << word << endl;
            while(word.length()>0)
            {
                //cout<<word<<" ";
                insert(root, word);
                word.erase (0,1);
            }
         }


        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){

            // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");

            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);


            for(auto &word : content){
               //cout << word << endl;
                while(word.length()>0)
                {
                    insert(root, word);
                    word.erase (0,1);
                }
            }
            //......
        }
        //cout<<original_title[0]<<endl;

        // CLOSE FILE
        fi.close();
    }

    //************************************************************
    //query
    fi.open(query+".txt", ios::in);
        if(!fi) cout<<"Open query failed";

        int flag=0;//1是/，2是+
        while(getline(fi,query_line)){

            // GET CONTENT WORD VECTOR
            tmp_string = split(query_line, " ");

            // PARSE QUERY
            vector<string> content = query_parse(tmp_string);

            for(auto &word : content){
               // cout<<word<<endl;
                if(word[0]=='"')
                {
                    word.erase(0,1);
                    word.erase(word.length()-1,1);
                   // cout<<word<<endl;

                    if(flag==1 || flag==0)
                    {
                        for(int i=0;i<=count_-1;i++)
                        {
                            if(search_exact(head_prefix[i], word))
                            {
                                ans_id.insert(i);
                            }

                        }

                    }
                    if(flag==2)
                    {
                       // cout<<"B"<<endl;
                        set<int> temp;
                        set<int> temp_ans=ans_id;
                        ans_id.clear();
                        for(int i=0;i<=count_-1;i++)
                        {
                            if(search_exact(head_prefix[i], word))
                            {
                                temp.insert(i);
                            }

                        }

                        for(const auto &s : temp)
                        {
                            if(temp_ans.count(s))
                                ans_id.insert(s);
                        }
                    }
                }

                else if(word[0]=='*')
                {
                    word.erase(0,1);
                    word.erase(word.length()-1,1);
                   // cout<<word<<endl;
                    if(flag==1 || flag==0)
                    {
                        for(int i=0;i<=count_-1;i++)
                        {
                            if(search_exact(head_suffix[i], word))
                            {
                                ans_id.insert(i);
                                //cout<<1<<endl;
                            }

                        }

                    }
                    if(flag==2)
                    {
                        set<int>temp;
                        set<int>temp_ans=ans_id;
                        ans_id.clear();
                        for(int i=0;i<=count_-1;i++)
                        {
                            if(search_exact(head_suffix[i], word))
                            {
                                temp.insert(i);
                            }

                        }
                        for(auto &s : temp)
                        {
                            if(temp_ans.count(s))
                                ans_id.insert(s);
                        }
                    }

                }

                else if(word[0]!='+' && word[0]!='/') //search prefix
                {
                    //cout<<word<<endl;
                    if(flag==1 || flag==0)
                    {
                        for(int i=0;i<=count_-1;i++)
                        {
                            if(search_prefix(head_prefix[i], word))
                            {
                                ans_id.insert(i);
                            }
                        }

                    }
                    if(flag==2)
                    {
                        //cout<<"A"<<endl;
                        set<int>temp;
                        set<int>temp_ans=ans_id;
                        ans_id.clear();
                        for(int i=0;i<=count_-1;i++)
                        {
                            if(search_prefix(head_prefix[i], word))
                            {
                                temp.insert(i);

                            }
                        }
                        for(auto &s : temp)
                        {
                            if(temp_ans.count(s))
                                ans_id.insert(s);
                        }
                    }
                }
                flag=0;
                if(word[0]=='+')
                {
                   // cout<<word<<endl;
                    flag=2;
                }
                if(word[0]=='/')
                {
                    //cout<<word<<endl;
                    flag=1;
                }
            }

            if(ans_id.size()>0)
            {
                for(auto &s : ans_id)
                {
                    output_txt<<original_title[s]<<endl;
                   // cout<<s<<endl;
                }
            }
            else
                output_txt<<"Not Found!"<<endl;

            ans_id.clear();
        }
    output_txt.close();
    fi.close();

    return 0;

}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
