/*
 * inicThread.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: OS1
 */

#include "inThread.h"
extern int retValue;
int userMain(int argc, char *argv[]);

inicThread::inicThread(int argc, char* argv[]):Thread(){
		this->argc = argc;
		this->argv = argv;
}

void inicThread::run(){
		retValue = userMain(argc,argv);
}
inicThread::~inicThread(){
	for( int i = 0 ; i < argc ; i++) delete argv[i];
	delete[] argv;
	argv = 0;
}


