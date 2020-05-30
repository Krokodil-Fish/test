#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


extern void DelayUS(Uint16 N_US);

Uint16 ch454_key=0;
Uint16 Key_Flag=0;
Uint16 Show_time=0;
Uint16 Show_time2=0;
Uint16 MinSpeedRPM=700;  

void iic_delay(void)//250ns
{


     asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
        asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
        asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	  asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
       asm(" NOP");
       asm(" NOP");
      asm(" NOP");
	 
      
    

}
 
 
   
 


void Read_key(void)
    {
    static Uint16 i=0,j=0,k=0,n=0,anjiancnt1=0,anjiancnt2=0,anjiancnt3=0,anjiancnt4=0,anjiancnt5=0;

i++;
if(i==800)
{
i=0;
    if(Key_Flag==0)
    {
    
    if((k>>6)==1)//有键按下
    {
       j=1;

    }
    else
    {
        if(j==1)
        {
             Key_Flag=1;//
             ch454_key=k;
             j=0;


        }
    }
    }

    if(Key_Flag==0)
    {

        if(GpioDataRegs.GPCDAT.bit.GPIO79==0)//anjian1
        {
            anjiancnt1++;
            if(anjiancnt1>1)
            {
            Key_Flag=1;
             ch454_key=0;
             
            }

        }
        else
        {anjiancnt1=0;}
        if(GpioDataRegs.GPBDAT.bit.GPIO38==0)//anjian2
        {
            anjiancnt2++;
            if(anjiancnt2>1)
            {
            Key_Flag=1;
             ch454_key=1;
            }

        }
        else
        {anjiancnt2=0;}
          if(GpioDataRegs.GPBDAT.bit.GPIO35==0)//anjian3
        {
            anjiancnt3++;
            if(anjiancnt3>1)
            {
            Key_Flag=1;
            n++;
            if(n%2==0)//zheng
            {ch454_key=2;}
            else//fan
            {
             ch454_key=3;
         }
          }

        }
          else
        {anjiancnt3=0;}
           if(GpioDataRegs.GPBDAT.bit.GPIO37==0)//anjian4
        {anjiancnt4++;
        if(anjiancnt4>1)
        {
            Key_Flag=1;
             ch454_key=4;
        }

        }
           else
           {
            anjiancnt4=0;

           }
          if(GpioDataRegs.GPBDAT.bit.GPIO40==0)//anjian5
        {anjiancnt5++;
        if(anjiancnt5>1)
        {
            Key_Flag=1;
             ch454_key=5;
        }

        }
         else
         {anjiancnt5=0;}
        




    }
}


}
 


 

void deal_key_lcd(void)
{
    float32 temp;
    if(Key_Flag==1)
    {
        Key_Flag=0;
      
	switch(ch454_key)
	{ 
	case 0:	 //++
	     
        if(speed_give<100)
        {
            speed_give=1+speed_give;
        SpeedRef=speed_give*1.0/100;
        
        } 

       
        
       Key_Flag=0;
         ch454_key=0xef;
		
		break;
        case 1://-- 

	   
        if(speed_give>0)
        {
            speed_give=speed_give-1;
        SpeedRef=speed_give*1.0/100;
        
        }
        


       
       
       Key_Flag=0;
         ch454_key=0xef;
		
		break;
	case 4:	 //运行
	if(Run_PMSM==2)
	{
	     DC_ON_0;
         DC_ON_flag=1;
         Run_PMSM=0;
         temp=MinSpeedRPM*100.0;
         temp=temp/BaseSpeed;
         speed_give=temp;
         SpeedRef=speed_give*1.0/100;
         
         
	}
	     
         Key_Flag=0;
         ch454_key=0xef;
		break;
	
	case 3:
		
	
        
        Key_Flag=0;
         ch454_key=0xef;
		
		break;
 
	case 5: //停止
	    DC_ON_1;
        if(Run_PMSM==0)//在等待继电器闭合过程中停机
        {
            Run_PMSM=2;
        }
        DC_ON_flag=1;	
        DC_ON_OPEN=0;
        Key_Flag=0;
         ch454_key=0xef;
		break;

	case 2://正转
	
	
       
        Key_Flag=0;
         ch454_key=0xef; 
		
		break;
        default:
            Key_Flag=0;
         ch454_key=0xef;
         break;
	}


    }



}


void deal_key(void)
{
    deal_key_lcd();

}





	
//===========================================================================
// End of file.
//===========================================================================

 
