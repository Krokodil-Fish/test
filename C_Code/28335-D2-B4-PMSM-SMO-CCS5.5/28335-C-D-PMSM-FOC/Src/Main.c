//*****************************************************************************************************
//Flash��RAM�����汾�л�˵��(����Ĭ��Ϊram�汾)
//
//һ.�л�ΪFlash��д�汾����
//1.���������е�:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  ��������ȡ��ע��
//2.�������е�28335_RAM_lnk.cmd�ӹ�����ɾ��������CMD�ļ����µ�F28335.cmd�ļ���ȫ����һ�μ�����д��
//
//��.�л�ΪRAM���߷���汾����
//1.���������е�:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  ��������ע�͵�
//2.�������е�F28335.cmd�ӹ�����ɾ��������CMD�ļ����µ�28335_RAM_lnk.cmd�ļ���ȫ����һ�μ��ɡ�
//
//*****************************************************************************************************


#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File



// Prototype statements for functions found within this file.
interrupt void cpu_timer0_isr(void); 
interrupt void EPWM_1_INT(void);
interrupt void SCIBRX_ISR(void);
interrupt void INT3_ISR(void);
void Init_SiShu(void);

//*****************************************************************************************************
//ȫ�ֱ����������ʼ��
//***************************************************************************************************** 
float32 i=0;
float32 j=0;
float32 k=0;
Uint16 l=0;
Uint16 IsrTicker = 0;
Uint16 BackTicker = 0; //���ڴ�������
Uint16 T1Period=0;     // T1��ʱ������(Q0)
Uint16 T3Period = 0;   
float32 Modulation=0.25;    // ���Ʊ�
int16 MPeriod=0;
int32 Tmp=0;

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
Uint32 OpenLoopcnt=0;
_iq MechTheta = 0;                   
_iq	AnglePU=0;                       //�Ƕȱ��ۻ�
_iq	Cosine=0;
_iq	Sine=0;
_iq ElecTheta2=0;
_iq  OpenLoop_IqGiven=_IQ(0.5);  

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
_iq MinSpeedLimit=_IQ(0.23);       
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
_iq LineEncoder=_IQ(1000);  
_iq Speed_Up=0;
_iq Speed_Up1=0;
_iq Speed_Ui=0;
_iq Speed_OutPreSat=0;
_iq Speed_SatError=0;
_iq Speed_OutMax=_IQ(0.99999);
_iq Speed_OutMin=-_IQ(0.99999);
_iq Speed_Out=0;  
Uint16 Speed_run=0;
Uint16 BaseSpeed=3000;            
//===============SVPWM����==================================== 
Uint16 Sector = 0; 
_iq	Ualfa=0;  		
_iq	Ubeta=0;		
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
_iq	Ta=0;
_iq	Tb=0;
_iq	Tc=0;
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
Uint16 BuChang=0;
int16 TotalCnt=0;
_iq RawCnt1=0;
_iq RawCnt2=0;
Uint16 ShangDian_Err=0;
_iq MinSpeedRef=0;
_iq theta_err=0;
Uint16 kkkk=0;

Uint16 suduchangge=0;

_iq ElecTheta3=0;
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


//=====================��������========================================

float32 E_Ding_DianLiu=1.56;        //���õ���Ķ������Чֵ ��λA
 			      


  
 
 


void main(void)
{


   InitSysCtrl();
 
   InitGpio(); 
   Pwm_EN_1;

  
   DINT;
 
   InitPieCtrl(); 
   IER = 0x0000;
   IFR = 0x0000;
 
   InitPieVectTable();
 
   EALLOW;  // This is needed to write to EALLOW protected registers 
  // PieVectTable.TINT0 = &cpu_timer0_isr; 
   PieVectTable.EPWM1_INT=&EPWM_1_INT;
   PieVectTable.SCIRXINTB= &SCIBRX_ISR;   //���ô���B�����жϵ��ж�����
   PieVectTable.XINT3=&INT3_ISR;

   EDIS;    // This is needed to disable write to EALLOW protected registers
 
 // InitCpuTimers(); 
   InitSci_C();
   InitSci_B();
   InitSpi();

   
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   InitFlash();
  
   InitEPwm_1_2_3();//pwm��ʼ��
   QEP_Init(); //qep��ʼ��
   
   ADC_Soc_Init();
   Init_SiShu();
   
	eva_close();  
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   
   

   if(AD_BUF[4]<120)
   {
	Pwm_EN_0;//����PWMʹ��
    
    }
   else
   {
    Pwm_EN_1;//��ֹPWMʹ��
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
  Ad_CaiJi();  
   JiSuan_Dl();

    

if(Run_PMSM==1&&IPM_Fault==0)
{
    Modulation=0.95; 

    //void LuBo(_iq Ch1,_iq Ch2,_iq Ch3,_iq Ch4)//�������Ǳ�ôֵ Q��ʽ,֪����ô���˰�
   //----ch1--ch2-ch3-ch4
LuBo(ia, ib, ic,EstimatedSpeed);
    
     //clark�任
        ialfa=ia;
		ibeta=_IQmpy(ia,_IQ(0.57735026918963))+_IQmpy(ib,_IQ(1.15470053837926));  


// ------------------------------------------------------------------------------
//   SMO�Ƕȹ���
// ------------------------------------------------------------------------------
          smopos_calc();   

       if(OpenLoop==1)	//��������
	{
		if (Startup_Lock < LockTime)
		{
			Startup_Lock += 1;
		}
							 
		else if (Startup_Ramp < EndSpeed)
		{
			Startup_Ramp += DELTA_STARTUP_RAMP;
		}
		else
		{
              
		  OpenLoop=0;//����Ӧ���л����ջ�
		  theta_err=qAngle-ElecTheta;
          IQ_Given=OpenLoop_IqGiven;
          
            
		}
      
        
		qAngle -=(Startup_Ramp >> 17);
        
        
        
        if(qAngle>_IQ(1))
        {
            qAngle=qAngle-_IQ(1);

        }

         if(qAngle<_IQ(0))
        {
            qAngle=qAngle+_IQ(1);

        }
        
       	Sine = _IQsinPU(qAngle);
	   	Cosine = _IQcosPU(qAngle); 
    }
    else
     {
 
            suduchangge=1;

            
            Speed_Out=0;
            Speed_SatError=0;
            Speed_Ui=OpenLoop_IqGiven;
            
            SpeedRef=_IQtoF(EstimatedSpeed);

            if(SpeedRef<0)
        {
            SpeedRef=-SpeedRef;
            speed_give=SpeedRef*100;

            if(speed_give<100)
        {
            speed_give=speed_give;
        SpeedRef=speed_give*1.0/-100;
        
        }
        }
        else
        {
        
   speed_give=SpeedRef*100;
        }
            
         
       
        
       	Sine = _IQsinPU(ElecTheta);
	    Cosine = _IQcosPU(ElecTheta); 


    }      
        
        //park�任
		id = _IQmpy(ialfa,Cosine) +_IQmpy(ibeta,Sine);
		iq = _IQmpy(ibeta,Cosine)- _IQmpy(ialfa,Sine); 

       if (SpeedLoopCount>=SpeedLoopPrescaler)
	    {  
            SpeedLoopCount=1;


               if(EstimatedSpeed<0)
			{speed_dis=_IQtoF(_IQmpy(EstimatedSpeed, _IQ(-100)));}
			else{
            speed_dis=_IQtoF(_IQmpy(EstimatedSpeed, _IQ(100)));}

		    

//=================�ٶȻ�PI===================================
			Speed_Ref=_IQ(SpeedRef);
            if(Speed_Ref>_IQ(1))
            {
                Speed_Ref=_IQ(1);
            }
              if(Speed_Ref<_IQ(-1))
            {
                Speed_Ref=_IQ(-1);
            }

          if(_IQabs(Speed_Ref)<MinSpeedLimit)
            {
                
                    Speed_Ref=MinSpeedLimit;

                
                

            }


             
			Speed_Fdb=EstimatedSpeed;

			Speed_Error=Speed_Ref - Speed_Fdb;

			Speed_Up=_IQmpy(Speed_Kp,Speed_Error);
			Speed_Ui=Speed_Ui + _IQmpy(Speed_Ki,Speed_Up) + _IQmpy(Speed_Ki,Speed_SatError);

			Speed_OutPreSat=Speed_Up+Speed_Ui;

			if(Speed_OutPreSat>Speed_OutMax)
				Speed_Out=Speed_OutMax;
			else if(Speed_OutPreSat<Speed_OutMin)
	 			Speed_Out=Speed_OutMin;
			else 
				Speed_Out=Speed_OutPreSat;  
	
			Speed_SatError=Speed_Out-Speed_OutPreSat;  



            
             if(OpenLoop==0)
            {
                
            if(suduchangge==1)
            {
            
            IQ_Given=Speed_Out; 
            }
             
            
            }
            
            
           
		} 
          else 
            {
                SpeedLoopCount++; 
	    }

//======================================================================================================
//IQ����PID���ڿ���
//======================================================================================================  
        if(OpenLoop==1)
        {
        IQ_Ref=OpenLoop_IqGiven;
        }
        else
        {
            IQ_Ref=IQ_Given;
        }
        
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
		ID_Fdb=id;

		ID_Error=ID_Ref-ID_Fdb;

		ID_Up=_IQmpy(ID_Kp,ID_Error);    
		ID_Ui=ID_Ui+_IQmpy(ID_Ki,ID_Up)+_IQmpy(ID_Ki,ID_SatError);   

		ID_OutPreSat=ID_Up+ID_Ui;    

		if(ID_OutPreSat>ID_OutMax)   
			ID_Out=ID_OutMax;
		else if(ID_OutPreSat<ID_OutMin)
		 	ID_Out=ID_OutMin;
		else 
			ID_Out=ID_OutPreSat;  

		ID_SatError=ID_Out-ID_OutPreSat;     

		Ud=ID_Out;

        
	   

//======================================================================================================
//IPark�任
//====================================================================================================== 
		Ualfa = _IQmpy(Ud,Cosine) - _IQmpy(Uq,Sine);
		Ubeta = _IQmpy(Uq,Cosine) + _IQmpy(Ud,Sine);		  

      

//======================================================================================================
//SVPWMʵ��
//====================================================================================================== 
        B0=Ubeta;
		B1=_IQmpy(_IQ(0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 
		B2=_IQmpy(_IQ(-0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2

		Sector=0;
		if(B0>_IQ(0)) Sector =1;
		if(B1>_IQ(0)) Sector =Sector +2;
		if(B2>_IQ(0)) Sector =Sector +4; 

		X=Ubeta;//va
		Y=_IQmpy(_IQ(0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 vb
		Z=_IQmpy(_IQ(-0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2 vc

		
	 if(Sector==1)
		{
			t_01=Z;
			t_02=Y;

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

		MfuncD1=_IQmpy(_IQ(2),(_IQ(0.5)-Ta));
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


      
// ------------------------------------------------------------------------------
//   �ٶȹ���
// ------------------------------------------------------------------------------
    EstimatedTheta=ElecTheta;
    speed_est_calc();

       if(suduchangge==1)
        {

     
        if(EstimatedSpeed<0)//���л�����ʧ��
        {
            DC_ON_1;
            eva_close();
            Run_PMSM=2;


        }


    
    
    
    }
 
 
        
       
   


}

if(DC_ON_flag==1)
{
    if(Run_PMSM==0)
    {
    DC_ON_CNT++;
    if(DC_ON_CNT==15000)
    {
          if(U_dc_dis>15)//��⵽�и�������
        {
          Run_PMSM=1;
		eva_open(); 
        DC_ON_OPEN=0;
       
         }
          else
          {
            DC_ON_1;
            DC_ON_OPEN=1;//��������û��
            Run_PMSM=2;


          }
        
        DC_ON_flag=0;
		DC_ON_CNT=0;
       

    }
    }
    else
    {
        if(U_dc_dis<10)//ִ��ͣ������
        {
        eva_close();
        Run_PMSM=2;
        DC_ON_flag=0;
        DC_ON_CNT=0;




        }




    }
}


EPwm1Regs.ETCLR.bit.INT=1;//����жϱ�־λ
PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}



interrupt void SCIBRX_ISR(void)     // SCI-B
{
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;

}

void Init_SiShu(void)
{
     float32 temp=0;
 

 GuoliuZhi=15*E_Ding_DianLiu;
 E_Ding_DianLiu=1.414*E_Ding_DianLiu;
 OpenLoop_IqGiven=_IQdiv(OpenLoop_IqGiven, _IQ(E_Ding_DianLiu));
 

 temp=MinSpeedRPM*1.0;
 temp=temp/BaseSpeed;
 MinSpeedRef=_IQ(temp);
 
 
 

 ParaInt();
 

 }

interrupt void INT3_ISR(void)
{ 
   
PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

//===========================================================================
// No more.
//===========================================================================