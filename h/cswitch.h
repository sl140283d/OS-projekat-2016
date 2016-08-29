/*
 * cswitch.h
 *
 *  Created on: Aug 8, 2016
 *      Author: OS1
 */

#ifndef _cswitch_h_
#define _cswitch_h_

#define lock lockFlag=0;
#define unlock lockFlag=1;\
if (cs_req) {\
dispatch();\
}

void inic();
void restore();
void interrupt timer();
#endif
