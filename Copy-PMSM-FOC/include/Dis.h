 
#ifndef Dis_H
#define Dis_H

#ifdef __cplusplus
extern "C" {
#endif
 

//---------------------------------------------------------------------------
// GPI/O External References & Function Declarations:
//


/* CH454�ĳ��������� */
#define CH454_RESET     0x0201					// ��λ
#define CH454_CLR_BIT	0x0180					// ��λ��0,�����6λ����
#define CH454_SET_BIT	0x01C0					// ��λ��1,�����6λ����
#define CH454_NO_BCD    0x0540					// ����ֱ�����뷽ʽ   
#define CH454_TWINKLE   0x0600		            // ������˸����,�����8λ����
#define CH454_GET_KEY	0x0700					// ��ȡ����,���ذ�������
#define CH454_DIG0_LO   0x0800					// �����λ0LO��ʾ,�����8λ����
#define CH454_DIG1_LO   0x0900		            // �����λ1LO��ʾ,�����8λ����
#define CH454_DIG2_LO   0x0a00		            // �����λ2LO��ʾ,�����8λ����
#define CH454_DIG3_LO   0x0b00		            // �����λ3LO��ʾ,�����8λ����
#define CH454_DIG4_LO   0x0c00		            // �����λ4LO��ʾ,�����8λ����
#define CH454_DIG5_LO   0x0d00					// �����λ5LO��ʾ,�����8λ����
#define CH454_DIG6_LO   0x0e00					// �����λ6LO��ʾ,�����8λ����
#define CH454_DIG7_LO   0x0f00		            // �����λ7LO��ʾ,�����8λ����
#define CH454_DIG0_HI   0x1000					// �����λ0HI��ʾ,�����8λ����
#define CH454_DIG1_HI   0x1100		            // �����λ1HI��ʾ,�����8λ����
#define CH454_DIG2_HI   0x1200		            // �����λ2HI��ʾ,�����8λ����
#define CH454_DIG3_HI   0x1300		            // �����λ3HI��ʾ,�����8λ����
#define CH454_DIG4_HI   0x1400		            // �����λ4HI��ʾ,�����8λ����
#define CH454_DIG5_HI   0x1500					// �����λ5HI��ʾ,�����8λ����
#define CH454_DIG6_HI   0x1600					// �����λ6HI��ʾ,�����8λ����
#define CH454_DIG7_HI   0x1700		            // �����λ7HI��ʾ,�����8λ����
 

#define SCL_0 GpioDataRegs.GPBCLEAR.bit.GPIO37=1;
#define SCL_1 GpioDataRegs.GPBSET.bit.GPIO37=1;

#define SDL_0 GpioDataRegs.GPACLEAR.bit.GPIO28=1;
#define SDL_1 GpioDataRegs.GPASET.bit.GPIO28=1;
 

extern void Read_key(void);
extern void deal_key(void);
extern Uint16 Show_time;
extern Uint16 Show_time2;
extern long  Place_now;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_GPIO_H definition

//===========================================================================
// End of file.
//===========================================================================
