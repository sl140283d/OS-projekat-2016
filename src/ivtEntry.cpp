/*
 * ivtEntry.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#include "ivtEntry.h"
#include <dos.h>

IVT IVTEntry::ivt;

IVTEntry::IVTEntry(int ivtno, int old, pInterrupt newRoutine){
	callOld = old;
	this->ivtno = ivtno;
	#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtno);
	setvect(ivtno,newRoutine);
	#endif
	event = 0;
	IVTEntry::ivt.put(this,ivtno);
}

void IVTEntry::interr(){
	if ( callOld ) oldRoutine();
	if ( event != 0 ) event->signal();
}

IVTEntry::~IVTEntry(){
	#ifndef BCC_BLOCK_IGNORE
	setvect(ivtno,oldRoutine);
	#endif
	IVTEntry::ivt.put(0,ivtno);
	event = 0;
}
