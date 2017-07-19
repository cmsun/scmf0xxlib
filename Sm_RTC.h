#ifndef SM_RTC_H
#define SM_RTC_H

#include <time.h>

#include "Sm_Config.h"
#include "Sm_SysTick.h"

class Sm_RTC
{
public:
    static int initialize(void);
    static void setDateTime(uint8_t year, uint8_t month, uint8_t day,
                            uint8_t hour, uint8_t minute, uint8_t second);
    static void setDateAlarm(uint8_t date, uint8_t hour, uint8_t minute, uint8_t second);
    static void setWeekAlarm(uint8_t week, uint8_t hour, uint8_t minute, uint8_t second);
    static time_t getDateTime(void);
    static void getDateTime(RTC_DateTypeDef &date, RTC_TimeTypeDef &time);
    static void setNVICPriority(uint8_t priority = Default_RTC_NVIC_IRQChannelPriority);
    static void alarmITConfig(FunctionalState state);
    static void setAlarmCallback(Sm::SmCallback callback, void *arg = 0);
};

#endif
