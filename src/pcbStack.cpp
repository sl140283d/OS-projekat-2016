/*
 * pcbStack.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: OS1
 */

#include "pcbStack.h"

PCBStack::PCBStack(){
	size = 10;
	pcbStack = new PCB*[size];
}

void PCBStack::put(PCB *pcb, unsigned long id){
	if ( id >= size ){
		StackSize oldSize = size;
		size = id + 10;
		PCB **oldStack = pcbStack;
		pcbStack = new PCB*[size];
		for ( StackSize i = 0 ; i < oldSize ; i++ ){
			if ( oldStack[i] != 0 ) {
				pcbStack[i] = oldStack[i];
				oldStack[i] = 0;
			}
			else pcbStack[i] = 0;
		}
		delete[] oldStack;
		for ( StackSize j = oldSize ; j < size ; j++) pcbStack[j] = 0;
	}
	pcbStack[id] = pcb;
}

PCB* PCBStack::get(unsigned long id){
	if ( id >= size ) return 0;
	return pcbStack[id];
}

PCBStack::~PCBStack(){
	delete[] pcbStack;
}


