/*
 * idle.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: OS1
 */

#include "idle.h"
#include "pcb.h"

idleThread::idleThread():Thread(128,0){
	breakFlag = 0;
}

volatile int idleThread::unblFlag = 0;

void idleThread::run() {
	while ( breakFlag == 0 ){
		if ( unblFlag != 0 ){
			unblFlag = 0;
			dispatch();
		}
	}
}

idleThread::~idleThread(){
	breakFlag = 1;
	PCB::IDLE->state = finished;
	waitToComplete();
}
