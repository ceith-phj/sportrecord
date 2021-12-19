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

#ifndef RECORD_DETAIL_FB_UTILS_H
#define RECORD_DETAIL_FB_UTILS_H

#include "stdint.h"
#include "sport_summary.pb.h"
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <stdio.h>
#include <hlog.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include "sport_record_service.h"
#include "sport_quantification_fb_service.h"
#include "core/display_metrics.h"
#include <gre/sdk/grelib.h>
#include "hm_time.h"
#include <record/record_text_aglin_utils.h>
#include "sport_effect.pb.h"
#include "sport_effect.h"

#ifdef __cplusplus
extern "C"
{
#endif

const void FB_refTe(gr_application_t *app, char *itemPath, float aerobic,
                    float anAerobic, float EPOC);
const void FB_refMaxOv2(gr_application_t *app, char *itemPath,
                        float Vo2MaxValue, float Vo2MaxTime, float Vo2MaxStatus);
const void FB_refRecoveryTime2(gr_application_t *app, char *itemPath,
                               uint16_t RecoveryTime, uint32_t finishTime);
const void FB_refTrainingLoad(gr_application_t *app, char *itemPath,
                              float vlaue, uint16_t minValue, uint16_t maxValue, uint16_t bigMaxValue);
const uint32_t _sport_calculate_recovery_time(uint32_t recv_time,
        uint32_t finish_time);
#ifdef __cplusplus
}
#endif

#endif