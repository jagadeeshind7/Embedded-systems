/*Program req:
  Toggle an ALL LED at every 1sec
	interval using timer1 intr.
*/
#include <LPC21xx.H>
#define LED (1<<17)	//P0.17
#define SW ((IOPIN0>>5)&1)	//P0.5
unsigned int count2,count1;

void Timer1_Handler(void) __irq{
 static int flag;	
 T1IR=1;//*
 T1TCR=0;//stop timer1	
	
 /*led toggle logic*/	
 if(count1==2){
	 
	flag^=1;
	if(flag)
	 IOSET0=LED;
	else
	 IOCLR0=LED;
 }
 count1=0;//for next toggle
 //T1TC=0;
 VICVectAddr=0;
}

main(){
	IODIR0|=LED;
	/*vic init*/
	VICIntSelect=0;//all intrs are IRQs
	VICVectCntl0=5|(1<<5);//Init slot0
	VICVectAddr0=(unsigned int)Timer1_Handler;//Init Slot0
	VICIntEnable=(1<<5);//En Timer1 Intr in VIC
	
	/*timer1 init*/
	T1PC=0;
	T1PR=14999;
	T1TC=0;
	T1MCR=3;//Intr,reset,stop TC on MR0
	T1MR0=1000;//intr for every 1sec
	
	//def task of main
	while(1){
		if(SW==0){
			T1TCR=1;
			while(SW==0);//waiting for sw release
			count1++;
	}
 }
}

