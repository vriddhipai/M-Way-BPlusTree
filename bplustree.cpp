#include<bits/stdc++.h>
using namespace std;

//Order of M Way B+ Tree
//Default Value = 2
//Its value is changed in Initialise operation
long long int m = 2;

//Contains Key Value pairs for Search LR Type of Query
vector<pair<long long int,float> > searchList;

//Contains implementation for both Leaf and Non-Leaf nodes
class node{

	//0 for leaf
    //1 for other
    long long int nodeType;

    public:

	//List containing KV Pairs in Leaf Node
    vector<pair<long long int,float> > leaf_list;
    //List containing Keys and Pointers in Non-Leaf nodes
    vector<pair<long long int,node> > interim_list;
    //Pointer to parent
    node* parent;

    node(){
        leaf_list.clear();
        interim_list.clear();
    }

    ~node(){
        leaf_list.clear();
        interim_list.clear();
    }

	//Sets Node Type
	//Helps to identify Leaf or Non-leaf nodes
    void setNodeType(long long int type){
        nodeType = type;
    }

	//Getter method
    long long int getNodeType(){
        return nodeType;
    }

	//Custom comparator for <int,node> type elements
    static bool cmp(const std::pair<long long int,node>& p1, const std::pair<long long int,node>& p2){
        return p1.first<p2.first;
    }

	//Creates root and its child pointers during init phase
    void setRoot(long long int key){
        node left,right;
        left.setNodeType(0);
        right.setNodeType(0);
        left.leaf_list.clear();
        right.leaf_list.clear();
        interim_list.push_back(make_pair(-30000, left));
        interim_list.push_back(make_pair(key, right));
        left.parent = this;
        right.parent = this;
        sort(interim_list.begin(),interim_list.end(),cmp);
    }

	//Main Add Function
	//Calls itself when referencing to Non-Leaf Children
	//Or Calls Leaf-Node Add method for Leaf Children
    vector<pair<long long int,node> > add(long long int key, float value, long long int check=0){
        vector<pair<long long int,node> > right_half;
        long long int i;
        for(i=1;i<interim_list.size();i++)
        {
            if(interim_list[i-1].first <= key && key < interim_list[i].first){
                break;
            }
        }
        if(interim_list[i-1].second.getNodeType()==0){
            vector<pair<long long int,float> > ret_list = interim_list[i-1].second.leaf_add(key,value);
            interim_list[i-1].second.parent = this;
            if(ret_list.size()>0){
                node mid;
                mid.setNodeType(0);
                interim_list.push_back(make_pair(ret_list[0].first,mid));
                mid.parent = this;
                sort(interim_list.begin(),interim_list.end(),cmp);

                for(long long int j=0;j<ret_list.size();j++){
                    long long int tmp_check = 0;
                    if(j==ret_list.size()-1)
                        tmp_check = 1;
                    vector<pair<long long int,node> > empty_list = add(ret_list[j].first,ret_list[j].second, tmp_check);
                    if(empty_list.size()>0)
                        sort(empty_list.begin(),empty_list.end(),cmp);
                    right_half = empty_list;

                }
            }
        }
        else{

            vector<pair<long long int,node> > ret_list = interim_list[i-1].second.add(key,value);
            if(ret_list.size()!=0){
                long long int first_number = ret_list[0].first;
                ret_list[0].first = -30000;
                node mid;
                mid.setNodeType(1);
                interim_list.push_back(make_pair(first_number,mid));
                mid.parent = this;
                sort(interim_list.begin(),interim_list.end(),cmp);
                for(long long int j=0;j<ret_list.size();j++){
                    vector<pair<long long int,node> > empty_list = interim_list[i].second.add(ret_list[j].first,ret_list[j].second);
                }
            }
        }

        if(interim_list.size()>(m+1) && check==1){
            long long int right_size = interim_list.size() - (m/2 + 1);
            for(long long int i=0;i<right_size;i++){
                right_half.push_back(make_pair(interim_list.back().first,interim_list.back().second));
                interim_list.pop_back();
            }
        }
        return right_half;
    }

	//Adds <int,node> pairs to Key, Pointers List in Non-Leaf Nodes
    vector<pair<long long int,node> > add(long long int key, node other_node,long long int check = 0){
        vector<pair<long long int,node> > right_half;
        interim_list.push_back(make_pair(key,other_node));
        other_node.parent = this;
        sort(interim_list.begin(),interim_list.end(),cmp);
        return right_half;
    }

	//Adds KV Pairs to List in Leaf Node
    vector<pair<long long int,float> > leaf_add(long long int key, float value){
        vector<pair<long long int,float> > right_half;
        leaf_list.push_back(make_pair(key,value));
        sort(leaf_list.begin(),leaf_list.end());
        if(leaf_list.size()>m){
            long long int right_size = leaf_list.size() - (m/2);
            for(long long int i=0;i<right_size;i++){
                right_half.push_back(make_pair(leaf_list.back().first,leaf_list.back().second));
                leaf_list.pop_back();
            }
        }
        sort(right_half.begin(),right_half.end());
        return right_half;
    }

	//Mainly for Displaying B+ Tree
    void display(){
        cout<<"Complete node list: \n";
        for(long long int i=0;i<interim_list.size();i++){
            cout<<interim_list[i].first<<" , pointer, ";
        }
        cout<<endl;
        for(long long int i=0;i<interim_list.size();i++){
            cout<<endl<<interim_list[i].first<<" , pointer, \n";
            if(interim_list[i].second.getNodeType()==0)
                interim_list[i].second.leaf_display();
            else
                interim_list[i].second.display();
        }
    }

    void leaf_display()
    {
        cout<<"Leaf Display\n";
        cout<<leaf_list.size()<<endl;
        for(long long int it=0;it<leaf_list.size();it++)
        {
            cout<<"("<<leaf_list[it].first<<","<<leaf_list[it].second<<"), ";
        }
        cout<<endl;
    }

	//Delete Function
	//Returns whether deletion is stable or not for its parent to handle
    bool del(long long int key){
        if(nodeType==0){
            vector<pair<long long int,float> >::iterator iter;
            for(iter = leaf_list.begin();iter!=leaf_list.end();iter++)
            {
                if(iter->first == key){
                    break;
                }
            }
            leaf_list.erase(iter);
            if(leaf_list.size()<(m/2))
                return false;
            else
                return true;
        }
        else{
            long long int i;
            for(i=1;i<interim_list.size();i++)
            {
                if(interim_list[i-1].first <= key && key < interim_list[i].first){
                    break;
                }
            }
            bool safeDelete = interim_list[i-1].second.del(key);
            //Handle Unstable delete
            if(safeDelete==false){
                //Leaf ops
                if(interim_list[i-1].second.getNodeType()==0){
                    if(i<(interim_list.size())){
                        //borrow right
                        if(interim_list[i].second.leaf_list.size()>(m/2)){
                            pair<long long int,float> first_num = interim_list[i].second.leaf_list[0];
                            long long int second_num = interim_list[i].second.leaf_list[1].first;
                            vector<pair<long long int,float> >::iterator iter = interim_list[i].second.leaf_list.begin();
                            interim_list[i].second.leaf_list.erase(iter);
                            interim_list[i-1].second.leaf_list.push_back(first_num);
                            sort(interim_list[i-1].second.leaf_list.begin(),interim_list[i-1].second.leaf_list.end());
                            interim_list[i].first = second_num;
                            return true;
                        }
                        //combine right
                        else{
                            vector<pair<long long int,float> > rightSiblingChildren;
                            for(long long int j=0;j<interim_list[i].second.leaf_list.size();j++){
                                rightSiblingChildren.push_back(interim_list[i].second.leaf_list[j]);
                                if(j==0){
                                    node tmp_node = interim_list[i].second;
                                    while(1){
                                        if(tmp_node.getNodeType()==0)
                                            break;
                                        else
                                            tmp_node = tmp_node.interim_list[0].second;
                                    }
                                    rightSiblingChildren[j].first = tmp_node.leaf_list[0].first;
                                }
                            }

                            interim_list[i].second.leaf_list.clear();
                            interim_list.erase(interim_list.begin()+i);
                            for(long long int j=0;j<rightSiblingChildren.size();j++){
                                interim_list[i-1].second.leaf_list.push_back(rightSiblingChildren[j]);
                            }
                            sort(interim_list[i-1].second.leaf_list.begin(),interim_list[i-1].second.leaf_list.end());
                            if(interim_list.size()>=(m/2)+1)
                                return true;
                            else
                                return false;
                        }
                    }
                    else if((i-2)>=0){
                        //borrow left
                        if(interim_list[i-2].second.leaf_list.size()>(m/2)){
                            pair<long long int,float> last_num = interim_list[i-2].second.leaf_list[interim_list[i-2].second.leaf_list.size()-1];
                            interim_list[i-2].second.leaf_list.erase(interim_list[i-2].second.leaf_list.begin()+interim_list[i-2].second.leaf_list.size()-1);
                            interim_list[i-1].second.leaf_list.push_back(last_num);
                            sort(interim_list[i-2].second.leaf_list.begin(),interim_list[i-2].second.leaf_list.end());
                            interim_list[i-1].first = last_num.first;
                            return true;
                        }
                        //combine left
                        else{
                            vector<pair<long long int,float> > remainingChildren;
                            for(long long int j=0;j<interim_list[i-1].second.leaf_list.size();j++){
                                remainingChildren.push_back(interim_list[i-1].second.leaf_list[j]);
                                if(j==0){
                                    node tmp_node = interim_list[i-1].second;
                                    while(1){
                                        if(tmp_node.getNodeType()==0)
                                            break;
                                        else
                                            tmp_node = tmp_node.interim_list[0].second;
                                    }
                                    remainingChildren[j].first = tmp_node.leaf_list[0].first;
                                }
                            }
                            interim_list[i-1].second.leaf_list.clear();
                            interim_list.erase(interim_list.begin()+(i-1));
                            for(long long int j=0;j<remainingChildren.size();j++){
                                interim_list[i-2].second.leaf_list.push_back(remainingChildren[j]);
                            }
                            sort(interim_list[i-2].second.leaf_list.begin(),interim_list[i-2].second.leaf_list.end());
                            if(interim_list.size()>=(m/2)+1)
                                return true;
                            else
                                return false;
                        }
                    }
                }
                //other node ops
                else{
                    if(i<(interim_list.size())){
                        //borrow right
                        if(interim_list[i].second.interim_list.size()>(m/2)+1){
                            pair<long long int,node> first_num = interim_list[i].second.interim_list[0];
                            node tmp_node = interim_list[i].second;
                            while(1){
                                if(tmp_node.getNodeType()==0)
                                    break;
                                else
                                    tmp_node = tmp_node.interim_list[0].second;
                            }
                            first_num.first = tmp_node.leaf_list[0].first;
                            long long int second_num = interim_list[i].second.interim_list[1].first;
                            vector<pair<long long int,node> >::iterator iter = interim_list[i].second.interim_list.begin();
                            interim_list[i].second.interim_list.erase(iter);
                            interim_list[i-1].second.interim_list.push_back(first_num);
                            sort(interim_list[i-1].second.interim_list.begin(),interim_list[i-1].second.interim_list.end(),cmp);
                            interim_list[i].first = second_num;
                            return true;
                        }
                        //combine right
                        else{
                            vector<pair<long long int,node> > rightSiblingChildren;
                            for(long long int j=0;j<interim_list[i].second.interim_list.size();j++){
                                rightSiblingChildren.push_back(interim_list[i].second.interim_list[j]);
                                if(j==0){
                                    node tmp_node = interim_list[i].second;
                                    while(1){
                                        if(tmp_node.getNodeType()==0)
                                            break;
                                        else
                                            tmp_node = tmp_node.interim_list[0].second;
                                    }
                                    rightSiblingChildren[j].first = tmp_node.leaf_list[0].first;
                                }
                            }
                            interim_list[i].second.interim_list.clear();
                            interim_list.erase(interim_list.begin()+i);
                            for(long long int j=0;j<rightSiblingChildren.size();j++){
                                if(j>0){
                                    if(rightSiblingChildren[j].second.getNodeType()==0)
                                        rightSiblingChildren[j].first = rightSiblingChildren[j].second.leaf_list[0].first;
                                    else
                                        rightSiblingChildren[j].first = rightSiblingChildren[j-1].second.interim_list[rightSiblingChildren[j-1].second.interim_list.size()-1].first+1;
                                }
                                interim_list[i-1].second.interim_list.push_back(rightSiblingChildren[j]);
                            }
                            sort(interim_list[i-1].second.interim_list.begin(),interim_list[i-1].second.interim_list.end(),cmp);
                            if(interim_list.size()>=(m/2)+1)
                                return true;
                            else
                                return false;
                        }
                    }
                    else if((i-2)>=0){
                        //borrow left
                        if(interim_list[i-2].second.interim_list.size()>(m/2)+1){
                            pair<long long int,node> last_num = interim_list[i-2].second.interim_list[interim_list[i-2].second.interim_list.size()-1];
                            interim_list[i-2].second.interim_list.erase(interim_list[i-2].second.interim_list.begin()+interim_list[i-2].second.interim_list.size()-1);
                            interim_list[i-1].second.interim_list.push_back(last_num);
                            sort(interim_list[i-2].second.interim_list.begin(),interim_list[i-2].second.interim_list.end(),cmp);
                            interim_list[i-1].first = last_num.first;
                            return true;
                        }
                        //combine left
                        else{
                            vector<pair<long long int,node> > remainingChildren;
                            for(long long int j=0;j<interim_list[i-1].second.interim_list.size();j++){
                                remainingChildren.push_back(interim_list[i-1].second.interim_list[j]);
                                if(j==0){
                                    node tmp_node = interim_list[i-1].second;
                                    while(1){
                                        if(tmp_node.getNodeType()==0)
                                            break;
                                        else
                                            tmp_node = tmp_node.interim_list[0].second;
                                    }
                                    remainingChildren[j].first = tmp_node.leaf_list[0].first;
                                }
                            }
                            interim_list[i-1].second.interim_list.clear();
                            interim_list.erase(interim_list.begin()+(i-1));
                            for(long long int j=0;j<remainingChildren.size();j++){
                                interim_list[i-2].second.interim_list.push_back(remainingChildren[j]);
                            }
                            sort(interim_list[i-2].second.interim_list.begin(),interim_list[i-2].second.interim_list.end(),cmp);
                            if(interim_list.size()>=(m/2)+1)
                                return true;
                            else
                                return false;
                        }
                    }
                }
                if(interim_list.size()>(m/2))
                    return true;
                else
                    return false;
            }
            //safely deleted
            else
                return true;
        }
    }

	//Single Search Type function
    pair<bool,float> search(long long int key){
        if(nodeType==0){
            for(long long int i=0;i<leaf_list.size();i++){
                if(leaf_list[i].first>key)
                    break;
                else if(leaf_list[i].first==key)
                    return make_pair(true,leaf_list[i].second);
            }
            return make_pair(false,-1);
        }
        else{
            long long int i;
            for(i=1;i<interim_list.size();i++)
            {
                if(interim_list[i-1].first <= key && key < interim_list[i].first){
                    break;
                }
            }
            pair<bool,float> value = interim_list[i-1].second.search(key);
            return value;
        }
    }

	//Range Search function
    long long int searchLR(long long int l, long long int r){
        if(nodeType==0){
            for(long long int i=0;i<leaf_list.size();i++){
                if(leaf_list[i].first>r)
                    return 0;
                else if(leaf_list[i].first>=l && leaf_list[i].first<=r)
                    searchList.push_back(leaf_list[i]);
            }
            return 1;
        }
        else{
            long long int i;
            long long int continueFurther;
            for(i=1;i<interim_list.size();i++)
            {
                if(interim_list[i-1].first <= r && l <= interim_list[i].first)
                {
                    continueFurther = interim_list[i-1].second.searchLR(l,r);
                    if(continueFurther==0)
                        break;
                }
            }
            if(continueFurther==1){
                continueFurther = interim_list[i-1].second.searchLR(l,r);
            }
            return continueFurther;
        }
    }
};

class MWayBPTree{

	//Root node
	node root;

	public:
		MWayBPTree(){
		}

		//Initialise B+ Tree
		//Called during First Insertion
		void init(long long int key, float value=1012.134){
			root.setNodeType(1);
			root.setRoot(key);
			vector<pair<long long int,node> > ll = root.add(key,value);
		}

		//Display B+ Tree
		void display()
		{
		    cout<<endl;
		    root.display();
		    cout<<endl;
		}

		//Add Key Value Pair from here
        void insert(long long int key,float value=1012.134)
        {
            vector<pair<long long int,node> > ret_list = root.add(key,value);
            sort(ret_list.begin(),ret_list.end(),root.cmp);
            if(ret_list.size()!=0){

                long long int first_number = -30000;
                node newroot;
                root.parent = &newroot;
                newroot.interim_list.push_back(make_pair(first_number,root));

                long long int second_number = ret_list[0].first;
                ret_list[0].first = -30000;
                node mid2;
                mid2.setNodeType(1);
                mid2.parent = &newroot;
                newroot.interim_list.push_back(make_pair(second_number,mid2));
                sort(newroot.interim_list.begin(),newroot.interim_list.end(),newroot.cmp);
                root = newroot;
                for(long long int i=0;i<ret_list.size();i++){
                    long long int tmp_check = 0;
                    if(i==ret_list.size()-1)
                        tmp_check = 1;
                    vector<pair<long long int,node> > empty_list = root.interim_list[1].second.add(ret_list[i].first,ret_list[i].second,tmp_check);
                }
            }
        }

		//Function for Delete Key Value Pair
        void del(long long int key){
            bool safeDel = root.del(key);
            if(safeDel==false){
                if(root.interim_list.size()<2){
                    node newroot = root.interim_list[0].second;
                    root = newroot;
                }
            }
        }

		//Single Search
        void search(int key){
            pair<bool,double> value = root.search(key);
            if(value.first==true){
                cout<<value.second<<"\n";
            }
            else
                cout<<"Null\n";
        }

		//Range Search
        void search(int lower_limit, int upper_limit){
            searchList.clear();
            int discard = root.searchLR(lower_limit,upper_limit);
            if(searchList.size()!=0)
            {
                sort(searchList.begin(),searchList.end());
                for(int i=0;i<searchList.size();i++){
                    cout<<searchList[i].second;
                    if(i!=searchList.size()-1)
                        cout<<",";
                }
                cout<<endl;
            }
            else{
                cout<<"Null\n";
            }
        }
};


int main(int argc, char *argv[]) {

	//B+ Tree Object Declaration
    MWayBPTree tree;
    //Flag for First Time insert
    long long int flag = 0;
    //Open file passed as argument
    freopen("input1.txt","r",stdin);
//    freopen(argv[1],"r",stdin);
    freopen("output_file2.txt","w",stdout);
    string input_line;
    //Read full line at one go
    while(getline(cin,input_line)){
		cout<<" ";
    	//Intialise Method
        if(input_line.substr(0,4)=="Init")
        {
            long long int left_bracket = input_line.find("(");
            long long int right_bracket = input_line.find(")");
            string params = (string)input_line.substr(left_bracket+1,(right_bracket-left_bracket-1));
            long long int init_param = stoi(params);
            m = init_param;
        }
        //Insertion of KV Pairs
        if(input_line.substr(0,4)=="Inse")
        {
            long long int left_bracket = input_line.find("(");
            long long int right_bracket = input_line.find(")");
            string params = (string)input_line.substr(left_bracket+1,(right_bracket-left_bracket-1));
            long long int comma = params.find(",");
            string param1 = (string)params.substr(0,comma);
            string param2 = (string)params.substr(comma+2);
            long long int insert_param1 = stoi(param1);
            float insert_param2 = stof(param2);
            //For First Insertion Only
            if(flag==0){
                tree.init(insert_param1,insert_param2);
                flag = 1;
            }
            //For every other Insertion
            else{
                tree.insert(insert_param1,insert_param2);
            }
        }
        //Deletion of Key
        if(input_line.substr(0,4)=="Dele")
        {

            long long int left_bracket = input_line.find("(");
            long long int right_bracket = input_line.find(")");
            string params = (string)input_line.substr(left_bracket+1,(right_bracket-left_bracket-1));
            long long int del_param = stoi(params);
            tree.del(del_param);
        }
        //Searching
        if(input_line.substr(0,4)=="Sear")
        {

            long long int left_bracket = input_line.find("(");
            long long int right_bracket = input_line.find(")");
            string params = (string)input_line.substr(left_bracket+1,(right_bracket-left_bracket-1));
            long long int comma = params.find(",");
            //Single Search
            if(comma!=-1)
            {
                string param1 = (string)params.substr(0,comma);
                string param2 = (string)params.substr(comma+2);
                long long int search_param1 = stoi(param1);
                long long int search_param2 = stoi(param2);
                tree.search(search_param1,search_param2);
            }
            //Range Search
            else
            {
                long long int search_param = stoi(params);
                tree.search(search_param);
            }
        }
    }
    fclose(stdin);
    fclose(stdout);
    ofstream fo("output_file.txt");
    string str;
    ifstream fi("output_file2.txt");
    while(getline(fi,str))
    {
    	int ind= str.find_last_of(" ");
	if(ind!=-1)
	{
		str= str.substr(ind+1);
	}
	fo<<str<<endl;
    }
    fo.close();
    int status = remove("output_file2.txt");

    return 0;
}

