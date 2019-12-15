// TI File $Revision: /main/2 $
// Checkin $Date: March 1, 2007   16:06:07 $
//###########################################################################
//
// FILE:	DSP2833x_Sci.c
//
// TITLE:	DSP2833x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
Uint16 speed_dis=0;
 

void InitSci_B(void)//232
{    
    
 	EALLOW;
    ScibRegs.SCICCR.all=0x0007;//一个停止位，无效验，8位数据位
    ScibRegs.SCICTL1.all=0x0003;//使能接收和发送
    ScibRegs.SCICTL2.all=0x0002;//使能接收中断，禁止发送中断，采用查询方式发送

    ScibRegs.SCIHBAUD = 0x00;//波特率为115200
 	ScibRegs.SCILBAUD = 0x27;
    
    ScibRegs.SCIFFTX.all=0xC028;
    ScibRegs.SCIFFRX.all=0x0026;//6char
    ScibRegs.SCIFFCT.all=0x00;
   
    ScibRegs.SCICTL1.bit.SWRESET=1;//软件复位
    
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
   ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
  	EDIS;
    
}


void InitSci_C(void)//485
{    
    
 	ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback 
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK, 
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all =0x0003; 
	ScicRegs.SCICTL2.bit.RXBKINTENA =1;
    ScicRegs.SCIHBAUD    =0x0001;//9600
    ScicRegs.SCILBAUD    =0x00e7;
	ScicRegs.SCICCR.bit.LOOPBKENA =0; // disable loop back  
	ScicRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset 
	ScicRegs.SCIFFRX.all=0x204f;
    ScicRegs.SCIFFCT.all=0x0;
	ScicRegs.SCIFFRX.bit.RXFFIENA = 1;
    
}

        



 

 
 

	
//===========================================================================
// End of file.
//===========================================================================
