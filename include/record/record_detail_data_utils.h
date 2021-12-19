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

#ifndef RECORD_DETAIL_DATA_UTILS_H
#define RECORD_DETAIL_DATA_UTILS_H

#include "hm_time.h"
#include "hlog.h"
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <presenter/record_detail_presenter.h>
#include <record/record_unit_utils.h>
#include <framework/base/core/time/datetime_format.h>
#include <record/record_detail_swim_dis.h>
#include "packages/services/sport/voicebroad/sport_voice_service.h"

typedef struct _voice_play_data
{
    float distance;                     /* 距离 */
    uint32_t pace_avg;                  /* 平均配速 */
    float speed_avg;                    /* 平均速度 */
    uint32_t altitude_total_up;         /* 累计上升 */
    uint32_t altitude_total_down;       /* 累计下降 */
    float climb_up_floors_in_min;       /* 平均每分钟上楼层数 */
    uint32_t climb_up_total_altitude;   /* 上楼高度 */
    uint32_t climb_up_floors;           /* 上楼层数 */
    uint32_t hr_avg;                    /* 平均心率 */
    uint32_t consume;                   /* 消耗 */
    uint32_t glide_count;               /* 滑降次数 */
    uint32_t glide_total_distance;      /* 累计滑降距离 */
    uint32_t boating_avg_freq;          /* 平均划频 */
    uint32_t brandish_total_cnt;        /* 总挥拍次数 */
    uint32_t count_total;               /* 总计数 */
    uint32_t freq_avg;                  /* 平均频率 */
    uint32_t swimmingTime;              /* 游泳用时 */
    uint32_t cyclingTime;               /* 骑行用时 */
    uint32_t runningTime;               /* 跑步用时 */
    uint32_t totalGroups;               /* 总组数 */
    uint32_t exerciseTime;              /* 运动用时 */
    sport_swim_type_t swim_main_type;   /* 主泳姿 */
} voice_play_data;

#ifdef __cplusplus
extern "C"
{
#endif

const void RecordDate_getRecordDate(uint32_t time, char buffer[]);
const void RecordMain_getRecordMainValue(SportRecordDetailData
        sportRecordDetailData, int sportType, char *itemPath);
const void RecordFirst_getRecordFirstValue(SportRecordDetailData
        sportRecordDetailData, int sportType, char *itemPath);
const void RecordSecond_getRecordSecondValue(SportRecordDetailData
        sportRecordDetailData, int sportType, char *itemPath);
const void getRecordExerciseTime(uint32_t exerciseTime, const char *value,
                                 char *itemPath);
const void getRecordInt(uint32_t data, const char *value, char *itemPath);
const void getRecordFloat(float data, const char *value, char *itemPath);
const void getReordSwimFloat(float data, const char *value, char *itemPath);
const void getRecoedPace(float data, const char *value, char *itemPath);
const void getRecordSwimmingStyle(uint32_t data, const char *value,
                                  char *itemPath);
const void getInvalid(char *format, const char *value, char *itemPath);
const void getHeartInt(uint32_t data, const char *value, char *itemPath);
const void sportRecordVoicePlay(sport_voice_sport_record_type recordtype, float floatdata, uint32_t intdata, sport_swim_type_t swimdata);
const void sportEndRedordVoiceDataSet(uint16_t sport_type);
const float getFloatPoint2(float data);

#ifdef __cplusplus
}
#endif

#endif
