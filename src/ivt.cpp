/*
 * ivt.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#include "ivt.h"

IVT::IVT(){
	ivt = new IVTEntry*[256];
}

void IVT::put(IVTEntry *ivte, int ivtno){
	ivt[ivtno] = ivte;
}

IVTEntry* IVT::get(int ivtno){
	return ivt[ivtno];
}

IVT::~IVT(){
	delete[] ivt;
}
