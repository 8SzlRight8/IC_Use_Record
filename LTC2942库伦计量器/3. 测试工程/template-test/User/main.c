#include "stm32f4xx.h"
#include "dri_LTC2942.h"


int main(void)
{
	LTC2942_Init();  //初始化LCT2942
	
  getbatteryCapacity(); //获取库伦量
	getbatteryPercent();  //获取电量百分比
	
	
  while (1)
  {      
  }  

}
