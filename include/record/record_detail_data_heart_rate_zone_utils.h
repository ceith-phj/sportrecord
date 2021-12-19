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

#ifndef RECORD_DETAIL_DATA_HEART_RATE_ZONE_UTILS_H
#define RECORD_DETAIL_DATA_HEART_RATE_ZONE_UTILS_H
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <presenter/record_detail_presenter.h>
#include <gre/sdk/grelib.h>
#include "hlog.h"

#ifdef __cplusplus
extern "C"
{
#endif

const void RecordHHZ_getRecordHHZValue(gr_application_t *app,
                                       SportRecordDetailData sportRecordDetailData, char *itemPath);

#ifdef __cplusplus
}
#endif
#endif