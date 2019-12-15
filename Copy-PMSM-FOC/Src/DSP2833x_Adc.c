// TI File $Revision: /main/5 $
// Checkin $Date: October 23, 2007   13:34:09 $
//###########################################################################
//
// FILE:	DSP2833x_Adc.c
//
// TITLE:	DSP2833x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

extern _iq ia;
extern _iq ib;
extern _iq ic;


#define ADC_usDELAY  5000L

Uint16 AD_BUF[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
int16 ADC_U;
int16 ADC_V; 

Uint16 I_A=0;
Uint16 I_B=0;
Uint16 I_C=0;

float32 IA_MAX=0;
float32 IB_MAX=0;
float32 IC_MAX=0;

int16 HALL_U=0; //U相霍尔的零点
int16 HALL_V=0; //V相霍尔的零点

float32 U_dc=0;
Uint16 U_dc_dis=0;
Uint16 GuoliuZhi=0;
Uint16 O_Current=0;
Uint16 DC_ON_flag=0;
Uint16 DC_ON_OPEN=0;
Uint32 DC_ON_CNT=0;

void DelayUS(Uint16 N_US) //1US延时 
{
    Uint16 i=0;  

	for(i=0;i<N_US;i++)
	{
	  asm("	NOP");
	
	}

} 
 
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);


    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

	    EALLOW;
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;   //使能ADC时钟与系统时钟同步
		ADC_cal();   //？
		EDIS;




    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.

    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
    DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels  5ms

}

void ADC_Soc_Init(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

   EALLOW; 
   SysCtrlRegs.HISPCP.all = 0x3;	// HSPCLK(11)六分频     150/6=25M
   EDIS;


    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

	    EALLOW;
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
		ADC_cal();
		EDIS;




    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.

    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
    DELAY_US(6000L);         // Delay before converting ADC channels  延迟6ms


     AdcRegs.ADCMAXCONV.bit.MAX_CONV1= 0x000f;       	// 总共16个
     AdcRegs.ADCTRL1.bit.ACQ_PS=0xf;//采集宽度 16个ADCLK
     AdcRegs.ADCTRL3.bit.ADCCLKPS=0X1;//12.5M
     AdcRegs.ADCTRL1.bit.SEQ_CASC=0x1;//级联
     AdcRegs.ADCTRL3.bit.SMODE_SEL=0;//顺序采样
     AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
     AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0;  // Enable SEQ1 interrupt (every EOS)



     
     //顺序指定
      AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
      AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x1;
      AdcRegs.ADCCHSELSEQ1.bit.CONV02=0x2;
      AdcRegs.ADCCHSELSEQ1.bit.CONV03=0x3;
      AdcRegs.ADCCHSELSEQ2.bit.CONV04=0x4;
      AdcRegs.ADCCHSELSEQ2.bit.CONV05=0x5;
      AdcRegs.ADCCHSELSEQ2.bit.CONV06=0x6;
      AdcRegs.ADCCHSELSEQ2.bit.CONV07=0x7;
      AdcRegs.ADCCHSELSEQ3.bit.CONV08=0x8;
      AdcRegs.ADCCHSELSEQ3.bit.CONV09=0x9;
      AdcRegs.ADCCHSELSEQ3.bit.CONV10=0xa;
      AdcRegs.ADCCHSELSEQ3.bit.CONV11=0xb;
      AdcRegs.ADCCHSELSEQ4.bit.CONV12=0xc;
      AdcRegs.ADCCHSELSEQ4.bit.CONV13=0xd;
      AdcRegs.ADCCHSELSEQ4.bit.CONV14=0xe;
      AdcRegs.ADCCHSELSEQ4.bit.CONV15=0xf;
      
     // Set up Event Trigger with CNT_zero enable for Time-base of EPWM1
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;     // Enable SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL = 1;    // Enable CNT_zero event for SOCA
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;     // Generate SOCA on the 1st event
	EPwm1Regs.ETCLR.bit.SOCA = 1;       // Clear SOCA flag





}


void Ad_CaiJi(void)
{	
           //读取
while (AdcRegs.ADCST.bit.SEQ1_BSY == 1)   //==1——SEQ1忙
        {;}

       AD_BUF[0]= AdcRegs.ADCRESULT0>>4;
       AD_BUF[1]= AdcRegs.ADCRESULT1>>4;
       AD_BUF[2]= AdcRegs.ADCRESULT2>>4;
       AD_BUF[3]= AdcRegs.ADCRESULT3>>4;
       AD_BUF[4]= AdcRegs.ADCRESULT4>>4;
       AD_BUF[5]= AdcRegs.ADCRESULT5>>4;
       AD_BUF[6]= AdcRegs.ADCRESULT6>>4;
       AD_BUF[7]= AdcRegs.ADCRESULT7>>4;
       AD_BUF[8]= AdcRegs.ADCRESULT8>>4;
       AD_BUF[9]= AdcRegs.ADCRESULT9>>4;
       AD_BUF[10]= AdcRegs.ADCRESULT10>>4;
       AD_BUF[11]= AdcRegs.ADCRESULT11>>4;
       AD_BUF[12]= AdcRegs.ADCRESULT12>>4;
       AD_BUF[13]= AdcRegs.ADCRESULT13>>4;
       AD_BUF[14]= AdcRegs.ADCRESULT14>>4;
       AD_BUF[15]= AdcRegs.ADCRESULT15>>4;

     if(Run_PMSM==2)
     {
          HALL_V=AD_BUF[11]+AD_BUF[10]+AD_BUF[9]+AD_BUF[8];//
          HALL_V=HALL_V>>2;
          
          HALL_U=AD_BUF[15]+AD_BUF[14]+AD_BUF[13]+AD_BUF[12];//
          HALL_U=HALL_U>>2;

     }

     ADC_V=AD_BUF[11]+AD_BUF[10]+AD_BUF[9]+AD_BUF[8];
     ADC_V=ADC_V>>2;

     ADC_U=AD_BUF[15]+AD_BUF[14]+AD_BUF[13]+AD_BUF[12];//
     ADC_U=ADC_U>>2;
//0.732421875=3000mv/4096
//0.010986328125=0.732421875*0.01*1.5  1mv 0.01安    0.01:霍尔电流传感器系数  1.5：放大倍数
     ic=_IQdiv(_IQmpy((_IQ(ADC_U)-_IQ(HALL_U)),_IQ(0.010986328125)),_IQ(E_Ding_DianLiu));//MV
    
     ib=_IQdiv(_IQmpy((_IQ(ADC_V)-_IQ(HALL_V)),_IQ(0.010986328125)),_IQ(E_Ding_DianLiu));//MV
     
     ia=-ic-ib;    // Compute phase-c current
}


void DC_Link(void)//计算母线电压
{
   
    U_dc=0.2951*(AD_BUF[7]+AD_BUF[6]);  //硬件：ADCIN3——Udc 母线电压  0.2951：电压采集系数
	U_dc=U_dc*0.732421875/2.0;//电压取平均
 
    U_dc_dis=U_dc; 

    if((Run_PMSM==1)&&(DC_ON_flag==0))//运行中掉电 则停机
    {
        if(U_dc_dis<5)//执行停机
        {
        DC_ON_1;   //
        DC_ON_flag=1;
        DC_ON_OPEN=2;
        }
    }

        if(U_dc_dis>370)//过压，执行停机
        {
        DC_ON_1; //警报灯
        DC_ON_flag=1;
        DC_ON_OPEN=3;
        }

     if(GpioDataRegs.GPBDAT.bit.GPIO32==1)
     {
        if(U_dc_dis>30)
        {
	     DC_ON_1;//警报灯
         DC_ON_flag=1;
         DC_ON_OPEN=4;
         Pwm_EN_1;
        }
     }
}

void JiSuan_Dl(void)//计算电流
{
       float32 IA,IB,IC;
    static Uint16 i=0;
    
    IC=(ADC_U-HALL_U)*0.10986328125;//放大10倍 
    IB=(ADC_V-HALL_V)*0.10986328125;//放大10倍
    IA=0-IB-IC;
    if(IB<0.0)
    {IB=-IB;}
    if(IA<0.0)
    {IA=-IA;}
     if(IC<0.0)
    {IC=-IC;}

    if(IA>IA_MAX)
    {
        IA_MAX=IA;
    }
    if(IB>IB_MAX)
    {
        IB_MAX=IB;
    }
      if(IC>IC_MAX)
    {
        IC_MAX=IC;
    }
    i++;
    if(i==300)
    {
    I_A=IA_MAX/1.414;//有效值
    I_B=IB_MAX/1.414;//有效值
    I_C=IC_MAX/1.414;//有效值
    i=0;
    IB_MAX=0;
    IA_MAX=0;
    IC_MAX=0;

    
 if((I_A>GuoliuZhi)||(I_B>GuoliuZhi)||(I_C>GuoliuZhi))    //过流保护
    {
        DC_ON_1;   //警报灯
        eva_close();//过流保护， 停机
        Pwm_EN_1;
        O_Current=1;
        Run_PMSM=2;

    }
    }

  if((IA>80.0)||(IB>80.0)||(IC>80.0))
    {
        DC_ON_1;
        eva_close();//过流保护， 停机
        Pwm_EN_1;
        O_Current=2;
        Run_PMSM=2;        

    }

} 






void JiSuan_AvgSpeed(void)//计算平均转速
{
    
    static Uint16 i=0;
    static _iq temp=0;

    if(Run_PMSM==1)
    { 
    temp+=Speed;
    i++;
    
    if(i==10000)
    {
     
    temp=_IQabs(_IQdiv(temp, _IQ(10000)));//求平均值
    speed_dis =_IQtoF(_IQmpy(_IQ(BaseSpeed),temp)); 

     temp=0;
     i=0;  
        }
    }
    else
    {
     temp=0;
     i=0;  
    }
} 


