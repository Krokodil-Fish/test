//*****************************************************************************************************
//Flash��RAM����汾�л�˵��(����Ĭ��Ϊram�汾)
//
//һ.�л�ΪFlash��д�汾����
//1.���������е�:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  ��������ȡ��ע��
//2.�������е�28335_RAM_lnk.cmd�ӹ�����ɾ�������CMD�ļ����µ�F28335.cmd�ļ���ȫ����һ�μ�����д��
//
//��.�л�ΪRAM���߷���汾����
//1.���������е�:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  ��������ע�͵�
//2.�������е�F28335.cmd�ӹ�����ɾ�������CMD�ļ����µ�28335_RAM_lnk.cmd�ļ���ȫ����һ�μ��ɡ�
//
//*****************************************************************************************************


#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
interrupt void cpu_timer0_isr(void); 
interrupt void EPWM_1_INT(void);    //���������Ŀ���Ƶ��10K�����ж����� 100us
interrupt void SCIBRX_ISR(void);
interrupt void INT3_ISR(void);
void Init_SiShu(void);

//*****************************************************************************************************
//ȫ�ֱ����������ʼ��
//***************************************************************************************************** 
float32 i=0;
float32 j=0;
float32 k=0;
Uint16 IsrTicker = 0;
Uint16 BackTicker = 0; //���ڴ�������
Uint16 T1Period=0;     // T1��ʱ������(Q0)
Uint16 T3Period = 0;   
float32 Modulation=0.25;    // ���Ʊ�
int16 MPeriod=0;
int32 Tmp=0;
_iq PolePairs=_IQ(4);    ////���������

//:::::::::::::::::::::::::::λ�û���������:::::::::::::::::::::::::::
long PlaceError=0,Place_now=0, Now_P=0,//Ȧ��
              OutPreSat_Place=0;//λ�ñ���ֵ����
Uint16 PlaceSetBit=0;  //λ���趨��־λ

int32 	PosCount = 0;


float32 MfuncF1=0;
float32 MfuncF2=0;
float32 MfuncF3=0;  
//===============ת�ӳ�ʼλ�ö�λ=============================  
Uint16 LocationFlag=1;
Uint16 LocationEnd=0; 
Uint16 Position=1;
Uint16 PositionPhase60=1;
Uint16 PositionPhase120=2;
Uint16 PositionPhase180=3; 
Uint16 PositionPhase240=4;
Uint16 PositionPhase300=5;
Uint16 PositionPhase360=6;  

//===============DACģ��===================================== 
_iq DACTemp0=0;
_iq DACTemp1=0;
_iq DACTemp2=0; 

_iq MfuncC1=0;
_iq MfuncC2=0;
_iq MfuncC3=0; 
Uint16 ZhengFan=1;  
 
//===============ת���ٶȼ���===================================== 
Uint16 SpeedLoopPrescaler = 10;     // �ٶȻ�����
Uint16 SpeedLoopCount = 1;          // �ٶȻ�����  
_iq NewRawTheta=0;
_iq OldRawTheta=0; 
_iq SpeedRpm=0;                     //�ٶȣ���λ��ת/ÿ����
Uint16 Hall_Fault=0;
_iq RawThetaTmp=0;
float32 SpeedRef=0;
_iq Speed=0;                        //�ٶȣ�����ֵ

//===============ת�ӽǶȼ���===================================
Uint16 DirectionQep=0;               //ת����ת����
_iq RawTheta=0;
_iq OldRawThetaPos = 0;


_iq TotalPulse=0; 

_iq MechTheta = 0;                   //��е�Ƕȣ���λ����
_iq ElecTheta = 0;                   //�����Ƕȣ���λ����
_iq	AnglePU=0;                       //�Ƕȱ��ۻ�
_iq	Cosine=0;
_iq	Sine=0;



//===============���������������============================ 
_iq ia=0;
_iq ib=0;
_iq ic=0;
_iq ialfa=0;
_iq ibeta=0; 
_iq id=0;
_iq iq=0; 

//===============PI��������������============================ 
_iq ID_Given=0;
_iq ID_Ref=0;
_iq ID_Fdb=0;
_iq ID_Error=0;  

_iq ID_Up=0;
_iq ID_Up1=0;
_iq ID_Ui=0;
_iq ID_OutPreSat=0;
_iq ID_SatError=0;
_iq ID_OutMax=_IQ(1);
_iq ID_OutMin=_IQ(-1); 
_iq ID_Out=0;

_iq IQ_Given=0;
_iq IQ_Ref=0;
_iq IQ_Fdb=0;
_iq IQ_Error=0; 
 


_iq IQ_Up=0;
_iq IQ_Up1=0;
_iq IQ_Ui=0;
_iq IQ_OutPreSat=0;
_iq IQ_SatError=0;
_iq IQ_OutMax=_IQ(1);
_iq IQ_OutMin=_IQ(-1); 
_iq IQ_Out=0; 

_iq Speed_Given=_IQ(0.2); //�ٶȸ���    ����ֵ 0.2==>600RPM�����ת��1.0==>3000RPM
_iq Speed_Ref=0;
_iq Speed_Fdb=0;
_iq Speed_Error=0; 
 

_iq Speed_Up=0;
_iq Speed_Up1=0;
_iq Speed_Ui=0;
_iq Speed_OutPreSat=0;
_iq Speed_SatError=0;
_iq Speed_OutMax=_IQ(0.99999);
_iq Speed_OutMin=-_IQ(0.99999);
_iq Speed_Out=0;  
Uint16 Speed_run=0;

//===============SVPWM����==================================== 
Uint16 Sector = 0; 
_iq	Ualfa=0;  		//���룺�ο���ѹʸ����alph��ķ���
_iq	Ubeta=0;		//���룺�ο���ѹʸ����beta��ķ���
_iq	Ud=0;		
_iq	Uq=0;			
_iq	B0=0;			
_iq	B1=0;
_iq	B2=0;
_iq	X=0;
_iq	Y=0;
_iq	Z=0;
_iq	t1=0;
_iq	t2=0;
_iq	Ta=0;        //�����A������Ƚ����ıȽ�ʱ��Tcmpa
_iq	Tb=0;		//�����B������Ƚ����ıȽ�ʱ��Tcmpa
_iq	Tc=0;		//�����C������Ƚ����ıȽ�ʱ��Tcmpa
_iq	MfuncD1=0;
_iq	MfuncD2=0;
_iq	MfuncD3=0; 
//===================================================================
Uint16 Run_PMSM=2;
float32 TEMP2=0;
_iq MechScaler=_IQ(0.0);          
_iq SpeedScaler=_IQ(0.00);        
Uint16 speed_give=0;
Uint16 HallAngle=0;
Uint16 BuChang=416;
int16 TotalCnt=0;
_iq RawCnt1=0;
_iq RawCnt2=0;
Uint16 ShangDian_Err=0;


//========================�ٶȻ�PI����=================================
_iq Speed_Kp=_IQ(8);
_iq Speed_Ki=_IQ(0.005);
//=====================================================================

//========================Q�������PI����==============================
_iq IQ_Kp=_IQ(0.3);
_iq IQ_Ki=_IQ(0.002);
//=====================================================================

//========================D�������PI����==============================
_iq ID_Kp=_IQ(0.3);
_iq ID_Ki=_IQ(0.002);
//=====================================================================

long PlaceSet=1000000;//λ�û�������
Uint16 PlaceEnable=0;//λ�û�ʹ��  1 ʹ�� ;  0 ��ֹ

//=====================��������========================================
float32 E_Ding_DianLiu=4.2;        //���õ���Ķ��������Чֵ  ��λA    ��С���㣬�õ����ȸ�����
Uint16 BaseSpeed=3000;              //���õ���ת��            �޷�������
            
                
 


void main(void)
{


   InitSysCtrl();
 
   InitGpio(); 
   Pwm_EN_1;     //GPIO29 ΪGPIO���� �����ʽʱ    ������ ����Ϊ �ߵ�ƽ���Ӷ�������һ��оƬ���ã���3.3Vת��Ϊ5V���Ӷ�����IPM����оƬ�������Ӧ���� 135 246

  
   DINT;
 
   InitPieCtrl(); 
   IER = 0x0000;
   IFR = 0x0000;
 
   InitPieVectTable();
 
   EALLOW;  // This is needed to write to EALLOW protected registers 
  // PieVectTable.TINT0 = &cpu_timer0_isr; 
   PieVectTable.EPWM1_INT=&EPWM_1_INT;     //ָ���жϺ���  PWM�ж�
   PieVectTable.SCIRXINTB= &SCIBRX_ISR;   //���ô���B�����жϵ��ж�����
   PieVectTable.XINT3=&INT3_ISR;        //�ⲿ�ж�

   EDIS;    // This is needed to disable write to EALLOW protected registers
 
 // InitCpuTimers(); 
   InitSci_C();     //485���ڵĳ�ʼ������  9600
   InitSci_B();		//232���ڵĳ�ʼ������  115200
   InitSpi();

   
   
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);   //ȡ��ע������д��������
   InitFlash();
  
   //ePWM.c
   InitEPwm_1_2_3();//����epwm��ʼ������
   QEP_Init(); //qep��ʼ��

   Init_SiShu();
   ADC_Soc_Init();   //ADCģ���ʼ��
   
 
	eva_close();   //��ʸ������ϵͳ��ʼ����ǰ��ǿ��ePWM123��I/O�ڲ����pwm��
   Ad_CaiJi();    //�����û������ʱ��������
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   

   if(AD_BUF[7]<150)     //150���� ��ô����  ֱ��ĸ�ߵ�ѹ
   {
	Pwm_EN_0;//�͵�ƽ ����PWMʹ��
    
    }
   else
   {
    Pwm_EN_1;//�ߵ�ƽ ��ֹPWMʹ��
	ShangDian_Err=1;

   }
    
    
    DELAY_US(1000000);
    
   
   IER |= M_INT3;
   IER |= M_INT9;
   IER |= M_INT12;
   //PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//timer0
   PieCtrlRegs.PIEIER3.bit.INTx1=1;//epwm1int
   PieCtrlRegs.PIEIER9.bit.INTx3=1;//scib
   PieCtrlRegs.PIEIER12.bit.INTx1=1;//xint3
    
     Init_lcd();
 
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM
    

   for(;;)
     {
        CPU_RUN();
        DC_Link();
        deal_key();
        LCD_DIS();
	    TX232_ChuLi();   

   }

}

interrupt void EPWM_1_INT(void)
{
       _iq t_01,t_02;
       IPM_BaoHu();
    Show_time++;
    Show_time2++;
       if(Show_time2==1000)//1��
    {
        Show_time2=0;
        lcd_dis_flag=1;


    }
    
   
  Read_key();
  Ad_CaiJi();   //
   JiSuan_Dl();
   JiSuan_AvgSpeed();

    

if(Run_PMSM==1&&IPM_Fault==0)
{

    //DAC1_out(_iq data);  ע�� data �Ǳ�ôֵŶ  dacֻ�����0��5v����ѹ ,���dataС��0 �ͻ����Ϊ0

     //DAC1_out(Speed_Fdb);//����ٶȷ���
   // DAC2_out(IQ_Fdb);//���iQ��������
//void LuBo(_iq Ch1,_iq Ch2,_iq Ch3,_iq Ch4)//�������Ǳ�ôֵ Q��ʽ,֪����ô���˰�
   //----ch1--ch2-ch3-ch4
LuBo(ia, ib, ic,Speed);

   

 // DAC1_out(ID_Fdb);//���iD��������

	if(LocationFlag!=LocationEnd)
	{ 
            Modulation=0.95;
            HallAngle=0;
        if(GpioDataRegs.GPCDAT.bit.GPIO78) //W
        {
		HallAngle+=1;
        }
		if(GpioDataRegs.GPCDAT.bit.GPIO77)//V
		{
            HallAngle+=2;
            }
		
		if(GpioDataRegs.GPCDAT.bit.GPIO76)//U
		{
            HallAngle+=4;

        }
        switch(HallAngle)
			{
				case 5:
					Position=PositionPhase60;
                    LocationFlag=LocationEnd;//��λ����
				EQep1Regs.QPOSCNT =BuChang*0+BuChang/2;     // ��ͬ������Ӧ��ͬ�Ƕ�  ��    QPOSCNT 208�� 2288  ��������� 416
                 OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				
				break;
				case 1:
					Position=PositionPhase360;
                     LocationFlag=LocationEnd;//��λ����
				EQep1Regs.QPOSCNT =BuChang*5+BuChang/2; 
               OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;
				case 3:
					Position=PositionPhase300;
                     LocationFlag=LocationEnd;//��λ����
				EQep1Regs.QPOSCNT =BuChang*4+BuChang/2; 
                 OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;
				case 2:
					Position=PositionPhase240;
                     LocationFlag=LocationEnd;//��λ����
				EQep1Regs.QPOSCNT =BuChang*3+BuChang/2; 
                  OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;
				case 6:
					Position=PositionPhase180;
                     LocationFlag=LocationEnd;//��λ����
				EQep1Regs.QPOSCNT =BuChang*2+BuChang/2; 
                  OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;
				case 4:
					Position=PositionPhase120;
                     LocationFlag=LocationEnd;//��λ����
				    EQep1Regs.QPOSCNT =BuChang*1+BuChang/2;  
                     OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;
                default:
                    DC_ON_1;
                    Run_PMSM=2;
                    eva_close();
                    Hall_Fault=1;//�����źŴ�������ֹͣ
                    break;
			}
       

        
	} 
//=====================================================================================================
//��ʼλ�ö�λ��������ʼ�ջ�����
//=====================================================================================================
	else if(LocationFlag==LocationEnd)
	{  
		  
       
//======================================================================================================
//QEP�Ƕȼ���
//====================================================================================================== 

// ��ת�����ж� 
		DirectionQep = EQep1Regs.QEPSTS.bit.QDF;//  ���ת������ 0˳1��
		
        RawTheta = _IQ(EQep1Regs.QPOSCNT);   //���λ�ü�������ֵ
        
		if(DirectionQep ==1) //��������������˳ʱ�룻
		{
	
		 
              if((OldRawThetaPos>324403200) && (RawTheta<_IQ(900)))   //��  ��������ͬʱ��Ҫ����ʶ���Ȧ  324403200��9900��Q15��ʽ
			{
				PosCount += TotalCnt;  //���ۼ�  ����Ƕ�Ϊ�� 0~360
			}

             
			Place_now= _IQtoF(RawTheta)+PosCount;  //һȦ360��ֳ���10000�����壬ÿ������0.036��
			OldRawThetaPos = RawTheta;
			
            
            
            
		}
		else if(DirectionQep ==0)//�ݼ�������������ʱ��
		{		 
             
			
              if((RawTheta>294912000) && (OldRawThetaPos<_IQ(1000)))    //ʶ���Ȧ  294912000��9000��Q15��ʽ��if������������ 9500 500Ҳ��
			{
				PosCount -= TotalCnt; //TotalCnt 4*2500  PosCount�Ǹ���ֵ�����ۼ�    ����������Ƕ�Ϊ�� -360~0
			}
			Place_now = _IQtoF(RawTheta)+PosCount;
			OldRawThetaPos = RawTheta;
               
               
               
            
		} 
		MechTheta = _IQmpy(1179,RawTheta);   //1179��0.036��Q15��ʽ  �������100us����һ�λ�е�Ƕȣ�����ת���˶������壬Ȼ������Ƕ�
         if(MechTheta>_IQ(360))    //2pi    �Ƕ����� 0~360
        {MechTheta=MechTheta-_IQ(360);}
         if(MechTheta<_IQ(-360))
        {MechTheta=MechTheta+_IQ(360);}
		ElecTheta = _IQmpy(PolePairs,MechTheta);   
	
		AnglePU=_IQdiv(ElecTheta,_IQ(360))+14876;   //����ֵ=ʵ��ֵ/�ֵ   +У��0.45��
	   	Sine = _IQsinPU(AnglePU); ///IQ15��ʽ���������ұ���ֵ
	   	Cosine = _IQcosPU(AnglePU);    

      


//======================================================================================================
//QEP�ٶȼ���
//====================================================================================================== 

	    if (SpeedLoopCount>=SpeedLoopPrescaler)       //�����ٶȻ�������10*100us=1ms
	    {   
// ��ת�����ж� 
			DirectionQep = EQep1Regs.QEPSTS.bit.QDF;		//�������巽��	0˳1��
 			NewRawTheta =_IQ(EQep1Regs.QPOSCNT);            //���32λλ�ü�������ֵ
// �����е�Ƕ�
			if(DirectionQep ==1) //����������
			{
               
                
				RawThetaTmp =  OldRawTheta-NewRawTheta ; //����OLD 9500 new500
				if(RawThetaTmp > _IQ(0))     //��Ȧ
				{
				 RawThetaTmp = RawThetaTmp - TotalPulse;    //9500-500-10000 =-1000
				}
                
                
                
			}
			else if(DirectionQep ==0) //�ݼ�����
			{

              
                
                RawThetaTmp =OldRawTheta-NewRawTheta;   //����old500 ��new 9500
				if(RawThetaTmp < _IQ(0))
				{
				 RawThetaTmp = RawThetaTmp + TotalPulse;   //500-9500+10000=1000
				}
                
                
                
                
			}
			//�ٶȼ��㣬DirectionQep ==0 ʱ��ת �ٶ�Ϊ����=1ʱ��ת �ٶ�Ϊ��
			Speed = _IQmpy(RawThetaTmp,65);  				//M�����٣�T=1ms��RawThetaTmp*60/��2500*4��*0.001 r/min =6*RawThetaTmp PU=6*RawThetaTmp*2^15/����3000  �õ�  65��������
			OldRawTheta = NewRawTheta; 
		    SpeedLoopCount=1; 
			RawThetaTmp=0; 

//=================λ�û�����===================================
//  if(PlaceEnable ==1)    //ת�Ӷ�λ����
//    {
//        PlaceError = PlaceSet + Place_now;   //λ���趨ֵ������������������ź�ֵ     Place_now�����ɸ���֧������ת  PlaceSet��ֵ���Ǳ�����2s������������
  
//		OutPreSat_Place = PlaceError;   //λ��ƫ��
//		if((PlaceError<=10000)&&(PlaceError>=-10000))    //10000 ��λ��������
//        {
//           OutPreSat_Place = PlaceError/3;  //λ�û��ı���������ڣ��޻��ֵ���
//		}

		
//        if (OutPreSat_Place> 2000)      //���ٶȸ����޶��ڶת���ڣ���֧������ת
//        {
//          SpeedRef =  0.5;
//        }
//        else if (OutPreSat_Place< -2000)
//        {
//          SpeedRef =  -0.5;
//        }
 //       else
//        {
//          SpeedRef = OutPreSat_Place/(float32)BaseSpeed;  //�ٶȸ������ۻ�
//        }

	   
//   	}

//=================�ٶȻ�PI===================================
//			Speed_Ref=_IQ(SpeedRef);   //�ٶȲο�ֵ��ô���� ͨ��������  ������λ�û�PI���ں�����OutPreSat_Place��λ���趨��ǰ��ֵPlaceSet
//			Speed_Fdb=Speed;      //��������������QEP�ٶȼ���
//
//			Speed_Error=Speed_Ref - Speed_Fdb;
//
//			Speed_Up=_IQmpy(Speed_Kp,Speed_Error);
//			Speed_Ui=Speed_Ui + _IQmpy(Speed_Ki,Speed_Up) + _IQmpy(Speed_Ki,Speed_SatError);

//			Speed_OutPreSat=Speed_Up+Speed_Ui;

//			if(Speed_OutPreSat>Speed_OutMax)      //�ٶȻ�������޷�ֵ0.99999��ô����
//				Speed_Out=Speed_OutMax;
//			else if(Speed_OutPreSat<Speed_OutMin)
//	 			Speed_Out=Speed_OutMin;
//			else
//				Speed_Out=Speed_OutPreSat;
	
//			Speed_SatError=Speed_Out-Speed_OutPreSat;

//			IQ_Given=Speed_Out;
//            Speed_run=1;  // ִ���ٶȻ�ʱ���˱�־��1
//		}
//	    else       // if (SpeedLoopCount>=SpeedLoopPrescaler)
//            {
//                SpeedLoopCount++;
//	    }
//        if(Speed_run==1)    // ia ib ic theta��ô��ȡ��
//       {

            //�ɰ�װ���������ڲ�ÿ���ϵĻ������������������ģ����Ÿ�Ӧ��Ϊ������ѹ�ź�
	    ialfa=ia;   //abc_to_alphbeta  ��ֵ�����CLARKE�任�任����ϵ��Ϊ2/3
		ibeta=_IQmpy(ia,_IQ(0.57735026918963))+_IQmpy(ib,_IQ(1.15470053837926));  

		id = _IQmpy(ialfa,Cosine) +_IQmpy(ibeta,Sine);  //��alphbeta����ϵ�µĵ���ת����dq����ϵ PARK�任
		iq = _IQmpy(ibeta,Cosine)- _IQmpy(ialfa,Sine) ; 

//======================================================================================================
//IQ����PID���ڿ���
//======================================================================================================  
		IQ_Ref=_IQ(0.5); 			//��0��1  �޸�   ��Ӧ0��2.4Nm   ��ת�ؿ���ʱ�������ٶȻ� λ�û�
		IQ_Fdb=iq;

		IQ_Error=IQ_Ref-IQ_Fdb;

		IQ_Up=_IQmpy(IQ_Kp,IQ_Error);
		IQ_Ui=IQ_Ui + _IQmpy(IQ_Ki,IQ_Up) + _IQmpy(IQ_Ki,IQ_SatError);

		IQ_OutPreSat=IQ_Up+IQ_Ui;

		if(IQ_OutPreSat>IQ_OutMax)
			IQ_Out=IQ_OutMax;
		else if(IQ_OutPreSat<IQ_OutMin)
		 	IQ_Out=IQ_OutMin;
		else 
			IQ_Out=IQ_OutPreSat;  

		IQ_SatError=IQ_Out-IQ_OutPreSat;  

		Uq=IQ_Out;

//======================================================================================================
//ID����PID���ڿ���
//======================================================================================================  
		ID_Ref=ID_Given;
		ID_Fdb=id;      //alphbeta_to_dq�任��id

		ID_Error=ID_Ref-ID_Fdb;

		ID_Up=_IQmpy(ID_Kp,ID_Error);    //����
		ID_Ui=ID_Ui+_IQmpy(ID_Ki,ID_Up)+_IQmpy(ID_Ki,ID_SatError);   //���ֱ������ID_SatError�������ֱ��ʹ���anti-windup

		ID_OutPreSat=ID_Up+ID_Ui;    

		if(ID_OutPreSat>ID_OutMax)   //�ﵽ������  �޷�����
			ID_Out=ID_OutMax;
		else if(ID_OutPreSat<ID_OutMin)  //�ﵽ������  �޷�����
		 	ID_Out=ID_OutMin;
		else 
			ID_Out=ID_OutPreSat;  

		ID_SatError=ID_Out-ID_OutPreSat;     //���ֱ�����ʵ��ֵ��anti-windup�ļ���ֵ

		Ud=ID_Out;

//======================================================================================================
//IPark�任  Ҫ�õ�eQEP�ǶȲ����������d����alph��֮��ļн�
//====================================================================================================== 
		Ualfa = _IQmpy(Ud,Cosine) - _IQmpy(Uq,Sine);
		Ubeta = _IQmpy(Uq,Cosine) + _IQmpy(Ud,Sine); 
    
//======================================================================================================
//SVPWMʵ��
//====================================================================================================== 
        B0=Ubeta;       // �൱�ڶ��ྲֹ����--�����ྲֹ�任��Uabc
		B1=_IQmpy(_IQ(0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 
		B2=_IQmpy(_IQ(-0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2

		Sector=0;          //�����б�����N=a+2b+4c
		if(B0>_IQ(0)) Sector =1;  //A=1,else A=0
		if(B1>_IQ(0)) Sector =Sector +2;  //B=1,else B=0
		if(B2>_IQ(0)) Sector =Sector +4;   //C=1,else C=0

		X=Ubeta;//va        ����ʸ������ʱ���� X Y Z���������
		Y=_IQmpy(_IQ(0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 vb
		Z=_IQmpy(_IQ(-0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2 vc

		
	 if(Sector==1)
		{
			t_01=Z;
			t_02=Y;

       if((t_01+t_02)>_IQ(1))         //�Թ�����������е���
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));  //����ʱ�����
			Ta=Tb+t1;
			Tc=Ta+t2;
		}
		else if(Sector==2)
		{
			t_01=Y;
			t_02=-X;

             if((t_01+t_02)>_IQ(1))
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Ta=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tc=Ta+t1;
			Tb=Tc+t2;
 		} 
		else if(Sector==3)
 	    {
			t_01=-Z;
			t_02=X;

             if((t_01+t_02)>_IQ(1))
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Ta=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tb=Ta+t1;
			Tc=Tb+t2;	
	    } 
	    else if(Sector==4)
	    {
			t_01=-X;
			t_02=Z;
             if((t_01+t_02)>_IQ(1))
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Tc=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tb=Tc+t1;
			Ta=Tb+t2;
	    } 
	    else if(Sector==5)
	    {
			t_01=X;
			t_02=-Y;
             if((t_01+t_02)>_IQ(1))
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Tc=Tb+t1;
	 		Ta=Tc+t2;
		}
		else if(Sector==6)
		{
			t_01=-Y;
			t_02=-Z;
             if((t_01+t_02)>_IQ(1))
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Tc=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));
			Ta=Tc+t1;
			Tb=Ta+t2;
		} 

		MfuncD1=_IQmpy(_IQ(2),(_IQ(0.5)-Ta));     //SVPWM��Ta/Tb/Tc��ȡֵ��Χ�任������һ
		MfuncD2=_IQmpy(_IQ(2),(_IQ(0.5)-Tb));
		MfuncD3=_IQmpy(_IQ(2),(_IQ(0.5)-Tc)); 

//======================================================================================================
//EVAȫ�Ƚ���������ֵ�������������
//====================================================================================================== 
	MPeriod = (int16)(T1Period * Modulation);              // Q0 = (Q0 * Q0)

	Tmp = (int32)MPeriod * (int32)MfuncD1;                    // Q15 = Q0*Q15������ȫ�Ƚ���CMPR1��ֵ
	 EPwm1Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD2;                    // Q15 = Q0*Q15������ȫ�Ƚ���CMPR2��ֵ
	 EPwm2Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD3;                    // Q15 = Q0*Q15������ȫ�Ƚ���CMPR3��ֵ
	 EPwm3Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2) 

         
	}     // if (SpeedLoopCount>=SpeedLoopPrescaler)       //�����ٶȻ�������10*100us=1ms
	}   //	else if(LocationFlag==LocationEnd)



}   //if run_pmsm==1&fault==0

if(DC_ON_flag==1)
{

        if(U_dc_dis<10)//ִ��ͣ������
        {
        eva_close();
        Run_PMSM=2;
        DC_ON_flag=0;

        }
    
}


EPwm1Regs.ETCLR.bit.INT=1;//����жϱ�־λ
PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}



interrupt void SCIBRX_ISR(void)     // SCI-B
{
	
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
    
 
	
}

void Init_SiShu(void)      //��������
{ 

 GuoliuZhi=15*E_Ding_DianLiu;     //�涨
 E_Ding_DianLiu=1.414*E_Ding_DianLiu;
 
 

 }

interrupt void INT3_ISR(void)
{ 
   
PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

//===========================================================================
// No more.
//===========================================================================
