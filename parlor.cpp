#include <iostream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <cmath>
using namespace std;

// Still need to create late strike counter and game end prompt
// sorting is a bit messy if a driver "cuts" another driver
// issue with tips displaying cents correctly; might just do whole dollars anyway


int hr = 11; // hr counter for clock
int m = 0; // minute counter for clock
string day[7] = {"MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"}; // weekly "stages"
int week = 1; // starting week "1"
float t = 1; // time modifier
int g = 0; // dispatch count temp
int c = 1; // dispatch count and flag set to 1
int input[4] = {0, 0, 0, 0}; // input up to four elements; max number of orders a driver can be dispatched
double tipAmount = 0; // counting tip total 
double tipTemp = 0; // temp variable used for function purpose
float tipMod = 0; // variable used in function to modify tips depending on lateness
int lateStrikes = 0; // late strikes set to 0
int driverNum = 4; // number of drivers(3) including null 
unsigned sleep(unsigned seconds); // time delay function

struct driverData // variable structure tied to drivers
{
	bool clockedIn; // is the driver clocked in?
	bool inStore; // is the driver in store?
	bool onDrive; // is the driver on a drive?
	string driver; // name of driver
	double bank; // accumulated tips of driver between drives
	int timeIn; // how long the driver has been in store (minutes)
	float timeOut; // how long the driver has been on drive (minutes)
	int timeLimit; // calculated time driver is out store
};

struct driverData driver[4] = // the values for drivers; null is 0
{
	{0, 0, 0, "null           ", 0, 0, 0, 0},
	{0, 0, 0, "Abraham, A.    ", 0, 0, 0, 0},
	{0, 0, 0, "Bobby, B.      ", 0, 0, 0, 0},
	{0, 0, 0, "Charlie, C.    ", 0, 0, 0, 0}
};

struct driverData driverTemp[1] = // a temp driver struct for sorting purposes
{0, 0, 0, "null           ", 0, 0, 0, 0};

struct Store // flag structure if a driver and/or order is ready
{
	bool driverUp; // is a driver ready?
	bool orderUp; // is an order ready?
};

Store parlor = {0, 0}; // initial set values for store flags

struct orderData // variable structure tied to orders
{
	int orderNum; // the order number; null is 0; starts at 1;
	int timeHour; // hour the order is started; paired with timeMinute
	int timeMinute; // minute the order is started; paired with timeHour
	int inStoreTime; // how long the order has been in store
	bool orderInstore; // is this particular order in store?
	bool orderUp; // is this particular order ready?
	string address; // address of order
	string gridMap; // where its located on gridmap; soon to be made
	double gridA; // number representing location on grid x axis
	double gridB; // number representing location on grid y axis
	double tip; // how much $tip the order is worth; might change to formula based off distance/time
};

struct orderData order[10] = // the values of the orders
{
    {0, 11,  5, 0, 0, 0, "null     ", "00 ", 0, 0, 0.00},
    {1, 11,  7, 0, 0, 0, "422 Cliff St.    ", "G1 ", 7, 1, 8.00},
    {2, 11, 12, 0, 0, 0, "317 Duke Ave.    ", "C4 ", 3, 4, 7.00},
    {3, 11, 15, 0, 0, 0, "1720 Bury Rt.    ", "D6 ", 4, 6, 10.00},
	{4, 11, 21, 0, 0, 0, "50 Ocean Ln.     ", "E3 ", 5, 3, 8.00},
	{5, 11, 23, 0, 0, 0, "2613 Pearl Rd.   ", "H2 ", 8, 2, 10.00},
	{6, 11, 30, 0, 0, 0, "101 True Dr.     ", "E1 ", 5, 1, 6.00},
	{7, 11, 35, 0, 0, 0, "61 Mill Rd.      ", "G1 ", 7, 1, 8.00},
	{8, 11, 42, 0, 0, 0, "136 Joey St.     ", "C4 ", 3, 4, 7.00},
	{9, 11, 57, 0, 0, 0, "buffer   ", "00 ", 0, 0, 0.00}
};
int totalNumOrders = (sizeof(order) / sizeof(*order))-1; // variable assigned the size of the array minus null


// function to "clockin" drivers
void clockIN() {
	if (hr == 11 && m == 1)
		driver[1].clockedIn = 1, driver[1].inStore = 1;

	if (hr == 11 && m == 3)
		driver[2].clockedIn = 1, driver[2].inStore = 1;

	if (hr == 11 && m == 5)
		driver[3].clockedIn = 1, driver[3].inStore = 1;
}

// if any driver is instore flags 
void flagSTORE() { 
	if (driver[1].inStore == 1)
		parlor.driverUp = 1;
	else
		parlor.driverUp = 0;
}

// flags for drivers
void flagDRIVER() { 
	clockIN();
	if (driver[1].inStore==1) driver[1].timeIn++;
	else driver[1].timeIn =0;
	if (driver[2].inStore==1) driver[2].timeIn++;
	else driver[2].timeIn =0;
	if (driver[3].inStore==1) driver[3].timeIn++;
	else driver[3].timeIn =0;
}

// display drivers in available in store
void displayDRIVER() { 
	cout << "\n\n-------------- Drivers in Store --------------\n";
	cout << " Name                             Elapsed Time\n";
	for(int i = 1; i < 4; i++){
		if (driver[i].inStore == 1){
			driver[i].timeOut = 0;
			if (driver[i].driver != "null           "){
				cout << "\n   " << driver[i].driver << "                         " << driver[i].timeIn;
			}
		}
	}
	if (driver[0].driver != "null           " && driver[0].inStore == 1){
		cout << "\n   " << driver[0].driver << "                         " << driver[0].timeIn;
	}
}

// displays drivers on drives; might be sorting issues if drivers come back to store before others in line
void displayDRIVEROUT(){

	cout << "\n\n-------------- Drivers out ------------------\n";
	cout << " Name                             Elapsed Time\n";

	for(int i = 0; i < 4; i++){
		if (driver[i].inStore == 0){
			if (driver[i].driver != "null           "){
				cout << "\n   " << driver[i].driver << "                         " << driver[i].timeOut;
			}
		}
	}
}

// displays your accumulated TIPS $$$
void displayTIPS() { 
	cout << "[TIPS: $" << setw(5) << setprecision(2) << tipAmount << "]" << " ";
		if(tipMod > 0)
			cout << "+$" << setw(5) << setprecision(2) << (tipMod) << "!";
}

// displays your late strikes; still need to create function for counting late strikes
void displayLATE() { 
	cout << "[LATE: " << lateStrikes << "]" << " ";
}

// display week; stages seperated by days in the week
void displayWEEK() {
	cout << "[WEEK: "<< week << "] ";
}

// displays the day of the week; the "stages"
void displayDATE(){ 
	int d = 0;
	cout << "[" << day[d] << "] ";
}

// displays the clock :)
void displayCLOCK() { 
	cout << "[" << setw(2) << setfill('0') << hr << ":" << setw(2) << setfill('0') << m << "] ";
}

// cross references times in struct orders and decides when the order is received
void flagORDER() {
	for(int i = 1; i < totalNumOrders; i++){
		if (hr == order[i].timeHour && m == order[i].timeMinute){
			order[i].orderInstore = 1;
		}
	}
}

// display the orders instore in order of instore time
void displayORDER() { 
	cout << "\n\n--------------- Orders in Store --------------\n" << " Order#  Map  Address             Elapsed Time\n";
	for(int n = 1; n < totalNumOrders; n++){
		if (order[n].orderInstore == 1) order[n].inStoreTime++,
			cout << "\n   " << order[n].orderNum << "     " << order[n].gridMap << "  " << order[n].address << "          " << order[n].inStoreTime;
		if (order[n].inStoreTime > 7 && order[n].orderInstore == 1) 
			parlor.orderUp = 1, cout << " !";
	}
}

// sorts the drivers as the leave the store; always keep null at driver[0]; this might cause issues down the road as its not very sophisticated
void driverSORT() { 
	driverTemp[1] = driver[1];
	driver[1] = driver[2];
	driver[2] = driver[3];
	driver[3] = driverTemp[1];
}

// when an order and a driver are ready prompts user to dispatch; "0" skips by 1 minutes; needs consolidating and split into better organized functions 
void driverOUT() {
	cout << "\n\n----------------------------------------------\n";
	cout << " > ";
	if (parlor.driverUp == 1 && parlor.orderUp == 1){
		if (driver[1].driver == "null           ")
			driverSORT();
		cout << "How many orders to dispatch? ", cin >> c;
		cout << "\n > Dispatch " << driver[1].driver << "\n   on which order(s)? ";
		g = c;
  		for(c = 0; c < g ; c++) {
      		cin >> input[c];
			order[input[c]].orderInstore = 0;
			driver[1].timeLimit += (order[input[c]].gridA * order[input[c]].gridB);
			driver[1].bank += order[input[c]].tip; // create formula for tip;  		 
		}
	}
	if (g > 0){
    	driver[1].inStore = 0;
		driver[1].onDrive = 0;
		driver[1].timeIn  = 0;
		cout << "\n > " << driver[1].driver << "\n   is dispatched!" << "\n   Round Trip Estimate: " << driver[1].timeLimit << " minutes.";
		sleep(2);
		driverSORT();
		g = 0;
	}
	if (c == 0)
		t = .1;
}

// checks if a driver is back in store based off calculated timeOut variable; then various flags for that driver
void driverIN(){
	for(int i = 0; i < driverNum; i++){
		if (driver[i].onDrive == 0){
			driver[i].timeOut++;
			if (driver[i].timeOut == driver[i].timeLimit){
				tipMod = ((driver[i].bank) - (((driver[i].timeOut)/100)*100)+3); //fuck
				parlor.driverUp = 1;
				driver[i].inStore = 1;
				driver[i].onDrive = 0;
				driver[i].timeOut = 0;
				driver[i].timeLimit = 0;
				tipTemp = tipAmount;
				tipAmount += tipMod;
				driver[i].bank = 0;
			}
		}
	}
}

// main game enclosed in a clock counter
int main() {
	for (hr = 11; hr < 20; hr++){
		for (m = 0; m < 60; m++) {
			flagSTORE();
			flagORDER();
			flagDRIVER();
			displayWEEK();
			displayDATE();
			displayCLOCK();
			displayLATE();
			displayTIPS();
			displayDRIVER();
			displayDRIVEROUT();
			displayORDER();
			driverIN();
			driverOUT();
			sleep(t);
			system("clear");
		}
	}
    return 0;
}