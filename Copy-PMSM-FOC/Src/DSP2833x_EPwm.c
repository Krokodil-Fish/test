// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:19 $
//###########################################################################
//
// FILE:   DSP2833x_EPwm.c
//
// TITLE:  DSP2833x ePWM Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#define deadtime 320
_iq LineEncoder=81920000;         


//---------------------------------------------------------------------------
// InitEPwm: 
//---------------------------------------------------------------------------
// This function initializes the ePWM(s) to a known state.
//
void InitEPwm(void)
{
   // Initialize ePWM1/2/3/4/5/6

   //tbd...
 
}

void  QEP_Init(void)
{
 

    EALLOW;
    EQep1Regs.QUPRD=1500000;			// Unit Timer for 100Hz at 150 MHz SYSCLKOUT
    EQep1Regs.QDECCTL.all=0x400;
    EQep1Regs.QEPCTL.all=0x820a;//QEPI����ʱ��λλ�ü�������QEPI�����س�ʼ����������QEP����ʹ�ܡ�ʹ�ܼ�ʱ��
    EQep1Regs.QPOSCNT=0;//λ�ü���������
    EQep1Regs.QPOSINIT=0;//λ�üĴ�����ʼ���Ĵ���
    TotalPulse=_IQmpy(_IQ(4),LineEncoder);//һȦ�������ź���=4��2500
    EQep1Regs.QPOSMAX= (Uint16)(TotalPulse>>15);//���λ�üĴ�����ֵ   1 0000��Q0��
    EQep1Regs.QCAPCTL.all=0x8075;//ʹ�ܲ���Ԫ����λλ��32��Ƶ��CAP����128��Ƶ
    EDIS;
    
}



void Init_Pwm_DAC()  //��ģת����
{
    EALLOW;

    GpioCtrlRegs.GPAMUX1.bit.GPIO6= 1;   // pwm7 dac1 
    GpioCtrlRegs.GPAMUX1.bit.GPIO7= 1;   // pwm8 dac2


    // EPWM Module 4 config

EPwm4Regs.TBPRD= 375000/100; // Period = 1600 TBCLKcounts Tpwm=TBPRD*Ttbclk,100��ʾ20k
EPwm4Regs.TBPHS.half.TBPHS = 0; // Set Phaseregister to zero
EPwm4Regs.TBCTR=0;
EPwm4Regs.TBCTL.bit.FREE_SOFT=0;//��������
EPwm4Regs.TBCTL.bit.PHSDIR=1;//��������
EPwm4Regs.TBCTL.bit.CLKDIV=0;
EPwm4Regs.TBCTL.bit.HSPCLKDIV=1;
EPwm4Regs.TBCTL.bit.SYNCOSEL=0x3; 
EPwm4Regs.TBCTL.bit.PRDLD=0;//Ӱ��ģʽ
EPwm4Regs.TBCTL.bit.PHSEN=0;//
EPwm4Regs.TBCTL.bit.CTRMODE=0;//������


EPwm4Regs.CMPCTL.all=0x0000;//cmpbӰ��װ�أ�cmpaӰ��װ�أ����ǹ�0װ��

EPwm4Regs.AQCTLA.bit.PRD =  AQ_SET;
EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;
EPwm4Regs.AQCTLB.bit.PRD =  AQ_SET;
EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;

EPwm4Regs.DBCTL.bit.IN_MODE=0;//epwma source
EPwm4Regs.DBCTL.bit.OUT_MODE = DB_DISABLE; // ����������

EPwm4Regs.CMPA.half.CMPA=1000;//dac1
EPwm4Regs.CMPB=2000;//dac2


EDIS;

//pwm dac ˵���� pwm ռ�ձ� 3750 ��Ӧ���� 5v  ����ֵ����� �Ͷ��˰ɣ�ֻ���������ѹ��Ŷ��

}

void DAC1_out(_iq data)//data�Ǳ�ôֵ
{
    Uint16 temp=0;
    if(data<0)
    {
        EPwm4Regs.CMPA.half.CMPA=0;//dac1
    }
    else
    {
    temp=_IQtoF(_IQmpy(_IQ(3750), data));


    EPwm4Regs.CMPA.half.CMPA=temp;//dac1
    }


}

void DAC2_out(_iq data)//data�Ǳ�ôֵ
{
    Uint16 temp=0;
    if(data<0)
    {
        EPwm4Regs.CMPB=0;//dac2
    }
    else
    {
    temp=_IQtoF(_IQmpy(_IQ(3750), data));


    EPwm4Regs.CMPB=temp;//dac2
    }


}















void InitEPwm_1_2_3(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   //��ֹPWMģ��ʱ��ͬ����ʱ���趨��������ʱ�ӣ�
    EDIS;
    EALLOW;
    
EPwm1Regs.TBCTR=0;    //ʱ������������
EPwm1Regs.TBPRD= 375000/100; // Period = 1600 TBCLKcounts Tpwm=2*TBPRD*Ttbclk,100��ʾ10k
T1Period=EPwm1Regs.TBPRD;
EPwm1Regs.TBPHS.half.TBPHS = 0; // Set Phaseregister to zero
//EPwm1Regs.TBCTL.all=0xa092;//�������У���0��ʼ����,clkdiv=1,hspclkdiv=2,TBCLK=75m,ctr=0,tbprdӰ��װ��

EPwm1Regs.TBCTL.bit.FREE_SOFT=0;//��������
EPwm1Regs.TBCTL.bit.PHSDIR=1;//��λ����λ��ͬ���ź�����ʱ�����ϼ���
EPwm1Regs.TBCTL.bit.CLKDIV=0;
EPwm1Regs.TBCTL.bit.HSPCLKDIV=1;
EPwm1Regs.TBCTL.bit.SYNCOSEL=0x3; //ͬ���ź��������ֹͬ���ź����
EPwm1Regs.TBCTL.bit.PRDLD=0;//Ӱ��ģʽ
EPwm1Regs.TBCTL.bit.PHSEN=0;//��ֹװ��
EPwm1Regs.TBCTL.bit.CTRMODE=2;//��������

EPwm1Regs.CMPCTL.all=0x0000;//�ڼ���ֵΪ��ʱcmpbӰ��װ�ء�cmpaӰ��װ�أ�

EPwm1Regs.AQCTLA.bit.CAU=AQ_CLEAR;//ipm�͵�ƽ��Ч 1a
EPwm1Regs.AQCTLA.bit.CAD=AQ_SET;

EPwm1Regs.DBCTL.bit.IN_MODE=0;//epwma source
EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // =3��˫����ʱ
EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC; // =1��ePWMA��ת��ePWMB����ת
EPwm1Regs.DBFED= deadtime; // FED = 200 TBCLKs
EPwm1Regs.DBRED= deadtime; // RED = 200 TBCLKs

EPwm1Regs.ETSEL.bit.INTSEL=1;//�����жϣ�TBCTR=0ʱ�������ж�
EPwm1Regs.ETSEL.bit.INTEN=1;//ʹ��EPWM1_INT�ж�
EPwm1Regs.ETPS.bit.INTPRD = ET_1ST; // �ڵ�һ���¼�����ʱ�������ж�
EPwm1Regs.ETCLR.bit.INT=1;//����жϱ�־λ

// EPWM Module 2 config

EPwm2Regs.TBPRD= 375000/100; // Period = 1600 TBCLKcounts Tpwm=2*TBPRD*Ttbclk,100��ʾ10k
EPwm2Regs.TBPHS.half.TBPHS = 0; // Set Phaseregister to zero
EPwm2Regs.TBCTR=0;
EPwm2Regs.TBCTL.bit.FREE_SOFT=0;//��������
EPwm2Regs.TBCTL.bit.PHSDIR=1;//��������
EPwm2Regs.TBCTL.bit.CLKDIV=0;
EPwm2Regs.TBCTL.bit.HSPCLKDIV=1;
EPwm2Regs.TBCTL.bit.SYNCOSEL=0x3; 
EPwm2Regs.TBCTL.bit.PRDLD=0;//Ӱ��ģʽ
EPwm2Regs.TBCTL.bit.PHSEN=0;//
EPwm2Regs.TBCTL.bit.CTRMODE=2;//����

EPwm2Regs.CMPCTL.all=0x0000;//cmpbӰ��װ�أ�cmpaӰ��װ�أ����ǹ�0װ��

EPwm2Regs.AQCTLA.bit.CAU=AQ_CLEAR;//ipm�͵�ƽ��Ч 1a
EPwm2Regs.AQCTLA.bit.CAD=AQ_SET;

EPwm2Regs.DBCTL.bit.IN_MODE=0;//epwma source
EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC; // Active Hi complementary
EPwm2Regs.DBFED= deadtime; // FED = 200 TBCLKs
EPwm2Regs.DBRED= deadtime; // RED = 200 TBCLKs


// EPWM Module 3 config
EPwm3Regs.TBPRD= 375000/100; // Period = 1600 TBCLKcounts Tpwm=2*TBPRD*Ttbclk,100��ʾ10k
EPwm3Regs.TBPHS.half.TBPHS = 0; // Set Phaseregister to zero
EPwm3Regs.TBCTR=0;
EPwm3Regs.TBCTL.bit.FREE_SOFT=0;//��������
EPwm3Regs.TBCTL.bit.PHSDIR=1;//��������
EPwm3Regs.TBCTL.bit.CLKDIV=0;
EPwm3Regs.TBCTL.bit.HSPCLKDIV=1;
EPwm3Regs.TBCTL.bit.SYNCOSEL=0x3; 
EPwm3Regs.TBCTL.bit.PRDLD=0;//Ӱ��ģʽ
EPwm3Regs.TBCTL.bit.PHSEN=0;//
EPwm3Regs.TBCTL.bit.CTRMODE=2;//����

EPwm3Regs.CMPCTL.all=0x0000;//cmpbӰ��װ�أ�cmpaӰ��װ�أ����ǹ�0װ��

EPwm3Regs.AQCTLA.bit.CAU=AQ_CLEAR;//ipm�͵�ƽ��Ч 1a
EPwm3Regs.AQCTLA.bit.CAD=AQ_SET;

EPwm3Regs.DBCTL.bit.IN_MODE=0;//epwma source
EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // enable Dead-band module
EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC; // Active Hi complementary
EPwm3Regs.DBFED= deadtime; // FED = 200 TBCLKs
EPwm3Regs.DBRED= deadtime; // RED = 200 TBCLKs

EPwm1Regs.TBCTR=0;
EPwm2Regs.TBCTR=0;
EPwm3Regs.TBCTR=0;

EDIS;

 Init_Pwm_DAC();

EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;  //ʹ��PWMģ��ʱ��ʱ��ͬ��
    EDIS;
 

}

 

void InitEPwmGpio(void)
{
   InitEPwm1Gpio();
   
}

void InitEPwm1Gpio(void)
{
   EALLOW;
   


    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)   
   GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;    // Enable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (EPWM3B)


    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
     GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
   
    EDIS;
}

void InitEPwm2Gpio(void)
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    

/* Configure ePWM-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM2 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
   
    EDIS;
}



void InitEPwm3Gpio(void)
{
   EALLOW;
   


    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (EPWM3B)


    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
	
    EDIS;
}


#if DSP28_EPWM4
void InitEPwm4Gpio(void)
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;    // Enable pull-up on GPIO7 (EPWM4B)

/* Configure ePWM-4 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM4 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B
	
    EDIS;
}
#endif // endif DSP28_EPWM4  


#if DSP28_EPWM5
void InitEPwm5Gpio(void)
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;    // Enable pull-up on GPIO9 (EPWM5B)

/* Configure ePWM-5 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM5 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
	
    EDIS;
}
#endif // endif DSP28_EPWM5


#if DSP28_EPWM6
void InitEPwm6Gpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;    // Enable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up on GPIO11 (EPWM6B)

/* Configure ePWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
	
    EDIS;
}
#endif // endif DSP28_EPWM6  

//---------------------------------------------------------------------------
// Example: InitEPwmSyncGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as ePWM Synch pins
//

void InitEPwmSyncGpio(void)
{

   EALLOW;

/* Configure EPWMSYNCI  */
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pull-up on GPIO32 (EPWMSYNCI)    

/* Set qualification for selected pins to asynch only */
// This will select synch to SYSCLKOUT for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 0;// Synch to SYSCLKOUT GPIO6 (EPWMSYNCI)��ϵͳʱ��ͬ��
// GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 0;  // Synch to SYSCLKOUT GPIO32 (EPWMSYNCI)    

/* Configure EPwmSync pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPwmSync functional pins.
// Comment out other unwanted lines.   

   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 2;    // Enable pull-up on GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 2;   // Enable pull-up on GPIO32 (EPWMSYNCI)    



/* Configure EPWMSYNC0  */

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNC0)
   GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pull-up on GPIO33 (EPWMSYNC0)    

// GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 3;    // Enable pull-up on GPIO6 (EPWMSYNC0)
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 2;   // Enable pull-up on GPIO33 (EPWMSYNC0)    

}



//---------------------------------------------------------------------------
// Example: InitTzGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitTzGpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
   GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
   GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
   GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
   GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)

   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
// GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up on GPIO28 (TZ5)

   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6) 
// GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up on GPIO29 (TZ6)  
   
/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
// GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (TZ5)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6) 
// GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (TZ6)  

   
/* Configure TZ pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be TZ functional pins.
// Comment out other unwanted lines.   
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // Configure GPIO14 as TZ3
   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // Configure GPIO15 as TZ4

   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // Configure GPIO16 as TZ5
// GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // Configure GPIO28 as TZ5

   GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // Configure GPIO17 as TZ6               
// GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // Configure GPIO29 as TZ6  

   EDIS;
}



//===========================================================================
// End of file.
//===========================================================================
