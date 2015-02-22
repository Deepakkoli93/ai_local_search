#include <iostream>
#include <fstream>
#include <malloc.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <random>
#include <time.h>
#define max 10000
using namespace std;

struct node // bid node structure
{
	int cid; //company
	double price;
	int norc;  // no. of regions in one bid
	int region[max];
};

clock_t starttime;
float tim;
int nor;
int nob;
int noc;
struct node tob[max]; // total no. of bids

bool com[max],reg[max]; //keeps record of which companies and regions can be selected in the remaining unprocessed bids
bool bid[max]; // final bids


//function to take input - read from console by redirection
void readfile(char name[])
{
	//ifstream iFile("inputfile.txt");
	ifstream iFile(name);
	string g;
	string temp;
	getline(iFile,temp);
	tim = atof(temp.c_str());
    tim = tim * 60;
    starttime = clock();
	getline(iFile,temp);
	getline(iFile,temp);
	nor = atoi(temp.c_str());
	getline(iFile,temp);
	getline(iFile,temp);
	nob=atoi(temp.c_str());
	getline(iFile,temp);
	getline(iFile,temp);
	noc = atoi(temp.c_str());
	getline(iFile,temp);
	//scanf("%f\n\n",&tim);
	//scanf("%d\n\n",&nor);
	//scanf("%d\n\n",&nob);
	//scanf("%d\n\n",&noc);
	for(int i=0;i<nob;i++)
	{
		cout<<flush;
		string ch;
		getline(iFile,ch);
		int t=0;int j=0;
		char ch1[max];
		while(ch[t]!=' ')
		{
			ch1[j]=ch[t];
			j++;t++;
		}

		ch1[j]='\0';         // ch1 now has the first word, delimited by space--- company id
		tob[i].cid=atoi(ch1); // convert string to integer

		ch1[0]='\0';j=0;t++; // getting ready to get the second word
		while(ch[t]!=' ')
		{
			ch1[j]=ch[t];
			j++;t++;
		}
		ch1[j]='\0';			// ch1 has 2nd word -- the amount in dollars
		tob[i].price=strtod (ch1, NULL); // convert string to double
		t++;

		int x=0;
		int w=t;
		while(ch[t]!='#')
		{
			if(ch[t]==' ')
			{	x++;}
			t++;
		}                       //x has the number of regions the bid is offfering
		tob[i].norc=x;          // norc i.e. no of regions in the current bid is set
		t=w;
		for(int qq=0;qq<x;qq++)
		{
			ch1[0]='\0';j=0;
			while(ch[t]!=' ')
			{
				ch1[j]=ch[t];
				j++;t++;
			}
			t++;
			ch1[j]='\0';
			tob[i].region[qq]=atoi(ch1); //setting the regions in the current bid
		}
		getline(iFile,g);
	}
	iFile.close();
}
bool search_rbids(int);
void fill(int);
bool checkReg(int);
bool generate_neighbours(std::ofstream& myfile);
int get_most_profitable_bid(int [],int);
int get_a_good_bid(int [], int);
void copy_best_bids();
int get_total_price(int sbids[], int nosbids);

int sbids[max];
int nosbids=0; //currently selected bids and number of selected bids
int bestbids[max];
int nobestbids=0;
int bestprice=0;
int rbids[max];
int norbids=0;;

//modify this function to produce the best output(following the conditions mentioned in the assignment)
void getrandom(char name[])
{  std::ofstream myfile;
myfile.open (name);
std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,nob-1); 

	int num1,i,j;
	//srand(time(0));
	//num1=rand()%nob; //a random bid number
	num1 = dist6(rng);

	//myfile<<" first random bid number is = "<<num1<<"\n";
	nosbids = 1; // 1 bid is now selected
	sbids[0] = num1; // first selected bid number in sbids[0]
	rbids[norbids] = num1;
	fill(num1);
	/*for(i=(num1+1)%nob;i!=num1;i=(i+1)%nob)
	{
		if(com[tob[i].cid] || checkReg(i))
			continue;
		fill(i);
	}
	for(i=0;i<nob;i++)
	{
		if(bid[i]) {//{for(int j=0;j<tob[i].norc;j++)cout<<tob[i].region[j]<<" ";cout<<"# "<<tob[i].cid;cout<<endl;}
			cout<<i<<" ";}
	}
	cout<<"#"<<endl;*/

	while(true){

    bool status;
		status = generate_neighbours(myfile);
		if (status){continue;}
		else {
			if(((float)(clock()-starttime))/CLOCKS_PER_SEC<tim)
			{int totalprice=get_total_price(sbids,nosbids);
               if(totalprice>bestprice)
               { bestprice = totalprice;
                  copy_best_bids();
                  //myfile<<"best price is = "<<bestprice;
               }
                  nosbids=0;
            	   for(int z=0;z<max;z++){
            	   com[z]=false;
            	   reg[z]=false;
            	   bid[z]=false;}
            	   //getrandom();
            	   //srand(time(NULL));
            	   //int num2 = rand()%nob;
            	   int num2;
                   std::uniform_int_distribution<std::mt19937::result_type> dist6(1,nob-1); 
            	   num2 = dist6(rng);
            	   /*while(true){
            	   std::uniform_int_distribution<std::mt19937::result_type> dist6(1,nob-1); 
            	   num2 = dist6(rng);
                   if(search_rbids(num2))
                   	continue;
                   else
                   	break;

            	   }*/

            	   //myfile<<" second random bid number is = "<<num2<<"\n";
            	   nosbids=1;
            	   sbids[0] = num2;
            	   fill(num2);
                   continue;}

			else
			{ int sum1,sum2=0;
				for(int i=0;i<nobestbids;i++)
			{   sum1 += tob[bestbids[i]].price;

				//myfile<<bestbids[i]<<" "<<tob[bestbids[i]].cid<<" "<<tob[bestbids[i]].price<<"\n";
				myfile<<bestbids[i]<<" ";

			}

			myfile<<"#";
				/*for(int i=0;i<nob;i++){
					sum2 += tob[i].price;
					if(bid[i])
						myfile<<i<<" ";
				}
			myfile<<"#"<<"\n"<<sum2;*/
			//myfile<<"SHOULD BREAK OUT OF IT NOW!!!! \n";
			break;}

		}


	}
}

bool search_rbids(int num2){

  for(int i=0;i<norbids;i++){
  	if(num2==rbids[i])
  		return true;
  }
  return false;
}
//helper function of getrandom function
void fill(int bidno)
{
	com[tob[bidno].cid]=true;
	bid[bidno]=true;
	for(int i=0;i<tob[bidno].norc;i++)
	{
		reg[tob[bidno].region[i]]=true;
	}
}

//helper function of getrandom function
bool checkReg(int bidno)
{
	for(int i=0;i<tob[bidno].norc;i++)
	{
		if(reg[tob[bidno].region[i]]==true)
			return true;
	}
	return false;
}

bool generate_neighbours(ofstream& myfile){
	int possible_neighbours[max];
	int nopn=0;
    int theone; // number of possible neighbours
    //srand(time(0));
    for(int i=0;i<nob;i++)
    	{
    		if(com[tob[i].cid] || checkReg(i))
    			{//myfile<<tob[i].cid<<" " <<tob[i].price<<" was rejected \n";
    		   //myfile<<"cidalready?? "<<com[tob[i].cid]<<"\n";
    		   //myfile<<"regs "<<checkReg(i)<<"\n";

    			continue;}

    		possible_neighbours[nopn++]=i;
    		//myfile<<"next possible neighbour is "<<i<<"\n";
    	}
     if(nopn==0){return false;} // no possible neighbors found
     else{
     //theone=get_most_profitable_bid(possible_neighbours,nopn);
     	theone = get_a_good_bid(possible_neighbours,nopn);
     fill(possible_neighbours[theone]);
     //myfile<<"the chosen one is "<<possible_neighbours[theone]<<'\n';
     sbids[nosbids++]=possible_neighbours[theone];
     return true;
     }

}
int get_total_price(int sbids[],int nosbids){
	int sum=0;
	for(int i=0;i<nosbids;i++){

		 sum+=tob[sbids[i]].price;

	}
 return sum;
}

int get_most_profitable_bid(int sbids[], int nosbids){
	int max_price = tob[sbids[0]].price;
	int max_bid_at_index=0;
	for(int i=0;i<nosbids;i++){
		if(tob[sbids[i]].price>max_price)
			max_bid_at_index=i;
	}
return max_bid_at_index;
}

int get_a_good_bid(int arr[], int n){ // arr has the possible neighbours and n is the numbers of possible neighours
int max_price = (tob[arr[0]].price)/(tob[arr[0]].norc);
int max_bid_at_index=0;
for(int i=0;i<n;i++){
    if((tob[arr[i]].price / tob[arr[i]].norc) > max_price)
    	max_bid_at_index=i;
}
return max_bid_at_index;

}

void copy_best_bids(){

	for(int i=0;i<nosbids;i++){
		bestbids[i] = sbids[i];
	}

	nobestbids = nosbids;

}

int main(int argc, char *argv[])
{
    starttime=time(0);

	readfile(argv[1]);
	getrandom(argv[2]);
	return 0;
}
