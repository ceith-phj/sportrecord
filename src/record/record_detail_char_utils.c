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

#include <record/record_detail_char_utils.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#define TAG "record_detail_char_utils"

#define CHAR_HEART_RATE_COLOR 0xC42742
#define CHAR_SPEED_COLOR 0xDFA032
#define CHAR_PACE_COLOR 0xDFA032
#define CHAR_STROKE_FREQUENCY_COLOR 0x17C06E
#define CHAR_STROKE_RATE_COLOR 0x17C06E
#define CHAR_ALTITUDE_COLOR 0x119E9A

static void _RecordChar_HeartRateGraphData(char *itemPath,
        sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message);
static void _RecordChar_PaceGraphData(char *itemPath,
                                      sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message);
static void _RecordChar_SpeedGraphData(char *itemPath,
                                       sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message);
static void _RecordChar_AltitudeGraphData(char *itemPath,
        sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message);
static void _RecordChar_StrokeFrequencyData(char *itemPath,
        sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message);
static void _RecordChar_StrokeRateData(char *itemPath,
                                       sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message);
static void _RecordChar_Title(char *itemPath, const char *titleName);
static void _RecordChar_Unit(char *itemPath, const char *uintName);
static void _RecordChar_StartColor(char *itemPath, uint32_t startcolor);
static void _RecordChar_EndColor(char *itemPath, uint32_t endColor);
static void _RecordChar_SetUpdateY(char *itemPath,
                                   SportRecord_ChartType_e char_type, float maxValue, float minValue);
static void _RecordChar_SetShowIntData(char *itemPath, uint32_t avgValue,
                                       uint32_t maxValue, uint32_t minValue);
static void _RecordChar_SetShowFloatData(char *itemPath,
        SportRecord_ChartType_e char_type, float avgValue, float maxValue,
        float minValue);
// static void _RecordChar_paceImperial2MinuteSec(float pace, int *minute,int *sec);
static void _RecordChar_pace2MinuteSec(float pace, int *minute, int *sec);

const char *showCharLayers[] =
{
    "SportRecordCharFirstLayer",
    "SportRecordCharSecondLayer"
};

const void RecordChar_GraphConfig(SportSummaryMessage sport_summary_message,
                                  int sportType, sport_Chart_data_t sportHrChartData,
                                  sport_Chart_data_t sportSpaceChartData, sport_Chart_data_t sportSpeedChartData,
                                  sport_Chart_data_t sportSwimChartData,
                                  sport_Chart_data_t sportAltutileChartData, sport_Chart_data_t sportRowChartData)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);
    char buff[51];
    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[0], sizeof(buff));
            _RecordChar_HeartRateGraphData(buff, sportHrChartData, sport_summary_message);
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[1], sizeof(buff));
            _RecordChar_PaceGraphData(buff, sportSpaceChartData, sport_summary_message);
            break;
        }
        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_SOCCER:               /* 足球 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[0], sizeof(buff));
            _RecordChar_HeartRateGraphData(buff, sportHrChartData, sport_summary_message);
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[1], sizeof(buff));
            _RecordChar_SpeedGraphData(buff, sportSpeedChartData, sport_summary_message);
            break;
        }
        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_3:                 /* 类型3 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        case SPORT_GROUP_ROPE_SKIPPING:     /* 跳绳 */
        case SPORT_GROUP_STAIR_CLIMBING:    /* 爬楼 */
        {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[0], sizeof(buff));
            _RecordChar_HeartRateGraphData(buff, sportHrChartData, sport_summary_message);
            break;
        }
        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[0], sizeof(buff));
            _RecordChar_StrokeRateData(buff, sportSwimChartData, sport_summary_message);
            break;
        }
        case SPORT_GROUP_MOUNTAINEERING: /* 登山 */
        case SPORT_GROUP_TRAIL_RUNNING:  /* 越野跑 */
        case SPORT_GROUP_SKIING:         /* 滑雪 */
        case SPORT_GROUP_PARASAILING:    /* 滑翔伞 */
        {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[0], sizeof(buff));
            _RecordChar_HeartRateGraphData(buff, sportHrChartData, sport_summary_message);
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[1], sizeof(buff));
            _RecordChar_AltitudeGraphData(buff, sportAltutileChartData,
                                          sport_summary_message);
            break;
        }
        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[0], sizeof(buff));
            _RecordChar_HeartRateGraphData(buff, sportHrChartData, sport_summary_message);
            memset(buff, 0, sizeof(buff));
            strncpy(buff, showCharLayers[1], sizeof(buff));
            _RecordChar_StrokeFrequencyData(buff, sportRowChartData, sport_summary_message);
            break;
        }
        case SPORT_GROUP_TENNIS: /* 网球 */
            break;
        case SPORT_GROUP_TRIATHLON: /* 铁人三项 （不支持实时数据配置） */
            break;
        case SPORT_GROUP_MULTISPORT: /* 复合运动 （不支持实时数据配置） */
            break;
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
};

const void _RecordChar_HeartRateGraphData(char *itemPath,
        sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message)
{
    _RecordChar_Title(itemPath, System_getLocaleText("id_rt_chart_hr"));
    _RecordChar_Unit(itemPath, System_getLocaleText("id_rt_chart_hr_unit"));
    _RecordChar_StartColor(itemPath, CHAR_HEART_RATE_COLOR);
    _RecordChar_EndColor(itemPath, CHAR_HEART_RATE_COLOR);
    _RecordChar_SetUpdateY(itemPath, SPORT_RECORD_CHART_HR, sportChartData.dataMax,
                           sportChartData.dataMin);
    _RecordChar_SetShowIntData(itemPath,
                               sport_summary_message.heartRate.averageHeartRate,
                               sport_summary_message.heartRate.maximumHeartRate,
                               sport_summary_message.heartRate.minimumHeartRate);
};
const void _RecordChar_PaceGraphData(char *itemPath,
                                     sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message)
{
    _RecordChar_Title(itemPath, System_getLocaleText("id_rt_chart_pace"));
    _RecordChar_Unit(itemPath, System_getLocaleText("id_rt_chart_pace_unit"));
    _RecordChar_StartColor(itemPath, CHAR_PACE_COLOR);
    _RecordChar_EndColor(itemPath, CHAR_PACE_COLOR);
    _RecordChar_SetUpdateY(itemPath, SPORT_RECORD_CHART_PACE,
                           sportChartData.dataMax, sportChartData.dataMin);
    _RecordChar_SetShowFloatData(itemPath, SPORT_RECORD_CHART_PACE,
                                 sport_summary_message.pace.averagePace, sport_summary_message.pace.bestPace, 0);
};
const void _RecordChar_SpeedGraphData(char *itemPath,
                                      sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message)
{
    _RecordChar_Title(itemPath, System_getLocaleText("id_rt_chart_speed"));
    _RecordChar_Unit(itemPath, System_getLocaleText("id_rt_chart_speed_unit"));
    _RecordChar_StartColor(itemPath, CHAR_SPEED_COLOR);
    _RecordChar_EndColor(itemPath, CHAR_SPEED_COLOR);
    _RecordChar_SetUpdateY(itemPath, SPORT_RECORD_CHART_SPEED,
                           sportChartData.dataMax, sportChartData.dataMin);
    _RecordChar_SetShowFloatData(itemPath, SPORT_RECORD_CHART_PACE,
                                 sport_summary_message.pace.averagePace, sport_summary_message.pace.bestPace, 0);
};
const void _RecordChar_AltitudeGraphData(char *itemPath,
        sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message)
{
    _RecordChar_Title(itemPath, System_getLocaleText("id_rt_chart_altitude"));
    _RecordChar_Unit(itemPath, System_getLocaleText("id_rt_chart_altitude_unit"));
    _RecordChar_StartColor(itemPath, CHAR_ALTITUDE_COLOR);
    _RecordChar_EndColor(itemPath, CHAR_ALTITUDE_COLOR);
    _RecordChar_SetUpdateY(itemPath, SPORT_RECORD_CHART_ALTITUDE,
                           sportChartData.dataMax / 100, sportChartData.dataMin / 100);
    _RecordChar_SetShowIntData(itemPath,
                               sport_summary_message.altitude.averageAltitude / 100,
                               sport_summary_message.altitude.highestAltitude / 100,
                               sport_summary_message.altitude.lowestAltitude / 100);
};
const void _RecordChar_StrokeFrequencyData(char *itemPath,
        sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message)
{
    _RecordChar_Title(itemPath, System_getLocaleText("id_rt_chart_stroke_frep"));
    _RecordChar_Unit(itemPath,
                     System_getLocaleText("id_rt_chart_stroke_frep_unit"));
    _RecordChar_StartColor(itemPath, CHAR_STROKE_FREQUENCY_COLOR);
    _RecordChar_EndColor(itemPath, CHAR_STROKE_FREQUENCY_COLOR);
    _RecordChar_SetUpdateY(itemPath, SPORT_RECORD_CHART_ROW_STROKE_FREQ,
                           sportChartData.dataMax, sportChartData.dataMin);
    _RecordChar_SetShowIntData(itemPath,
                               sport_summary_message.frequency.averageFrequency,
                               sport_summary_message.frequency.highestFrequency, 0);
};
const void _RecordChar_StrokeRateData(char *itemPath,
                                      sport_Chart_data_t sportChartData, SportSummaryMessage sport_summary_message)
{
    _RecordChar_Title(itemPath, System_getLocaleText("id_rt_chart_stroke_speed"));
    _RecordChar_Unit(itemPath,
                     System_getLocaleText("id_rt_chart_stroke_speed_unit"));
    _RecordChar_StartColor(itemPath, CHAR_STROKE_RATE_COLOR);
    _RecordChar_EndColor(itemPath, CHAR_STROKE_RATE_COLOR);
    _RecordChar_SetUpdateY(itemPath, SPORT_RECORD_CHART_SWIM_STROKE_SPEED,
                           sportChartData.dataMax, sportChartData.dataMin);
    _RecordChar_SetShowIntData(itemPath,
                               sport_summary_message.swim.averageStrokeRate,
                               sport_summary_message.swim.maximumStrokeRate, 0);
};

static void _RecordChar_Title(char *itemPath, const char *titleName)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.charTitle", itemPath);
    setSBStringData(getAppInstance(), buffer, titleName);
};

static void _RecordChar_Unit(char *itemPath, const char *unitName)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.char_unit", itemPath);
    setSBStringData(getAppInstance(), buffer, unitName);
};

static void _RecordChar_StartColor(char *itemPath, uint32_t startcolor)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.CharDrowControl.startColor",
             itemPath);
    setSBIntData(getAppInstance(), buffer, startcolor);
};

static void _RecordChar_EndColor(char *itemPath, uint32_t endColor)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.CharDrowControl.endColor",
             itemPath);
    setSBIntData(getAppInstance(), buffer, endColor);
};

static void _RecordChar_SetUpdateY(char *itemPath,
                                   SportRecord_ChartType_e char_type, float maxValue, float minValue)
{
    char buffer[256] = {0};
    switch (char_type)
    {
        case SPORT_RECORD_CHART_HR:
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.minValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)minValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.maxValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)maxValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.avgValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)(maxValue + minValue) / 2.0f);
            break;
        }
        case SPORT_RECORD_CHART_PACE:
        {
            int minute_max, sec_max, minute_min, sec_min, minute_avg, sec_avg;
            ;
            float pace1 = minValue / 1000.0f;
            float pace3 = maxValue / 1000.0f;
            float pace2 = (minValue + maxValue) / 1000.0f / 2.0f;
            _RecordChar_pace2MinuteSec(pace1, &minute_max, &sec_max);
            _RecordChar_pace2MinuteSec(pace3, &minute_min, &sec_min);
            _RecordChar_pace2MinuteSec(pace2, &minute_avg, &sec_avg);
            char strff[256];
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d\'%02d", minute_max, sec_max);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.minValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d\'%02d", minute_min, sec_min);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.maxValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d\'%02d", minute_avg, sec_avg);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.avgValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }
        case SPORT_RECORD_CHART_SPEED:
        {
            float coeff = 1000.0f;
            // if (imperial)
            //     coeff = 1609.344f;
            if (maxValue > minValue)
            {
                float speed1 = maxValue / coeff;
                float speed3 = minValue / coeff;
                float speed2 = (2.0f * speed1 + speed3) / 2.0f;
                int ispeed1 = (int)((speed1 + 0.005f) * 100.0f);
                int v = ispeed1 % 10;
                if (v >= 1 && v < 5)
                {
                    ispeed1 -= v;
                    ispeed1 += 5;
                }
                else if (v >= 5 && v <= 9)
                {
                    ispeed1 -= v;
                    ispeed1 += 10;
                }
                speed1 = (float)ispeed1 / 100.0f;
                int ispeed3 = (int)((speed3 + 0.005f) * 100.0f);
                v = ispeed3 % 10;
                if (v >= 1 && v < 5)
                {
                    ispeed3 -= v;
                }
                else if (v >= 5 && v <= 9)
                {
                    ispeed3 -= v;
                    ispeed3 += 5;
                }
                speed3 = (float)ispeed3 / 100.0f;
                char tmpStrff[256];
                memset(tmpStrff, 0, sizeof(tmpStrff));
                snprintf(tmpStrff, sizeof(tmpStrff), "%.2f", speed3);
                memset(buffer, 0, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.minValue", itemPath);
                setSBStringData(getAppInstance(), buffer, tmpStrff);
                memset(tmpStrff, 0, sizeof(tmpStrff));
                snprintf(tmpStrff, sizeof(tmpStrff), "%.2f", speed1);
                memset(buffer, 0, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.maxValue", itemPath);
                setSBStringData(getAppInstance(), buffer, tmpStrff);
                memset(tmpStrff, 0, sizeof(tmpStrff));
                snprintf(tmpStrff, sizeof(tmpStrff), "%.2f", speed2);
                memset(buffer, 0, sizeof(buffer));
                snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.avgValue", itemPath);
                setSBStringData(getAppInstance(), buffer, tmpStrff);
                break;
            }
        }
        case SPORT_RECORD_CHART_ALTITUDE:
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.minValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)minValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.maxValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)maxValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.avgValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)(maxValue + minValue) / 2.0f);
            break;
        }
        case SPORT_RECORD_CHART_ROW_STROKE_FREQ:
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.minValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)minValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.maxValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)maxValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.avgValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)(maxValue + minValue) / 2.0f);
            break;
        }
        case SPORT_RECORD_CHART_SWIM_STROKE_SPEED:
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.minValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)minValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.maxValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)maxValue);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.avgValue", itemPath);
            setSBIntData(getAppInstance(), buffer, (int)(maxValue + minValue) / 2.0f);
            break;
        }
        default:
            break;
    }
};

static void _RecordChar_SetShowIntData(char *itemPath, uint32_t avgValue,
                                       uint32_t maxValue, uint32_t minValue)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.charValue", itemPath);
    setSBIntData(getAppInstance(), buffer, avgValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.upValue", itemPath);
    setSBIntData(getAppInstance(), buffer, maxValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.downValue", itemPath);
    setSBIntData(getAppInstance(), buffer, minValue);
};

static void _RecordChar_SetShowFloatData(char *itemPath,
        SportRecord_ChartType_e char_type, float avgValue, float maxValue,
        float minValue)
{
    char strff[256];
    char buffer[256] = {0};
    switch (char_type)
    {
        case SPORT_RECORD_CHART_PACE:
        {
            int minute_max, sec_max, minute_min, sec_min, minute_avg, sec_avg;
            ;
            float pace1 = minValue / 1000.0f;
            float pace3 = maxValue / 1000.0f;
            float pace2 = avgValue / 1000.0f;
            _RecordChar_pace2MinuteSec(pace1, &minute_max, &sec_max);
            _RecordChar_pace2MinuteSec(pace3, &minute_min, &sec_min);
            _RecordChar_pace2MinuteSec(pace2, &minute_avg, &sec_avg);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d\'%02d", minute_max, sec_max);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.upValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d\'%02d", minute_min, sec_min);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.downValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%d\'%02d", minute_avg, sec_avg);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.charValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }
        case SPORT_RECORD_CHART_SPEED:
        {
            float speed1 = sportEngine_utils()->pace2speedKmh(avgValue);
            float speed2 = sportEngine_utils()->pace2speedKmh(maxValue);
            float speed3 = sportEngine_utils()->pace2speedKmh(minValue);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%.2f", speed1);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.charValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%.2f", speed2);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.upValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%.2f", speed3);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s.CharGroup.item.downValue", itemPath);
            setSBStringData(getAppInstance(), buffer, strff);
            break;
        }
        default:
            break;
    }
};

// static void _RecordChar_paceImperial2MinuteSec(float pace, int *minute, int *sec){
//     *minute = 0;
//     *sec = 0;
//     if (pace > 0.0f) {
//         *minute = (int)((1609.344f * pace) / 60.0f);
//         *sec = (int)(1609.344f * pace - 60.0f * (*minute));
//     }
// }

static void _RecordChar_pace2MinuteSec(float pace, int *minute, int *sec)
{
    *minute = 0;
    *sec = 0;
    if (pace > 0.0f)
    {
        *minute = (int)((1000.0f * pace) / 60.0f);
        *sec = (int)(1000.0f * pace - 60.0f * (*minute));
    }
}