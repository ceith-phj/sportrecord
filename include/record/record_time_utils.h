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

#ifndef RECORD_TIME_UTILS_H
#define RECORD_TIME_UTILS_H

#include <stdint.h>
#include <hlog.h>
#include "hm_time.h"
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>

#ifdef __cplusplus
extern "C"
{
#endif

const void Time_getRecordTime(uint32_t time, char buffer[]);
const void Time_getExerciseTime(uint32_t exerciseTime, char buffer[]);

#ifdef __cplusplus
}
#endif

#endif