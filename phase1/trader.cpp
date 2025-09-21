#include "receiver.h"
#include <vector>


int qwe = 0;
// class start
class RedBlackTree
{
private:
    // Node creating subclass
    struct Node
    {
        std::string data;
        Node *left;
        Node *right;
        char colour;
        Node *parent;
        std::string value;
        std::string sellvalue;
        std::string buyvalue;

        Node(std::string data , std::string value) : data(data), left(nullptr), right(nullptr), colour('R'), parent(nullptr), value(value) ,sellvalue(" ") , buyvalue(" ") {}
    };
//trader
    Node *root;
    bool ll; // Left-Left Rotation flag
    bool rr; // Right-Right Rotation flag
    bool lr; // Left-Right Rotation flag
    bool rl; // Right-Left Rotation flag

    // Function to perform Left Rotation
    Node *rotateLeft(Node *node)
    {
        Node *x = node->right;
        Node *y = x->left;
        x->left = node;
        node->right = y;
        node->parent = x;
        if (y != nullptr)
            y->parent = node;
        return x;
    }

    // Function to perform Right Rotation
    Node *rotateRight(Node *node)
    {
        Node *x = node->left;
        Node *y = x->right;
        x->right = node;
        node->left = y;
        node->parent = x;
        if (y != nullptr)
            y->parent = node;
        return x;
    }

    // Helper function for insertion
    Node *insertHelp(Node *root, std::string data , std::string value)
    {
        bool f = false; // Flag to check RED-RED conflict

        if (root == nullptr)
            return new Node(data,value);
        else if (data < root->data)
        {
            root->left = insertHelp(root->left, data,value);
            root->left->parent = root;
            if (root != this->root)
            {
                if (root->colour == 'R' && root->left->colour == 'R')
                    f = true;
            }
        }
        else
        {
            root->right = insertHelp(root->right, data , value);
            root->right->parent = root;
            if (root != this->root)
            {
                if (root->colour == 'R' && root->right->colour == 'R')
                    f = true;
            }
        }

        // Perform rotations
        if (ll)
        {
            root = rotateLeft(root);
            root->colour = 'B';
            root->left->colour = 'R';
            ll = false;
        }
        else if (rr)
        {
            root = rotateRight(root);
            root->colour = 'B';
            root->right->colour = 'R';
            rr = false;
        }
        else if (rl)
        {
            root->right = rotateRight(root->right);
            root->right->parent = root;
            root = rotateLeft(root);
            root->colour = 'B';
            root->left->colour = 'R';
            rl = false;
        }
        else if (lr)
        {
            root->left = rotateLeft(root->left);
            root->left->parent = root;
            root = rotateRight(root);
            root->colour = 'B';
            root->right->colour = 'R';
            lr = false;
        }

        // Handle RED-RED conflicts
        if (f)
        {
            if (root->parent->right == root)
            {
                if (root->parent->left == nullptr || root->parent->left->colour == 'B')
                {
                    if (root->left != nullptr && root->left->colour == 'R')
                        rl = true;
                    else if (root->right != nullptr && root->right->colour == 'R')
                        ll = true;
                }
                else
                {
                    root->parent->left->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            else
            {
                if (root->parent->right == nullptr || root->parent->right->colour == 'B')
                {
                    if (root->left != nullptr && root->left->colour == 'R')
                        rr = true;
                    else if (root->right != nullptr && root->right->colour == 'R')
                        lr = true;
                }
                else
                {
                    root->parent->right->colour = 'B';
                    root->colour = 'B';
                    if (root->parent != this->root)
                        root->parent->colour = 'R';
                }
            }
            f = false;
        }
        return root;
    }

    Node* searchhelp(Node *root, std::string data)
    {
        if(root == NULL || root->data == data) return root;
        
        if (root->data < data){
            return searchhelp(root->right, data); 
        }else{
            return searchhelp(root->left, data);
        }
    }

public:
    RedBlackTree() : root(nullptr), ll(false), rr(false), lr(false), rl(false) {}

    // Function to insert data into the tree
    void insert(std::string data , std::string value )
    {
        if (root == nullptr)
        {
            root = new Node(data , value);
            root->colour = 'B';
        }
        else
            root = insertHelp(root, data , value );
    }

    Node* search(std::string data){
        return searchhelp(root , data);
    }

    bool solve1(std::string name , std::string price , std::string bs){
        
        Node* ss;
        ss = search(name);

        if(ss==NULL){
            insert(name,price);
            Node* kk;
            kk = search(name);
            kk->value = price;
            return true;
        }

        if(bs == "b"){//sell

            qwe = 5;

            if(ss->buyvalue != " "){
                if(std::stoi(ss->buyvalue) < std::stoi(price)){
                    ss->buyvalue = price;
                    if(ss->buyvalue == ss->sellvalue){
                        ss->sellvalue = " ";
                        ss->buyvalue = " ";
                        return false;
                    }
                }
                else{
                    return false;               
                }

            }else{
                ss->buyvalue = price;
                if(ss->buyvalue == ss->sellvalue){
                    ss->sellvalue = " ";
                    ss->buyvalue = " ";
                    return false;
                }
            }
            

            if(std::stoi(ss->value) < std::stoi(price)){
                ss->value = price ;
                ss->buyvalue = " ";
                return true;
            }
            return false;
        }

        if(bs == "s"){

            if(ss->sellvalue != " "){
                if(std::stoi(ss->sellvalue) > std::stoi(price)){
                    ss->sellvalue = price;
                    if(ss->buyvalue == ss->sellvalue){
                        ss->sellvalue = " ";
                        ss->buyvalue = " ";
                        return false;
                    }
                }else{
                    return false;
                }
            }else{
                ss->sellvalue = price;
                if(ss->buyvalue == ss->sellvalue){
                    ss->sellvalue = " ";
                    ss->buyvalue = " ";
                    return false;
                }
            }

            if(std::stoi(ss->value) > std::stoi(price)){
                ss->value = price ;
                ss->sellvalue = " ";
                return true;
            }
            return false;         
        }
    }

};


void part1(std::string message){
    
    int i=-1;

    RedBlackTree tree;

    while(i==-1 || message[i] != '$'){

        std::string name , price , bs ;

        i++;

        //reading name
        while(message[i]!=' '){
            name += message[i];
            i++;
        }
        i++;

        //reading price
        while(message[i]!=' '){
            price += message[i];
            i++;
        }
        i++;
        
        //reading bs
        bs = message[i];
        i = i+2;

        bool ans;

        ans = tree.solve1(name , price , bs);

        if(!ans){
            std::cout << "No Trade" <<std::endl ;
        }else{
            if(bs == "b"){
                std::cout << name << " "<<price<<" "<<"s"<<std::endl ;
            }
            if(bs == "s"){
                std::cout << name << " "<<price<<" "<<"b"<<std::endl ;
            }
        }
    }

}


//part 1 motham ayindha inka kadilinchakudadhu

struct everyl
{
    std::vector<int> qqq;
    int ppp;
    char b_s;
};

    std::vector<everyl> line_data;
    std::vector<everyl> line_data2;
    std::vector<std::string> all_company;
    std::vector<std::string> all_company2;
    std::vector<int> quant_start;



void arbitarage(std::vector<int> line_presence,std::vector<int>& arb_presence,std::vector<std::vector<int>>& all_arbitarage,std::vector<int>& all_arb_prices,int index)
{
    if(line_presence.size() == index){
        everyl add;

        add.ppp = 0;
        bool vbg = false;

        for(int q=0;q<all_company.size();q++){
            add.qqq.push_back(0);
        }

        for(int q=0;q<line_presence.size();q++){

            if(arb_presence[q]==1 && line_data[q].b_s=='b'){
                vbg = true;
                for(int w=0;w<all_company.size();w++){
                    add.qqq[w] -= line_data[q].qqq[w];
                }
                add.ppp += line_data[q].ppp;
            }
            if(arb_presence[q]==1 && line_data[q].b_s=='s'){
                vbg = true;
                for(int w=0;w<all_company.size();w++){
                    add.qqq[w] += line_data[q].qqq[w];
                }
                add.ppp -= line_data[q].ppp;
            }
        }

        bool check = true;
        for(int q=0;q<all_company.size();q++){
            if(add.qqq[q]!=0){
                check=false;
                break;
            }
        }

        if(check && vbg && add.ppp > 0){
            all_arbitarage.push_back(arb_presence);
            all_arb_prices.push_back(add.ppp);
        }

        return;
    }

    if(line_presence[index]==1){

    arb_presence.push_back(1);

    arbitarage(line_presence,arb_presence,all_arbitarage,all_arb_prices,index+1);
    arb_presence.pop_back(); 
    
    }  

    if(index != line_presence.size()-1){
        arb_presence.push_back(0);
        arbitarage(line_presence,arb_presence,all_arbitarage,all_arb_prices,index+1);
        arb_presence.pop_back();
    }

}


//cancelation oka sari chudali

void cancelation(std::vector<int>& line_presence,std::vector<std::string> lines2){

    std::vector<std::string> permu;
    std::vector<std::vector<int>> sbia; 

    // finding line_presence
    for(int i=0;i<line_presence.size();i++){
        //if line is present
        if(line_presence[i]==1){

            bool found = false;
            int j;
            
            for(j=0;j<permu.size();j++){
                if(permu[j]==lines2[i]){
                    found = true;
                    break;
                }
            }

            // if line is found
            if(found==true){
                // if the given line is buy
                if(line_data[i].b_s =='b'){
                    //if there are no buy before it
                    if(sbia[j][1]==-1){

                        sbia[j][0] = line_data[i].ppp;
                        sbia[j][1] = i;

                        // cancelation
                        if(sbia[j][3] != -1){

                            if(sbia[j][0]==sbia[j][2]){
                                line_presence[sbia[j][1]] = 0;
                                line_presence[sbia[j][3]] = 0;
                                sbia[j][1]=-1;
                                sbia[j][3] = -1;
                            }

                        }
                    }
                    // if there is line before it
                    else{
                        // if the given line preferd
                        if(line_data[i].ppp > sbia[j][0]){
                            sbia[j][0] = line_data[i].ppp;
                            line_presence[sbia[j][1]] = 0;
                            sbia[j][1] = i;
                        }
                        else{
                            line_presence[i] = 0;
                        }

                        if(sbia[j][3] != -1){

                            if(sbia[j][0]==sbia[j][2]){
                                line_presence[sbia[j][1]] = 0;
                                line_presence[sbia[j][3]] = 0;
                                sbia[j][1]=-1;
                                sbia[j][3] = -1;
                            }

                        }
                        
                    }
                }

                // if the given order is sell
                if(line_data[i].b_s =='s'){
                    //if  there is no sell before hand
                    if(sbia[j][3]==-1){

                        sbia[j][2] = line_data[i].ppp;
                        sbia[j][3] = i;

                        if(sbia[j][1] != -1){

                            if(sbia[j][0]==sbia[j][2]){
                                line_presence[sbia[j][1]] = 0;
                                line_presence[sbia[j][3]] = 0;
                                sbia[j][1]=-1;
                                sbia[j][3] = -1;
                            }

                        }
                    }
                    //if there is sell before
                    else{

                        if(line_data[i].ppp < sbia[j][2]){
                            sbia[j][2] = line_data[i].ppp;
                            line_presence[sbia[j][3]] = 0;
                            sbia[j][3] = i;
                        }
                        else{
                            line_presence[i] = 0;
                        }

                        if(sbia[j][1] != -1){

                            if(sbia[j][0]==sbia[j][2]){
                                line_presence[sbia[j][1]] = 0;
                                line_presence[sbia[j][3]] = 0;
                                sbia[j][1]=-1;
                                sbia[j][3] = -1;
                            }

                        }
                        
                    }
                }

            }
            else{

                permu.push_back(lines2[i]);

                std::vector<int> x(4);

                if(line_data[i].b_s == 'b'){
                    x[0] = line_data[i].ppp;
                    x[1] = i;
                    x[2] = 0;
                    x[3] = -1;
                    sbia.push_back(x);
                }
                if(line_data[i].b_s == 's'){
                    x[2] = line_data[i].ppp;
                    x[3] = i;
                    x[0] = 0;
                    x[1] = -1;
                    sbia.push_back(x);
                }
            }
        }
    }
}


void part2(std::string message)
{
    std::vector<std::string> lines;

    int i = -1;

    while(i==-1 || message[i] != '$'){

        std::string line;
        i++;
        //reading line
    
        while(message[i]!='#'){
            line += message[i];
            i++;
        }
        i++;

        lines.push_back(line);
        //end of reading line
        // removing buy or sell
        line.erase(line.size()-2);


        //removing  price of that line

        int pos;

        for(pos = line.size()-1;pos>=0;pos--){
            if(line[pos]==' '){break;}
        }

        line  = line.substr(0,pos);


        //adding company to the all company vector

        std::istringstream iss(line);
        std::string token;

        while (iss >> token) {
            if((token[0]>=65 && token[0]<=90)||(token[0]>=97 && token[0]<=122)){

                bool found = false;

                for (int v=0;v<all_company.size();v++) {
                    if (all_company[v] == token) {
                        found = true;
                        break;
                    }
                }

                if(found==false){
                    all_company.push_back(token);
                }
            }
        }
    }

    //sorting all companies the order

    std::sort(all_company.begin(), all_company.end());
    

    for(int x=0;x<lines.size();x++){
        
        std::string curr  =lines[x];

        everyl ccc;

        ccc.b_s = curr[curr.size()-1];

        curr.erase(curr.size()-2);

        //removing  price of that line

        int pos;

        for(pos = curr.size()-1;pos>=0;pos--){
            if(curr[pos]==' '){break;}
        }

        ccc.ppp = std::stoi(curr.substr(pos+1));

        curr  = curr.substr(0,pos);

        //adding company to the all company vector

        std::istringstream iss(curr);
        std::string token;

        for(int o=0;o<all_company.size();o++){
            ccc.qqq.push_back(0);
        }

        int h;

        while (iss >> token) {
            if(token[0]>=65 && token[0]<=90){

                for (int v=0;v<all_company.size();v++) {
                    if (all_company[v] == token) {
                        h = v;
                        break;
                    }
                }
            }
            else{
                ccc.qqq[h] = std::stoi(token);
            }
        }

        line_data.push_back(ccc);
    }

    std::vector<int> line_presence;
    int ans = 0;

    std::vector<std::string> lines2(line_data.size());

    for(int r=0;r < line_data.size();r++){
        for(int w = 0;w < all_company.size();w++){
            lines2[r] += std::to_string(line_data[r].qqq[w]);
        }
    }


    for(int e=0;e<line_data.size();e++){

        std::vector<int> arb_presence;


        line_presence.push_back(1);

        cancelation(line_presence,lines2);

        std::vector<std::vector<int>> all_arbitarage;
        std::vector<int> all_arb_prices;


        arbitarage(line_presence,arb_presence,all_arbitarage,all_arb_prices,0);


        if(all_arbitarage.size()==0){
            std::cout<<"No Trade"<<std::endl;
        }
        else{
            int z = 0;
            for(int r=0;r<all_arb_prices.size();r++){
                if(all_arb_prices[r]>all_arb_prices[z]){
                    z = r;
                }
            }

            ans += all_arb_prices[z];

            for(int y = 0;y<line_presence.size();y++){
                if(all_arbitarage[z][y]==1){
                    line_presence[y]=0;
                }
            }

            for(int d=line_presence.size()-1;d>=0;d--){
                if(all_arbitarage[z][d]==1){
                    if(lines[d][lines[d].size()-1]=='b'){
                        std::string substring = lines[d].substr(0, lines[d].size() - 1);
                        std::cout << substring << "s" <<std::endl;
                    }
                    else{
                        std::string substring = lines[d].substr(0, lines[d].size() - 1);
                        std::cout << substring << "b" <<std::endl;                        
                    }
                }
            }

        }


    }

    std::cout << ans;

    return;
}


//till part 2 perfectly done

void arbitarage2(std::vector<int> line_presence,std::vector<int>& arb_presence,std::vector<std::vector<int>>& all_arbitarage,std::vector<int>& all_arb_prices,int index)
{

    if(line_presence.size() == index){

        everyl add;

        add.ppp = 0;
        bool vbg = false;

        for(int q=0;q<all_company2.size();q++){
            add.qqq.push_back(0);
        }

        for(int q=0;q<line_presence.size();q++){

            if(arb_presence[q]>0 && line_data2[q].b_s=='b'){
                vbg = true;
                for(int w=0;w<all_company2.size();w++){
                    add.qqq[w] -= arb_presence[q]*line_data2[q].qqq[w];
                }
                add.ppp += arb_presence[q]*line_data2[q].ppp;
            }
            if(arb_presence[q]>0 && line_data2[q].b_s=='s'){
                vbg = true;
                for(int w=0;w<all_company2.size();w++){
                    add.qqq[w] += arb_presence[q]*line_data2[q].qqq[w];
                }
                add.ppp -= arb_presence[q]*line_data2[q].ppp;
            }
        }

        bool check = true;
        for(int q=0;q<all_company2.size();q++){
            if(add.qqq[q]!=0){
                check=false;
                break;
            }
        }

        if(check && vbg && add.ppp > 0){
            all_arbitarage.push_back(arb_presence);
            all_arb_prices.push_back(add.ppp);
        }

        return;

    }

    if(index != line_presence.size()-1){
        arb_presence.push_back(0);
        arbitarage2(line_presence,arb_presence,all_arbitarage,all_arb_prices,index+1);
        arb_presence.pop_back();
    }


    for(int y=1;y<=line_presence[index];y++){
        arb_presence.push_back(y);
        arbitarage2(line_presence,arb_presence,all_arbitarage,all_arb_prices,index+1);
        arb_presence.pop_back();            
    }

}

struct cance{
    std::string name;
    std::vector<int> buy;
    std::vector<int> buy_index;
    std::vector<int> sell;
    std::vector<int> sell_index;
};

    std::vector<cance> aa;

void cancelation2(std::vector<int>& line_presence,std::vector<std::string> lines2,int index){

    int i;
    bool found=false;

    for(i=0;i<aa.size();i++){
        if(aa[i].name == lines2[index]){
            found = true;
            break;
        }
    }

    if(found == true){
        if(line_data2[index].b_s =='b'){
            for(int p=0;p<aa[i].sell.size();p++){
                    if(aa[i].sell[p]==line_data2[index].ppp){
                    if(line_presence[aa[i].sell_index[p]]>line_presence[index]){
                        line_presence[aa[i].sell_index[p]] -= line_presence[index];
                        line_presence[index] = 0;
                    }
                    else{
                        line_presence[index] -= line_presence[aa[i].sell_index[p]];
                        line_presence[aa[i].sell_index[p]];
                    }
                }
            }
            if(line_presence[index] > 0){

                bool ch = false;
                int h=0;

                for(int y=0;y<aa[i].buy.size();y++){
                    if(aa[i].buy[y]==line_data2[index].ppp){
                        h = y;
                        ch = true;
                        break;
                    }
                }

                if(ch){
                    line_presence[index] += line_presence[aa[i].buy_index[h]];
                    line_presence[aa[i].buy_index[h]] = 0;
                }
                else{
                    aa[i].buy.push_back(line_data2[index].ppp);
                    aa[i].buy_index.push_back(index);                    
                }

                  
            }
        }
        if(line_data2[index].b_s =='s'){
            for(int p=0;p<aa[i].buy.size();p++){
                    if(aa[i].buy[p]==line_data2[index].ppp){
                    if(line_presence[aa[i].buy_index[p]]>line_presence[index]){
                        line_presence[aa[i].buy_index[p]] -= line_presence[index];
                        line_presence[index] = 0;
                    }
                    else{
                        line_presence[index] -= line_presence[aa[i].buy_index[p]];
                        line_presence[aa[i].buy_index[p]];
                    }
                }
            }
            if(line_presence[index] > 0){

                bool ch = false;
                int h=0;

                for(int y=0;y<aa[i].buy.size();y++){
                    if(aa[i].buy[y]==line_data2[index].ppp){
                        h = y;
                        ch = true;
                        break;
                    }
                }

                if(ch){
                    line_presence[index] += line_presence[aa[i].buy_index[h]];
                    line_presence[aa[i].buy_index[h]] = 0;
                }
                else{
                    aa[i].buy.push_back(line_data2[index].ppp);
                    aa[i].buy_index.push_back(index);                    
                }


            }
        }
    }
    else{
        cance nn;
        nn.name = lines2[index];
        if(line_data2[index].b_s =='s'){
            nn.sell.push_back(line_data2[index].ppp);
            nn.sell_index.push_back(index);
        }
        if(line_data2[index].b_s =='b'){
            nn.buy.push_back(line_data2[index].ppp);
            nn.buy_index.push_back(index);
        }

        aa.push_back(nn);
    }
}


void part3(std::string message)
{

    std::vector<std::string> lines;

    int i = -1;

    while(i==-1 || message[i] != '$'){

        std::string line;
        i++;
        //reading line
    
        while(message[i]!='#'){
            line += message[i];
            i++;
        }
        i++;

        lines.push_back(line);

        //end of reading line
        // removing buy or sell
        line.erase(line.size()-2);

        //removing  price of that line

        int pos;

        for(pos = line.size()-1;pos>=0;pos--){
            if(line[pos]==' '){break;}
        }
        pos--;

        line  = line.substr(0,pos);

        for(pos = line.size()-1;pos>=0;pos--){
            if(line[pos]==' '){break;}
        }
        pos--;

        line  = line.substr(0,pos+1);

        //adding company to the all company vector

        std::istringstream iss(line);
        std::string token;

        while (iss >> token) {
            if((token[0]>=65 && token[0]<=90)||(token[0]>= 97&& token[0]<=122)){

                bool found = false;

                for (int v=0;v<all_company2.size();v++) {
                    if (all_company2[v] == token) {
                        found = true;
                        break;
                    }
                }

                if(found==false){
                    all_company2.push_back(token);
                }
            }
        }


    }

    //sorting all companies the order
    std::sort(all_company2.begin(), all_company2.end());

    for(int x=0;x<lines.size();x++){
        
        std::string curr  =lines[x];
        int no_of_sets;

        everyl ccc;

        ccc.b_s = curr[curr.size()-1];


        curr.erase(curr.size()-2);

        //removing  price of that line

        int pos;

        for(pos = curr.size()-1;pos>=0;pos--){
            if(curr[pos]==' '){break;}
        }

        no_of_sets = std::stoi(curr.substr(pos+1));
        quant_start.push_back(no_of_sets);

        curr  = curr.substr(0,pos);


        for(pos = curr.size()-1;pos>=0;pos--){
            if(curr[pos]==' '){break;}
        }

        ccc.ppp = std::stoi(curr.substr(pos+1));

        curr  = curr.substr(0,pos);


        //adding company to the all company vector

        std::istringstream iss(curr);
        std::string token;

        for(int o=0;o<all_company2.size();o++){
            ccc.qqq.push_back(0);
        }

        int h;

        while (iss >> token) {
            if(token[0]>=65 && token[0]<=90){

                for (int v=0;v<all_company2.size();v++) {
                    if (all_company2[v] == token) {
                        h = v;
                        break;
                    }
                }
            }
            else{
                ccc.qqq[h] = std::stoi(token);
            }
        }

        line_data2.push_back(ccc);

    }



    std::vector<int> line_presence;
    int ans = 0;

    std::vector<std::string> lines2(line_data2.size());

    for(int r=0;r < line_data2.size();r++){
        for(int w = 0;w < all_company2.size();w++){
            lines2[r] += std::to_string(line_data2[r].qqq[w]);
        }
    }


    for(int e=0;e<line_data2.size();e++){

        std::vector<int> arb_presence;

        line_presence.push_back(quant_start[e]);

        std::vector<std::vector<int>> all_arbitarage;
        std::vector<int> all_arb_prices;

        cancelation2(line_presence,lines2,e);


        arbitarage2(line_presence,arb_presence,all_arbitarage,all_arb_prices,0); 


        if(all_arbitarage.size()==0){
            std::cout<<"No Trade"<<std::endl;
        }
        else{

            int z = 0;

            for(int r=0;r<all_arb_prices.size();r++){
                if(all_arb_prices[r]>all_arb_prices[z]){
                    z = r;
                }
            }

            ans += all_arb_prices[z];

            for(int y = 0;y<line_presence.size();y++){
                line_presence[y] -=all_arbitarage[z][y] ;
            }

            // for(int i=0;i<line_presence.size();i++){
            //     std::cout << line_presence[i] << " ";
            // }

            for(int d=line_presence.size()-1;d>=0;d--){

                if(all_arbitarage[z][d]>0){
                    
                    for(int h=0;h<all_company2.size();h++){
                        if(line_data2[d].qqq[h] != 0){
                            std::cout << all_company2[h] << " " << line_data2[d].qqq[h] <<" ";
                        }
                    }

                    std::cout <<line_data2[d].ppp;

                    std::cout << " " << all_arbitarage[z][d]<< " ";

                    if(line_data2[d].b_s == 'b'){
                        std::cout << "s" <<std::endl; 
                    }
                    else{
                        std::cout << "b" <<std::endl;
                    }

                }
            }

        }

    }

  std::cout << ans;


}

int main(int argc, char **argv)
{

    Receiver rcv;
    std::string message = rcv.readIML();
    while(message[message.size()-1]!='$'){message.append(rcv.readIML());}

    if (argv[1][0] == '1')
    {
        part1(message);
    }
    else if (argv[1][0] == '2')
    {
        part2(message);
    }
    else
    {
        part3(message);
    }
    return 0;
}