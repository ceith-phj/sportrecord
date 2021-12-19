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

#include <record/record_detail_pb_utils.h>
#include "system_locale.h"
#include <gre/sdk/grelib.h>
#define PB_FIRST_PAGE "SportRecordPBFirstLayer"
#define PB_SECOND_PAGE "SportRecordPBSecondLayer"
#define PB_THREE_PAGE "SportRecordPBThreeLayer"
#define PB_FOUR_PAGE "SportRecordPBFourLayer"
#define PB_FIVE_PAGE "SportRecordPBFiveLayer"
#define PB_SIX_PAGE "SportRecordPBSixLayer"
#define PB_SEVEB_PAGE "SportRecordPBSevenLayer"
#define TAG "record_detail_pb_utils"

static pb_show_data _pb_show_data = {0};
static void _RecordPb_refData(char *itemPath, SportPBInfoData sportPBInfoData,
                              uint16_t sporType);
static void _RecordPb_getPbShowData(SportPBInfoData sportPBInfoData,
                                    uint16_t sporType);
static char *_RecordPb_getTime(uint32_t time);
static char *_RecordPb_getName(uint8_t type, uint32_t value);
static char *_RecordPb_getRunDis(uint32_t value);
static char *_RecordPb_getAilDis(uint32_t value);
static void _RecordPb_setPageNum(char *itemPath, uint32_t page,
                                 uint32_t totlePage);

const void RrcordPb_refPbData(gr_application_t *app,
                              sport_record_detail_pb_data *sportRecordDetailPbData, uint16_t sporType)
{
    for (int i = 0; i < sportRecordDetailPbData->pbNum; i++)
    {
        switch (i)
        {
            case 0:
                _RecordPb_refData(PB_FIRST_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_FIRST_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            case 1:
                _RecordPb_refData(PB_SECOND_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_SECOND_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            case 2:
                _RecordPb_refData(PB_THREE_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_THREE_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            case 3:
                _RecordPb_refData(PB_FOUR_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_FOUR_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            case 4:
                _RecordPb_refData(PB_FIVE_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_FIVE_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            case 5:
                _RecordPb_refData(PB_SIX_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_SIX_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            case 6:
                _RecordPb_refData(PB_SEVEB_PAGE, sportRecordDetailPbData->sportPbInfoData[i],
                                  sporType);
                _RecordPb_setPageNum(PB_SEVEB_PAGE, i + 1, sportRecordDetailPbData->pbNum);
                break;

            default:
                break;
        }
    }
}
static void _RecordPb_refData(char *itemPath, SportPBInfoData sportPBInfoData,
                              uint16_t sporType)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbhite.hite", itemPath);
    setSBStringData(getAppInstance(), buffer, sportType_getTypeNameChar(sporType));
    memset(&_pb_show_data, 0, sizeof(pb_show_data));
    _RecordPb_getPbShowData(sportPBInfoData, sporType);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbSportName.sportName", itemPath);
    setSBStringData(getAppInstance(), buffer, _pb_show_data.hite);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbValue.value", itemPath);
    setSBStringData(getAppInstance(), buffer, _pb_show_data.value);
    if(_pb_show_data.unit && *_pb_show_data.unit != '\0'){
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbValue", itemPath);
        H_Int32 titleWidth = gre_get_control_w(getAppInstance(),buffer);
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbValue.fontSize", itemPath);
        H_Int32 fontSize = getSBIntData(getAppInstance(),buffer);
        gr_string_size_t sz = {0};
        gre_get_string_size(getAppInstance(),_pb_show_data.value,
                            "fonts/huamifont.ttf", fontSize, titleWidth, &sz, false, 100);
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbValueUnit", itemPath);
        int16_t x = (STORYBOARD_DISPLAY_WIDTH / 2) + (sz.width / 2) + 10;
        gre_set_control_x(getAppInstance(),buffer,x);
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbValueUnit.pbValueUnit", itemPath);
        setSBStringData(getAppInstance(), buffer, _pb_show_data.unit);
    }
}

static void _RecordPb_setPageNum(char *itemPath, uint32_t page,
                                 uint32_t totlePage)
{
    char buffer[256] = {0};
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "(%d / %d)", (int)page, (int)totlePage);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.PBGroup.pbPageNum.pageNum", itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

static void _RecordPb_getPbShowData(SportPBInfoData sportPBInfoData,
                                    uint16_t sporType)
{
    sport_type_t sportTypet = (sport_type_t)sporType;
    sport_group_t group = sportGroup_get(sportTypet);
    HLOG_I(TAG,
           "%s type:%d name:%d reserverd:%d trackId:%d value:%d timestamp:%d \r\n",
           __func__, sportPBInfoData.type, sportPBInfoData.name, sportPBInfoData.reserverd,
           sportPBInfoData.trackId, sportPBInfoData.value, sportPBInfoData.timestamp);

    if (group == SPORT_GROUP_OUTDOOR_RUNNING)
    {
        switch (sportPBInfoData.name)
        {
            case SPORT_PB_RUN_3_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 3);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RUN_5_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 5);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RUN_10_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 10);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RUN_3_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 3);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RUN_5_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 5);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RUN_10_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 10);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RUN_LONGEST_DISTANCE:
            {
                _pb_show_data.hite =
                    System_getLocaleText("id_sportrecord_pb_farthest_distance");
                _pb_show_data.value = _RecordPb_getRunDis(sportPBInfoData.value);
                _pb_show_data.unit = getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_pb_uint_km");
                break;
            }

            case SPORT_PB_RUN_LONGEST_TIME:
            {
                _pb_show_data.hite = System_getLocaleText("id_sportrecord_pb_longest_time");
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_HALF_MARATHON:
            {
                _pb_show_data.hite = System_getLocaleText("id_sportrecord_pb_half_code");
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_FULL_MARATHON:
            {
                _pb_show_data.hite = System_getLocaleText("id_sportrecord_pb_full_code");
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            default:
                break;
        }
    }

    if (group == SPORT_GROUP_OUTDOOR_CYCLING)
    {
        switch (sportPBInfoData.name)
        {
            case SPORT_PB_RIDE_5_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 5);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_10_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 10);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_20_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 20);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_40_KM:
            {
                _pb_show_data.hite = _RecordPb_getName(1, 40);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_5_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 5);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_10_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 10);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_20_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 20);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_40_MI:
            {
                _pb_show_data.hite = _RecordPb_getName(2, 40);
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_LONGEST_DISTANCE:
            {
                _pb_show_data.hite =
                    System_getLocaleText("id_sportrecord_pb_farthest_distance");
                _pb_show_data.value = _RecordPb_getRunDis(sportPBInfoData.value);
                _pb_show_data.unit = getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_pb_uint_km");
                break;
            }

            case SPORT_PB_RIDE_LONGEST_TIME:
            {
                _pb_show_data.hite = System_getLocaleText("id_sportrecord_pb_longest_time");
                _pb_show_data.value = _RecordPb_getTime(sportPBInfoData.value);
                break;
            }

            case SPORT_PB_RIDE_MAX_CLIMB:
            {
                _pb_show_data.hite = System_getLocaleText("id_sportrecord_pb_maximum_rise");
                _pb_show_data.value = _RecordPb_getAilDis(sportPBInfoData.value);
                _pb_show_data.unit =  getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_data_centi_unit");
                break;
            }

            default:
                break;
        }
    }

    if (group == SPORT_GROUP_MOUNTAINEERING)
    {
        switch (sportPBInfoData.name)
        {
            case SPORT_PB_MOUNTAIN_HIGHEST_ALTITUDE:
            {
                _pb_show_data.hite = System_getLocaleText("id_sportrecord_pb_highest_altitude");
                _pb_show_data.value = _RecordPb_getAilDis(sportPBInfoData.value);
                _pb_show_data.unit =  getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_data_centi_unit");
                break;
            }

            case SPORT_PB_MOUNTAIN_CUMULATIVE_RISE:
            {
                _pb_show_data.hite =
                    System_getLocaleText("id_sportrecord_pb_maximum_cumulative_rise");
                _pb_show_data.value = _RecordPb_getAilDis(sportPBInfoData.value);
                _pb_show_data.unit =  getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_data_centi_unit");
                break;
            }

            case SPORT_PB_MOUNTAIN_CUMULATIVE_DECLINE:
            {
                _pb_show_data.hite =
                    System_getLocaleText("id_sportrecord_pb_maximum_cumulative_decline");
                _pb_show_data.value = _RecordPb_getAilDis(sportPBInfoData.value);
                _pb_show_data.unit =  getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_data_centi_unit");
                break;
            }

            case SPORT_PB_MOUNTAIN_CUMULATIVE_CLIMB:
            {
                _pb_show_data.hite =
                    System_getLocaleText("id_sportrecord_pb_maximum_cumulative_climb");
                _pb_show_data.value = _RecordPb_getAilDis(sportPBInfoData.value);
                _pb_show_data.unit =  getMileageUnit() == 1 ? System_getLocaleText("id_sportrecord_pb_uint_mile") : System_getLocaleText("id_sportrecord_data_centi_unit");
                break;
            }

            default:
                break;
        }
    }
}

static char *_RecordPb_getTime(uint32_t time)
{
    static char strff[256];
    memset(strff, 0, sizeof(strff));
    int hour = time / 3600;
    int minute = (time - hour * 3600) / 60;
    int sec = (time - hour * 3600 - minute * 60);

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

    return strff;
}

static char *_RecordPb_getName(uint8_t type, uint32_t value)
{
    static char strff[256];

    if (type == 1)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]),
                 System_getLocaleText("id_sportrecord_pb_km_hite"), value);
    }
    else
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]),
                 System_getLocaleText("id_sportrecord_pb_mile_hite"), value);
    }

    return strff;
}

static char *_RecordPb_getRunDis(uint32_t value)
{
    float showDistance = RecordMile_to_km(value / 1000.0f);
    static char strff[256];
    snprintf(strff, sizeof(strff), "%0.2f", showDistance);
    return strff;
}

static char *_RecordPb_getAilDis(uint32_t value)
{
    static char strff[256];
    snprintf(strff, sizeof(strff), "%d", (int)value);
    return strff;
}