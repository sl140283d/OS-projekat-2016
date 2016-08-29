/*
 * ivt.h
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#ifndef _ivt_h_
#define _ivt_h_

#include "ivtEntry.h"
#include "myEvent.h"
#include "event.h"

class IVTEntry;

class IVT{
private:
	IVTEntry **ivt;
public:
	IVT();
	void put(IVTEntry *ivte, int i);
	IVTEntry* get(int i);
	~IVT();
};

#endif
