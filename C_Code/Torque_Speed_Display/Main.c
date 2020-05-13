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


#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
interrupt void cpu_timer0_isr(void); 
interrupt void EPWM_1_INT(void);    //功率器件的开关频率10K，即中断周期 100us
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
_iq PolePairs=_IQ(4);    ////电机极对数

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
_iq	Ualfa=0;  		//输入：参考电压矢量在alph轴的分量
_iq	Ubeta=0;		//输入：参考电压矢量在beta轴的分量
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
_iq	Ta=0;        //输出：A相脉冲比较器的比较时刻Tcmpa
_iq	Tb=0;		//输出：B相脉冲比较器的比较时刻Tcmpa
_iq	Tc=0;		//输出：C相脉冲比较器的比较时刻Tcmpa
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
float32 E_Ding_DianLiu=4.2;        //设置电机的额定电流的有效值  单位A    含小数点，用单精度浮点数
Uint16 BaseSpeed=3000;              //设置电机额定转速            无符号整型
            
                
 


void main(void)
{


   InitSysCtrl();
 
   InitGpio(); 
   Pwm_EN_1;     //GPIO29 为GPIO功能 输出方式时    会驱动 引脚为 高电平，从而控制另一个芯片作用，将3.3V转换为5V，从而驱动IPM驱动芯片输出到对应管子 135 246

  
   DINT;
 
   InitPieCtrl(); 
   IER = 0x0000;
   IFR = 0x0000;
 
   InitPieVectTable();
 
   EALLOW;  // This is needed to write to EALLOW protected registers 
  // PieVectTable.TINT0 = &cpu_timer0_isr; 
   PieVectTable.EPWM1_INT=&EPWM_1_INT;     //指向中断函数  PWM中断
   PieVectTable.SCIRXINTB= &SCIBRX_ISR;   //设置串口B接受中断的中断向量
   PieVectTable.XINT3=&INT3_ISR;        //外部中断

   EDIS;    // This is needed to disable write to EALLOW protected registers
 
 // InitCpuTimers(); 
   InitSci_C();     //485串口的初始化函数  9600
   InitSci_B();		//232串口的初始化函数  115200
   InitSpi();

   
   
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);   //取消注释是烧写进板子里
   InitFlash();
  
   //ePWM.c
   InitEPwm_1_2_3();//调用epwm初始化函数
   QEP_Init(); //qep初始化

   Init_SiShu();
   ADC_Soc_Init();   //ADC模块初始化
   
 
	eva_close();   //在矢量控制系统开始工作前，强制ePWM123的I/O口不输出pwm波
   Ad_CaiJi();    //电机还没有启动时的零点电流
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   Ad_CaiJi(); 
   

   if(AD_BUF[7]<150)     //150？？ 怎么来的  直流母线电压
   {
	Pwm_EN_0;//低电平 允许PWM使能
    
    }
   else
   {
    Pwm_EN_1;//高电平 禁止PWM使能
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
       if(Show_time2==1000)//1秒
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

    //DAC1_out(_iq data);  注意 data 是标么值哦  dac只能输出0到5v正电压 ,如果data小于0 就会输出为0

     //DAC1_out(Speed_Fdb);//输出速度反馈
   // DAC2_out(IQ_Fdb);//输出iQ反馈电流
//void LuBo(_iq Ch1,_iq Ch2,_iq Ch3,_iq Ch4)//参数都是标么值 Q格式,知道怎么用了吧
   //----ch1--ch2-ch3-ch4
LuBo(ia, ib, ic,Speed);

   

 // DAC1_out(ID_Fdb);//输出iD反馈电流

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
                    LocationFlag=LocationEnd;//定位结束
				EQep1Regs.QPOSCNT =BuChang*0+BuChang/2;     // 不同霍尔对应不同角度  从    QPOSCNT 208到 2288  间隔步进角 416
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
		DirectionQep = EQep1Regs.QEPSTS.bit.QDF;//  检测转动方向 0顺1逆
		
        RawTheta = _IQ(EQep1Regs.QPOSCNT);   //检测位置计数器的值
        
		if(DirectionQep ==1) //递增计数，代表顺时针；
		{
	
		 
              if((OldRawThetaPos>324403200) && (RawTheta<_IQ(900)))   //与  两个条件同时都要满足识别过圈  324403200是9900的Q15格式
			{
				PosCount += TotalCnt;  //正累加  输出角度为正 0~360
			}

             
			Place_now= _IQtoF(RawTheta)+PosCount;  //一圈360°分成了10000个脉冲，每个脉冲0.036度
			OldRawThetaPos = RawTheta;
			
            
            
            
		}
		else if(DirectionQep ==0)//递减计数，代表逆时针
		{		 
             
			
              if((RawTheta>294912000) && (OldRawThetaPos<_IQ(1000)))    //识别过圈  294912000是9000的Q15格式，if条件可以随意 9500 500也行
			{
				PosCount -= TotalCnt; //TotalCnt 4*2500  PosCount是个负值。负累加    减计数输出角度为负 -360~0
			}
			Place_now = _IQtoF(RawTheta)+PosCount;
			OldRawThetaPos = RawTheta;
               
               
               
            
		} 
		MechTheta = _IQmpy(1179,RawTheta);   //1179是0.036的Q15格式  含义就是100us计算一次机械角度，两次转过了多少脉冲，然后算出角度
         if(MechTheta>_IQ(360))    //2pi    角度限制 0~360
        {MechTheta=MechTheta-_IQ(360);}
         if(MechTheta<_IQ(-360))
        {MechTheta=MechTheta+_IQ(360);}
		ElecTheta = _IQmpy(PolePairs,MechTheta);   
	
		AnglePU=_IQdiv(ElecTheta,_IQ(360))+14876;   //标幺值=实际值/额定值   +校正0.45°
	   	Sine = _IQsinPU(AnglePU); ///IQ15格式的正弦余弦标幺值
	   	Cosine = _IQcosPU(AnglePU);    

      


//======================================================================================================
//QEP速度计算
//====================================================================================================== 

	    if (SpeedLoopCount>=SpeedLoopPrescaler)       //进入速度环，周期10*100us=1ms
	    {   
// 旋转方向判定 
			DirectionQep = EQep1Regs.QEPSTS.bit.QDF;		//正交脉冲方向	0顺1逆
 			NewRawTheta =_IQ(EQep1Regs.QPOSCNT);            //检测32位位置计数器的值
// 计算机械角度
			if(DirectionQep ==1) //递增计数，
			{
               
                
				RawThetaTmp =  OldRawTheta-NewRawTheta ; //假设OLD 9500 new500
				if(RawThetaTmp > _IQ(0))     //过圈
				{
				 RawThetaTmp = RawThetaTmp - TotalPulse;    //9500-500-10000 =-1000
				}
                
                
                
			}
			else if(DirectionQep ==0) //递减计数
			{

              
                
                RawThetaTmp =OldRawTheta-NewRawTheta;   //假设old500 ，new 9500
				if(RawThetaTmp < _IQ(0))
				{
				 RawThetaTmp = RawThetaTmp + TotalPulse;   //500-9500+10000=1000
				}
                
                
                
                
			}
			//速度计算，DirectionQep ==0 时正转 速度为正，=1时反转 速度为负
			Speed = _IQmpy(RawThetaTmp,65);  				//M法测速，T=1ms，RawThetaTmp*60/（2500*4）*0.001 r/min =6*RawThetaTmp PU=6*RawThetaTmp*2^15/基速3000  得到  65？？？？
			OldRawTheta = NewRawTheta; 
		    SpeedLoopCount=1; 
			RawThetaTmp=0; 

//=================位置环控制===================================
//  if(PlaceEnable ==1)    //转子定位过程
//    {
//        PlaceError = PlaceSet + Place_now;   //位置设定值与编码器反馈的脉冲信号值     Place_now可正可负，支持正反转  PlaceSet的值就是编码器2s发出的脉冲数
  
//		OutPreSat_Place = PlaceError;   //位置偏差
//		if((PlaceError<=10000)&&(PlaceError>=-10000))    //10000 单位是脉冲数
//        {
//           OutPreSat_Place = PlaceError/3;  //位置环的比例增益调节，无积分调节
//		}

		
//        if (OutPreSat_Place> 2000)      //把速度给定限定在额定转速内，，支持正反转
//        {
//          SpeedRef =  0.5;
//        }
//        else if (OutPreSat_Place< -2000)
//        {
//          SpeedRef =  -0.5;
//        }
 //       else
//        {
//          SpeedRef = OutPreSat_Place/(float32)BaseSpeed;  //速度给定标幺化
//        }

	   
//   	}

//=================速度环PI===================================
//			Speed_Ref=_IQ(SpeedRef);   //速度参考值怎么给？ 通过按键给  ，它是位置环PI调节后的输出OutPreSat_Place及位置设定的前馈值PlaceSet
//			Speed_Fdb=Speed;      //来自正交编码器QEP速度计算
//
//			Speed_Error=Speed_Ref - Speed_Fdb;
//
//			Speed_Up=_IQmpy(Speed_Kp,Speed_Error);
//			Speed_Ui=Speed_Ui + _IQmpy(Speed_Ki,Speed_Up) + _IQmpy(Speed_Ki,Speed_SatError);

//			Speed_OutPreSat=Speed_Up+Speed_Ui;

//			if(Speed_OutPreSat>Speed_OutMax)      //速度环的输出限幅值0.99999怎么来的
//				Speed_Out=Speed_OutMax;
//			else if(Speed_OutPreSat<Speed_OutMin)
//	 			Speed_Out=Speed_OutMin;
//			else
//				Speed_Out=Speed_OutPreSat;
	
//			Speed_SatError=Speed_Out-Speed_OutPreSat;

//			IQ_Given=Speed_Out;
//            Speed_run=1;  // 执行速度环时，此标志置1
//		}
//	    else       // if (SpeedLoopCount>=SpeedLoopPrescaler)
//            {
//                SpeedLoopCount++;
//	    }
//        if(Speed_run==1)    // ia ib ic theta怎么读取的
//       {

            //由安装在驱动器内部每相上的霍尔电流传感器反馈的，将磁感应变为电流电压信号
	    ialfa=ia;   //abc_to_alphbeta  幅值不变的CLARKE变换变换矩阵系数为2/3
		ibeta=_IQmpy(ia,_IQ(0.57735026918963))+_IQmpy(ib,_IQ(1.15470053837926));  

		id = _IQmpy(ialfa,Cosine) +_IQmpy(ibeta,Sine);  //将alphbeta坐标系下的电流转换到dq坐标系 PARK变换
		iq = _IQmpy(ibeta,Cosine)- _IQmpy(ialfa,Sine) ; 

//======================================================================================================
//IQ电流PID调节控制
//======================================================================================================  
		IQ_Ref=_IQ(0.5); 			//从0到1  修改   对应0到2.4Nm   恒转矩控制时，屏蔽速度环 位置环
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
//ID电流PID调节控制
//======================================================================================================  
		ID_Ref=ID_Given;
		ID_Fdb=id;      //alphbeta_to_dq变换的id

		ID_Error=ID_Ref-ID_Fdb;

		ID_Up=_IQmpy(ID_Kp,ID_Error);    //比例
		ID_Ui=ID_Ui+_IQmpy(ID_Ki,ID_Up)+_IQmpy(ID_Ki,ID_SatError);   //积分饱和误差ID_SatError，抗积分饱和处理anti-windup

		ID_OutPreSat=ID_Up+ID_Ui;    

		if(ID_OutPreSat>ID_OutMax)   //达到上限制  限幅处理
			ID_Out=ID_OutMax;
		else if(ID_OutPreSat<ID_OutMin)  //达到下限制  限幅处理
		 	ID_Out=ID_OutMin;
		else 
			ID_Out=ID_OutPreSat;  

		ID_SatError=ID_Out-ID_OutPreSat;     //积分饱和误差即实际值与anti-windup的计算值

		Ud=ID_Out;

//======================================================================================================
//IPark变换  要用到eQEP角度测量函数里的d轴与alph轴之间的夹角
//====================================================================================================== 
		Ualfa = _IQmpy(Ud,Cosine) - _IQmpy(Uq,Sine);
		Ubeta = _IQmpy(Uq,Cosine) + _IQmpy(Ud,Sine); 
    
//======================================================================================================
//SVPWM实现
//====================================================================================================== 
        B0=Ubeta;       // 相当于二相静止坐标--到三相静止变换出Uabc
		B1=_IQmpy(_IQ(0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 
		B2=_IQmpy(_IQ(-0.8660254),Ualfa)- _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2

		Sector=0;          //扇区判别，扇区N=a+2b+4c
		if(B0>_IQ(0)) Sector =1;  //A=1,else A=0
		if(B1>_IQ(0)) Sector =Sector +2;  //B=1,else B=0
		if(B2>_IQ(0)) Sector =Sector +4;   //C=1,else C=0

		X=Ubeta;//va        基本矢量作用时间是 X Y Z的线性组合
		Y=_IQmpy(_IQ(0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta);// 0.8660254 = sqrt(3)/2 vb
		Z=_IQmpy(_IQ(-0.8660254),Ualfa)+ _IQmpy(_IQ(0.5),Ubeta); // 0.8660254 = sqrt(3)/2 vc

		
	 if(Sector==1)
		{
			t_01=Z;
			t_02=Y;

       if((t_01+t_02)>_IQ(1))         //对过调制情况进行调整
       {
        t1=_IQmpy(_IQdiv(t_01, (t_01+t_02)),_IQ(1));
       t2=_IQmpy(_IQdiv(t_02, (t_01+t_02)),_IQ(1));

       }
       else
       { t1=t_01;
       t2=t_02;
       }

			Tb=_IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));  //作用时间分配
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

		MfuncD1=_IQmpy(_IQ(2),(_IQ(0.5)-Ta));     //SVPWM。Ta/Tb/Tc的取值范围变换到正负一
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

         
	}     // if (SpeedLoopCount>=SpeedLoopPrescaler)       //进入速度环，周期10*100us=1ms
	}   //	else if(LocationFlag==LocationEnd)



}   //if run_pmsm==1&fault==0

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

void Init_SiShu(void)      //电流保护
{ 

 GuoliuZhi=15*E_Ding_DianLiu;     //规定
 E_Ding_DianLiu=1.414*E_Ding_DianLiu;
 
 

 }

interrupt void INT3_ISR(void)
{ 
   
PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

//===========================================================================
// No more.
//===========================================================================
