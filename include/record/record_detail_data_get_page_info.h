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

#ifndef RECORD_DETAIL_DATA_GET_PAGE_INFO_H
#define RECORD_DETAIL_DATA_GET_PAGE_INFO_H

#include "sport_group_auto.h"
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include <record/record_parameter.h>
#include <gre/sdk/grelib.h>
#include "hlog.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _record_detail_page_info
{
    /* data */
    int pageNumber;
    const char **showLayers;
} record_detail_page_info;

const record_detail_page_info RecordDetail_getPageInfo(int sportType);
struct _pageScrollerPage *RecordDetail_getHPageInfo(gr_application_t *app,
        record_h_page_data recordHPageData);

#ifdef __cplusplus
}
#endif

#endif