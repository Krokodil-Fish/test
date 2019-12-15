 
#ifndef LCD128x64_H
#define LCD128x64_H

#ifdef __cplusplus
extern "C" {
#endif
 

//---------------------------------------------------------------------------
// GPI/O External References & Function Declarations:
//
extern void Init_lcd_Gpio(void);
extern void iic_delay(void);//250ns
#define SCL_LCD_0 GpioDataRegs.GPBCLEAR.bit.GPIO44=1;
#define SCL_LCD_1 GpioDataRegs.GPBSET.bit.GPIO44=1;

#define SDL_LCD_0 GpioDataRegs.GPBCLEAR.bit.GPIO45=1;
#define SDL_LCD_1 GpioDataRegs.GPBSET.bit.GPIO45=1;
extern void hanzi_Disp(Uint16 x,Uint16 y,char *s);
extern void LCD_DIS(void);
extern Uint16 ch454_key;
extern Uint16 ZhengFan;
extern Uint16 lcd_dis_flag;
extern void Init_lcd(void);
extern Uint16 ShangDian_Err;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_GPIO_H definition

//===========================================================================
// End of file.
//===========================================================================
