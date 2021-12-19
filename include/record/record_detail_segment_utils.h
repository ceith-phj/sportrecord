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

#ifndef RECORD_DETAIL_SEGMENT_UTILS_H
#define RECORD_DETAIL_SEGMENT_UTILS_H
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "sport_group_auto.h"
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <gre/sdk/grelib.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include "sport_record_save.h"
#include "hlog.h"

#define ITEM_TITLE_PATH "SportRecordSegmentItemLayer.SportRecordSegmentItemTitle"
#define ITEM_SEGMENT_GROUP_PATH "SportRecordSegmentItemLayer.SportRecordSegmentItem"
#define SETHEMTN_TITLE_PATH "SportRecordSegmentItemLayer.SportRecordSegmentTitle"

#ifdef __cplusplus
extern "C"
{
#endif

const void RecordSegment_refSegmentDetailTitle(gr_application_t *app,
        uint32_t group, uint32_t segment, uint32_t lesson, uint16_t sport_type);
const void RecordSegment_setSegmentGroupDetailData(gr_application_t *app,
        uint32_t serial, uint16_t sport_type, const char *itemPath,
        lap_detail_data_t lap_detail_data_t);
const void RecordSegment_setLessonDetailData(gr_application_t *app,
        uint32_t serial, uint16_t sport_type, const char *itemPath,
        lap_detail_data_t lap_detail_data_t);

#ifdef __cplusplus
}
#endif

#endif