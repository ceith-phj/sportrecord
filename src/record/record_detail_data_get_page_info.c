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

#include <record/record_detail_data_get_page_info.h>
#include "sb_screen_util.h"
#include <hm_system_status_bar.h>
#include "packages/services/syssevice/manager/system_locale.h"
#define DETAIL_PAGE "SportRecordDetailScreen.SportRecordDetailLayer"
#define SEGMENT_PAGE "SportRecordDetailScreen.SportRecordSegmentLayer"
#define PB_FIRST_PAGE "SportRecordDetailScreen.SportRecordPBFirstLayer"
#define PB_SECOND_PAGE "SportRecordDetailScreen.SportRecordPBSecondLayer"
#define PB_THREE_PAGE "SportRecordDetailScreen.SportRecordPBThreeLayer"
#define PB_FOUR_PAGE "SportRecordDetailScreen.SportRecordPBFourLayer"
#define PB_FIVE_PAGE "SportRecordDetailScreen.SportRecordPBFiveLayer"
#define PB_SIX_PAGE "SportRecordDetailScreen.SportRecordPBSixLayer"
#define PB_SEVEB_PAGE "SportRecordDetailScreen.SportRecordPBSevenLayer"
#define FB_MAX_OX_PAGE "SportRecordDetailScreen.SportRecordVomaxLayer"
#define FB_TE_PAGE "SportRecordDetailScreen.SportRecordTrainingEffeLayer"
#define FB_REV_TIME "SportRecordDetailScreen.SportRecordDetailRecoveryTimeLayer"
#define GOAL_PAGE "SportRecordDetailScreen.SportRecordGoalLayer"
#define CLOSE_PAGE "SportRecordDetailScreen.SportRecordCloseLayer"

#define MAX_SHOW_PAGE   14

static void _init_layer(gr_application_t *app);

static record_detail_page_info recordDetailPageInfo = {0};

const char *showLayersTypeSix[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem",
    "SportRecordDetailDataSecondItemLayer.SportRecordDetailDataSecondItem",
    "SportRecordDetailHhzLayer.SportRecordDetailHhzitem",
    "SportRecordCharFirstLayer.chartGroup",
    "SportRecordCharSecondLayer.chartGroup"
};
const char *showLayersTypeFive[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem",
    "SportRecordDetailDataSecondItemLayer.SportRecordDetailDataSecondItem",
    "SportRecordDetailHhzLayer.SportRecordDetailHhzitem",
    "SportRecordCharFirstLayer.chartGroup"
};
const char *showLayersTypeFour[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem",
    "SportRecordDetailHhzLayer.SportRecordDetailHhzitem",
    "SportRecordCharFirstLayer.chartGroup"
};
const char *showLayersTypeFourOne[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem",
    "SportRecordDetailHhzLayer.SportRecordDetailHhzitem",
    "SportRecordCharFirstLayer.chartGroup",
    "SportRecordCharSecondLayer.chartGroup"
};
const char *showLayersTypeFourTwo[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem",
    "SportRecordDetailDataSecondItemLayer.SportRecordDetailDataSecondItem",
    "SportRecordDetailHhzLayer.SportRecordDetailHhzitem",
    "SportRecordCharFirstLayer.chartGroup",
    "SportRecordCharSecondLayer.chartGroup"

};
const char *showLayersTypeThree[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem",
    "SportRecordDetailDataSecondItemLayer.SportRecordDetailDataSecondItem"
};

const char *showLayersTypeTwo[] =
{
    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem",
    "SportRecordDetailDataFirstItemLayer.SportRecordDetailDataFirstItem"
};

const record_detail_page_info RecordDetail_getPageInfo(int sportType)
{
    sport_type_t sportTypet = (sport_type_t)sportType;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:      /* 户外跑 */
        case SPORT_GROUP_TREADMILL:            /* 跑步机 */
        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_MOUNTAINEERING:       /* 登山 */
        case SPORT_GROUP_TRAIL_RUNNING:        /* 越野跑 */
        case SPORT_GROUP_SKIING:               /* 滑雪 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            recordDetailPageInfo.pageNumber = 6;
            recordDetailPageInfo.showLayers = showLayersTypeSix;
            break;
        }

        case SPORT_GROUP_ROPE_SKIPPING: /* 跳绳 */
        {
            recordDetailPageInfo.pageNumber = 5;
            recordDetailPageInfo.showLayers = showLayersTypeFive;
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_3:                 /* 类型3 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        case SPORT_GROUP_STAIR_CLIMBING:    /* 爬楼 */
        {
            recordDetailPageInfo.pageNumber = 4;
            recordDetailPageInfo.showLayers = showLayersTypeFour;
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        case SPORT_GROUP_SOCCER:         /* 足球 */
        case SPORT_GROUP_1:              /* 类型1 */
        case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        case SPORT_GROUP_PARASAILING:    /* 滑翔伞 */
        {
            recordDetailPageInfo.pageNumber = 5;
            recordDetailPageInfo.showLayers = showLayersTypeFourOne;
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            recordDetailPageInfo.pageNumber = 6;
            recordDetailPageInfo.showLayers = showLayersTypeFourTwo;
            break;
        }

        case SPORT_GROUP_TENNIS: /* 网球 */
        {
            recordDetailPageInfo.pageNumber = 3;
            recordDetailPageInfo.showLayers = showLayersTypeThree;
            break;
        }

        case SPORT_GROUP_TRIATHLON:  /* 铁人三项 （不支持实时数据配置） */
        case SPORT_GROUP_MULTISPORT: /* 复合运动 （不支持实时数据配置） */
        {
            recordDetailPageInfo.pageNumber = 2;
            recordDetailPageInfo.showLayers = showLayersTypeFour;
            break;
        }

        case SPORT_GROUP_4: /* 类型4 */
            break;

        case SPORT_GROUP_5: /* 类型5 */
            break;

        case SPORT_GROUP_6: /* 类型6 */
            break;

        case SPORT_GROUP_GOLF: /* 高尔夫 */
            break;

        case SPORT_GROUP_GOLF_SWING: /* 高尔夫挥杆 */
            break;

        default:
            break;
    }

    return recordDetailPageInfo;
}

struct _pageScrollerPage *RecordDetail_getHPageInfo(gr_application_t *app,
        record_h_page_data recordHPageData)
{
    _init_layer(app);
    static struct _pageScrollerPage page_scroll_page[MAX_SHOW_PAGE];
    struct _pageScrollerPage *page;
    memset(page_scroll_page, 0, sizeof(struct _pageScrollerPage) * MAX_SHOW_PAGE);

    for (int i = 0; i < recordHPageData.pageNum; i++)
    {
        page = &page_scroll_page[i];

        if (recordHPageData.isPersonalRecordFirst)
        {
            gre_set_layer_visible(app, PB_FIRST_PAGE, 1);
            recordHPageData.isPersonalRecordFirst = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_FIRST_PAGE, 64);
            page->layers[0] = PB_FIRST_PAGE;
            continue;
        }

        if (recordHPageData.isPersonalRecordSeocnd)
        {
            gre_set_layer_visible(app, PB_SECOND_PAGE, 1);
            recordHPageData.isPersonalRecordSeocnd = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_SECOND_PAGE, 64);
            page->layers[0] = PB_SECOND_PAGE;
            continue;
        }

        if (recordHPageData.isPersonalRecordThree)
        {
            gre_set_layer_visible(app, PB_THREE_PAGE, 1);
            recordHPageData.isPersonalRecordThree = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_THREE_PAGE, 64);
            page->layers[0] = PB_THREE_PAGE;
            continue;
        }

        if (recordHPageData.isPersonalRecordFour)
        {
            gre_set_layer_visible(app, PB_FOUR_PAGE, 1);
            recordHPageData.isPersonalRecordFour = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_FOUR_PAGE, 64);
            page->layers[0] = PB_FOUR_PAGE;
            continue;
        }

        if (recordHPageData.isPersonalRecordFive)
        {
            gre_set_layer_visible(app, PB_FIVE_PAGE, 1);
            recordHPageData.isPersonalRecordFive = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_FIVE_PAGE, 64);
            page->layers[0] = PB_FIVE_PAGE;
            continue;
        }

        if (recordHPageData.isPersonalRecordSix)
        {
            gre_set_layer_visible(app, PB_SIX_PAGE, 1);
            recordHPageData.isPersonalRecordSix = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_SIX_PAGE, 64);
            page->layers[0] = PB_SIX_PAGE;
            continue;
        }

        if (recordHPageData.isPersonalRecordSenven)
        {
            gre_set_layer_visible(app, PB_SEVEB_PAGE, 1);
            recordHPageData.isPersonalRecordSenven = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], PB_SEVEB_PAGE, 64);
            page->layers[0] = PB_SEVEB_PAGE;
            continue;
        }

        if (recordHPageData.isMaximalOxygenUptake)
        {
            gre_set_layer_visible(app, FB_MAX_OX_PAGE, 1);
            recordHPageData.isMaximalOxygenUptake = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], FB_MAX_OX_PAGE, 64);
            page->layers[0] = FB_MAX_OX_PAGE;
            continue;
        }

        if (recordHPageData.isTrainingEff)
        {
            gre_set_layer_visible(app, FB_TE_PAGE, 1);
            recordHPageData.isTrainingEff = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], FB_TE_PAGE, 64);
            page->layers[0] = FB_TE_PAGE;
            continue;
        }

        if (recordHPageData.isRecoveryTime)
        {
            gre_set_layer_visible(app, FB_REV_TIME, 1);
            recordHPageData.isRecoveryTime = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], FB_REV_TIME, 64);
            page->layers[0] = FB_REV_TIME;
            continue;
        }

        if (recordHPageData.isGoal)
        {
            gre_set_layer_visible(app, GOAL_PAGE, 1);
            recordHPageData.isGoal = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], GOAL_PAGE, 64);
            page->layers[0] = GOAL_PAGE;
            continue;
        }

        if (recordHPageData.isDetail)
        {
            gre_set_layer_visible(app, DETAIL_PAGE, 1);
            recordHPageData.isDetail = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], DETAIL_PAGE, 64);
            page->layers[0] = DETAIL_PAGE;
            continue;
        }

        if (recordHPageData.isSegment)
        {
            gre_set_layer_visible(app, SEGMENT_PAGE, 1);
            recordHPageData.isSegment = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], SEGMENT_PAGE, 64);
            page->layers[0] = SEGMENT_PAGE;
            continue;
        }

        if (recordHPageData.isClose)
        {
            gre_set_layer_visible(app, CLOSE_PAGE, 1);
            recordHPageData.isClose = false;
            page->layerCnt = 1;
            // strncpy(page->layers[0], CLOSE_PAGE, 64);
            page->layers[0] = CLOSE_PAGE;
            continue;
        }
    }

    return page_scroll_page;
}

static void _init_layer(gr_application_t *app)
{
    gre_set_layer_visible(app, PB_FIRST_PAGE, 0);
    gre_set_layer_visible(app, PB_SECOND_PAGE, 0);
    gre_set_layer_visible(app, PB_THREE_PAGE, 0);
    gre_set_layer_visible(app, PB_FOUR_PAGE, 0);
    gre_set_layer_visible(app, PB_FIVE_PAGE, 0);
    gre_set_layer_visible(app, PB_SIX_PAGE, 0);
    gre_set_layer_visible(app, PB_SEVEB_PAGE, 0);
    gre_set_layer_visible(app, SEGMENT_PAGE, 0);
    gre_set_layer_visible(app, DETAIL_PAGE, 0);
    gre_set_layer_visible(app, FB_MAX_OX_PAGE, 0);
    gre_set_layer_visible(app, FB_TE_PAGE, 0);
    gre_set_layer_visible(app, FB_REV_TIME, 0);
    gre_set_layer_visible(app, GOAL_PAGE, 0);
    gre_set_layer_visible(app, CLOSE_PAGE, 0);
}
