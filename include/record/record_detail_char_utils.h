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

#ifndef RECORD_DETAIL_CHAR_UTILS_H
#define RECORD_DETAIL_CHAR_UTILS_H

#include "sport_engine_utils.h"
#include "sport_group_auto.h"
#include <string.h>
#include <stdio.h>
#include <hlog.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include "sport_record_service.h"
#include "sport_summary.pb.h"

#ifdef __cplusplus
extern "C"
{
#endif

const void RecordChar_GraphConfig(SportSummaryMessage sport_summary_message,
                                  int sportType, sport_Chart_data_t sportHrChartData,
                                  sport_Chart_data_t sportSpaceChartData, sport_Chart_data_t sportSpeedChartData,
                                  sport_Chart_data_t sportSwimChartData,
                                  sport_Chart_data_t sportAltutileChartData,
                                  sport_Chart_data_t sportRowChartData);

#ifdef __cplusplus
}
#endif

#endif