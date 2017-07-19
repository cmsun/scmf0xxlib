#include "Sm_RTC.h"

Sm::SmCallback Sm_RTC_SecCallback;
void *Sm_RTC_SecArg;

Sm::SmCallback Sm_RTC_AlarmCallback;
void *Sm_RTC_AlarmArg;

/*
********************************************************************************
*                               RTC_IRQHandler 
*
* Description : stm32的RTC时钟的秒中断和闹钟中断共用函数
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : none
********************************************************************************
*/
extern "C" void RTC_IRQHandler(void)
{
#ifdef Sm_UCOS_Support 
    OSIntEnter();
#endif

    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)    //闹钟中断
    {
        if(Sm_RTC_AlarmCallback)
            Sm_RTC_AlarmCallback(Sm_RTC_AlarmArg);
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);
    }

#ifdef Sm_UCOS_Support 
    OSIntExit();
#endif
}

/*
********************************************************************************
*                               Init 
*
* Description : 初始化时实时钟
*
* Arguments   : none
*
* Returns     : 返回值-1：RTC时钟初始化失败。
*               返回值0 ：RTC初始化成功，但是还没有设置过时间，需要设置时间。
*               返回值1 ：RTC时钟已经初始化过且已经设置过时间，不需要重新设置时间。
*
* Note(s)     : none
********************************************************************************
*/
int Sm_RTC::initialize(void)
{
    int ret = 1, loop = 0;
#ifdef Sm_UCOS_Support
    OS_ERR err;
#endif

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);                        //允许访问备份寄存器

    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5050)   //如果没有设置过RTC时钟
    {
        RCC_LSEConfig(RCC_LSE_ON);                  //初始化外部低速晶振

        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            if(++loop >= 250)
            {
                Sm_Debug("Initializes RTC failed!");
                return -1;                          //初始化RTC失败，晶振有问题
            }
#ifdef Sm_UCOS_Support
            OSTimeDly(2, OS_OPT_TIME_PERIODIC, &err);
#else
            Sm_SysTick::delay_ms(10);
#endif
        }

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);         //选择LSE为RTC的时钟源
        RCC_RTCCLKCmd(ENABLE);                          //使能RTC时钟
        RTC_WaitForSynchro();                           //等待 RTC 寄存器同步

        RTC_InitTypeDef rtc_init;
        rtc_init.RTC_HourFormat = RTC_HourFormat_24;
        rtc_init.RTC_AsynchPrediv = 0x7F;
        rtc_init.RTC_SynchPrediv = 0xFF;
        if(RTC_Init(&rtc_init) == ERROR)
        {
            ret = -1;
        }
        else
        {
            RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5050);   //写备份寄存器，表明RTC已经设置过
            ret = 0;                                        //RTC初始化成功，但未设置时间
        }
    }

    //设置RTC默认中断优先级, 并使能中断线。
    NVIC_SetPriority(RTC_IRQn, Default_RTC_NVIC_IRQChannelPriority);
    NVIC_EnableIRQ(RTC_IRQn);

    return ret;
}

/*
********************************************************************************
*                               setDateTime 
*
* Description : 设置RTC时间。
*
* Arguments   : year: 0-99
*               month：1-12
*               day: 1-31
*               hour: 0-23
*               minute: 0-59
*               second: 0-59
*
* Returns     : none
*
* Note(s)     : none
********************************************************************************
*/
void Sm_RTC::setDateTime(uint8_t year, uint8_t month, uint8_t day,
                         uint8_t hour, uint8_t minute, uint8_t second)
{
    RTC_TimeTypeDef rtc_time;
    rtc_time.RTC_Hours = hour;
    rtc_time.RTC_Minutes = minute;
    rtc_time.RTC_Seconds = second;
    RTC_SetTime(RTC_Format_BIN, &rtc_time);

    RTC_DateTypeDef rtc_date;
    rtc_date.RTC_Year = year;
    rtc_date.RTC_Month = month;
    rtc_date.RTC_Date = day;
    rtc_date.RTC_WeekDay = 1;   //星期会按给的日期自动调整。
    RTC_SetDate(RTC_Format_BIN, &rtc_date);
}

/*
********************************************************************************
*                               setDataAlarm
*
* Description : 设置月闹钟
*
* Arguments   : 
*
* Returns     : none
*
* Note(s)     : none
********************************************************************************
*/
void Sm_RTC::setDateAlarm(uint8_t monthday, uint8_t hour, uint8_t minute, uint8_t second)
{
    RTC_AlarmTypeDef rtc_alarm;
    rtc_alarm.RTC_AlarmTime.RTC_Hours = hour;
    rtc_alarm.RTC_AlarmTime.RTC_Minutes = minute;
    rtc_alarm.RTC_AlarmTime.RTC_Seconds = second;

    rtc_alarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
    rtc_alarm.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
    rtc_alarm.RTC_AlarmDateWeekDay = monthday;

    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &rtc_alarm);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

/*
********************************************************************************
*                               setWeekAlarm 
*
* Description : 设置星期闹钟。
*
* Arguments   : none
*
* Returns     : none
*
* Note(s)     : none
********************************************************************************
*/
void Sm_RTC::setWeekAlarm(uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second)
{
    RTC_AlarmTypeDef rtc_alarm;
    rtc_alarm.RTC_AlarmTime.RTC_Hours = hour;
    rtc_alarm.RTC_AlarmTime.RTC_Minutes = minute;
    rtc_alarm.RTC_AlarmTime.RTC_Seconds = second;

    rtc_alarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
    rtc_alarm.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
    rtc_alarm.RTC_AlarmDateWeekDay = weekday;

    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &rtc_alarm);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

time_t Sm_RTC::getDateTime(void)
{
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    RTC_GetDate(RTC_Format_BIN, &date);
    RTC_GetTime(RTC_Format_BIN, &time);

    struct tm t;
    t.tm_sec = time.RTC_Seconds;                //范围0-59
    t.tm_min = time.RTC_Minutes;                //范围0-59
    t.tm_hour = time.RTC_Hours;                 //范围0-23
    t.tm_mday = date.RTC_Date;                  //一个月中的第几天，范围1-31
    t.tm_mon = date.RTC_Month-1;                //月份，范围0-11，0代表1月
    t.tm_year = (2000+date.RTC_Year) - 1900;    //实际年分减去1900
    t.tm_wday = 0;                              //星期几，范围0-6，0代表星期天
    t.tm_yday = 0;                              //一年中的第几天, 范围0-364
    t.tm_isdst = -1;

    return mktime(&t); 
}

void Sm_RTC::getDateTime(RTC_DateTypeDef &date, RTC_TimeTypeDef &time)
{
    RTC_GetDate(RTC_Format_BIN, &date);
    RTC_GetTime(RTC_Format_BIN, &time);
}

void Sm_RTC::setNVICPriority(uint8_t priority)
{
    Sm_assert(IS_NVIC_PRIORITY(priority));

    NVIC_SetPriority(RTC_IRQn, priority);
}

void Sm_RTC::alarmITConfig(FunctionalState state)
{
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);

    EXTI_InitTypeDef exti_init;
    exti_init.EXTI_Line = EXTI_Line17;
    exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
    exti_init.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_init);

    RTC_ITConfig(RTC_IT_ALRA, state);
    RTC_WaitForSynchro();
}

void Sm_RTC::setAlarmCallback(Sm::SmCallback callback, void *arg)
{
    Sm_RTC_AlarmCallback = callback;
    Sm_RTC_AlarmArg = arg;    

    alarmITConfig(ENABLE);
}

