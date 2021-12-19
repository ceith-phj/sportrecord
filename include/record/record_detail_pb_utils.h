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

#ifndef RECORD_DETAIL_PB_UTILS_H
#define RECORD_DETAIL_PB_UTILS_H
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <gre/sdk/grelib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <packages/apps/sports/include/manager/record_detail_manager.h>
#include "sport_group_auto.h"
#include <sport_type_utils.h>
#include <record/record_unit_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _pb_show_data
{
    const char *hite;
    const char *value;
    const char *unit;
} pb_show_data;

const void RrcordPb_refPbData(gr_application_t *app,
                              sport_record_detail_pb_data  *sportRecordDetailPbData, uint16_t sporType);
#ifdef __cplusplus
}
#endif

#endif