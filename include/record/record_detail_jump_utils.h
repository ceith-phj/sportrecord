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

#ifndef RECORD_DETAIL_JUMO_UTILS_H
#define RECORD_DETAIL_JUMO_UTILS_H
#include <stdint.h>
#include <manager/record_list_manager.h>
#include <record/record_parameter.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <hlog.h>

#ifdef __cplusplus
extern "C"
{
#endif
const void singleSportjumpDetail(uint32_t item_id, ScreenFlag flag,
                                 Record_Detail_Come_From recordDetailComeFrom);
const void singleSportjumpDetailUtiles(uint32_t item_id, ScreenFlag flag);
#ifdef __cplusplus
}
#endif

#endif