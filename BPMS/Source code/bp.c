#include<reg51.h>
#define dataport P2
#define adc_input P1
sbit rs = P3^0;
sbit rw = P3^1;
sbit e = P3^2;
sbit rd= P3^4;
sbit wr= P3^5;
sbit intr= P3^6;
void delay(unsigned int);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_data_string(unsigned char*);
void lcd_init();
void adc_conv();
void adc_read();
void lcd_data_adc(unsigned int);
int num[10];
float v_out,pressure;
void main()
{
dataport=0x00;
adc_input=0xff;
P3=0x00;
lcd_init();
lcd_cmd(0x84);
lcd_data_string("BP:");
while(1)
{
lcd_cmd(0xc5);
adc_conv();
adc_read();
lcd_data_string("mmHg");
delay(50);
 }
}
void lcd_init()
{
lcd_cmd(0x01);
delay(1);
lcd_cmd(0x06);
delay(1);
lcd_cmd(0x0e);
delay(1);
lcd_cmd(0x38);
delay(1);
lcd_cmd(0x80);
delay(1);
}
void delay(unsigned int sec )
{
int i ,j ;
for(i=0;i<sec;i++)
  for(j=0; j<1275; j++);
}
void lcd_cmd(unsigned char item)
{
dataport=item;
rs= 0;
rw=0;
e=1;
delay(1);
e=0;
return;
}
void lcd_data(unsigned char item) 
{
dataport = item;
rs= 1;
rw=0;
e=1;
delay(1);
e=0;
}
void adc_conv()
{                      
wr = 0;
delay(2);                     
wr = 1;                     
while(intr);
delay(2);
intr=1; 
}
void adc_read()
{
unsigned int value;                  
rd = 0;
delay(2);
value=adc_input;
v_out=(5.0/256)*value;      //divide by 256 as it's a 8 bit converter
pressure=(((v_out/5.0)+0.09)/0.009-1)*7.50062; // convert output voltage(volt) to KPA and conversion from Kpa to mmHg
delay(1);
rd=1; 
lcd_data_adc(pressure);                                          
}
void lcd_data_adc(unsigned int i)
{
int p;
int k=0;
while(i>0)
{
  num[k]=i%10;
  i=i/10;
  k++;
}
k--;
for (p=k;p>=0;p--)
{
dataport=num[p]+48;
rw = 0;
rs = 1;
e = 1;
delay(1);
e = 0;
}
}
void lcd_data_string(char *x)
{
while(*x!='\0')
    {
        lcd_data(*x);
        delay(2);
        x++;
    }
}