 
#ifndef TongXin_H
#define TongXin_H

#ifdef __cplusplus
extern "C" {
#endif

extern Uint16 RS232_RX[6];
extern Uint16 RS232_OK;
extern Uint16 CRC_16(Uint16 *DATA,Uint16 N);
extern void RS232_ChuLi(void);
extern void TX232_ChuLi(void);
extern float32 BinToDec(Uint16 N);
extern Uint16 BaseSpeed;
extern float32 Offset_Angle;
extern _iq BaseRpm;
extern _iq PolePairs_XuanBian;
extern Uint16 Drive_Type;
extern Uint16 jiduishu;
extern Uint16 f_given;
extern float32  Rr;
extern float32  Lr;
extern _iq PolePairs;
extern long PlaceSet;
extern void Init_SiShu(void);
extern Uint16 ZhuanSu_Mode;
extern Uint16 ZhuanJu_Mode;
extern Uint16 PlaceEnable;
extern long Place_now;
extern float32 WeiZhi_Kp;
extern _iq Speed_Ki;
extern _iq Speed_Kp;
extern _iq IQ_Ki;
extern _iq IQ_Kp;
extern _iq ID_Ki;
extern _iq ID_Kp;
extern _iq SpeedRpm;
extern Uint16 I_C;
extern void LuBo(_iq Ch1,_iq Ch2,_iq Ch3,_iq Ch4);
extern Uint32 RS232_CNT;
extern Uint16 Encoder_Type;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_GPIO_H definition

//===========================================================================
// End of file.
//===========================================================================
