#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include <math.h>


#define DIFF_MAX_LIMIT  	_IQ(0.80)
#define DIFF_MIN_LIMIT  	_IQ(0.20)
Uint16 LockTime=0;
Uint16 AccumThetaCnt=0;
float32 LockTimeInSec=3;            
     
Uint16 OpenLoop=1;
float32  Rs=0.901; 				  
 
Uint16 Startup_Lock=0; //锁定时间计数器
Uint32 Startup_Ramp=0;//斜坡计数器
Uint32 EndSpeed=0;
Uint16 PolePairs=4;                

_iq    qAngle=0;
float32 OPENLOOPTIMEINSEC=4.0;  
_iq    qVdSquared=0;
float32  Ts=0.0001;	//采样时间 单位 S 
_iq SMOError=_IQ(0.6);  
float32  Ls=0.006552;				  

//========================SMO相关变量==============================
_iq Fsmopos=30076;			//  constant using in observed current calculation 
_iq Gsmopos=14090;			// constant using in observed current calculation
_iq Kslide=_IQ(0.53);//滑动控制增益
_iq Kslf=_IQ(0.157);//滑动控制滤波增益 
_iq ElecTheta = 0;  //SMO估算出来的电气角度，标么值
_iq EstimatedSpeed=0;//SMO估算出来的速度，标么值
_iq  EstIalpha=0;   // Variable: Estimated stationary alfa-axis stator current
_iq  EstIbeta=0;    // Variable: Estimated stationary beta-axis stator current 
_iq  Ealpha=0;   	// Variable: Stationary alfa-axis back EMF 
_iq  Zalpha=0;      // Output: Stationary alfa-axis sliding control 
_iq  IalphaError=0; // Variable: Stationary alfa-axis current error  
_iq  IbetaError=0;  // Variable: Stationary beta-axis current error 
_iq  Ebeta=0;  		// Variable: Stationary beta-axis back EMF 
_iq  Zbeta=0;      	// Output: Stationary beta-axis sliding control 
_iq EstimatedTheta=0;  	// Input: Electrical angle (pu)
_iq21 K1=_IQ21(50);// Parameter: Constant for differentiator (Q21) - independently with global Q
_iq K2=_IQ(0.99);     				// Parameter: Constant for low-pass filter (pu)
_iq K3=_IQ(0.01);     				// Parameter: Constant for low-pass filter (pu)
_iq OldEstimatedTheta=0;   // History: Electrical angle at previous step (pu)




void StopParaInt(void)
{
    OpenLoop=1;
    Startup_Lock=0;
    Startup_Ramp=0;
    qAngle=0;
    EstimatedSpeed=0;
    EstimatedTheta=0;
    OldEstimatedTheta=0;
    EstIalpha=0;
    EstIbeta=0;
    Ualfa=0;
    Ubeta=0;
    Ealpha=0;
    Ebeta=0;
    Zalpha=0;
    Zbeta=0;
    ElecTheta=0;
    suduchangge=0;
    OpenLoopcnt=0;



}

void ParaInt(void)
{
    float32 temp=0;
    temp=LockTimeInSec*10000.0;//10000.0表示10K的采样频率
    LockTime=temp;
    EndSpeed = ENDSPEEDOPENLOOP * PolePairs * LOOPTIMEINSEC * 65536 * 65536 / 60.0;

}


void speed_est_calc(void)//滑模速度估算
{

   _iq Temp1; 
   if ((EstimatedTheta < DIFF_MAX_LIMIT)&(EstimatedTheta > DIFF_MIN_LIMIT))
	 
   		Temp1 = _IQmpy(K1,(OldEstimatedTheta-EstimatedTheta  ));
   else Temp1 = _IQtoIQ21(EstimatedSpeed);
 
   Temp1 = _IQmpy(K2,_IQtoIQ21(EstimatedSpeed))+_IQmpy(K3,Temp1);  

   if (Temp1>_IQ21(1))
     EstimatedSpeed = _IQ(1);
   else if (Temp1<_IQ21(-1))
     EstimatedSpeed = _IQ(-1);      
   else
     EstimatedSpeed = _IQ21toIQ(Temp1);
 
    OldEstimatedTheta = EstimatedTheta;
 

}


void smopos_calc(void)//滑模角度估算
{	
    
    
   
    
// 估算电流
    EstIalpha = _IQmpy(Fsmopos,EstIalpha) + _IQmpy(Gsmopos,(Ualfa-Ealpha-Zalpha));//估算的IALPHA
    EstIbeta = _IQmpy(Fsmopos,EstIbeta) + _IQmpy(Gsmopos,(Ubeta-Ebeta-Zbeta));//估算的IBETA

// 计算电流误差
    IalphaError = EstIalpha - ialfa;
    IbetaError = EstIbeta - ibeta ;

// 滑模控制
    if (_IQabs(IalphaError) < SMOError)
       Zalpha = _IQmpy(Kslide,_IQdiv(IalphaError,SMOError));  
    else if (IalphaError >= SMOError) 
       Zalpha = Kslide;
    else if (IalphaError <= -SMOError) 
       Zalpha = -Kslide;

    if (_IQabs(IbetaError) < SMOError)
       Zbeta = _IQmpy(Kslide,_IQdiv(IbetaError,SMOError));  
    else if (IbetaError >= SMOError) 
       Zbeta = Kslide;
    else if (IbetaError <= -SMOError) 
       Zbeta = -Kslide;

// 计算反电动势
    Ealpha = Ealpha + _IQmpy(Kslf,(Zalpha-Ealpha));
    Ebeta = Ebeta + _IQmpy(Kslf,(Zbeta-Ebeta));

// 计算角度
    
    ElecTheta = _IQatan2PU(-Ealpha,Ebeta); 
    
     
    
        
      if(ElecTheta>_IQ(1))
        {
            ElecTheta=ElecTheta-_IQ(1);

        }

         if(ElecTheta<_IQ(-1))
        {
            ElecTheta=ElecTheta+_IQ(1);

        } 



   
}




 

 












