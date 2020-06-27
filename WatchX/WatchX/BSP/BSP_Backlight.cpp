#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "BSP.h"
#include "rtc.h"

static void Backlight_AnimCallback(void * obj, int16_t brightness)
{
    Backlight_SetValue(brightness);
}

void Backlight_Init()
{
    PWM_Init(TFT_LED_Pin, 1000, 20000);
}

void Backlight_SetGradual(uint16_t target, uint16_t time)
{
    static lv_anim_t a;
    lv_obj_add_anim(
        NULL, &a,
        (lv_anim_exec_xcb_t)Backlight_AnimCallback,
        Backlight_GetValue(), target,
        time
    );
}

uint16_t Backlight_GetValue()
{
    return timer_get_compare(PIN_MAP[TFT_LED_Pin].TIMx, PIN_MAP[TFT_LED_Pin].TimerChannel);
}

void Backlight_SetValue(int16_t val)
{
    __LimitValue(val, 0, 1000);
    analogWrite(TFT_LED_Pin, val);
}

uint16_t Backlight_GetBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //ʹ��PWR��BKP����ʱ��
    PWR_BackupAccessCmd(ENABLE);  //ʹ�ܺ󱸼Ĵ�������
    
    uint16_t value = RTC_ReadBackupRegister(RTC_BKP_DR2);
    
    if(value == 0 || value > 1000)
    {
        value = 1000;
    }
    
    return value;
}

void Backlight_UpdateBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //ʹ��PWR��BKP����ʱ��
    PWR_BackupAccessCmd(ENABLE);  //ʹ�ܺ󱸼Ĵ�������
    RTC_WaitForSynchro();   //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_WriteBackupRegister(RTC_BKP_DR2, Backlight_GetValue()); //����ȡ������д��󱸼Ĵ���
    RTC_WaitForSynchro();   //�ȴ����һ�ζ�RTC�Ĵ�����д�������
}

