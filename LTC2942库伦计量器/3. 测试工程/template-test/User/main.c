#include "stm32f4xx.h"
#include "dri_LTC2942.h"


int main(void)
{
	LTC2942_Init();  //��ʼ��LCT2942
	
  getbatteryCapacity(); //��ȡ������
	getbatteryPercent();  //��ȡ�����ٷֱ�
	
	
  while (1)
  {      
  }  

}
