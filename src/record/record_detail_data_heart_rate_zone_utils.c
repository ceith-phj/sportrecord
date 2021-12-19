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

#include <record/record_detail_data_heart_rate_zone_utils.h>
#include "system_locale.h"
#include "core/display_metrics.h"

#define TAG "record_detail_data_heart_rate_zone_utils"

#ifdef HM_FEATURE_BLED_UI
#define MAX_WIDTH getDimension(270)
#else
#define MAX_WIDTH getDimension(320)
#endif

static void _rate_time(uint32_t time,
        char *buffer)
{
    char strff[256];
    memset(strff, 0, sizeof(strff));
    int hour = time / 3600;
    int minute = (time % 3600) / 60;
    int sec = (time % 60);

    if (hour == 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%02d:%02d", minute, sec);
    }
    else
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%d:%02d:%02d", hour, minute,
                 sec);
    }

    setSBStringData(getAppInstance(), buffer, strff);
}

const void RecordHHZ_getRecordHHZValue(gr_application_t *app,
                                       SportRecordDetailData sportRecordDetailData, char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.title",
             itemPath);
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText("id_sportrecord_hrz_title"));

    if (sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.type ==
            1)
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.vomaxTitle",
                 itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Vo2Max"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.anaerobicTitle", itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Anaerobic"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.aerobicTitle", itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Aerobic"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.intensiveTitle", itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Intensive"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.lightTitle",
                 itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Light"));
    }
    else
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.vomaxTitle",
                 itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Interval4"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.anaerobicTitle", itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Interval3"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.aerobicTitle", itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Interval2"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.intensiveTitle", itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Interval1"));
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.lightTitle",
                 itemPath);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_hrz_Interval0"));
    }

    float re = (float)
               sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[1];
    float zf = (float)
               sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[2];
    float xl = (float)
               sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[3];
    float nl = (float)
               sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[4];
    float wY = (float)
               sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[5];
    float totalTime = (float)sportRecordDetailData.recordDetailDate.sport_summary_message.times.totalTime;

    HLOG_I(TAG,
           "%s rsTime:%f zfTime:%f xlTime:%f nlTime:%f wYtime:%f totalTime:%f\r\n",
           __func__, re, zf, xl, nl, wY, totalTime);
    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.light",
             itemPath);
     _rate_time(sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[1],buffer);
    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.intensive",
             itemPath);
    _rate_time(sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[2],buffer);

    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.aerobic",
             itemPath);
    _rate_time(sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[3],buffer);
    
    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.anaerobic",
             itemPath);
    _rate_time(sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[4],buffer);
    
    
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.vomax",
             itemPath);
    _rate_time(sportRecordDetailData.recordDetailDate.sport_summary_message.hrZone.zone[5],buffer);

    if (totalTime > 0)
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.vomaxWidth",
                 itemPath);

        if (((wY / totalTime) * MAX_WIDTH) > MAX_WIDTH)
        {
            setSBIntData(getAppInstance(), buffer, MAX_WIDTH);
        }
        else
        {
            setSBIntData(getAppInstance(), buffer,
                         (wY / totalTime) * MAX_WIDTH);
        }

        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.anaerobicWidth", itemPath);

        if (((nl / totalTime) * MAX_WIDTH) > MAX_WIDTH)
        {
            setSBIntData(getAppInstance(), buffer, MAX_WIDTH);
        }
        else
        {
            setSBIntData(getAppInstance(), buffer,
                         (nl / totalTime) * MAX_WIDTH);
        }

        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.aerobicWidth", itemPath);

        if (((xl / totalTime) * MAX_WIDTH) > MAX_WIDTH)
        {
            setSBIntData(getAppInstance(), buffer, MAX_WIDTH);
        }
        else
        {
            setSBIntData(getAppInstance(), buffer,
                         (xl / totalTime) * MAX_WIDTH);
        }

        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.SportRecordDetailHhzitem.item.intensiveWidth", itemPath);

        if (((zf / totalTime) * MAX_WIDTH) > MAX_WIDTH)
        {
            setSBIntData(getAppInstance(), buffer, MAX_WIDTH);
        }
        else
        {
            setSBIntData(getAppInstance(), buffer,
                         (zf / totalTime) * MAX_WIDTH);
        }

        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailHhzitem.item.lightWidth",
                 itemPath);

        if (((re / totalTime) * MAX_WIDTH) > MAX_WIDTH)
        {
            setSBIntData(getAppInstance(), buffer, MAX_WIDTH);
        }
        else
        {
            setSBIntData(getAppInstance(), buffer,
                         (re / totalTime) * MAX_WIDTH);
        }
    }
}
