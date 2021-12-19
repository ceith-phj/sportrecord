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
#include <hm_system_status_bar.h>
#include <record/record_detail_scroll_bar.h>
#ifndef HM_DISPLAY_IS_ROUND
#ifdef HM_FEATURE_BLED_UI
#define SCROLL_BAR_MAX 112
#else
#define SCROLL_BAR_MAX 150
#endif
#else
#define SCROLL_BAR_START -20
#define SCROLL_BAR_MAX 40
#endif

#ifndef HM_DISPLAY_IS_ROUND

const void RecordDetail_initScrollBar(gr_application_t *app, int viewHei,
                                      int totalHei)
{
    int pageNum = totalHei / viewHei;
    if (pageNum > 0)
    {
        char key[256] = {0};
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.start_y", SCROLL_BAR_PATH);
        setSBFloatData(app, key, 0);
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.start_height", SCROLL_BAR_PATH);
        float endAgele = ((float)SCROLL_BAR_MAX / pageNum);
        setSBFloatData(app, key, endAgele);
    }
}

const void RecordDetail_scrollBar(gr_application_t *app, int viewHei,
                                  int totalHei, int pageIndex)
{

    int pageNum = totalHei / viewHei;
    if (pageNum > 0)
    {
        char key[256] = {0};
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.start_y", SCROLL_BAR_PATH);
        float startAngle = pageIndex * ((float)SCROLL_BAR_MAX /
                           pageNum);
        setSBFloatData(app, key, startAngle);
        // memset(key, 0, sizeof(key));
        // snprintf(key, sizeof(key), "%s.start_height", SCROLL_BAR_PATH);
        // //  float endAgele = SCROLL_BAR_START + pageIndex * ((float)SCROLL_BAR_MAX /
        // //                   pageNum) + ((float)SCROLL_BAR_MAX / pageNum);
        // //float endAgele =SCROLL_BAR_START+(float)SCROLL_BAR_MAX / pageNum;
        // setSBFloatData(app, key, ((float)SCROLL_BAR_MAX / pageNum));
    }
}
#else

const void RecordDetail_initScrollBar(gr_application_t *app, int viewHei,
                                      int totalHei)
{
    int pageNum = totalHei / viewHei;
    if (pageNum > 0)
    {
        char key[256] = {0};
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.startAngle", SCROLL_BAR_PATH);
        setSBFloatData(app, key, SCROLL_BAR_START);
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.endAngle", SCROLL_BAR_PATH);
        float endAgele = SCROLL_BAR_START + ((float)SCROLL_BAR_MAX / pageNum);
        setSBFloatData(app, key, endAgele);
    }
}

const void RecordDetail_scrollBar(gr_application_t *app, int viewHei,
                                  int totalHei, int pageIndex)
{
    int pageNum = totalHei / viewHei;
    if (pageNum > 0)
    {
        char key[256] = {0};
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.startAngle", SCROLL_BAR_PATH);
        float startAngle = SCROLL_BAR_START + pageIndex * ((float)SCROLL_BAR_MAX /
                           pageNum);
        setSBFloatData(app, key, startAngle);
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "%s.endAngle", SCROLL_BAR_PATH);
        float endAgele = SCROLL_BAR_START + pageIndex * ((float)SCROLL_BAR_MAX /
                         pageNum) + ((float)SCROLL_BAR_MAX / pageNum);
        setSBFloatData(app, key, endAgele);
    }
}

#endif
