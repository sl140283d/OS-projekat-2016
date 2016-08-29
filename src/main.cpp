/*
 * main.cpp
 *
 *  Created on: Aug 8, 2016
 *      Author: OS1
 */

#include "pcb.h"
#include "inThread.h"

int retValue;

int main(int argc, char* argv[]){
	inic();
	inicThread iThread(argc,argv);
	iThread.start();
	dispatch();
	return retValue;
}
