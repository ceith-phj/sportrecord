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

#include <record/record_unit_utils.h>
#include <record/record_detail_swim_dis.h>

#define TAG "record_unit_utils"

const void RecordUnit_getRecordMainDataUnit1(int sportType, char *unit,
        int charLength, uint8_t unitType)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:      /* 户外跑 */
        case SPORT_GROUP_TREADMILL:            /* 跑步机 */
        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_TRAIL_RUNNING:        /* 越野跑 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        case SPORT_GROUP_MULTISPORT:           /* 复合运动 （不支持实时数据配置） */
        case SPORT_GROUP_SKIING:               /* 滑雪 */
        case SPORT_GROUP_SOCCER:               /* 足球 */
        {
            if (getMileageUnit() == 1)
            {
                snprintf(unit, charLength, "%s",
                         System_getLocaleText("id_sportrecord_pb_uint_mile"));
            }
            else
            {
                snprintf(unit, charLength, "%s",
                         System_getLocaleText("id_sportrecord_unit_km"));
            }

            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        {
            if (unitType == 1)
            {
                snprintf(unit, charLength, "%s",
                         System_getLocaleText("id_sportrecord_pb_uint_ma"));
            }
            else
            {
                snprintf(unit, charLength, "%s",
                         System_getLocaleText("id_sportrecord_unit_meter"));
            }

            break;
        }

        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            if (getMileageUnit() == 1)
            {
                snprintf(unit, charLength, "%s",
                         System_getLocaleText("id_sportrecord_pb_uint_ma"));
            }
            else
            {
                snprintf(unit, charLength, "%s",
                         System_getLocaleText("id_sportrecord_unit_meter"));
            }

            break;
        }

        case SPORT_GROUP_TENNIS:         /* 网球 */
        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
            snprintf(unit, charLength, "%s",
                     System_getLocaleText("id_sportrecord_unit_times"));
            break;

        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_3:                 /* 类型3 */
            snprintf(unit, charLength, "%s",
                     System_getLocaleText("id_sportrecord_unit_cal"));
            break;

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
            snprintf(unit, charLength, "%s",
                     System_getLocaleText("id_sportrecord_unit_floor"));
            break;

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
            snprintf(unit, charLength, "%s",
                     System_getLocaleText("id_sportrecord_unit_number"));
            break;

        // case SPORT_GROUP_TRIATHLON: /* 铁人三项 （不支持实时数据配置） */
        // break;
        // case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        // break;
        // case SPORT_GROUP_PARASAILING: /* 滑翔伞 */
        // break;
        // case SPORT_GROUP_4: /* 类型4 */
        // break;
        // case SPORT_GROUP_5: /* 类型5 */
        // break;
        // case SPORT_GROUP_6: /* 类型6 */
        // break;
        // case SPORT_GROUP_GOLF: /* 高尔夫 */
        // break;
        // case SPORT_GROUP_GOLF_SWING: /* 高尔夫挥杆 */
        // break;
        default:
            break;
    }
}

const void RecordUnit_getRecordMainDataUnit(int sportType, char buffer[],
        uint8_t unitType)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:      /* 户外跑 */
        case SPORT_GROUP_TREADMILL:            /* 跑步机 */
        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_TRAIL_RUNNING:        /* 越野跑 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        case SPORT_GROUP_MULTISPORT:           /* 复合运动 （不支持实时数据配置） */
        case SPORT_GROUP_SKIING:               /* 滑雪 */
        case SPORT_GROUP_SOCCER:               /* 足球 */
        {
            if (getMileageUnit() == 1)
            {
                setSBStringData(getAppInstance(), buffer,
                                System_getLocaleText("id_sportrecord_pb_uint_mile"));
            }
            else
            {
                setSBStringData(getAppInstance(), buffer,
                                System_getLocaleText("id_sportrecord_unit_km"));
            }

            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        {
            if (unitType == 1)
            {
                setSBStringData(getAppInstance(), buffer,
                                System_getLocaleText("id_sportrecord_pb_uint_ma"));
            }
            else
            {
                setSBStringData(getAppInstance(), buffer,
                                System_getLocaleText("id_sportrecord_unit_meter"));
            }

            break;
        }

        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            if (getMileageUnit() == 1)
            {
                setSBStringData(getAppInstance(), buffer,
                                System_getLocaleText("id_sportrecord_pb_uint_ma"));
            }
            else
            {
                setSBStringData(getAppInstance(), buffer,
                                System_getLocaleText("id_sportrecord_unit_meter"));
            }

            break;
        }

        case SPORT_GROUP_TENNIS:         /* 网球 */
        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_unit_times"));
            break;

        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_3:                 /* 类型3 */
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_unit_cal"));
            break;

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_unit_floor"));
            break;

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_unit_number"));
            break;

        // case SPORT_GROUP_TRIATHLON: /* 铁人三项 （不支持实时数据配置） */
        // break;
        // case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        // break;
        // case SPORT_GROUP_PARASAILING: /* 滑翔伞 */
        // break;
        // case SPORT_GROUP_4: /* 类型4 */
        // break;
        // case SPORT_GROUP_5: /* 类型5 */
        // break;
        // case SPORT_GROUP_6: /* 类型6 */
        // break;
        // case SPORT_GROUP_GOLF: /* 高尔夫 */
        // break;
        // case SPORT_GROUP_GOLF_SWING: /* 高尔夫挥杆 */
        // break;
        default:
            break;
    }
}

const void RecordData_getRecordMainData1(int sportType, float data,
        char buffer[],uint8_t unit)
{
    HLOG_D(TAG, "%s mainData:%f\r\n", __func__, data);
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);
    char strff[256];
    float showData = 0.0f;

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:
        case SPORT_GROUP_TREADMILL:
        case SPORT_GROUP_WALKING:
        case SPORT_GROUP_INDOOR_WALKING:
        case SPORT_GROUP_OUTDOOR_CYCLING:
        case SPORT_GROUP_1:
        case SPORT_GROUP_MOUNTAINEERING:
        case SPORT_GROUP_TRAIL_RUNNING:
        case SPORT_GROUP_CROSS_COUNTRY_SKIING:
        case SPORT_GROUP_SOCCER:
        case SPORT_GROUP_OUTDOOR_ROWING:
        case SPORT_GROUP_PARASAILING:
        case SPORT_GROUP_DRIVING:
        case SPORT_GROUP_SKIING:
        {
            showData = RecordMile_to_km(data / 1000.0);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%.2f", ((int)(showData * 100)) / 100.0f);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:
        case SPORT_GROUP_OPEN_WATER_SWIMMING:
        {
            
            showData = (unit == 1 ? _SwimPoolLenght(sportEngine_utils()->meter2yard(data)) : data);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d", (int)showData);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:
        case SPORT_GROUP_2:
        case SPORT_GROUP_3:
        case SPORT_GROUP_STRENGTH_TRAINING:
        case SPORT_GROUP_INDOOR_FITNESS:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d", (int)data);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d", (int)data);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_TENNIS:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d", (int)data);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d", (int)data);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }
        break;

        case SPORT_GROUP_STAIR_CLIMBING:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d", (int)data);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        default:
            break;
    }
}
const void RecordData_getRecordMainData(int sportType, float data,
                                        char buffer[], uint8_t unit)
{
    HLOG_D(TAG, "%s mainData:%f\r\n", __func__, data);
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);
    char strff[256];
    float showData = 0.0f;
    char mainDataUnit[21] = {0};
    RecordUnit_getRecordMainDataUnit1(sportType, mainDataUnit, 21, unit);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:
        case SPORT_GROUP_TREADMILL:
        case SPORT_GROUP_WALKING:
        case SPORT_GROUP_INDOOR_WALKING:
        case SPORT_GROUP_OUTDOOR_CYCLING:
        case SPORT_GROUP_1:
        case SPORT_GROUP_MOUNTAINEERING:
        case SPORT_GROUP_TRAIL_RUNNING:
        case SPORT_GROUP_CROSS_COUNTRY_SKIING:
        case SPORT_GROUP_SOCCER:
        case SPORT_GROUP_OUTDOOR_ROWING:
        case SPORT_GROUP_PARASAILING:
        case SPORT_GROUP_DRIVING:
        case SPORT_GROUP_SKIING:
        {
            showData = RecordMile_to_km(data / 1000.0);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%.2f %s ", ((int)(showData * 100)) / 100.0f,
                     mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:
        case SPORT_GROUP_OPEN_WATER_SWIMMING:
        {
            showData = (unit == 1 ? _SwimPoolLenght(sportEngine_utils()->meter2yard(data)) : data);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d %s", (int)showData, mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:
        case SPORT_GROUP_2:
        case SPORT_GROUP_3:
        case SPORT_GROUP_STRENGTH_TRAINING:
        case SPORT_GROUP_INDOOR_FITNESS:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d %s ", (int)data, mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d %s ", (int)data, mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_TENNIS:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d %s ", (int)data, mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d %s ", (int)data, mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }
        break;

        case SPORT_GROUP_STAIR_CLIMBING:
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d %s ", (int)data, mainDataUnit);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }

        default:
            break;
    }
}

const void RecordExercisetime_getRecordExercisetime(uint32_t exercisetime,
        char buffer[])
{
    HLOG_D(TAG, "%s Exercisetime:%d\r\n", __func__, exercisetime);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    int hour = exercisetime / 3600;
    int minute = (exercisetime - hour * 3600) / 60;
    int sec = (exercisetime - hour * 3600 - minute * 60);

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

const void RecordTime_getRecordTime(uint32_t time, char buffer[])
{
    HLOG_D(TAG, "%s time:%d\r\n", __func__, time);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    HmTimeDate timeDate = {0};
    HmTime rspTime = {0};
    rspTime.sec = time;
    hmTime_getLocalTime(&rspTime, &timeDate);
    HmTimeDate sysTimeDate = {0};
    HmTime sysTime = {0};
    hmTime_getTime(&sysTime);
    hmTime_getLocalTime(&sysTime, &sysTimeDate);
    char formatTime[80];

    if (timeDate.month == sysTimeDate.month)
    {
        if (timeDate.day < sysTimeDate.day)
        {
            snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%02d/%02d", timeDate.month,
                     timeDate.day);
        }
        else
        {
            datetime_formatTimeWithAmPm(&rspTime, formatTime, sizeof(formatTime));
            snprintf(strff, sizeof(strff), "%s", formatTime);
        }
    }
    else
    {
        if (timeDate.month < sysTimeDate.month)
        {
            snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%02d/%02d", timeDate.month,
                     timeDate.day);
        }
        else
        {
            datetime_formatTimeWithAmPm(&rspTime, formatTime, sizeof(formatTime));
            snprintf(strff, sizeof(strff), "%s", formatTime);
        }
    }

    setSBStringData(getAppInstance(), buffer, strff);
}

const float RecordKm_to_Mile(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->km2mile(num);
    }

    return num;
}

const float RecordMeter_to_foot(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->meter2foot(num);
    }

    return num;
}

const float RecordCm_to_inch(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->cm2inch(num);
    }

    return num;
}

const float RecordMeter_to_yard(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->meter2yard(num);
    }

    return num;
}

const float Recordm_to_mile(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->meter2mile(num);
    }

    return num;
}

const float RecordMile_to_km(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->km2mile(num);
    }

    return num;
}

const float RecordKm_to_mile_pace(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->mile2km(num);
    }

    return num;
}

const float RecordHm_to_hy_pace(float num)
{
    if (getMileageUnit() == 1)
    {
        return sportEngine_utils()->yard2meter(num);
    }

    return num;
}

const H_Int32 getMileageUnit()
{
    H_Int32 milUint = 0;
    Sysprop_getInt(SETTINGS_MILEAGE_UNIT, &milUint, milUint);
    return milUint;
}
