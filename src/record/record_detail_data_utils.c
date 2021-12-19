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

#include <record/record_detail_data_utils.h>
#define TAG "record_detail_data_utils"
#include "sport_engine_utils.h"


const char *mainFirstValue = "%s.SportRecordDetailTrackItem.item.firstvalue";
const char *mainSecondValue = "%s.SportRecordDetailTrackItem.item.secondvalue";
const char *assistFirstValue =
    "%s.SportRecordDetailDataFirstItem.item.firstvalue";
const char *assistSecondValue =
    "%s.SportRecordDetailDataFirstItem.item.secondvalue";
const char *assistThreValue =
    "%s.SportRecordDetailDataFirstItem.item.threvalue";
const char *assistFourValue =
    "%s.SportRecordDetailDataSecondItem.item.firstvalue";
const char *assistFiveValue =
    "%s.SportRecordDetailDataSecondItem.item.secondvalue";
const char *assistSixValue =
    "%s.SportRecordDetailDataSecondItem.item.threvalue";

voice_play_data _voice_play_data = {0};

const void RecordDate_getRecordDate(uint32_t time, char buffer[])
{
    HLOG_I(TAG, "%s time:%d\r\n", __func__, time);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    HmTimeDate timeDate = {0};
    HmTime rspTime = {0};
    rspTime.sec = time;
    hmTime_getLocalTime(&rspTime, &timeDate);
    char fromatTime[80];
    datetime_formatDateTimeDefault(&rspTime, fromatTime, sizeof(fromatTime));
    snprintf(strff, sizeof(strff), "%s", fromatTime);
    setSBStringData(getAppInstance(), buffer, strff);
};
const void getRecoedPace(float data, const char *value, char *itemPath)
{
    HLOG_I(TAG, "%s date:%f value:%s itemPath:%s\r\n", __func__, data, value,
           itemPath);
    char strff[256];
    char buffer[256] = {0};
    int min = (int)(data / 60.0f);
    int sec = (int)(data - 60.0f * min);

    if (min > 0 || sec > 0)
    {
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%d\'%02d", min, sec);
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), value, itemPath);
        setSBStringData(getAppInstance(), buffer, strff);
    }
    else
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), value, itemPath);
        setSBStringData(getAppInstance(), buffer, "--");
    }
}

const void getRecordSwimmingStyle(uint32_t data, const char *value,
                                  char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);

    switch (data)
    {
        case 1: //蛙泳
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_breaststroke"));
            break;

        case 2: //自由式
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_freestyle"));
            break;

        case 3: //仰泳
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_backstroke"));
            break;

        case 4: //蝴蝶
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_butterfly"));
            break;

        case 5: //未知
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_unknow"));
            break;

        case 6://混合泳
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_medley"));
            break;

        default:
            setSBStringData(getAppInstance(), buffer,
                            System_getLocaleText("id_sportrecord_swim_style_unknow"));
            break;
    }
}


const void RecordMain_getRecordMainValue(SportRecordDetailData
        sportRecordDetailData, int sportType, char *itemPath)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);
    float avgVel = 0.0f;
    float showDistance = 0.0f;
    float avgPace = 0.0f;

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        case SPORT_GROUP_TRAIL_RUNNING:   /* 越野跑 */
        {
            showDistance = RecordMile_to_km(
                               sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance /
                               1000.0f);
            getRecordFloat(((int)(showDistance * 100)) / 100.0f, mainFirstValue, itemPath);
            avgPace = RecordKm_to_mile_pace(
                          sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace *
                          1000.0f);
            getRecoedPace(avgPace, mainSecondValue, itemPath);
            _voice_play_data.distance = getFloatPoint2(((int)(showDistance * 100)) / 100.0f);
            _voice_play_data.pace_avg = (int32_t)avgPace;

            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            showDistance = (sportRecordDetailData.recordDetailDate.sport_summary_message.swim.unit == 1 ?
                _SwimPoolLenght(sportEngine_utils()->meter2yard(sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance)) : sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance);
            getRecordInt(showDistance, mainFirstValue, itemPath);
            avgPace = RecordHm_to_hy_pace(
                          sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace *
                          100.0f);
            getRecoedPace(avgPace, mainSecondValue, itemPath);
            _voice_play_data.distance = (int32_t)showDistance;
            _voice_play_data.pace_avg = (int32_t)avgPace;
            break;
        }

        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        case SPORT_GROUP_SOCCER:               /* 足球 */
        case SPORT_GROUP_OUTDOOR_ROWING:       /* 户外划船 */
        case SPORT_GROUP_PARASAILING:          /* 滑翔伞 */
        {
            showDistance = RecordMile_to_km(
                               sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance /
                               1000.0f);
            getRecordFloat(((int)(showDistance * 100)) / 100.0f, mainFirstValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace
                    > 0.0f)
            {
                avgVel = RecordKm_to_Mile(1 /
                                          sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace *
                                          3.6);
            }

            getRecordFloat(avgVel, mainSecondValue, itemPath);
            _voice_play_data.distance = getFloatPoint2(((int)(showDistance * 100)) / 100.0f);
            _voice_play_data.speed_avg = getFloatPoint2(avgVel);
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING: /* 室内骑行 */
        case SPORT_GROUP_2:              /* 类型2 */
        case SPORT_GROUP_3:              /* 类型3 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                mainFirstValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate,
                mainSecondValue, itemPath);
            _voice_play_data.consume = sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal;
            _voice_play_data.hr_avg = sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate;
            break;
        }

        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.other.totalGroups,
                mainFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                mainSecondValue, itemPath);
            _voice_play_data.totalGroups = sportRecordDetailData.recordDetailDate.sport_summary_message.other.totalGroups;
            _voice_play_data.consume = sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal;
            break;
        }

        case SPORT_GROUP_SKIING: /* 滑雪 */
        {
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(sportRecordDetailData.recordDetailDate.sport_summary_message.downhill.totalDistanceOfDownhill),
                mainFirstValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace
                    > 0.0f)
            {
                avgVel = RecordKm_to_Mile(1 /
                                          sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace *
                                          3.6);
            }

            getRecordFloat(avgVel, mainSecondValue, itemPath);
            _voice_play_data.glide_total_distance = sportRecordDetailData.recordDetailDate.sport_summary_message.downhill.totalDistanceOfDownhill;
            _voice_play_data.speed_avg = getFloatPoint2(avgVel);
            _voice_play_data.glide_count = sportRecordDetailData.recordDetailDate.sport_summary_message.downhill.timesOfDownhill;
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        {
            // showDistance = RecordMile_to_km(
            //                    sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance /
            //                    1000.0f);
            // getRecordFloat(((int)(showDistance * 100)) / 100.0f, mainFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.rowing.averageRowingFrequency,
                mainSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.number.totalTimesOfRowing,
                mainFirstValue, itemPath);
            _voice_play_data.boating_avg_freq = sportRecordDetailData.recordDetailDate.sport_summary_message.rowing.averageRowingFrequency;
            break;
        }

        case SPORT_GROUP_TENNIS: /* 网球 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swing.totalTimesOfSwing,
                mainFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                mainSecondValue, itemPath);
            _voice_play_data.brandish_total_cnt = sportRecordDetailData.recordDetailDate.sport_summary_message.swing.totalTimesOfSwing;
            _voice_play_data.consume = sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal;
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.number.totalNumberOfJumps,
                mainFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.frequency.averageFrequency,
                mainSecondValue, itemPath);
            _voice_play_data.count_total = sportRecordDetailData.recordDetailDate.sport_summary_message.number.totalNumberOfJumps;
            _voice_play_data.freq_avg = sportRecordDetailData.recordDetailDate.sport_summary_message.frequency.averageFrequency;
            break;
        }

        case SPORT_GROUP_TRIATHLON:  /* 铁人三项 （不支持实时数据配置） */
        case SPORT_GROUP_MULTISPORT: /* 复合运动 （不支持实时数据配置） */
        {
            showDistance = RecordMile_to_km(
                               sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance /
                               1000.0f);
            getRecordFloat(((int)(showDistance * 100)) / 100.0f, mainFirstValue, itemPath);
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                mainSecondValue, itemPath);
            _voice_play_data.distance = getFloatPoint2(((int)(showDistance * 100)) / 100.0f);
            break;
        }

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.floors.upstairFloors,
                mainFirstValue, itemPath);
            getRecordFloat(
                sportRecordDetailData.recordDetailDate.sport_summary_message.floors.averageFloorsClimbedPerMinute,
                mainSecondValue, itemPath);
            _voice_play_data.climb_up_floors = sportRecordDetailData.recordDetailDate.sport_summary_message.floors.upstairFloors;
            _voice_play_data.climb_up_floors_in_min = getFloatPoint2(sportRecordDetailData.recordDetailDate.sport_summary_message.floors.averageFloorsClimbedPerMinute);
            _voice_play_data.climb_up_total_altitude = sportRecordDetailData.recordDetailDate.sport_summary_message.floors.upstairHeight;
            break;
        }

        case SPORT_GROUP_4: /* 类型4 */
            break;

        case SPORT_GROUP_5: /* 类型5 */
            break;

        case SPORT_GROUP_6: /* 类型6 */
            break;

        case SPORT_GROUP_GOLF: /* 高尔夫 */
            break;

        case SPORT_GROUP_GOLF_SWING: /* 高尔夫挥杆 */
            break;

        default:
            break;
    }
}
const void getRecordExerciseTime(uint32_t exerciseTime, const char *value,
                                 char *itemPath)
{
    HLOG_I(TAG, "%s date:%d value:%s itemPath:%s\r\n", __func__, (int)exerciseTime,
           value, itemPath);
    char strff[256];
    char buffer[256] = {0};
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

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}
const void getRecordInt(uint32_t data, const char *value, char *itemPath)
{
    HLOG_I(TAG, "%s date:%d value:%s itemPath:%s\r\n", __func__, (int)data, value,
           itemPath);
    char strff[256];
    char buffer[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%d", (int)data);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}
const void getHeartInt(uint32_t data, const char *value, char *itemPath)
{
    HLOG_I(TAG, "%s date:%d value:%s itemPath:%s\r\n", __func__, (int)data, value,
           itemPath);
    char strff[256];
    char buffer[256] = {0};

    if (data > 0)
    {
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%d", (int)data);
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), value, itemPath);
        setSBStringData(getAppInstance(), buffer, strff);
    }
    else
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), value, itemPath);
        setSBStringData(getAppInstance(), buffer, "--");
    }
}
const void getInvalid(char *format, const char *value, char *itemPath)
{
    char strff[256];
    char buffer[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", format);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}
const void getRecordFloat(float data, const char *value, char *itemPath)
{
    HLOG_I(TAG, "%s date:%f value:%s itemPath:%s\r\n", __func__, data, value,
           itemPath);
    char strff[256];
    char buffer[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%0.2f", data);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

const void getReordSwimFloat(float data, const char *value, char *itemPath)
{
    HLOG_I(TAG, "%s date:%f value:%s itemPath:%s\r\n", __func__, data, value,
           itemPath);
    char strff[256];
    char buffer[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%.1f", data);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

const float getFloatPoint2(float data)
{
    HLOG_I(TAG, "%s date:%f \r\n", __func__, data);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%0.2f", data);
    return atof(strff);
}

const void RecordFirst_getRecordFirstValue(SportRecordDetailData
        sportRecordDetailData, int sportType, char *itemPath)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);
    float maxSpeed = 0.0f;

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            float averageCadence =
                sportRecordDetailData.recordDetailDate.sport_summary_message.steps.averageCadence
                * 60;
            int cadence = averageCadence > ((int)(averageCadence) + 0.5) ? (int)(
                              averageCadence) + 1 : (int)(averageCadence);
            getRecordInt(cadence, assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.steps.averageStride,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;

            break;
        }

        case SPORT_GROUP_WALKING:        /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING: /* 室内步行 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.steps.steps,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING: /* 室内骑行 */
        case SPORT_GROUP_2:              /* 类型2 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.maximumHeartRate,
                assistSecondValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.minimumHeartRate,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_OUTDOOR_CYCLING: /* 户外骑行 */
        case SPORT_GROUP_1:               /* 类型1 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace >
                    0.0f)
            {
                maxSpeed = RecordKm_to_Mile(1 /
                                            sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace *
                                            3.6);
            }

            getRecordFloat(maxSpeed, assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_3: /* 类型3 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pressure.averagePressure
                    >= 255)
            {
                getInvalid("--", assistSecondValue, itemPath);
            }
            else
            {
                getRecordInt(
                    sportRecordDetailData.recordDetailDate.sport_summary_message.pressure.averagePressure,
                    assistSecondValue, itemPath);
            }

            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.restTimeBetweenGroups,
                assistSecondValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING: /* 泳池游泳 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            // getRecordInt(sportRecordDetailData.recordDetailDate.sport_summary_message.swim.mainSwimmingStyle, assistSecondValue, itemPath);
            getRecordSwimmingStyle(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swim.mainSwimmingStyle,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swim.trips,
                assistThreValue, itemPath);
            _voice_play_data.swim_main_type = sportRecordDetailData.recordDetailDate.sport_summary_message.swim.mainSwimmingStyle;
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordSwimmingStyle(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swim.mainSwimmingStyle,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.swim_main_type = sportRecordDetailData.recordDetailDate.sport_summary_message.swim.mainSwimmingStyle;
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_MOUNTAINEERING: /* 登山 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.slope.averageSlope,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            _voice_play_data.altitude_total_up = sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationGain / 100;
            _voice_play_data.altitude_total_down = sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationLoss / 100;
            break;
        }

        case SPORT_GROUP_TRAIL_RUNNING: /* 越野跑 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            float averageCadence =
                sportRecordDetailData.recordDetailDate.sport_summary_message.steps.averageCadence
                * 60;
            int cadence = averageCadence > ((int)(averageCadence) + 0.5) ? (int)(
                              averageCadence) + 1 : (int)(averageCadence);
            getRecordInt(cadence, assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.slope.averageSlope,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_SKIING: /* 滑雪 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.downhill.timesOfDownhill,
                assistSecondValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace >
                    0.0f)
            {
                maxSpeed = RecordKm_to_Mile(1 /
                                            sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace *
                                            3.6);
            }

            getRecordFloat(maxSpeed, assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        case SPORT_GROUP_SOCCER:               /* 足球 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace >
                    0.0f)
            {
                maxSpeed = RecordKm_to_Mile(1 /
                                            sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace *
                                            3.6);
            }

            getRecordFloat(maxSpeed, assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.other.totalGroups,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_TENNIS: /* 网球 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate,
                assistSecondValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.maximumHeartRate,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.restTimeBetweenGroups,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.other.totalGroups,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_TRIATHLON:  /* 铁人三项 （不支持实时数据配置） */
        case SPORT_GROUP_MULTISPORT: /* 复合运动 （不支持实时数据配置） */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.switchTime1,
                assistFirstValue, itemPath);
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.switchTime2,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.rowing.averageRowingFrequency,
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            getRecordInt(
                (int)RecordMeter_to_foot(floor(
                    sportRecordDetailData.recordDetailDate.sport_summary_message.floors.upstairHeight)),
                assistSecondValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_PARASAILING: /* 滑翔伞 */
        {
            getRecordExerciseTime(
                sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime,
                assistFirstValue, itemPath);
            //getRecordFloat(sportRecordDetailData.recordDetailDate.sport_summary_message.floors.upstairHeight,assistSecondValue,itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistThreValue, itemPath);
            _voice_play_data.exerciseTime = sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;
            break;
        }

        case SPORT_GROUP_4: /* 类型4 */
        {
            break;
        }

        case SPORT_GROUP_5: /* 类型5 */
        {
            break;
        }

        case SPORT_GROUP_6: /* 类型6 */
        {
            break;
        }

        case SPORT_GROUP_GOLF: /* 高尔夫 */
        {
            break;
        }

        case SPORT_GROUP_GOLF_SWING: /* 高尔夫挥杆 */
        {
            break;
        }

        default:
            break;
    }
}

const void RecordSecond_getRecordSecondValue(SportRecordDetailData
        sportRecordDetailData, int sportType, char *itemPath)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);
    float avgSpeed = 0.0f;

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        {
            getRecordInt(
               (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationGain /
                100)), assistFourValue, itemPath);
            getRecordInt(
                 (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationLoss /
                100)), assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_TREADMILL: /* 跑步机 */
        {
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate,
                assistFourValue, itemPath);

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace
                    > 0.0f)
            {
                avgSpeed = RecordKm_to_Mile(1 /
                                            sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace *
                                            3.6);
            }

            getRecordFloat(avgSpeed, assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_WALKING:        /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING: /* 室内步行 */
        {
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate,
                assistFourValue, itemPath);
            float averageCadence =
                sportRecordDetailData.recordDetailDate.sport_summary_message.steps.averageCadence
                * 60;
            int cadence = averageCadence > ((int)(averageCadence) + 0.5) ? (int)(
                              averageCadence) + 1 : (int)(averageCadence);
            getRecordInt(cadence, assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.steps.averageStride,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING: /* 室内骑行 */
        {
            break;
        }

        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationGain /
                100)), assistFourValue, itemPath);
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationLoss /
                100)), assistFiveValue, itemPath);
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.totalClimbDistance /
                100)), assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_1: /* 类型1 */
        {
            break;
        }

        case SPORT_GROUP_2: /* 类型2 */
        {
            break;
        }

        case SPORT_GROUP_3: /* 类型3 */
        {
            break;
        }

        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        {
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING: /* 泳池游泳 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swim.swolf,
                assistFourValue, itemPath);
            getRecordFloat(sportRecordDetailData.recordDetailDate.sport_summary_message.swim.unit == 1 ?
                sportEngine_utils()->meter2yard(sportRecordDetailData.recordDetailDate.sport_summary_message.swim.averageDistancePerStroke /
                100.0f) : (sportRecordDetailData.recordDetailDate.sport_summary_message.swim.averageDistancePerStroke /100.0f), assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swim.swolf,
                assistFourValue, itemPath);
           getRecordFloat(sportRecordDetailData.recordDetailDate.sport_summary_message.swim.unit == 1 ?
                sportEngine_utils()->meter2yard(sportRecordDetailData.recordDetailDate.sport_summary_message.swim.averageDistancePerStroke /
                100.0f) : (sportRecordDetailData.recordDetailDate.sport_summary_message.swim.averageDistancePerStroke /100.0f), assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swim.totalStrokes,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_TRAIL_RUNNING: /* 越野跑 */
        {
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationGain /
                100)), assistFourValue, itemPath);
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(floor(sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.elevationLoss /
                100)), assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_SKIING: /* 滑雪 */
        {
            getRecordInt(
                 (uint32_t)RecordMeter_to_foot(sportRecordDetailData.recordDetailDate.sport_summary_message.downhill.totalDownhillDrop),
                assistFourValue, itemPath);
            getRecordInt(
                (uint32_t)RecordMeter_to_foot(sportRecordDetailData.recordDetailDate.sport_summary_message.downhill.maximumDownhillDrop),
                assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        {
            break;
        }

        case SPORT_GROUP_SOCCER: /* 足球 */
        {
            break;
        }

        case SPORT_GROUP_TENNIS: /* 网球 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swing.timesOfForehandSwing,
                assistFourValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swing.timesOfBackhandSwing,
                assistFiveValue, itemPath);
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.swing.serveTimes,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            getRecordInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal,
                assistFourValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate,
                assistFiveValue, itemPath);
            getHeartInt(
                sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.maximumHeartRate,
                assistSixValue, itemPath);
            break;
        }

        case SPORT_GROUP_TRIATHLON:  /* 铁人三项 （不支持实时数据配置） */
        case SPORT_GROUP_MULTISPORT: /* 复合运动 （不支持实时数据配置） */
        {
            break;
        }

        case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        {
            break;
        }

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
        {
            break;
        }

        case SPORT_GROUP_PARASAILING: /* 滑翔伞 */
        {
            break;
        }

        case SPORT_GROUP_4: /* 类型4 */
        {
            break;
        }

        case SPORT_GROUP_5: /* 类型5 */
        {
            break;
        }

        case SPORT_GROUP_6: /* 类型6 */
        {
            break;
        }

        case SPORT_GROUP_GOLF: /* 高尔夫 */
        {
            break;
        }

        case SPORT_GROUP_GOLF_SWING: /* 高尔夫挥杆 */
        {
            break;
        }

        default:
            break;
    }
}

const void sportRecordVoicePlay(sport_voice_sport_record_type recordtype, float floatdata, uint32_t intdata, sport_swim_type_t swimdata)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    sport_voice_record_data voicedata;

    memset(&voicedata, 0, sizeof(sport_voice_record_data));
    voicedata.recordtype = recordtype;
    if(floatdata != 0.0){
        voicedata.m_pdata[0].floatdata = floatdata;
    }else if(intdata != 0){
        voicedata.m_pdata[0].intdata = intdata;
    }else if(swimdata != 0){
        voicedata.m_pdata[0].swimdata = swimdata;
    }
    HLOG_I(TAG, "recordtype:%d data: %f,%d,%d \r\n", voicedata.recordtype,
            voicedata.m_pdata[0].floatdata, voicedata.m_pdata[0].intdata, voicedata.m_pdata[0].swimdata);
    sportVoice_broadSportRecord(voicedata);
}

const void sportEndRedordVoiceDataSet(uint16_t sport_type)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    sport_type_t sportTypet = (sport_type_t)sport_type;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:      /* 户外跑 */
        case SPORT_GROUP_TREADMILL:            /* 跑步机 */
        case SPORT_GROUP_TRAIL_RUNNING:        /* 越野跑 */
        {
            /* 距离 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_DISTANCE, _voice_play_data.distance, 0, 0);
            /* 平均配速 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGEPACE_KM, 0.0, _voice_play_data.pace_avg, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_WALKING:               /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:        /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:       /* 户外骑行 */
        case SPORT_GROUP_1:                     /* 类型1 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING:  /* 越野滑雪 */
        case SPORT_GROUP_SOCCER:                /* 足球 */
        case SPORT_GROUP_OUTDOOR_ROWING:        /* 户外划船 */
        case SPORT_GROUP_PARASAILING:           /* 滑翔伞 */
        case SPORT_GROUP_DRIVING:               /* 驾车 */
        {
             /* 距离 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_DISTANCE, _voice_play_data.distance, 0, 0);
            /* 平均速度 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGESPEED, _voice_play_data.speed_avg, 0, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_INDOOR_CYCLING:        /* 室内骑行 */
        case SPORT_GROUP_2:                     /* 类型2 */
        case SPORT_GROUP_3:                     /* 类型3 */
        {
             /* 消耗 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_CONSUME, 0.0, _voice_play_data.consume , 0);
            /* 平均心率 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGEHR, 0.0, _voice_play_data.hr_avg, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_STRENGTH_TRAINING:     /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:        /* 室内健身 */
        {
            /* 总组数 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_TOTALGROUPS, 0.0, _voice_play_data.totalGroups, 0);
            /* 消耗 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_CONSUME, 0.0, _voice_play_data.consume , 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_POOL_SWIMMING:         /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING:   /* 公开水域游泳 */
        {
             /* 距离 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_DISTANCE_M, _voice_play_data.distance, 0, 0);
            /* 平均配速 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGEPACE, 0.0, _voice_play_data.pace_avg, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
             /* 主泳姿 */
            //sportRecordVoicePlay(SPORT_VOICE_RECORD_MAINSWIMSTYLE, 0.0, 0, _voice_play_data.swim_main_type == 0 ? SPORT_SWIM_TYPE_UNKNOW : _voice_play_data.swim_main_type);
            break;
        }
        case SPORT_GROUP_MOUNTAINEERING:        /* 登山 */
        {
            /* 距离 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_DISTANCE, _voice_play_data.distance, 0, 0);
            /* 平均速度 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGESPEED, _voice_play_data.speed_avg, 0, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            /* 累计上升 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_HASRISEN, 0.0, _voice_play_data.altitude_total_up, 0);
            /* 累计下降 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_HASDECLINE, 0.0, _voice_play_data.altitude_total_down, 0);
            break;
        }
        case SPORT_GROUP_SKIING:                /* 滑雪 */
        {
            /* 累计滑降距离 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_HASDOWNHILLDIS, 0.0, _voice_play_data.glide_total_distance, 0);
            /* 平均速度 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGESPEED, _voice_play_data.speed_avg, 0, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            /* 滑降次数 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_DOWNHILLTIMES, 0.0, _voice_play_data.glide_count, 0);
            break;
        }
        case SPORT_GROUP_ROWING_MACHINE:        /* 划船机 */
        {
            /* 距离 */
            //sportRecordVoicePlay(SPORT_VOICE_RECORD_DISTANCE, _voice_play_data.distance, 0, 0);
            /* 平均划频 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGEROWFREQ, 0.0, _voice_play_data.boating_avg_freq, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_TENNIS:                /* 网球 */
        {
            /* 总挥拍次数 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_TOTALSWINGTIMES, 0.0, _voice_play_data.brandish_total_cnt, 0);
            /* 消耗 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_CONSUME, 0.0, _voice_play_data.consume , 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_ROPE_SKIPPING:         /* 跳绳 */
        {
            /* 总计数 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_TOTALCOUNT, 0.0, _voice_play_data.count_total, 0);
            /* 平均频率 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_AVERAGEFREQ, 0.0, _voice_play_data.freq_avg, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_TRIATHLON:             /* 铁人三项 （不支持实时数据配置） */
        {
            /* 距离 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_DISTANCE, _voice_play_data.distance, 0, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            /* 游泳用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SWIMTIME, 0.0, _voice_play_data.swimmingTime, 0);
            /* 骑行用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_CYCLETIME, 0.0, _voice_play_data.cyclingTime, 0);
            /* 跑步用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_RUNTIME, 0.0, _voice_play_data.runningTime, 0);
            break;
        }
        case SPORT_GROUP_MULTISPORT:            /* 复合运动 （暂不支持） */
            break;
        case SPORT_GROUP_STAIR_CLIMBING:        /* 爬楼 */
        {
            /* 上楼层数 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_UPFLOORNUM, 0.0, _voice_play_data.climb_up_floors, 0);
            /* 平均每分钟上楼层数 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_UPFLOORPERMIN, _voice_play_data.climb_up_floors_in_min, 0, 0);
            /* 上楼高度 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_UPFLOORHEIGHT, 0.0, _voice_play_data.climb_up_total_altitude, 0);
            /* 运动用时 */
            sportRecordVoicePlay(SPORT_VOICE_RECORD_SPORTTIME, 0.0, _voice_play_data.exerciseTime, 0);
            break;
        }
        case SPORT_GROUP_4:                     /* 类型4 */
        case SPORT_GROUP_5:                     /* 类型5 */
        case SPORT_GROUP_6:                     /* 类型6 */
        case SPORT_GROUP_GOLF:                  /* 高尔夫 */
        case SPORT_GROUP_GOLF_SWING:            /* 高尔夫挥杆 */
        default:
            break;
    }

}