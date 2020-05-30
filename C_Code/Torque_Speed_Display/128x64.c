#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


Uint16 addr_tab[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//��һ�к���λ��
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//�ڶ��к���λ��
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//�����к���λ��
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//�����к���λ��
}; 
Uint16 lcd_dis_flag=0;

char Lcd_Dat[6]={0};

void DelayUS2(Uint16 N_US) //1US��ʱ 
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
     LCD_SendByte(0xf8);  //����ָ��            //11111,RW(0),RS(0),0
     LCD_SendByte(0xf0&dat);        //����λ
     LCD_SendByte(0xf0&(dat<<4));   //����λ(��ִ��<<)
     
}

void Lcd_WriteData(Uint16 dat)
{
    
     //CheckBusy();
     DelayUS2(15000);
     LCD_SendByte(0xfa);              //11111,RW(0),RS(1),0
     LCD_SendByte(0xf0&dat);        //����λ
     LCD_SendByte(0xf0&(dat<<4));   //����λ(��ִ��<<)
     
}

void hanzi_Disp(Uint16 x,Uint16 y,char  *s)
{
 Lcd_WriteCmd(addr_tab[8*x+y]);  //д��ַ
 while(*s>0)
    {
  Lcd_WriteData(*s);    //д����
  DelayUS2(1);
  s++;   
    } 
}

void Init_lcd(void)
{
    Init_lcd_Gpio();
     DelayUS2(50000);
    Lcd_WriteCmd(0x30);        //ѡ�����ָ�
    DelayUS2(10000);
    Lcd_WriteCmd(0x30);          //ѡ��8bit������
 DelayUS2(10000);
    Lcd_WriteCmd(0x0c);          //����ʾ(���αꡢ������)
 DelayUS2(10000);
    Lcd_WriteCmd(0x01);          //�����ʾ�������趨��ַָ��Ϊ00H
 DelayUS2(10000);

  hanzi_Disp(0,0,"���ܿ������г�");
  hanzi_Disp(1,0,"UDC:000 ����");//
  hanzi_Disp(2,0,"IA:00.0 IB:00.0");
//  hanzi_Disp(3,0,"+000  ֹͣ  ����");
  hanzi_Disp(3,0,"ģʽ  ֹͣ  ����");
}


void Lcd_Dis_Udc(void)//��ʾ��ѹ
{
    Uint16 i=0;
    i= U_dc_dis/100;//��
    Lcd_Dat[0]=0x30+i;
    i= (U_dc_dis/10)%10;//ʮ
    Lcd_Dat[1]=0x30+i;
    i= U_dc_dis%10;//��
    Lcd_Dat[2]=0x30+i;
    Lcd_Dat[3]=' ';
    Lcd_Dat[4]='\0';
    hanzi_Disp(1,2,Lcd_Dat);//��ʾ��ѹ
}

void Lcd_Dis_RPM(void)//��ʾת��
{
	Uint16 i=0;
    Uint16 j=0;
  if(mode==1)
	{
	hanzi_Disp(1,4,"RPM:0000");
    j=speed_dis;
    i= j/1000;
    Lcd_Dat[0]=0x30+i;//ǧ
    i= (j/100)%10; //��
    Lcd_Dat[1]=0x30+i;
    i= (j/10)%10;//ʮ
    Lcd_Dat[2]=0x30+i;
    i= (j)%10;//��
    Lcd_Dat[3]=0x30+i; 
    Lcd_Dat[4]='\0';
    hanzi_Disp(1,6,Lcd_Dat);//��ʾת��
	}
 else if (mode==2)
    {
	 hanzi_Disp(1,4,"TOR:0000");
	 j=iq_give;
	 i= j/1000;
	 Lcd_Dat[0]=0x30+i;//ǧ
	 i= (j/100)%10; //��
	 Lcd_Dat[1]=0x30+i;
	 i= (j/10)%10;//ʮ
	 Lcd_Dat[2]=0x30+i;
	 i= (j)%10;//��
     Lcd_Dat[3]=0x30+i;
	 Lcd_Dat[4]='\0';
	 hanzi_Disp(1,6,Lcd_Dat);
    }
}

void Lcd_Dis_DL(void)//��ʾ����
{
    Uint16 i=0;
    Lcd_Dat[0]=':';
    i= I_A/100;//��
    Lcd_Dat[1]=0x30+i;
    i= (I_A/10)%10;//ʮ
    Lcd_Dat[2]=0x30+i;
    Lcd_Dat[3]='.';
    i= I_A%10;//��
    Lcd_Dat[4]=0x30+i;
    Lcd_Dat[5]='\0';
    hanzi_Disp(2,1,Lcd_Dat);//��ʾa��


     Lcd_Dat[0]=':';
    i= I_B/100;//��
    Lcd_Dat[1]=0x30+i;
    i= (I_B/10)%10;//ʮ
    Lcd_Dat[2]=0x30+i;
    Lcd_Dat[3]='.';
    i= I_B%10;//��
    Lcd_Dat[4]=0x30+i;
    Lcd_Dat[5]='\0';
    hanzi_Disp(2,5,Lcd_Dat);//��ʾb��
}


void Lcd_Dis_sheding(void)//��ʾģʽ
{
//    Uint16 i=0;
    if(mode==1)
    {
    	hanzi_Disp(3,0,"ת��  ֹͣ  ����");
    }
    else if(mode==2)
    {
    	hanzi_Disp(3,0,"ת��  ֹͣ  ����");
    }
  //   {
 //         Lcd_Dat[0]='+';//��ת
 //    }
  //   else
  //   {
  //        Lcd_Dat[0]='-';//��ת
   //    }
//    i= speed_give/100;//��
 //    Lcd_Dat[1]=0x30+i;
  //    i= (speed_give/10)%10;//ʮ
 //    Lcd_Dat[2]=0x30+i;
//    i= speed_give%10;//��
   //    Lcd_Dat[3]=0x30+i;
   //    Lcd_Dat[4]='\0';
  //    hanzi_Disp(3,0,Lcd_Dat);//��ʾ�趨ת��


     if(Run_PMSM==1)
    {
        hanzi_Disp(3,3,"����");
    }
     else
     {
          hanzi_Disp(3,3,"ֹͣ");
     }
       if(IPM_Fault==1)
    {
         hanzi_Disp(3,6,"����");
    }
    else if(DC_ON_OPEN==1)
    {
         hanzi_Disp(3,6,"����");
    }
     else if(DC_ON_OPEN==2)
    {
         hanzi_Disp(3,6,"����");
    }
     else if(DC_ON_OPEN==3)
    {
         hanzi_Disp(3,6,"��ѹ");
    }
       else if(DC_ON_OPEN==4)
    {
         hanzi_Disp(3,6,"����");
    }
     else if(O_Current==1)
     {
        hanzi_Disp(3,6,"����");
     }
     else if(O_Current==2)
     {
        hanzi_Disp(3,6,"����");
     }
     else if(Hall_Fault==1)
     {
        hanzi_Disp(3,6,"����");
     }
     else if(ShangDian_Err==1)
     {
        hanzi_Disp(3,6,"����");//�ϵ�ʱ�����
     }
     else
     {
        hanzi_Disp(3,6,"����");
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

