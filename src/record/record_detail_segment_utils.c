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

#include <record/record_detail_segment_utils.h>
#include <record/record_unit_utils.h>
#include <record/record_text_aglin_utils.h>
#define TAG "record_detail_segemnt_utils"

#define SEGMENT_VALUE_LENGTH 128

static void _RecordSegment_refSegmentDetailHite(uint16_t sport_type);
static void _RecordSegment_setSegmentDetailHite(const char *firstValue,
        const char *secondValue, const char *threeValue);
static void _RecordSegment_setSegmentGroupValue(char *serio, char *firstValue,
        char *secondValue, char *threeValue, const char *itemPath);
static void _RecordSegment_setLessonValue(char *serio, char *firstValue,
        char *secondValue, char *threeValue, const char *itemPath);
static void _RecordSegment_setTime(char *strff, uint32_t time);
static void _RecordSegment_setFloat(char *strff, float dis);
static void _RecordSegemnt_setPace(char *strff, float pace);
static void _RecordSegemnt_setInt(char *strff, int value);
static void _RecordSegmnt_setNum(char *strff, int value, uint8_t lessonType);
static void _RecordSegment_setString(char *strff, char *buffer);

const void RecordSegment_refSegmentDetailTitle(gr_application_t *app,
        uint32_t group, uint32_t segment, uint32_t lesson, uint16_t sport_type)
{
    char buffer[256] = {0};

    if (group > 0)
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.item.title", SETHEMTN_TITLE_PATH);
        set_text_aglin(buffer,
                       System_getLocaleText("id_sportrecord_Segment_Group_title"),
                       "fonts/allfont-Medium.ttf", 40);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_Segment_Group_title"));
    }
    else if (segment > 0)
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.item.title", SETHEMTN_TITLE_PATH);
        set_text_aglin(buffer,
                       System_getLocaleText("id_sportrecord_Segment_Segment_title"),
                       "fonts/allfont-Medium.ttf", 40);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_Segment_Segment_title"));
    }
    else if (lesson > 0)
    {
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.item.title", SETHEMTN_TITLE_PATH);
        set_text_aglin(buffer,
                       System_getLocaleText("id_sportrecord_Segment_Lesson_title"),
                       "fonts/allfont-Medium.ttf", 40);
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportrecord_Segment_Lesson_title"));
    }

    _RecordSegment_refSegmentDetailHite(sport_type);
}

const void RecordSegment_setSegmentGroupDetailData(gr_application_t *app,
        uint32_t serial, uint16_t sport_type, const char *itemPath,
        lap_detail_data_t lap_detail_data_t)
{
    sport_type_t sportTypet = (sport_type_t)sport_type;
    sport_group_t group = sportGroup_get(sportTypet);
    char firstValue[SEGMENT_VALUE_LENGTH] = {0};
    char secondValue[SEGMENT_VALUE_LENGTH] = {0};
    char threeValue[SEGMENT_VALUE_LENGTH] = {0};
    char serials[SEGMENT_VALUE_LENGTH] = {0};

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        case SPORT_GROUP_TRAIL_RUNNING:   /* 越野跑 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegment_setFloat(secondValue,
                                    RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance / 1000.0f));
            _RecordSegemnt_setPace(threeValue,
                                   RecordKm_to_mile_pace(lap_detail_data_t.lapData.pace));
            break;
        }

        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_OUTDOOR_ROWING:       /* 户外划船 */
        case SPORT_GROUP_PARASAILING:          /* 滑翔伞 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegment_setFloat(secondValue,
                                    RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance / 1000.0f));

            if (lap_detail_data_t.lapData.pace > 0)
            {
                _RecordSegment_setFloat(threeValue,
                                        RecordKm_to_Mile(3600.0f / (float)lap_detail_data_t.lapData.pace));
            }
            else
            {
                _RecordSegment_setFloat(threeValue, 0.0f);
            }

            break;
        }

        case SPORT_GROUP_SOCCER:  /* 足球 */
        {
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_3:                 /* 类型3 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);

            if (lap_detail_data_t.lapData.avg_hr == 0)
            {
                _RecordSegment_setString(secondValue, "--");
            }
            else
            {
                _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.avg_hr);
            }

            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.cal);
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.skip_count);
            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.skip_fre);
            break;
        }

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.stair_floor);
            _RecordSegment_setFloat(threeValue, lap_detail_data_t.lapData.floor_per_min);
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue,
                                  RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance));
            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.avgStrokes);
            break;
        }

        case SPORT_GROUP_SKIING: /* 滑雪 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegment_setFloat(secondValue,
                                    RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance / 1000.0f));

            if (lap_detail_data_t.lapData.maxPace > 0)
            {
                _RecordSegment_setFloat(threeValue,
                                        RecordKm_to_Mile(3600.0f / (float)lap_detail_data_t.lapData.maxPace));
            }
            else
            {
                _RecordSegment_setFloat(threeValue, 0.0f);
            }

            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.row_count);
            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.row_fre);
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
    memset(serials,0,sizeof(serials));
    _RecordSegemnt_setInt(serials, serial + 1);
    memset(serials,0,sizeof(serials));
    _RecordSegment_setSegmentGroupValue(serials, firstValue, secondValue,
                                        threeValue, itemPath);
}

const void RecordSegment_setLessonDetailData(gr_application_t *app,
        uint32_t serial, uint16_t sport_type, const char *itemPath,
        lap_detail_data_t lap_detail_data_t)
{
    sport_type_t sportTypet = (sport_type_t)sport_type;
    sport_group_t group = sportGroup_get(sportTypet);
    char firstValue[SEGMENT_VALUE_LENGTH] = {0};
    char secondValue[SEGMENT_VALUE_LENGTH] = {0};
    char threeValue[SEGMENT_VALUE_LENGTH] = {0};
    char serials[SEGMENT_VALUE_LENGTH] = {0};

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        case SPORT_GROUP_TRAIL_RUNNING:   /* 越野跑 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegment_setFloat(secondValue,
                                    RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance / 1000.0f));
            _RecordSegemnt_setPace(threeValue,
                                   RecordKm_to_mile_pace(lap_detail_data_t.lapData.pace));
            break;
        }

        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_OUTDOOR_ROWING:       /* 户外划船 */
        case SPORT_GROUP_PARASAILING:          /* 滑翔伞 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegment_setFloat(secondValue,
                                    RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance / 1000.0f));

            if (lap_detail_data_t.lapData.pace > 0)
            {
                _RecordSegment_setFloat(threeValue,
                                        RecordKm_to_Mile(1 / lap_detail_data_t.lapData.pace * 3.6));
            }
            else
            {
                _RecordSegment_setFloat(threeValue, 0.0f);
            }

            break;
        }

        case SPORT_GROUP_SOCCER:  /* 足球 */
        {
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_3:                 /* 类型3 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);

            if (lap_detail_data_t.lapData.avg_hr == 0)
            {
                _RecordSegment_setString(secondValue, "--");
            }
            else
            {
                _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.avg_hr);
            }

            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.cal);
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.skip_count);
            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.skip_fre);
            break;
        }

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.stair_floor);
            _RecordSegment_setFloat(threeValue, lap_detail_data_t.lapData.floor_per_min);
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue,
                                  RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance));
            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.avgStrokes);
            break;
        }

        case SPORT_GROUP_SKIING: /* 滑雪 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegment_setFloat(secondValue,
                                    RecordKm_to_Mile(lap_detail_data_t.lapData.lab_distance / 1000.0f));

            if (lap_detail_data_t.lapData.maxPace > 0)
            {
                _RecordSegment_setFloat(threeValue,
                                        RecordKm_to_Mile(3600.0f / (float)lap_detail_data_t.lapData.maxPace));
            }
            else
            {
                _RecordSegment_setFloat(threeValue, 0.0f);
            }

            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        {
            _RecordSegment_setTime(firstValue, lap_detail_data_t.lapData.lab_time);
            _RecordSegemnt_setInt(secondValue, lap_detail_data_t.lapData.row_count);
            _RecordSegemnt_setInt(threeValue, lap_detail_data_t.lapData.row_fre);
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
    memset(serials,0,sizeof(serials));
    _RecordSegmnt_setNum(serials, serial + 1,
                         lap_detail_data_t.lapData.training_group);
    memset(serials,0,sizeof(serials));
    _RecordSegment_setLessonValue(serials, firstValue, secondValue, threeValue,
                                  itemPath);
}
static void _RecordSegment_refSegmentDetailHite(uint16_t sport_type)
{
    sport_type_t sportTypet = (sport_type_t)sport_type;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        case SPORT_GROUP_TRAIL_RUNNING:   /* 越野跑 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_distance"),
                System_getLocaleText("id_sportrecord_average_pace"));
            break;
        }

        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_OUTDOOR_ROWING:       /* 户外划船 */
        case SPORT_GROUP_PARASAILING:          /* 滑翔伞 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_distance"),
                System_getLocaleText("id_sportrecord_average_velocity"));
            break;
        }

        case SPORT_GROUP_SOCCER:  /* 足球 */
        {
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_3:                 /* 类型3 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_average_heart_rate"),
                System_getLocaleText("id_sportrecord_consumption"));
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_total_number"),
                System_getLocaleText("id_sportrecord_average_frequency"));
            break;
        }

        case SPORT_GROUP_STAIR_CLIMBING: /* 爬楼 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_floors"),
                System_getLocaleText("id_sportrecord_average_floors"));
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_distance"),
                System_getLocaleText("id_rt_swim_stroke_speed"));
            break;
        }

        case SPORT_GROUP_SKIING: /* 滑雪 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_glide_distance"),
                System_getLocaleText("id_sportrecord_Maximum_speed"));
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        {
            _RecordSegment_setSegmentDetailHite(
                System_getLocaleText("id_sportrecord_Segment_Second_Time"),
                System_getLocaleText("id_sportrecord_segment_row"),
                System_getLocaleText("id_sportrecord_segment_stroke_frequency"));
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
}
static void _RecordSegment_setSegmentDetailHite(const char *firstValue,
        const char *secondValue, const char *threeValue)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.firsthite", ITEM_TITLE_PATH);
    setSBStringData(getAppInstance(), buffer, firstValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.secondhite", ITEM_TITLE_PATH);
    setSBStringData(getAppInstance(), buffer, secondValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.threehite", ITEM_TITLE_PATH);
    setSBStringData(getAppInstance(), buffer, threeValue);
}
static void _RecordSegment_setSegmentGroupValue(char *serio, char *firstValue,
        char *secondValue, char *threeValue, const char *itemPath)
{
    HLOG_I(TAG,
           "segment %s serio:%s firstValue:%s secondValue:%s threeValue:%s itemPath:%s\r\n",
           __func__, serio, firstValue, secondValue, threeValue, itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.serial", itemPath);
    setSBStringData(getAppInstance(), buffer, serio);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.firstvalue", itemPath);
    setSBStringData(getAppInstance(), buffer, firstValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.secondvalue", itemPath);
    setSBStringData(getAppInstance(), buffer, secondValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.threevalue", itemPath);
    setSBStringData(getAppInstance(), buffer, threeValue);
}
static void _RecordSegment_setLessonValue(char *serio, char *firstValue,
        char *secondValue, char *threeValue, const char *itemPath)
{
    HLOG_I(TAG,
           "lesson %s serio:%s firstValue:%s secondValue:%s threeValue:%s itemPath:%s\r\n",
           __func__, serio, firstValue, secondValue, threeValue, itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.num", itemPath);
    setSBStringData(getAppInstance(), buffer, serio);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.firstvalue", itemPath);
    setSBStringData(getAppInstance(), buffer, firstValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.secondvalue", itemPath);
    setSBStringData(getAppInstance(), buffer, secondValue);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.threevalue", itemPath);
    setSBStringData(getAppInstance(), buffer, threeValue);
}
static void _RecordSegmnt_setNum(char *strff, int value, uint8_t lessonType)
{
    TRAINING_GROUP_TYPE_E lensson_type = (TRAINING_GROUP_TYPE_E)lessonType;

    switch (lensson_type)
    {
        case SPORT_TRAINING_GROUP_NONE:
            snprintf(strff, SEGMENT_VALUE_LENGTH,
                     System_getLocaleText("id_sportrecord_Lesson_type_nono"), value);
            break;

        case SPORT_TRAINING_GROUP_WARM_UP:
            snprintf(strff, SEGMENT_VALUE_LENGTH,
                     System_getLocaleText("id_sportrecord_Lesson_type_warm_up"), value);
            break;

        case SPORT_TRAINING_GROUP_TRAINING:
            snprintf(strff, SEGMENT_VALUE_LENGTH,
                     System_getLocaleText("id_sportrecord_Lesson_type_training"), value);
            break;

        case SPORT_TRAINING_GROUP_REST:
            snprintf(strff, SEGMENT_VALUE_LENGTH,
                     System_getLocaleText("id_sportrecord_Lesson_type_rest"), value);
            break;

        case SPORT_TRAINING_GROUP_RELAX:
            snprintf(strff, SEGMENT_VALUE_LENGTH,
                     System_getLocaleText("id_sportrecord_Lesson_type_relax"), value);
            break;

        case SPORT_TRAINING_GROUP_RECOVER:
            snprintf(strff, SEGMENT_VALUE_LENGTH,
                     System_getLocaleText("id_sportrecord_Lesson_type_restore"), value);
            break;

        default:
            break;
    }
}
static void _RecordSegment_setTime(char *strff, uint32_t time)
{
    int exercisetime = time / 1000;
    int hour = exercisetime / 3600;
    int minute = (exercisetime - hour * 3600) / 60;
    int sec = (exercisetime - hour * 3600 - minute * 60);

    if (hour == 0 && minute == 0)
    {
        snprintf(strff, SEGMENT_VALUE_LENGTH, "0:%02d", sec);
    }
    else if (hour == 0 && minute > 0)
    {
        snprintf(strff, SEGMENT_VALUE_LENGTH, "%d:%02d", minute, sec);
    }
    else if (hour > 0)
    {
        snprintf(strff, SEGMENT_VALUE_LENGTH, "%d:%02d:%02d", hour, minute, sec);
    }
}
static void _RecordSegment_setFloat(char *strff, float dis)
{
    snprintf(strff, SEGMENT_VALUE_LENGTH, "%0.2f", dis);
}
static void _RecordSegemnt_setPace(char *strff, float pace)
{
    int min = pace / 60;
    int sec = (int)pace % 60;

    if (min != 0 || sec != 0)
    {
        snprintf(strff, SEGMENT_VALUE_LENGTH, "%d\'%02d", min, sec);
    }
    else
    {
        snprintf(strff, SEGMENT_VALUE_LENGTH, "%s", "--");
    }
}
static void _RecordSegemnt_setInt(char *strff, int value)
{
    snprintf(strff, SEGMENT_VALUE_LENGTH, "%d", (int)value);
}
static void _RecordSegment_setString(char *strff, char *buffer)
{
    snprintf(strff, SEGMENT_VALUE_LENGTH, "%s", buffer);
}