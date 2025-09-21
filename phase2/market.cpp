#include "market.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream> 

struct singleOrder {
    int starttime;
    std::string trader;
    std::string sb;
    std::string stock;
    int price;
    int quantity;
    int totaltime;
    int endtime;
};
//market
struct sellorder{
    int starttime;
    std::string trader;
    int price;
    int quantity;
    int totaltime;
    int endtime;
};

struct buyorder{
    int starttime;
    std::string trader;
    int price;
    int quantity;
    int totaltime;
    int endtime;
};

struct tradername{
    std::string trader;
    int bought; 
    int sold;
    int net;
};

std::vector<singleOrder> orders; 
std::vector< std::pair<  std::string  ,  std::vector<sellorder>  >> slist;
std::vector< std::pair<  std::string  ,  std::vector<buyorder>   >> blist;
std::vector<tradername> traderlist;

market::market(int argc, char** argv)
{
	std::ifstream inputFile("output.txt");

    std::string line;
    while (std::getline(inputFile, line)) {

        if(line[0] == '!'){
            break; 
        }

        if (line.empty() || line[0] == '!' || line[0] == 'T' ) {
            continue;
        }


 
        int spaceCount = 0;
        for (char ch : line) {
            if (ch == ' ') {
                spaceCount++;
            }
        }

        if(spaceCount <= 6 ){

            std::istringstream iss(line);
            singleOrder order;
            std::string tempprice , tempquantity ;
            iss >> order.starttime >> order.trader >> order.sb >> order.stock >> tempprice >> tempquantity >> order.totaltime ;

            order.price = std::stoi(tempprice.substr(1)) ;
            order.quantity = std::stoi(tempquantity.substr(1)) ;

            if(order.totaltime == 0){
                order.endtime = order.starttime ;
            }else if(order.totaltime == -1){
                order.endtime = -1;
            }else{
                order.endtime = order.starttime + order.totaltime ; 
            }//order ending time is stored.

            int index = 0;

            for(int i=0; i<traderlist.size() ;i++){
                if(traderlist[i].trader == order.trader ){
                    index = 1;
                }
            }
            if(index == 0){
                tradername x;
                x.bought=0;
                x.sold=0;
                x.net=0;
                x.trader = order.trader;
                traderlist.push_back(x);
            }

            orders.push_back(order);

        }else{

            std::istringstream iss(line);
            singleOrder order;
            std::string tempprice , tempquantity , tempstock , tempnumber ;

            iss >> order.starttime >> order.trader >> order.sb;

            std::vector< std::pair< std::string , std::string> > stocklist;

            for(int j=0; j<(spaceCount-5)/2 ; j++){
                iss >> tempstock >> tempnumber ;
                stocklist.push_back(std::make_pair(tempstock , tempnumber  ));
            }

            iss >> tempprice >> tempquantity >> order.totaltime ;


            std::string stockname;

            int s=stocklist.size();
            
            for(int i=0;i<s;i++){
                int min = i;
                for(int j=i+1;j<s;j++){
                    if(stocklist[min].first > stocklist[j].first){
                        min = j;
                    }
                }
                std::swap( stocklist[i] , stocklist[min]);
            }


            for(int i=0; i<s-1;i++){
                stockname += stocklist[i].first + " " + stocklist[i].second + " ";
            }
            stockname += stocklist[s-1].first + " " + stocklist[s-1].second ;

            order.stock = stockname ;

            order.price = std::stoi(tempprice.substr(1)) ;
            order.quantity = std::stoi(tempquantity.substr(1)) ;

            if(order.totaltime == 0){
                order.endtime = order.starttime ;
            }else if(order.totaltime == -1){
                order.endtime = -1;
            }else{
                order.endtime = order.starttime + order.totaltime ; 
            }//order ending time is stored.

            int index = 0;

            for(int i=0; i<traderlist.size() ;i++){
                if(traderlist[i].trader == order.trader ){
                    index = 1;
                }
            }
            if(index == 0){
                tradername x;
                x.bought=0;
                x.sold=0;
                x.net=0;
                x.trader = order.trader;
                traderlist.push_back(x);
            }

            orders.push_back(order);

        }

    }


    inputFile.close();

}

void market::start()
{
	int money , completedtrades , sharedtrades;
    completedtrades=0;  
    money =0; 
    sharedtrades =0;


    for(const auto &order : orders ){
        if(order.sb == "SELL"){

            sellorder currorder;
            currorder.starttime = order.starttime;
            currorder.trader = order.trader;
            currorder.price = order.price;
            currorder.quantity = order.quantity;
            currorder.totaltime = order.totaltime;
            currorder.endtime = order.endtime;

            int index = -1;

            for(int i=0; i<blist.size() ; i++ ){
                if( blist[i].first == order.stock ){
                    index = i;
                }
            }

            if(index == -1){//no buy trade for that specific stock till now.
                
                
                    int index2 = -1;
                    
                    for(int i=0; i<slist.size() ; i++){
                        if( slist[i].first == order.stock){
                            index2 = i;
                        }
                    }//for rnd

                    if(index2 == -1){//no sell trade for that stock till now.
                        std::vector<sellorder> tempvector;
                        tempvector.push_back(currorder);
                        slist.push_back(make_pair( order.stock , tempvector ));
                    }else{//sell trade was present. Should add to the vector.
                        slist[index2].second.push_back(currorder);
                    }
                


            }else{

                int j=0;
                std::vector<int> possibleindices;
                
                while(j<blist[index].second.size()){
                
                    if((blist[index].second[j].endtime < currorder.starttime &&  blist[index].second[j].endtime != -1  ) || blist[index].second[j].quantity == 0 ){
                        std::swap(blist[index].second[j],blist[index].second[blist[index].second.size()-1]);
                        blist[index].second.pop_back();
                    }else{
                        if(blist[index].second[j].price >= currorder.price){
                            possibleindices.push_back(j);
                        }
                        j++;
                    }
                
                }

                //removes any order of past time and orders with 0 quantity left
                //also finds indices with higher price.
     
                int minindex;
                
                for(int j=0;j<possibleindices.size();j++){
                    minindex = j;
                
                    for(int k=j+1;k<possibleindices.size();k++){
                        if(blist[index].second[possibleindices[minindex]].price < blist[index].second[possibleindices[k]].price ){
                            minindex = k;
                        }else if(blist[index].second[possibleindices[minindex]].price == blist[index].second[possibleindices[k]].price){
                            if(blist[index].second[possibleindices[minindex]].starttime > blist[index].second[possibleindices[k]].starttime ){
                                minindex = k;
                            }else if(blist[index].second[possibleindices[minindex]].starttime == blist[index].second[possibleindices[k]].starttime ){
                                if(blist[index].second[possibleindices[minindex]].trader > blist[index].second[possibleindices[k]].trader){
                                    minindex =k;
                                }else if(blist[index].second[possibleindices[minindex]].trader == blist[index].second[possibleindices[k]].trader){
                                    if(blist[index].second[possibleindices[minindex]].totaltime > blist[index].second[possibleindices[k]].totaltime && blist[index].second[possibleindices[k]].totaltime != -1 ){
                                        minindex = k;
                                    }
                                }
                            }
                        }
                        
                    }
                
                    std::swap(possibleindices[j],possibleindices[minindex]);
                }   
                
                //sorts indices with higher price at start

                j=0;
                while( j<possibleindices.size() && currorder.quantity != 0){
                    
                    if(blist[index].second[possibleindices[j]].quantity >= currorder.quantity){
                    
                        std::cout << blist[index].second[possibleindices[j]].trader << " purchased " << currorder.quantity << " share of " << order.stock << " from " << currorder.trader << " for $" << blist[index].second[possibleindices[j]].price << "/share" << std::endl ;
                        money += (blist[index].second[possibleindices[j]].price)*(currorder.quantity);
                        completedtrades++;
                        sharedtrades+= currorder.quantity;
                    
                        for(int e=0; e<traderlist.size();e++){
                            if(traderlist[e].trader == blist[index].second[possibleindices[j]].trader){
                                traderlist[e].bought += currorder.quantity;
                                traderlist[e].net -= (blist[index].second[possibleindices[j]].price)*(currorder.quantity);
                            }
                            if(traderlist[e].trader == currorder.trader){
                                traderlist[e].sold += currorder.quantity;
                                traderlist[e].net += (blist[index].second[possibleindices[j]].price)*(currorder.quantity);
                            }
                        }
                        
                        blist[index].second[possibleindices[j]].quantity -= currorder.quantity;
                        currorder.quantity = 0;
                        //if the highest price offer has the required quantity.

                    }else{
                        
                        std::cout << blist[index].second[possibleindices[j]].trader << " purchased " << blist[index].second[possibleindices[j]].quantity << " share of " << order.stock << " from " << currorder.trader << " for $" << blist[index].second[possibleindices[j]].price << "/share" << std::endl ;

                        money += (blist[index].second[possibleindices[j]].price)*(blist[index].second[possibleindices[j]].quantity);
                        completedtrades++;
                        sharedtrades+= blist[index].second[possibleindices[j]].quantity;
                        
                        for(int e=0; e<traderlist.size();e++){
                            if(traderlist[e].trader == blist[index].second[possibleindices[j]].trader){
                                traderlist[e].bought += blist[index].second[possibleindices[j]].quantity;
                                traderlist[e].net -= (blist[index].second[possibleindices[j]].price)*(blist[index].second[possibleindices[j]].quantity);
                            }
                            if(traderlist[e].trader == currorder.trader){
                                traderlist[e].sold += blist[index].second[possibleindices[j]].quantity;
                                traderlist[e].net += (blist[index].second[possibleindices[j]].price)*(blist[index].second[possibleindices[j]].quantity);
                            }
                        }
                    
                        //if it dosent have enough quantity
                        currorder.quantity -= blist[index].second[possibleindices[j]].quantity;
                        blist[index].second[possibleindices[j]].quantity = 0;

                    }
                    j++;
                }

                if(currorder.quantity>0 ){   //if quantity>0 left after the trades then add into the sell list.
                    int index2 = -1;
                    
                    for(int i=0; i<slist.size() ; i++){
                        if( slist[i].first == order.stock){
                            index2 = i;
                        }
                    }

                    if(index2 == -1){//no sell trade for that stock till now.
                        std::vector<sellorder> tempvector;
                        tempvector.push_back(currorder);
                        slist.push_back(make_pair( order.stock , tempvector ));
                    }else{//sell trade was present. Should add to the vector.
                        slist[index2].second.push_back(currorder);
                    }

                }
            }

        }else{

            buyorder currorder;
            currorder.starttime = order.starttime;
            currorder.trader = order.trader;
            currorder.price = order.price;
            currorder.quantity = order.quantity;
            currorder.totaltime = order.totaltime;
            currorder.endtime = order.endtime;

            int index = -1;

            for(int i=0; i<slist.size() ; i++ ){
                if( slist[i].first == order.stock ){
                    index = i;
                }
            }

            if(index == -1){//no buy trade for that specific stock till now.
                

                    int index2 = -1;
                    
                    for(int i=0; i<blist.size() ; i++){
                        if( blist[i].first == order.stock){
                            index2 = i;
                        }
                    }//for rnd

                    if(index2 == -1){//no sell trade for that stock till now.
                        std::vector<buyorder> tempvector;
                        tempvector.push_back(currorder);
                        blist.push_back(make_pair( order.stock , tempvector ));
                    }else{//sell trade was present. Should add to the vector.
                        blist[index2].second.push_back(currorder);
                    }


            }else{

                int j=0;
                std::vector<int> possibleindices;
                
                while(j<slist[index].second.size()){
                
                    if((slist[index].second[j].endtime < currorder.starttime && slist[index].second[j].endtime != -1 ) || slist[index].second[j].quantity == 0 ){
                        std::swap(slist[index].second[j],slist[index].second[slist[index].second.size()-1]);
                        slist[index].second.pop_back();
                    }else{
                        if(slist[index].second[j].price <= currorder.price){
                            possibleindices.push_back(j);
                        }
                        j++;
                    }
                
                }

                //removes any order of past time and orders with 0 quantity left
                //also finds indices with higher price.

                int minindex;
                  
                for(int j=0;j<possibleindices.size();j++){
                    minindex = j;
                
                    for(int k=j+1;k<possibleindices.size();k++){
                        if(slist[index].second[possibleindices[minindex]].price > slist[index].second[possibleindices[k]].price ){
                            minindex = k;
                        }else if(slist[index].second[possibleindices[minindex]].price == slist[index].second[possibleindices[k]].price){
                            if(slist[index].second[possibleindices[minindex]].starttime > slist[index].second[possibleindices[k]].starttime ){
                                minindex = k;
                            }else if(slist[index].second[possibleindices[minindex]].starttime == slist[index].second[possibleindices[k]].starttime ){
                                if(slist[index].second[possibleindices[minindex]].trader > slist[index].second[possibleindices[k]].trader){
                                    minindex =k;
                                }else if(slist[index].second[possibleindices[minindex]].trader == slist[index].second[possibleindices[k]].trader){
                                    if(slist[index].second[possibleindices[minindex]].totaltime > slist[index].second[possibleindices[k]].totaltime && blist[index].second[possibleindices[k]].totaltime != -1 ){
                                        minindex = k;
                                    }
                                }
                            }
                        }
                        
                    }
                
                    std::swap(possibleindices[j],possibleindices[minindex]);
                }   
                
                //sorts indices with higher price at start

                j=0;
                while( j<possibleindices.size() && currorder.quantity != 0){
                    
                    if(slist[index].second[possibleindices[j]].quantity >= currorder.quantity){
                    
                        std::cout << currorder.trader  << " purchased " << currorder.quantity << " share of " << order.stock << " from " << slist[index].second[possibleindices[j]].trader << " for $" << slist[index].second[possibleindices[j]].price << "/share" << std::endl ;

                        money += (slist[index].second[possibleindices[j]].price)*(currorder.quantity);
                        completedtrades++;
                        sharedtrades+= currorder.quantity;
                        for(int e=0; e<traderlist.size();e++){
                            if(traderlist[e].trader == slist[index].second[possibleindices[j]].trader){
                                traderlist[e].sold += currorder.quantity;
                                traderlist[e].net += (slist[index].second[possibleindices[j]].price)*(currorder.quantity);
                            }
                            if(traderlist[e].trader == currorder.trader){
                                traderlist[e].bought += currorder.quantity;
                                traderlist[e].net -= (slist[index].second[possibleindices[j]].price)*(currorder.quantity);
                            }
                        }
                        
                        slist[index].second[possibleindices[j]].quantity -= currorder.quantity;
                        currorder.quantity = 0;
                        
                        //if the highest price offer has the required quantity.

                    }else{
                        
                        std::cout << currorder.trader << " purchased " << slist[index].second[possibleindices[j]].quantity << " share of " << order.stock << " from " <<  slist[index].second[possibleindices[j]].trader  << " for $" << slist[index].second[possibleindices[j]].price << "/share" << std::endl ;
                        money += (slist[index].second[possibleindices[j]].price)*(slist[index].second[possibleindices[j]].quantity);
                        completedtrades++;
                        sharedtrades+= slist[index].second[possibleindices[j]].quantity;
                        for(int e=0; e<traderlist.size();e++){
                            if(traderlist[e].trader == slist[index].second[possibleindices[j]].trader){
                                traderlist[e].sold += slist[index].second[possibleindices[j]].quantity;
                                traderlist[e].net += (slist[index].second[possibleindices[j]].price)*(slist[index].second[possibleindices[j]].quantity);
                            }
                            if(traderlist[e].trader == currorder.trader){
                                traderlist[e].bought += slist[index].second[possibleindices[j]].quantity;
                                traderlist[e].net -= (slist[index].second[possibleindices[j]].price)*(slist[index].second[possibleindices[j]].quantity);
                            }
                        }
                    
                        currorder.quantity -= slist[index].second[possibleindices[j]].quantity;
                        slist[index].second[possibleindices[j]].quantity = 0;
                        //if it dosent have enough quantity

                    }
                    j++;
                }

                if(currorder.quantity>0 ){   //if quantity>0 left after the trades then add into the sell list.
                    int index2 = -1;
                    
                    for(int i=0; i<blist.size() ; i++){
                        if( blist[i].first == order.stock){
                            index2 = i;
                        }
                    }

                    if(index2 == -1){//no sell trade for that stock till now.
                        std::vector<buyorder> tempvector;
                        tempvector.push_back(currorder);
                        blist.push_back(make_pair( order.stock , tempvector ));
                    }else{//sell trade was present. Should add to the vector.
                        blist[index2].second.push_back(currorder);
                    }
                    
                }
            }            
        }

    }

    std::cout << std::endl; 
    std::cout << "---End of Day---" << std::endl;
    std::cout << "Total Amount of Money Transferred: $" << money << std::endl;
    std::cout << "Number of Completed Trades: " << completedtrades << std::endl;
    std::cout << "Number of Shares Traded: " << sharedtrades << std::endl;

    for(int i=0;i<traderlist.size();i++){
        std::cout << traderlist[i].trader << " bought "<<traderlist[i].bought << " and sold "<<traderlist[i].sold<< " for a net transfer of $" << traderlist[i].net << std::endl ;
    }

}