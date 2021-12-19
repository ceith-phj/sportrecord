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

#ifndef RECORD_UNIT_UTILS__H
#define RECORD_UNIT_UTILS__H

#include "sport_group_auto.h"
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include "sport_engine_utils.h"
#include <hlog.h>
#include <string.h>
#include <stdio.h>
#include "hm_time.h"
#include "settings_keys_map.h"
#include "system_property.h"
#include <packages/services/syssevice/manager/system_locale.h>
#include <framework/base/core/time/datetime_format.h>
#include "sport_setting.h"
#include <record/record_detail_swim_dis.h>

#ifdef __cplusplus
extern "C"
{
#endif

const void RecordUnit_getRecordMainDataUnit(int sportType, char buffer[],
        uint8_t unitType);
const void RecordUnit_getRecordMainDataUnit1(int sportType, char *unit,
        int charLength, uint8_t unitType);
const void RecordData_getRecordMainData(int sportType, float data,
                                        char buffer[], uint8_t unit);
const void RecordData_getRecordMainData1(int sportType, float data,
        char buffer[],uint8_t unit);
const void RecordExercisetime_getRecordExercisetime(uint32_t exercisetime,
        char buffer[]);
const void RecordTime_getRecordTime(uint32_t time, char buffer[]);
const float RecordKm_to_Mile(float num);
const float RecordMeter_to_foot(float num);
const float RecordCm_to_inch(float num);
const float RecordMeter_to_yard(float num);
const float Recordm_to_mile(float num);
const float RecordMile_to_km(float num);
const float RecordKm_to_mile_pace(float num);
const float RecordHm_to_hy_pace(float num);
const H_Int32 getMileageUnit();

#ifdef __cplusplus
}
#endif

#endif