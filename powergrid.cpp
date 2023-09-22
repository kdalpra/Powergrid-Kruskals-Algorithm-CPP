
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <climits>
using namespace std;


int getElemNum(string inp, int numOfElem){ //splits a line by the commas and returns the nth component
	string temp;
	int tempnum;
	int compNum = 1;
	for(size_t i = 0; i < inp.length(); i++){
		if(inp[i] == ','){ //splits components by the space character
			if(compNum == numOfElem){
				stringstream klay(temp);
				klay >> tempnum;
				return tempnum;
			}
			else{
				temp = "";
				compNum++;
			}
		}
		else{
			temp += inp[i];
		}
	}
	return -1;
}

string getElemStr(string inp){ //returns the name of the street
	string temp;
	int compNum = 1;
	for(size_t i = 0; i < inp.length(); i++){
		if(inp[i] == ','){ //splits components by the , character
			if(compNum == 3){
				return inp.substr(i+1);
		}
		else{
			temp = "";
			compNum++;
				}
			}
			else{
				temp += inp[i];
			}
		}
		return "";
}


//gets the length of long by dividing by 10
long len(long num){
	long tempNum = 1;
	while(num > 9){
		num = num/10;
		tempNum += 1;
	}
	return tempNum;
}

//mergesort the strings by the weight (edge) value
void mergesort(string* array, string* scratch, int low, int high) {

	if(low < high){
		int mid = low +(high-low)/2;                  //mergesort from the pseudo code in lecture
		mergesort(array,scratch,low,mid);
		mergesort(array,scratch,mid+1,high); //slightly altered from inversions assignment
		int L = low;
		int H = mid+1;
		for(int k = low; k <= high; k++){
			if(L <= mid && (H > high || getElemNum(array[L],3) <= getElemNum(array[H],3))){ //compares 3rd component
				scratch[k] = array[L];
				L = L+1;
			}
			else{
				scratch[k] = array[H];
				H = H+1;
			}
		}
		for(int k = low; k <= high; k++){
			array[k] = scratch[k];
		}
	}

}
void mergesort2(string* array2, string* scratch2, int low, int high) { //merge sorts the output strings by the starting vertex value

	if(low < high){
		int mid = low +(high-low)/2;                  //mergesort from the pseudo code in lecture
		mergesort2(array2,scratch2,low,mid);     //modified to keep track of inversions
		mergesort2(array2,scratch2,mid+1,high);
		int L = low;
		int H = mid+1;
		for(int k = low; k <= high; k++){
			if(L <= mid && (H > high || getElemStr(array2[L]) < getElemStr(array2[H]))){
				scratch2[k] = array2[L];
				L = L+1;
			}
			else{
				scratch2[k] = array2[H];
				H = H+1;
			}
		}
		for(int k = low; k <= high; k++){
			array2[k] = scratch2[k];
		}
	}

}



bool checkNotThree(string inp){ //checks if the line has 4 components
	if(inp.length() < 7){ //cant be 4 components without atleast 7 chars
		return true;
	}
	int commas = 0;
	for(size_t i = 0; i < inp.length(); i++){
		if(inp[i] == ','){
			commas++;   //checks component count by # of commas
		}
	}
	return !(commas == 3);
}


bool checkNotAtoD(string inp, int num, int line){
	//checks if 1st and 2nd components of each line are within the vertex number on line 1
	string temp;
	int tempnum;
	int compNum = 1;
	for(size_t i = 0; i < inp.length(); i++){

		if(inp[i] == ','){ //splits components by the space character
			stringstream klay(temp);
			if(!(klay >> tempnum)){ // if not a num
				if(compNum == 1){
					cerr << "Error: Starting vertex \'"<< temp << "\' on line " << line << " is not among valid values 1-"<<num <<"."<<endl;
					return true;
				}
				if (compNum == 2){
					cerr << "Error: Ending vertex \'"<< temp << "\' on line " << line << " is not among valid values 1-"<<num <<"."<<endl;
					return true;
				}
				if (compNum == 3){
					cerr << "Error: Invalid edge weight \'"<< temp << "\' on line " << line << "."<<endl;
					return true;
				}
			}

			if((tempnum > num || tempnum< 1) && (compNum == 1 || compNum == 2)){ // is a number but not 1-num
				if(compNum == 1){
					cerr << "Error: Starting vertex \'"<< temp << "\' on line " << line << " is not among valid values 1-"<<num <<"."<<endl;
					return true;
				}
				if (compNum == 2){
					cerr << "Error: Ending vertex \'"<< temp << "\' on line " << line << " is not among valid values 1-"<<num <<"."<<endl;
					return true;
				}
			}
			if(compNum == 3 && tempnum < 1){
				cerr << "Error: Invalid edge weight \'"<< temp << "\' on line " << line << "."<<endl;
				return true;
			}

			else{
				temp = "";
				compNum++;
			}
		}
		else{
			temp += inp[i];
		}
	}
	return false;
}




int main(int argc, char *argv[]) {

	if(argc != 2){ //program needs 2 arguments, program and file.txt
		cerr << "Usage: ./powergrid <input file>"<<endl;
		return 1;
	}
	string fileName = argv[1]; //file.txt
	ifstream myfile;
	myfile.open(fileName);
	if(myfile){ //if the file exists and opened correctly,
		string line1;
		int line1num;

		getline(myfile,line1); //puts the first line into string
		stringstream klay(line1);
		if(!(klay >> line1num)){ //tries to put the first line into an int, which is should be
			cerr <<"Error: Invalid number of vertices \'" << line1 <<   "\' on line 1."<<endl;
			return 1;
		}

		if(line1num > 1000 || line1num < 1){ //if it is a number, it has to be 1-1000 inclusive
			cerr <<"Error: Invalid number of vertices \'" << line1 <<   "\' on line 1."<<endl;
			return 1;
		}

		int numOfLine = 2; //int to keep track of line number to find errors
		string lineTxt;
		while(getline(myfile,lineTxt)){
			if(checkNotThree(lineTxt)){ //checks if each line has 4 components
				cerr << "Error: Invalid edge data \'" << lineTxt<< "\' on line "<< numOfLine <<"."<<endl;
				return 1;
			}
			numOfLine++;
		}

		myfile.clear(); //resets the file so i can re-read it
		int seekNum = len(line1num) + 1;
		myfile.seekg(seekNum,myfile.beg); //start reading from line 2
		numOfLine = 2;

		while(getline(myfile,lineTxt)){

			if(checkNotAtoD(lineTxt,line1num, numOfLine)){ //checks first and 2nd component to make sure they are within the vertex number (1- line1num)
				return 1;
			}
			numOfLine++;
		}
		if(numOfLine-2 < line1num-1){ // checks for no solution
			cout<<"No solution."<<endl;
			return 0;
		}
		if(line1num == 1){
			cout<<"No solution."<<endl;
			return 0;
		}

		//file checked

		myfile.clear();
		myfile.seekg(seekNum,myfile.beg); //reading file from line 2 again



		int numOfEdges = numOfLine - 2;



		string *arr = new string[numOfEdges]; //puts the lines into an array

		string *scratch = new string[numOfEdges]; //scratch array for merge sort
		for(int i = 0; i < numOfEdges;i++){
			scratch[i] = "0,0,0,A";
		}

		int tempNum = 0;
		while(getline(myfile,lineTxt)){ //makes an array with a txt file line as an element
			arr[tempNum] = lineTxt;
			tempNum++;
		}

		int *roots = new int[line1num]; //array for keeping track of the roots.
		for(int i = 0; i < line1num; i++){
			roots[i] = 0;
		}

		string *last = new string[numOfEdges]; //array of output/solution edges
		for(int i = 0; i < numOfEdges;i++){
			last[i] = "0,0,0,A";
		}

		mergesort(arr,scratch,0,numOfEdges-1);

		//array is now sorted in increasing order by edge weight

		int edgesAdded = 0;
		for(int i = 0; i < numOfEdges;i++){
			if(edgesAdded != line1num - 1){

				int find1 = getElemNum(arr[i],1);
				int find2 = getElemNum(arr[i],2);
				while(roots[find1-1] != 0){   //loops to find root of a vertex
						find1 = roots[find1-1];
					}
				while(roots[find2 -1] !=0){     //loops to find root of ending vertex
					find2 = roots[find2 -1];
				}
				roots[getElemNum(arr[i],2) -1] = getElemNum(arr[i],1);      //ending vertex root is now updated to its edge/ starting vertex

				if(find1 != find2){
					last[i] = arr[i]; //if a cycle is not formed, add it
					edgesAdded++;
				}
			}
			}

		if(edgesAdded != line1num-1){
			cout <<"No solution."<<endl;
			return 0;
		}

		string *scratch2 = new string[numOfEdges]; //new scratch for sorting the solution edges
		for(int i = 0; i < numOfEdges;i++){
			scratch2[i] = "0,0,0,A";
		}

		ostringstream outs;
		int totWeight = 0;

		mergesort2(last,scratch2,0,numOfEdges-1);


		//solution edges are now sorted by street name
		unsigned int tempLen = 0; //will be length of longest street name
		for(int i = 0; i < numOfEdges; i++){
			if(getElemNum(last[i],1) !=0){
				if(getElemStr(last[i]).length() +len(getElemNum(last[i],3)) > tempLen){
					tempLen = getElemStr(last[i]).length() + len(getElemNum(last[i],3));
				}
			}
		}

		//outputs the solutions
		for(int i = 0; i < numOfEdges; i++){
			if(getElemNum(last[i],1) != 0){
				totWeight += getElemNum(last[i],3);
				string tempStr = " [" + to_string(getElemNum(last[i],3)) + "]";
				//int addNum = len(getElemNum(last[i],3));
				outs << getElemStr(last[i]) <<setw( tempLen - getElemStr(last[i]).length()+3 )<<tempStr<<endl;
			}
		}
		cout<< "Total wire length (meters): "<<totWeight <<endl <<outs.str();


		//valgrind cleanup


		delete[] scratch;
		delete[] arr;
		delete[] scratch2;
		delete[] last;
		delete[] roots;

		return 0;
	}
	else{ // file couldnt open/ doesnt exist
		cerr << "Error: Cannot open file \'" << fileName<< "\'."<<endl;
		return 1;
	}
}
