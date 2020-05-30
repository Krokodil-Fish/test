 
#ifndef SMO_H
#define SMO_H

#ifdef __cplusplus
extern "C" {
#endif


#define MINSPEEDINRPM	MinSpeedRPM  //最小闭环速度  转每分
#define ENDSPEEDOPENLOOP MINSPEEDINRPM    
#define LOOPTIMEINSEC    0.0001 //采样周期  单位 秒
#define DELTA_STARTUP_RAMP	(Uint16)(MINSPEEDINRPM*PolePairs*LOOPTIMEINSEC* \
							LOOPTIMEINSEC*65536*65536/(60*OPENLOOPTIMEINSEC))



extern float32 LockTimeInSec;
extern float32 OPENLOOPTIMEINSEC;

extern _iq qAngle;
extern _iq id;
extern _iq iq; 
extern _iq Speed_Kp;
extern _iq Speed_Ki;
extern _iq IQ_Kp;
extern _iq IQ_Ki; 
extern _iq ID_Kp;
extern _iq ID_Ki;
extern _iq Ud;
extern _iq Uq;
extern _iq OpenLoop_IqGiven;
extern Uint16 PolePairs;
extern void ParaInt(void);
extern void StopParaInt(void);
extern Uint16 MinSpeedRPM; 

extern Uint16 OpenLoop;
extern Uint16 Startup_Lock;
extern Uint32 Startup_Ramp;
extern Uint32 EndSpeed;
extern Uint16 LockTime;

extern void speed_est_calc(void);
extern void smopos_calc(void);
extern _iq SMOError;
extern float32 Rs;
extern float32  Ls;  			      
extern float32  Phase_Ib;
extern _iq ialfa;
extern _iq ibeta;
extern _iq Ualfa;
extern _iq Ubeta;
extern _iq EstimatedSpeed;
extern _iq EstimatedTheta;
extern _iq ElecTheta;
extern _iq MinSpeedRef;
extern Uint16 suduchangge;
extern Uint32  OpenLoopcnt;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   

//===========================================================================
// End of file.
//===========================================================================





















