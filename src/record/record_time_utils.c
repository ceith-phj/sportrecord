/*
 * =====================================================================================
 *
 *  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author: pengmin@huami.com
 *  Maintainer: pengmin@huami.com
 *
 * =====================================================================================
 */

#include <record/record_time_utils.h>
#define TAG "record_time_utils"

const void Time_getRecordTime(uint32_t time, char buffer[])
{
    HLOG_I(TAG, "%s time:%d\r\n", __func__, time);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    HmTimeDate timeDate = {0};
    HmTime rspTime = {0};
    rspTime.sec = time;
    hmTime_getSpecialZoneTime(&rspTime, &timeDate, UTC0_ZONE);
    snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%d/%02d/%02d %02d:%02d",
             timeDate.year, timeDate.month, timeDate.day, timeDate.hour, timeDate.minute);
    setSBStringData(getAppInstance(), buffer, strff);
};

const void Time_getExerciseTime(uint32_t exerciseTime, char buffer[])
{
    HLOG_I(TAG, "%s ExerciseTime:%d\r\n", __func__, (int)exerciseTime);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    int hour = exerciseTime / 3600;
    int minute = (exerciseTime - hour * 3600) / 60;
    int sec = (exerciseTime - hour * 3600 - minute * 60);
    if (hour == 0 && minute == 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "0:%02d", sec);
    }
    else if (hour == 0 && minute > 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%d:%02d", minute, sec);
    }
    else if (hour > 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%d:%02d:%02d", hour, minute,
                 sec);
    }
    setSBStringData(getAppInstance(), buffer, strff);
}