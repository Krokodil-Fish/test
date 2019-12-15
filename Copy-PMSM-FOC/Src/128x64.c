#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


Uint16 addr_tab[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//第一行汉字位置
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//第二行汉字位置
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//第三行汉字位置
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//第四行汉字位置
}; 
Uint16 lcd_dis_flag=0;

char Lcd_Dat[6]={0};

void DelayUS2(Uint16 N_US) //1US延时 
{
    Uint16 i=0;  

	for(i=0;i<N_US;i++)
	{
	  asm("	NOP");
	
	}

}

void Init_lcd_Gpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO45=0;//sdl_lcd
     GpioCtrlRegs.GPBDIR.bit.GPIO45=1;
     
     GpioCtrlRegs.GPBMUX1.bit.GPIO44=0;//scl_lcd
     GpioCtrlRegs.GPBDIR.bit.GPIO44=1;
     EDIS;

     SCL_LCD_0;

}





void LCD_SendByte(Uint16 dat)  //
{
    Uint16 i=0;
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO45=1;//shuchu
    EDIS;
    for(i=0;i<8;i++)
    {
        SCL_LCD_0;
        iic_delay();
        if( dat & 0x0080 ) 
		{
		    SDL_LCD_1;
		}
		else 
		{
		    SDL_LCD_0;
		}

        SCL_LCD_1;
        iic_delay();
        dat <<= 1;
        SCL_LCD_0;
		

    }
	
 
}

void Lcd_WriteCmd(Uint16 dat)
{
     
     //CheckBusy();
     DelayUS2(10000);
     LCD_SendByte(0xf8);  //控制指令            //11111,RW(0),RS(0),0
     LCD_SendByte(0xf0&dat);        //高四位
     LCD_SendByte(0xf0&(dat<<4));   //低四位(先执行<<)
     
}

void Lcd_WriteData(Uint16 dat)
{
    
     //CheckBusy();
     DelayUS2(15000);
     LCD_SendByte(0xfa);              //11111,RW(0),RS(1),0
     LCD_SendByte(0xf0&dat);        //高四位
     LCD_SendByte(0xf0&(dat<<4));   //低四位(先执行<<)
     
}

void hanzi_Disp(Uint16 x,Uint16 y,char  *s)
{
 Lcd_WriteCmd(addr_tab[8*x+y]);  //写地址
 while(*s>0)
    {
  Lcd_WriteData(*s);    //写数据
  DelayUS2(1);
  s++;   
    } 
}

void Init_lcd(void)
{
    Init_lcd_Gpio();
     DelayUS2(50000);
    Lcd_WriteCmd(0x30);        //选择基本指令集
    DelayUS2(10000);
    Lcd_WriteCmd(0x30);          //选择8bit数据流
 DelayUS2(10000);
    Lcd_WriteCmd(0x0c);          //开显示(无游标、不反白)
 DelayUS2(10000);
    Lcd_WriteCmd(0x01);          //清除显示，并且设定地址指针为00H
 DelayUS2(10000);

  hanzi_Disp(0,0,"智能康复自行车");
  hanzi_Disp(1,0,"UDC:000 参量");//
  hanzi_Disp(2,0,"IA:00.0 IB:00.0");
//  hanzi_Disp(3,0,"+000  停止  正常");
  hanzi_Disp(3,0,"模式  停止  正常");
}


void Lcd_Dis_Udc(void)//显示电压
{
    Uint16 i=0;
    i= U_dc_dis/100;//百
    Lcd_Dat[0]=0x30+i;
    i= (U_dc_dis/10)%10;//十
    Lcd_Dat[1]=0x30+i;
    i= U_dc_dis%10;//个
    Lcd_Dat[2]=0x30+i;
    Lcd_Dat[3]=' ';
    Lcd_Dat[4]='\0';
    hanzi_Disp(1,2,Lcd_Dat);//显示电压
}

void Lcd_Dis_RPM(void)//显示转速
{
	Uint16 i=0;
    Uint16 j=0;
  if(mode==1)
	{
	hanzi_Disp(1,4,"RPM:0000");
    j=speed_dis;
    i= j/1000;
    Lcd_Dat[0]=0x30+i;//千
    i= (j/100)%10; //百
    Lcd_Dat[1]=0x30+i;
    i= (j/10)%10;//十
    Lcd_Dat[2]=0x30+i;
    i= (j)%10;//个
    Lcd_Dat[3]=0x30+i; 
    Lcd_Dat[4]='\0';
    hanzi_Disp(1,6,Lcd_Dat);//显示转速
	}
 else if (mode==2)
    {
	 hanzi_Disp(1,4,"TOR:0000");
	 j=iq_give;
	 i= j/1000;
	 Lcd_Dat[0]=0x30+i;//千
	 i= (j/100)%10; //百
	 Lcd_Dat[1]=0x30+i;
	 i= (j/10)%10;//十
	 Lcd_Dat[2]=0x30+i;
	 i= (j)%10;//个
     Lcd_Dat[3]=0x30+i;
	 Lcd_Dat[4]='\0';
	 hanzi_Disp(1,6,Lcd_Dat);
    }
}

void Lcd_Dis_DL(void)//显示电流
{
    Uint16 i=0;
    Lcd_Dat[0]=':';
    i= I_A/100;//百
    Lcd_Dat[1]=0x30+i;
    i= (I_A/10)%10;//十
    Lcd_Dat[2]=0x30+i;
    Lcd_Dat[3]='.';
    i= I_A%10;//个
    Lcd_Dat[4]=0x30+i;
    Lcd_Dat[5]='\0';
    hanzi_Disp(2,1,Lcd_Dat);//显示a相


     Lcd_Dat[0]=':';
    i= I_B/100;//百
    Lcd_Dat[1]=0x30+i;
    i= (I_B/10)%10;//十
    Lcd_Dat[2]=0x30+i;
    Lcd_Dat[3]='.';
    i= I_B%10;//个
    Lcd_Dat[4]=0x30+i;
    Lcd_Dat[5]='\0';
    hanzi_Disp(2,5,Lcd_Dat);//显示b相
}


void Lcd_Dis_sheding(void)//显示模式
{
//    Uint16 i=0;
    if(mode==1)
    {
    	hanzi_Disp(3,0,"转速  停止  正常");
    }
    else if(mode==2)
    {
    	hanzi_Disp(3,0,"转矩  停止  正常");
    }
  //   {
 //         Lcd_Dat[0]='+';//正转
 //    }
  //   else
  //   {
  //        Lcd_Dat[0]='-';//反转
   //    }
//    i= speed_give/100;//百
 //    Lcd_Dat[1]=0x30+i;
  //    i= (speed_give/10)%10;//十
 //    Lcd_Dat[2]=0x30+i;
//    i= speed_give%10;//个
   //    Lcd_Dat[3]=0x30+i;
   //    Lcd_Dat[4]='\0';
  //    hanzi_Disp(3,0,Lcd_Dat);//显示设定转速


     if(Run_PMSM==1)
    {
        hanzi_Disp(3,3,"运行");
    }
     else
     {
          hanzi_Disp(3,3,"停止");
     }
       if(IPM_Fault==1)
    {
         hanzi_Disp(3,6,"故障");
    }
    else if(DC_ON_OPEN==1)
    {
         hanzi_Disp(3,6,"主电");
    }
     else if(DC_ON_OPEN==2)
    {
         hanzi_Disp(3,6,"掉电");
    }
     else if(DC_ON_OPEN==3)
    {
         hanzi_Disp(3,6,"过压");
    }
       else if(DC_ON_OPEN==4)
    {
         hanzi_Disp(3,6,"仿真");
    }
     else if(O_Current==1)
     {
        hanzi_Disp(3,6,"过流");
     }
     else if(O_Current==2)
     {
        hanzi_Disp(3,6,"限流");
     }
     else if(Hall_Fault==1)
     {
        hanzi_Disp(3,6,"霍尔");
     }
     else if(ShangDian_Err==1)
     {
        hanzi_Disp(3,6,"保护");//上电时序错误
     }
     else
     {
        hanzi_Disp(3,6,"正常");
     }
}







void LCD_DIS(void)
{
static Uint16 i=0;
if(lcd_dis_flag==1)
{
    if(i==0)
    {
    Lcd_Dis_Udc();
    i++;
    lcd_dis_flag=0;
    }
    else if(i==1)
    {
    Lcd_Dis_RPM();
    i++;
    lcd_dis_flag=0;
    }
    else if(i==2)
    {
    Lcd_Dis_DL();
    i++;
    lcd_dis_flag=0;
    }
    else if(i==3)
    {
    Lcd_Dis_sheding();
    i=0;
    lcd_dis_flag=0;
    }

}


}






	
//===========================================================================
// End of file.
//===========================================================================

