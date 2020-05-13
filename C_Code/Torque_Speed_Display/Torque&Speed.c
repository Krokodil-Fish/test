//*****************************************************************************************************
//Flash和RAM软件版本切换说明(程序默认为ram版本)
//
//一.切换为Flash烧写版本方法
//1.将主程序中的:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  两个函数取消注释
//2.将工程中的28335_RAM_lnk.cmd从工程中删除，添加CMD文件夹下的F28335.cmd文件，全编译一次即可烧写。
//
//二.切换为RAM在线仿真版本方法
//1.将主程序中的:MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//               InitFlash();
//  两个函数注释掉
//2.将工程中的F28335.cmd从工程中删除，添加CMD文件夹下的28335_RAM_lnk.cmd文件，全编译一次即可。
//
//*****************************************************************************************************
//**需要对主被动模式的智能切换做下一步编写，等采集好踏板力/力矩的信号导入到matlab中对它做力矩-速度的曲线，以仿真中的“检测反馈信号”，
//**形成实际中的转矩-速度对应关系表，查表即可修改iq的指令值

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
interrupt void cpu_timer0_isr(void); 
interrupt void EPWM_1_INT(void);
interrupt void SCIBRX_ISR(void);
interrupt void INT3_ISR(void);
void Init_SiShu(void);

//*****************************************************************************************************
//全局变量定义与初始化
//***************************************************************************************************** 
float32 i=0;
float32 j=0;
float32 k=0;
Uint16 IsrTicker = 0;
Uint16 BackTicker = 0; //用于次数计数
Uint16 T1Period=0;     // T1定时器周期(Q0)
Uint16 T3Period = 0;   
float32 Modulation=0.25;    // 调制比
int16 MPeriod=0;
int32 Tmp=0;
_iq PolePairs=_IQ(4); 
float limit=0.07;
Uint16 torquestart=0;
Uint16 speedstart=0;
//:::::::::::::::::::::::::::位置环变量定义:::::::::::::::::::::::::::
long PlaceError=0,Place_now=0, Now_P=0,//圈数
              OutPreSat_Place=0;//位置变量值定义
Uint16 PlaceSetBit=0;  //位置设定标志位

int32 	PosCount = 0;


float32 MfuncF1=0;
float32 MfuncF2=0;
float32 MfuncF3=0;  
//===============转子初始位置定位=============================  
Uint16 LocationFlag=1;
Uint16 LocationEnd=0; 
Uint16 Position=1;
Uint16 PositionPhase60=1;
Uint16 PositionPhase120=2;
Uint16 PositionPhase180=3; 
Uint16 PositionPhase240=4;
Uint16 PositionPhase300=5;
Uint16 PositionPhase360=6;  

//===============DAC模拟===================================== 
_iq DACTemp0=0;
_iq DACTemp1=0;
_iq DACTemp2=0; 

_iq MfuncC1=0;
_iq MfuncC2=0;
_iq MfuncC3=0; 
Uint16 ZhengFan=1;  
 
//===============转子速度计算===================================== 
Uint16 SpeedLoopPrescaler = 10;     // 速度环定标
Uint16 SpeedLoopCount = 1;          // 速度环计数  
_iq NewRawTheta=0;
_iq OldRawTheta=0; 
_iq SpeedRpm=0;                     //速度，单位：转/每分钟
Uint16 Hall_Fault=0;
_iq RawThetaTmp=0;
float32 SpeedRef=0;
_iq Speed=0;                        //速度，标幺值

//===============转子角度计算===================================
Uint16 DirectionQep=0;               //转子旋转方向
_iq RawTheta=0;
_iq OldRawThetaPos = 0;


_iq TotalPulse=0; 

_iq MechTheta = 0;                   //机械角度，单位：度
_iq ElecTheta = 0;                   //电气角度，单位：度
_iq	AnglePU=0;                       //角度标幺化
_iq	Cosine=0;
_iq	Sine=0;



//===============控制绕组电流计算============================ 
_iq ia=0;
_iq ib=0;
_iq ic=0;
_iq ialfa=0;
_iq ibeta=0; 
_iq id=0;
_iq iq=0; 

//===============PI控制器参数计算============================ 
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

_iq Speed_Given=_IQ(0.2); //速度给定    标幺值 0.2==>600RPM，最高转速1.0==>3000RPM
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

//===============SVPWM计算==================================== 
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
Uint16 BuChang=416;
int16 TotalCnt=0;
_iq RawCnt1=0;
_iq RawCnt2=0;
Uint16 ShangDian_Err=0;


//========================速度环PI参数=================================
_iq Speed_Kp=_IQ(8);
_iq Speed_Ki=_IQ(0.005);
//=====================================================================

//========================Q轴电流环PI参数==============================
_iq IQ_Kp=_IQ(0.3);
_iq IQ_Ki=_IQ(0.002);
//=====================================================================

//========================D轴电流环PI参数==============================
_iq ID_Kp=_IQ(0.3);
_iq ID_Ki=_IQ(0.002);
//=====================================================================

long PlaceSet=1000000;//位置环脉冲数
Uint16 PlaceEnable=0;//位置环使能  1 使能 ;  0 禁止

//=====================参数设置========================================
float32 E_Ding_DianLiu=4.2;        //设置电机的额定电流的有效值  单位A
Uint16 BaseSpeed=3000;              //设置电机额定转速
            
                
 


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
   PieVectTable.SCIRXINTB= &SCIBRX_ISR;   //设置串口B接受中断的中断向量
   PieVectTable.XINT3=&INT3_ISR;

   EDIS;    // This is needed to disable write to EALLOW protected registers
 
 // InitCpuTimers(); 
   InitSci_C();
   InitSci_B();
   InitSpi();

   
   
 //  MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
 //  InitFlash();
  
   InitEPwm_1_2_3();//pwm初始化
   QEP_Init(); //qep初始化

   Init_SiShu();
   ADC_Soc_Init();
   
 
	eva_close();  
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   

   if(AD_BUF[7]<150)
   {
	Pwm_EN_0;//允许PWM使能
    
    }
   else
   {
    Pwm_EN_1;//禁止PWM使能
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
    
     Init_lcd();  //初始化屏幕
 
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
       if(Show_time2==1000)//1秒
    {
        Show_time2=0;
        lcd_dis_flag=1;
    }
  Read_key();
  Ad_CaiJi();  
  JiSuan_Dl();
  JiSuan_AvgSpeed();

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////转速模式/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
 if(Run_PMSM==1&&IPM_Fault==0)
{
 if(speedstart==1&&torquestart==0)
 {
	 LuBo(ia, ib, Uq,Speed);   //示波器检测
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
					Position=PositionPhase60;// 1
                    LocationFlag=LocationEnd;//定位结束
				EQep1Regs.QPOSCNT =BuChang*0+BuChang/2;    //BuChang=416
                 OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
			    break;
				
				case 1:
					Position=PositionPhase360;
                     LocationFlag=LocationEnd;//定位结束
				EQep1Regs.QPOSCNT =BuChang*5+BuChang/2; 
               OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;

				case 3:
					Position=PositionPhase300;
                     LocationFlag=LocationEnd;//定位结束
				EQep1Regs.QPOSCNT =BuChang*4+BuChang/2; 
                 OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;

				case 2:
					Position=PositionPhase240;
                     LocationFlag=LocationEnd;//定位结束
				EQep1Regs.QPOSCNT =BuChang*3+BuChang/2; 
                  OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;

				case 6:
					Position=PositionPhase180;
                     LocationFlag=LocationEnd;//定位结束
				EQep1Regs.QPOSCNT =BuChang*2+BuChang/2; 
                  OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;

				case 4:
					Position=PositionPhase120;
                     LocationFlag=LocationEnd;//定位结束
				    EQep1Regs.QPOSCNT =BuChang*1+BuChang/2;  
                     OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
				break;

				default:
                    DC_ON_1;
                    Run_PMSM=2;
                    eva_close();
                    Hall_Fault=1;//霍尔信号错误启动停止
                    break;
			}
	} 
//=====================================================================================================
//初始位置定位结束，开始闭环控制
//=====================================================================================================
	else if(LocationFlag==LocationEnd)
	{  
		  
       
//======================================================================================================
//QEP角度计算
//====================================================================================================== 

// 旋转方向判定 
		DirectionQep = EQep1Regs.QEPSTS.bit.QDF;   //0逆时针方向 1顺时针方向
		
        RawTheta = _IQ(EQep1Regs.QPOSCNT);
        
		if(DirectionQep ==1) //递增计数，代表顺时针；
		{
	
		 
          if((OldRawThetaPos>324403200) && (RawTheta<_IQ(900)))  //324403200=_IQ(9900)
			{
				PosCount += TotalCnt;
			}

             
			Place_now= _IQtoF(RawTheta)+PosCount;
			OldRawThetaPos = RawTheta;

        }
		else if(DirectionQep ==0)//递减计数，代表逆时针
		    {
             
			
              if((RawTheta>294912000) && (OldRawThetaPos<_IQ(1000)))  //294912000=_IQ(9000)
			    {
				PosCount -= TotalCnt;
			    }
			Place_now = _IQtoF(RawTheta)+PosCount;
			OldRawThetaPos = RawTheta;
		    }
		 MechTheta = _IQmpy(1179,RawTheta);  //1179=360/10000*x2^15  是每个脉冲间隔代表的角度的q格式
		                                     //RawTheta是角度对应的脉冲数的q15格式，
		                                     //两个相乘  得到实际的转角角度的q15格式
         if(MechTheta>_IQ(360))              //如果大于360 说明加了一圈  就减去360
        {MechTheta=MechTheta-_IQ(360);}
         if(MechTheta<_IQ(-360))            //小于-360 说明反向转动多了一圈  加360即可
        {MechTheta=MechTheta+_IQ(360);}
		ElecTheta = _IQmpy(PolePairs,MechTheta);   //电角度=极对数*机械角度
	    AnglePU=_IQdiv(ElecTheta,_IQ(360))+14876;   //角度的标幺值=电角度的标幺值+矫正值（0.45）[厂家给定]
	   	Sine = _IQsinPU(AnglePU);
	   	Cosine = _IQcosPU(AnglePU);    

      


//======================================================================================================
//QEP速度计算     SpeedLoopCount计数10次，对速度环进行一次定标，运行一次PI控制      Epwm时基为100ms
//====================================================================================================== 

	  if (SpeedLoopCount>=SpeedLoopPrescaler)
	  {
// 旋转方向判定 
			DirectionQep = EQep1Regs.QEPSTS.bit.QDF;	//0——逆时针，1——顺时针
 			NewRawTheta =_IQ(EQep1Regs.QPOSCNT);        //当前角度对应的计数值
// 计算机械角度
			if(DirectionQep ==1) //递增计数，
			{
			RawThetaTmp = OldRawTheta-NewRawTheta ;
				if(RawThetaTmp > _IQ(0))
				{
				 RawThetaTmp = RawThetaTmp - TotalPulse;  
				}
			}
			else if(DirectionQep ==0) //递减计数
			{
            RawThetaTmp =OldRawTheta-NewRawTheta;
				if(RawThetaTmp < _IQ(0))
				{
				 RawThetaTmp = RawThetaTmp + TotalPulse;
				}
			}
			Speed = _IQmpy(RawThetaTmp,65);  	 //速度标幺值
//M法测速，T=1ms，RawThetaTmp*60/（2500*4）*0.001 r/min =6*RawThetaTmp PU=6*RawThetaTmp*2^15/基速3000  得到  65
			OldRawTheta = NewRawTheta; 
		    SpeedLoopCount=1; 
			RawThetaTmp=0; 

//=================位置环控制===================================
       if(PlaceEnable ==1)   //位置使能环 1使能 0禁止
       {
           PlaceError = PlaceSet + Place_now;   //100圈+0？
  		   OutPreSat_Place = PlaceError;
		 if((PlaceError<=10000)&&(PlaceError>=-10000))  //？怎么进入
        {
           OutPreSat_Place = PlaceError/3;   //1/3是为什么
		 }
            if (OutPreSat_Place> 2000)
             {
                SpeedRef =  0.5;
            }
             else if (OutPreSat_Place< -2000)
             {
                SpeedRef =  -0.5;
             }
              else
               {
               SpeedRef = OutPreSat_Place/(float32)BaseSpeed;
               }
  	   }

//=================速度环PI===================================
		    Speed_Ref=_IQ(SpeedRef);
			Speed_Fdb=Speed;
			Speed_Error=Speed_Ref - Speed_Fdb;
		    Speed_Up=_IQmpy(Speed_Kp,Speed_Error);
			Speed_Ui=Speed_Ui + _IQmpy(Speed_Ki,Speed_Up) + _IQmpy(Speed_Ki,Speed_SatError);
			Speed_OutPreSat=Speed_Up+Speed_Ui;   //PI控制
			if(Speed_OutPreSat>Speed_OutMax)
				Speed_Out=Speed_OutMax;
			else if(Speed_OutPreSat<Speed_OutMin)
	 			Speed_Out=Speed_OutMin;
			else
				Speed_Out=Speed_OutPreSat;

			Speed_SatError=Speed_Out-Speed_OutPreSat;  //超过限制才会有这项

			IQ_Given=Speed_Out;
           Speed_run=1;
	}
	else
      {
       SpeedLoopCount++;
	  }
//=============================================================

        if(Speed_run==1)
       {
	    ialfa=ia;
		ibeta=_IQmpy(ia,_IQ(0.57735026918963))+_IQmpy(ib,_IQ(1.15470053837926));  //幅值不变变换的克拉克变换
		     //第一个系数是根下1/3.第二个是2/根下3
		id = _IQmpy(ialfa,Cosine) +_IQmpy(ibeta,Sine);
		iq = _IQmpy(ibeta,Cosine)- _IQmpy(ialfa,Sine) ;    //坐标变化


//======================================================================================================
//IQ电流PID调节控制
//======================================================================================================  
		IQ_Ref=IQ_Given;
		IQ_Fdb=iq;
		IQ_Error=IQ_Ref-IQ_Fdb;
		IQ_Up=_IQmpy(IQ_Kp,IQ_Error);
		IQ_Ui=IQ_Ui + _IQmpy(IQ_Ki,IQ_Up) + _IQmpy(IQ_Ki,IQ_SatError);  //_IQmpy(IQ_Ki,IQ_SatError)由于超出限值引起的控制环增益

		IQ_OutPreSat=IQ_Up+IQ_Ui;

		if(IQ_OutPreSat>IQ_OutMax)
			IQ_Out=IQ_OutMax;
		else if(IQ_OutPreSat<IQ_OutMin)
		 	IQ_Out=IQ_OutMin;
		else 
			IQ_Out=IQ_OutPreSat;  

		IQ_SatError=IQ_Out-IQ_OutPreSat;  //超过限值时的变化量

		Uq=IQ_Out;

//======================================================================================================
//ID电流PID调节控制
//======================================================================================================  
		ID_Ref=ID_Given;   //0
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
//IPark变换
//======================================================================================================
		Ualfa = _IQmpy(Ud,Cosine) - _IQmpy(Uq,Sine);
		Ubeta = _IQmpy(Uq,Cosine) + _IQmpy(Ud,Sine);
//======================================================================================================
//SVPWM实现
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
               //七段式
			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));  //（t-t1-t2）/2
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
			Tb=Ta+t2;                                 //空间向量作用时间分别是2*t1、2*t2
		}
		MfuncD1=_IQmpy(_IQ(2),(_IQ(0.5)-Ta));
		MfuncD2=_IQmpy(_IQ(2),(_IQ(0.5)-Tb));
		MfuncD3=_IQmpy(_IQ(2),(_IQ(0.5)-Tc));
//======================================================================================================
//EVA全比较器参数赋值，用于驱动电机
//======================================================================================================
	MPeriod = (int16)(T1Period * Modulation);              // Q0 = (Q0 * Q0)

	Tmp = (int32)MPeriod * (int32)MfuncD1;                    // Q15 = Q0*Q15，计算全比较器CMPR1赋值
	 EPwm1Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD2;                    // Q15 = Q0*Q15，计算全比较器CMPR2赋值
	 EPwm2Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD3;                    // Q15 = Q0*Q15，计算全比较器CMPR3赋值
	 EPwm3Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)
	}
   }
//	speedstart=0;
 }
////////////////////////////////////////////////////////////////////////////////////////////////
//===============================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////转矩模式////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
  if(torquestart==1&&speedstart==0) //torquestart=1表示转矩模式：零转速，转矩可调
{
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
		Position=PositionPhase60;// 1
                LocationFlag=LocationEnd;//定位结束
		EQep1Regs.QPOSCNT =BuChang*0+BuChang/2;    //BuChang=416
                OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
	         break;

		case 1:
		Position=PositionPhase360;
                LocationFlag=LocationEnd;//定位结束
	        EQep1Regs.QPOSCNT =BuChang*5+BuChang/2;
                OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
		 break;

		case 3:
		Position=PositionPhase300;
                LocationFlag=LocationEnd;//定位结束
		EQep1Regs.QPOSCNT =BuChang*4+BuChang/2;
                OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
		 break;

		case 2:
		Position=PositionPhase240;
                LocationFlag=LocationEnd;//定位结束
		EQep1Regs.QPOSCNT =BuChang*3+BuChang/2;
                OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
	         break;

		case 6:
		Position=PositionPhase180;
                LocationFlag=LocationEnd;//定位结束
		EQep1Regs.QPOSCNT =BuChang*2+BuChang/2;
                OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
		 break;

		case 4:
		Position=PositionPhase120;
                LocationFlag=LocationEnd;//定位结束
                EQep1Regs.QPOSCNT =BuChang*1+BuChang/2;
                OldRawTheta=_IQ(EQep1Regs.QPOSCNT);
		 break;

		default:
                    DC_ON_1;
                    Run_PMSM=2;
                    eva_close();
                    Hall_Fault=1;//霍尔信号错误启动停止
                    break;
		}



	}
//=====================================================================================================
//初始位置定位结束，开始闭环控制
//=====================================================================================================
	else if(LocationFlag==LocationEnd)
	{


//======================================================================================================
//QEP角度计算
//======================================================================================================

// 旋转方向判定
		DirectionQep = EQep1Regs.QEPSTS.bit.QDF;   //0逆时针方向 1顺时针方向

        RawTheta = _IQ(EQep1Regs.QPOSCNT);

		if(DirectionQep ==1) //递增计数，代表顺时针；
		{


                    if((OldRawThetaPos>324403200) && (RawTheta<_IQ(900)))  //324403200=_IQ(9900) rawtheta在900到9900之间
			{
				PosCount += TotalCnt;
			}


			Place_now= _IQtoF(RawTheta)+PosCount;
			OldRawThetaPos = RawTheta;

        }
		    else if(DirectionQep ==0)//递减计数，代表逆时针
		       {

                         if((RawTheta>294912000) && (OldRawThetaPos<_IQ(1000)))  //294912000=_IQ(9000)
			    {
			    PosCount -= TotalCnt;
			    }
			    Place_now = _IQtoF(RawTheta)+PosCount;
			    OldRawThetaPos = RawTheta;
		       }
		 MechTheta = _IQmpy(1179,RawTheta);  //1179=360/10000*x2^15  是每个脉冲间隔代表的角度的q格式
		                                     //RawTheta是角度对应的脉冲数的q15格式，
		                                     //两个相乘  得到实际的转角角度的q15格式
                if(MechTheta>_IQ(360))              //如果大于360 说明加了一圈  就减去360
                {MechTheta=MechTheta-_IQ(360);}
                if(MechTheta<_IQ(-360))            //小于-360 说明反向转动多了一圈  加360即可
                {MechTheta=MechTheta+_IQ(360);}
		ElecTheta = _IQmpy(PolePairs,MechTheta);   //电角度=极对数*机械角度
	        AnglePU=_IQdiv(ElecTheta,_IQ(360))+14876;   //角度的标幺值=电角度的标幺值+矫正值（0.45）[厂家给定]
	   	Sine = _IQsinPU(AnglePU);
	   	Cosine = _IQcosPU(AnglePU);




//======================================================================================================
//QEP速度计算     SpeedLoopCount计数10次，对速度环进行一次定标，运行一次PI控制      Epwm时基为100us
//======================================================================================================

	  if (SpeedLoopCount>=SpeedLoopPrescaler)
	  {
// 旋转方向判定
	     DirectionQep = EQep1Regs.QEPSTS.bit.QDF;	//0——逆时针，1——顺时针
 	     NewRawTheta =_IQ(EQep1Regs.QPOSCNT);        //当前角度对应的计数值
// 计算机械角度
		if(DirectionQep ==1) //递增计数，
		{
	          RawThetaTmp = OldRawTheta-NewRawTheta ;
		  if(RawThetaTmp > _IQ(0))
		   {
		   RawThetaTmp = RawThetaTmp - TotalPulse;
		   }
		 }
		   else if(DirectionQep ==0) //递减计数
		     {
                       RawThetaTmp =OldRawTheta-NewRawTheta;
			if(RawThetaTmp < _IQ(0))
			{
			  RawThetaTmp = RawThetaTmp + TotalPulse;
		        }
		      }
			Speed = _IQmpy(RawThetaTmp,65);  	 //速度标幺值
//M法测速，T=1ms，RawThetaTmp*60/（2500*4）*0.001 r/min =6*RawThetaTmp PU=6*RawThetaTmp*2^15/基速3000  得到  65
			OldRawTheta = NewRawTheta;
		        SpeedLoopCount=1;
			RawThetaTmp=0;

//=================位置环控制===================================

//=================速度环PI===================================
		Speed_Ref=0;
	        Speed_Fdb=Speed;
		Speed_Error=Speed_Ref - Speed_Fdb;
		Speed_Up=_IQmpy(Speed_Kp,Speed_Error);
		Speed_Ui=Speed_Ui + _IQmpy(Speed_Ki,Speed_Up) + _IQmpy(Speed_Ki,Speed_SatError);
		Speed_OutPreSat=Speed_Up+Speed_Ui;   //PI控制
		if(Speed_OutPreSat>Speed_OutMax)
			Speed_Out=Speed_OutMax;
		else if(Speed_OutPreSat<Speed_OutMin)
	 		Speed_Out=Speed_OutMin;
		else
			Speed_Out=Speed_OutPreSat;

			Speed_SatError=Speed_Out-Speed_OutPreSat;  //超过限制才会有这项

			IQ_Given=Speed_Out;
           Speed_run=1;
	}
	else
      {
       SpeedLoopCount++;
	  }                   //时间环为1ms
//=============================================================

        if(Speed_run==1)
       {
	    ialfa=ia;
		ibeta=_IQmpy(ia,_IQ(0.57735026918963))+_IQmpy(ib,_IQ(1.15470053837926));  //幅值不变变换的克拉克变换
		     //第一个系数是根下1/3.第二个是2/根下3
		id = _IQmpy(ialfa,Cosine) +_IQmpy(ibeta,Sine);
		iq = _IQmpy(ibeta,Cosine)- _IQmpy(ialfa,Sine) ;    //坐标变化


//======================================================================================================
//IQ电流PID调节控制
//======================================================================================================
		IQ_Ref=IQ_Given;
		IQ_Fdb=iq;

		IQ_Error=IQ_Ref-IQ_Fdb;

		if(IQ_Error>_IQ(0))
			IQ_Error=_IQ(0);
		if(IQ_Error<-_IQ(limit))
			IQ_Error=-_IQ(limit);


		IQ_Up=_IQmpy(IQ_Kp,IQ_Error);
		IQ_Ui=IQ_Ui + _IQmpy(IQ_Ki,IQ_Up) + _IQmpy(IQ_Ki,IQ_SatError);  //_IQmpy(IQ_Ki,IQ_SatError)由于超出限值引起的控制环增益

		IQ_OutPreSat=IQ_Up+IQ_Ui;

		if(IQ_OutPreSat>IQ_OutMax)
			IQ_Out=IQ_OutMax;
		else if(IQ_OutPreSat<IQ_OutMin)
		 	IQ_Out=IQ_OutMin;
		else
			IQ_Out=IQ_OutPreSat;

		IQ_SatError=IQ_Out-IQ_OutPreSat;  //超过限值时的变化量

		Uq=IQ_Out;

//======================================================================================================
//ID电流PID调节控制
//======================================================================================================
		ID_Ref=ID_Given;   //0
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
//IPark变换
//====================================================================================================== 
		Ualfa = _IQmpy(Ud,Cosine) - _IQmpy(Uq,Sine);
		Ubeta = _IQmpy(Uq,Cosine) + _IQmpy(Ud,Sine); 
    
//======================================================================================================
//SVPWM实现
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
               //七段式
			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));  //（t-t1-t2）/2
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
			Tb=Ta+t2;                                 //空间向量作用时间分别是2*t1、2*t2
		} 

		MfuncD1=_IQmpy(_IQ(2),(_IQ(0.5)-Ta));
		MfuncD2=_IQmpy(_IQ(2),(_IQ(0.5)-Tb));
		MfuncD3=_IQmpy(_IQ(2),(_IQ(0.5)-Tc)); 

//======================================================================================================
//EVA全比较器参数赋值，用于驱动电机
//====================================================================================================== 
	MPeriod = (int16)(T1Period * Modulation);              // Q0 = (Q0 * Q0)

	Tmp = (int32)MPeriod * (int32)MfuncD1;                    // Q15 = Q0*Q15，计算全比较器CMPR1赋值
	 EPwm1Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD2;                    // Q15 = Q0*Q15，计算全比较器CMPR2赋值
	 EPwm2Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2)

	Tmp = (int32)MPeriod * (int32)MfuncD3;                    // Q15 = Q0*Q15，计算全比较器CMPR3赋值
	 EPwm3Regs.CMPA.half.CMPA = (int16)(Tmp>>16) + (int16)(T1Period>>1); // Q0 = (Q15->Q0)/2 + (Q0/2) 

         
	}
}
//	torquestart=0;
}
}
////////////////////////////////////////////////////////////////////////////////////////////////

if(DC_ON_flag==1)
{

        if(U_dc_dis<10)//执行停机命令
        {
        eva_close();
        Run_PMSM=2;
        DC_ON_flag=0;

        }
    
}


EPwm1Regs.ETCLR.bit.INT=1;//清除中断标志位
PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}



interrupt void SCIBRX_ISR(void)     // SCI-B
{
	
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
    
 
	
}

void Init_SiShu(void)
{ 

 GuoliuZhi=15*E_Ding_DianLiu;
 E_Ding_DianLiu=1.414*E_Ding_DianLiu;
 
 

 }

interrupt void INT3_ISR(void)
{ 
   
PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;  //外部中断XINT3--7
}

//===========================================================================
// No more.
//===========================================================================
