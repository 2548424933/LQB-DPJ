#ifndef __SYSTEM_H_
#define __SYSTEM_H_

typedef  unsigned  char  u8;
typedef int u16;

//键盘用的
#define	KEY_NO 0
#define  KEY_DOWN 1
#define  KEY_UP 2 


#include <STC15F2K60S2.H>
#include <intrins.h>                           //--------定时器要加上这个

#include "timer.h"
#include "Device.h"
#include "SMG.h"
#include "BTM.h"
#include "KBD.h"
#include "ADPCF.h"
#include "iicc.h"

#endif