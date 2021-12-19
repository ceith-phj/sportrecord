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

#ifndef RECORD_DETAIL_SCROLL_BAR_H
#define RECORD_DETAIL_SCROLL_BAR_H
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <gre/sdk/grelib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCROLL_BAR_PATH "SportRecordDetailLayer.scrollBarprogress"

#ifdef __cplusplus
extern "C"
{
#endif

const void RecordDetail_initScrollBar(gr_application_t *app, int viewHei,
                                      int totalHei);
const void RecordDetail_scrollBar(gr_application_t *app, int viewHei,
                                  int totalHei, int pageIndex);

#ifdef __cplusplus
}
#endif
#endif