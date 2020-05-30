// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:25 $
//###########################################################################
//
// FILE:	DSP2833x_Gpio.c
//
// TITLE:	DSP2833x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
Uint16 IPM_Fault=0;

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 
void InitGpio(void)
{
   EALLOW;
   
   // Each GPIO pin can be: 
   // a) a GPIO input/output
   // b) peripheral function 1
   // c) peripheral function 2
   // d) peripheral function 3
   // By default, all are GPIO Inputs 
   GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
   GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
   GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
   GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

   GpioCtrlRegs.GPAMUX2.bit.GPIO17=0;//设置led灯脚为GPIO 
   
   GpioCtrlRegs.GPAMUX2.bit.GPIO21=0;//AD760x RESTAD 引脚 设置为GPIO
   GpioCtrlRegs.GPAMUX2.bit.GPIO23=0;//AD760x CONVST 引脚 设置为GPIO
   GpioCtrlRegs.GPAMUX2.bit.GPIO27=0;//AD760x AD_RD 引脚 设置为GPIO
   GpioCtrlRegs.GPBMUX1.bit.GPIO33=0;//AD760x BUSYAD 引脚 设置为GPIO
   GpioCtrlRegs.GPBMUX2.bit.GPIO48=0;//AD760x AD_CS 引脚 设置为GPIO

   GpioCtrlRegs.GPBMUX1.bit.GPIO35=0;//LED_GREEN 引脚 设置为GPIO
   GpioCtrlRegs.GPBMUX1.bit.GPIO38=0;//LED_RED 引脚 设置为GPIO
   GpioCtrlRegs.GPAMUX2.bit.GPIO28=0;
   GpioCtrlRegs.GPAMUX2.bit.GPIO29=0;//pwm_en 为gpio

   GpioCtrlRegs.GPBMUX1.bit.GPIO39=0;//RDC_CS 引脚 设置为GPIO
   GpioCtrlRegs.GPAMUX1.bit.GPIO13=0;//设置 alarm为gpio

   GpioCtrlRegs.GPAMUX1.bit.GPIO14=0;//设置DC_ON 脚为GPIO 
   
   GpioCtrlRegs.GPAMUX1.bit.GPIO15=0;//设置DC_ON 脚为GPIO 
   GpioCtrlRegs.GPAMUX1.bit.GPIO14=0;//设置DC_ON 脚为GPIO 
   GpioCtrlRegs.GPAMUX2.bit.GPIO16=0;// 

   GpioCtrlRegs.GPAMUX1.bit.GPIO8=0;//设置CS_FM25 脚为GPIO 

   GpioCtrlRegs.GPAMUX2.bit.GPIO22=0;//设置INPUT1 脚为GPIO 
   GpioCtrlRegs.GPBMUX2.bit.GPIO57=0;//设置INPUT2 脚为GPIO
   GpioCtrlRegs.GPBMUX2.bit.GPIO58=0;//设置INPUT3 脚为GPIO

   GpioCtrlRegs.GPBMUX2.bit.GPIO59=0;//设置OUTPU1 脚为GPIO
   GpioCtrlRegs.GPBMUX2.bit.GPIO60=0;//设置OUTPU2 脚为GPIO
   GpioCtrlRegs.GPBMUX2.bit.GPIO61=0;//设置OUTPU3 脚为GPIO
   GpioCtrlRegs.GPBMUX2.bit.GPIO49=0;//设置OUTPU4 脚为GPIO

   GpioCtrlRegs.GPAMUX2.bit.GPIO20=0;//设置SYNC 脚为GPIO 

   GpioCtrlRegs.GPBMUX1.bit.GPIO41=0;//设置为GPIO W
   GpioCtrlRegs.GPBMUX1.bit.GPIO42=0;//设置为GPIO V
   GpioCtrlRegs.GPBMUX1.bit.GPIO43=0;//设置为GPIO U
   
   
  

   GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
   GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs   
   GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

   GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;      // 设置LED灯脚为输出
   
   GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;      // AD760x RESTAD 引脚 设置为输出
   GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;      // AD760x CONVST 引脚 设置为输出
   GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;      // AD760x AD_RD 引脚 设置为输出
   GpioCtrlRegs.GPBDIR.bit.GPIO33 = 0;      // AD760x BUSYAD 引脚 设置为输入
   GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;      // AD760x AD_CS 引脚 设置为输出

   GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;      // LED_GREEN 引脚 设置为输出 
   GpioCtrlRegs.GPBDIR.bit.GPIO38 = 1;      // LED_RED 引脚 设置为输出 
   GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;      // LED_YELLOW 引脚 设置为输出
   Pwm_EN_1;
   GpioCtrlRegs.GPADIR.bit.GPIO29= 1 ;//PEM_EN为输出

   GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;      // RDC_CS 引脚 设置为输出 
   GpioCtrlRegs.GPADIR.bit.GPIO13=0;//设置alarm为输入
   GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;      // 设置DC_ON为输出
   DC_ON_1;
    GpioCtrlRegs.GPADIR.bit.GPIO14= 1;      // 设置DC_ON为输出
   GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;      // 设置FAN_ON为输出

   GpioCtrlRegs.GPADIR.bit.GPIO8  = 1;      // 设置CS_FM25为输出
   
   GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;      // INPUT1 引脚 设置为输入
   GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;      // INPUT2 引脚 设置为输入
   GpioCtrlRegs.GPBDIR.bit.GPIO58 = 0;      // INPUT3 引脚 设置为输入

   GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;      // OUTPU1  引脚 设置为输出
   GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;      // OUTPU2  引脚 设置为输出
   GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;      // OUTPU3  引脚 设置为输出
   GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;      // OUTPU4  引脚 设置为输出

   GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;      // SYNC  引脚 设置为输出
   GpioCtrlRegs.GPBDIR.bit.GPIO41=0;//输入 W
   GpioCtrlRegs.GPBDIR.bit.GPIO42=0;//输入 V
   GpioCtrlRegs.GPBDIR.bit.GPIO43=0;//输入 U
   
   

   

   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT 

   // Pull-ups can be enabled or disabled. 
   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
   GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79

   GpioDataRegs.GPADAT.bit.GPIO17=1;//设置LED灯引脚输出高电平
   
   GpioDataRegs.GPADAT.bit.GPIO21=0;//AD760x RESTAD 引脚 输出低电平
   GpioDataRegs.GPADAT.bit.GPIO23=1;//AD760x CONVST 引脚 输出高电平
   GpioDataRegs.GPADAT.bit.GPIO27=1;//AD760x AD_RD 引脚 输出高电平
   GpioDataRegs.GPBDAT.bit.GPIO48=1;//AD760x AD_CS 引脚 输出高电平

   
   GpioDataRegs.GPBDAT.bit.GPIO35=1;// LED_red 引脚 
   GpioDataRegs.GPBDAT.bit.GPIO38=1;// LED_greed 引脚
   GpioDataRegs.GPADAT.bit.GPIO28=1;// 

   GpioDataRegs.GPBDAT.bit.GPIO39=1;// 

    GpioDataRegs.GPADAT.bit.GPIO15=1;//设置DC_ON 输出高电平,为低电平时 母线继电器闭合
     GpioDataRegs.GPADAT.bit.GPIO16=1;//

     GpioDataRegs.GPADAT.bit.GPIO8=1;//

    GpioDataRegs.GPBDAT.bit.GPIO59=1;//
    GpioDataRegs.GPBDAT.bit.GPIO60=1;//
    GpioDataRegs.GPBDAT.bit.GPIO61=1;//
    GpioDataRegs.GPBDAT.bit.GPIO49=1;//

    GpioDataRegs.GPADAT.bit.GPIO20=1;//设置 SYNC输出高电平 


    GpioCtrlRegs.GPBMUX2.bit.GPIO50=1;// 编码器 a
    GpioCtrlRegs.GPBMUX2.bit.GPIO51=1;// 编码器 b
     GpioCtrlRegs.GPBMUX2.bit.GPIO53=1;// 编码器 c

      GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL=53;//gpio53 作为外部中断3输入引脚
    XIntruptRegs.XINT3CR.bit.ENABLE=1;//外部中断打开
    XIntruptRegs.XINT3CR.bit.POLARITY=3;//上升沿有效

   EDIS;

   Init_Scib_Gpio();
   Init_Scic_Gpio();
   InitEPwmGpio();
   InitSpiaGpio();

   EALLOW;
   GpioDataRegs.GPBDAT.bit.GPIO35=1;// LED_red 引脚 
   EDIS;
   

}

void Init_Scib_Gpio(void)//232
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO9  (SCITXDB) 

	
  GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up for GPIO11 (SCIRXDB) 

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
//	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)

/* Configure SCI-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.
// Comment out other unwanted lines.

  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for SCITXDB operation 
	
  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation 
	
    EDIS;
}

void Init_Scic_Gpio(void)//485
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation
	
    EDIS;
}



void CPU_RUN(void)
{


   static Uint32 i=0;
   i++;
   if(i==5000)
   {
     EALLOW;
     GpioDataRegs.GPADAT.bit.GPIO17=1;
     
     EDIS;

   }
   else if(i==10000)
   {
    EALLOW;
    GpioDataRegs.GPADAT.bit.GPIO17=0;  
    EDIS;
    i=0;
   }

}

void eva_open(void)
{
     EALLOW;
       GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
     GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

        
   
   EDIS;


}

void eva_close(void)
{
     EALLOW;
       //  1.3.5强制高，2.4.6有效 

       GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;   // Configure GPIO0 as EPWM1A
     
     GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;   // Configure GPIO2 as EPWM2A
     
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;   // Configure GPIO4 as EPWM3A
    

    GpioCtrlRegs.GPADIR.bit.GPIO0=1;
    GpioCtrlRegs.GPADIR.bit.GPIO2=1;
    GpioCtrlRegs.GPADIR.bit.GPIO4=1;

    GpioDataRegs.GPADAT.bit.GPIO0=1;
    GpioDataRegs.GPADAT.bit.GPIO2=1;
    GpioDataRegs.GPADAT.bit.GPIO4=1; 

      
       
       
   
    EPwm1Regs.CMPA.half.CMPA =3375; // 
EPwm2Regs.CMPA.half.CMPA = 3375; // 
EPwm3Regs.CMPA.half.CMPA = 3375; // 
   EDIS;
     Run_PMSM=2;
   LocationFlag=1;
   Speed_Ui=0;
   ID_Ui=0;
   IQ_Ui=0;
   Position=1;
   j=0;
   speed_dis=0;
   IQ_Given=0;
   OldRawTheta=0;
   SpeedRef=0;
   speed_give=0;
   Modulation=0.25;    // 调制比
   O_Current=0;
   PosCount=0;
   OldRawThetaPos=0;
   Hall_Fault=0;
   Speed_run=0;

   StopParaInt();
}


void  IPM_BaoHu(void)
{
    static Uint32 i=0;
    Uint16 j=0;

    j=GpioDataRegs.GPADAT.bit.GPIO13;//读取故障位
    if(j==0)
    {
        i++;
        if(i==1)
        {
            DC_ON_1;
            Pwm_EN_1;
            eva_close();
            IPM_Fault=1;
            	

        }

    }
    else
    {i=0;
	
    
    }

    



}
	
//===========================================================================
// End of file.
//===========================================================================
