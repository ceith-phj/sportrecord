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

#define TAG "sport_record_detail"
#include <sb_screen_context.h>
#include <hlog.h>
#include <gre/sdk/grelib.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <presenter/record_detail_presenter.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <string.h>
#include <sport_type_utils.h>
#include <record/record_detail_data_utils.h>
#include <record/record_detail_data_heart_rate_zone_utils.h>
#include <record/record_detail_data_get_page_info.h>
#include <record/record_parameter.h>
// #include <record/record_detail_char_utils.h>
#include <widget/pageScrollerInLayer/pageVScrollPager.h>
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include "core/display_metrics.h"
#include <record/record_detail_segment_utils.h>
#include <widget/sportChart/sportChart.h>
#include <record/record_detail_scroll_bar.h>
#include <record/record_parameter.h>
#include "sport_group_auto.h"
#include <record/record_detail_pb_utils.h>
#include <record/record_detail_fb_utils.h>
#include <widget/hSchedule/hScedule.h>
#include <container/SportGoalContainer.h>
#include <framework/osadapter/hm_time.h>
#include "packages/services/sport/misc/sport_misc.h"
#include "message_manager.h"
#include "ui_handle.h"
#include "hm_power_manager.h"
#include <hm_system_status_bar.h>
#include <record/record_text_aglin_utils.h>
#if HMI_COMP_STATISTICS
#include "hmstatistics.h"
#endif
#include <settings_general_utils.h>

#ifndef HM_DISPLAY_IS_ROUND
#define VARIABLE_FVALUE_HALIGN    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.item.fvalue_hAlign"
#define VARIABLE_FTYPE_HALIGN    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.item.ftype_hAlign"
#define VARIABLE_SVALUE_HALIGN    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.item.svalue_hAlign"
#define VARIABLE_STYPE_HALIGN    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.item.stype_hAlign"
enum
{
    GR_RENDER_HALIGN_NONE = 0,
    GR_RENDER_HALIGN_LEFT,
    GR_RENDER_HALIGN_CENTER,
    GR_RENDER_HALIGN_RIGHT
};
#endif

#define ID_SPORT_RECORD_SCROLL "SportRecordDetailScreen.pageScrollMultiLayers"
#define RECORDLIST_SCREEN_LAYER "SportRecordDetailScreen.SportRecordDetailLayer"
#define ID_CHAR_FIRST_LINE "SportRecordCharFirstLayer.CharGroup.CharDrowControl"
#define ID_CHAR_SECOND_LINE "SportRecordCharSecondLayer.CharGroup.CharDrowControl"
#define ID_PAGESCROLLER_MULTILAYERS "SportRecordDetailScreen.pageScrollerMultiLayers"
#define ID_SEGMENT_LAYER "SportRecordDetailScreen.SportRecordSegmentLayer"
#define ID_GOAL_LAYER "SportRecordDetailScreen.SportRecordGoalLayer"
#define ID_SCHEDULE "SportRecordDetailScreen.SportRecordScheduleLayer"
#define ID_CLOSE "SportRecordCloseLayer.button"
#define ID_CLOSE_ICON "SportRecordCloseLayer.close"
#define ID_INDO "SportRecordDetailLayer.item0.info"
#define CHAR_MAX_POINT 350
#define SCROLL_DRAG_THREHOLD 20
#ifndef HM_DISPLAY_IS_ROUND
#define NO_TRAVEL_X (getDimension(124))
#define HAVA_TRAVEL_X (getDimension(32))
#else
#define NO_TRAVEL_X (getDimension(124))
#define HAVA_TRAVEL_X (getDimension(65))
#endif
#define SEGMENT_MAX_VALUE 50

#define SCREEN_NAME SportRecordDetail
SCREEN_DECLARE(SCREEN_NAME);

#define CHART_LINE_WIDTH    (getDimension(4))
#define TRACK_LINE_WIDTH    (getDimension(6))

// static TimerHandle timeHandle = H_NULL;

typedef struct
{
    int x;
    int y;
} SPoint;
typedef struct
{
    record_parameter_data recordParameterData;
    bool isDisplayVoice; /* 是否已播报语音标志 */
} SaveData;

typedef struct
{
    //传递参数
    record_parameter_data recordParameterData;
    SportRecordDetailCharData hr_char_data;
    SportRecordDetailCharData pace_char_data;
    SportRecordDetailCharData speed_char_data;
    SportRecordDetailCharData swim_stroke_speed_char_data;
    SportRecordDetailCharData altitule_char_data;
    SportRecordDetailCharData row_stroke_freq_char_data;
    record_detail_segment_group_data *record_detail_segment_group_detail;
    sport_record_detail_pb_data *sport_Record_Detail_Pb_Data;
    record_h_page_data recordHPageData;
    int hPageIndex;
    int hPageNum;
    bool isDisplayVoice; /* 是否已播报语音标志 */
    bool isHaveSegment;
    int viewHei;
    int totalHei;
    WakeLockHandle wakelock_handle;
} AppData;

const char *refreshLayers[] =
{
    "SportRecordDetailTrackLayer",
    "SportRecordDetailDataFirstItemLayer",
    "SportRecordDetailDataSecondItemLayer",
    "SportRecordDetailHhzLayer",
};

// ##
#include <unistd.h>
#include <widget/multiPolyLine/multiPolyLine.h>
#include "sport_track_analyze.h"
#include "sport_gpx_analyze.h"
#include "sport_service.h"

#define ID_TRACK_CONTAINER      "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area"
#define ID_TRACK_TOP_CONTAINER  "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area_top"

#define IMG_TRACK_START_X_ID    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area_top.img_track_start_x"
#define IMG_TRACK_START_Y_ID    "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area_top.img_track_start_y"
#define IMG_TRACK_END_X_ID      "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area_top.img_track_end_x"
#define IMG_TRACK_END_Y_ID      "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area_top.img_track_end_y"
#define ALPHA_ID                "SportRecordDetailTrackLayer.SportRecordDetailTrackItem.track_area_top.alpha"

static void *record_track_preload(void);
static void record_track_paint(void *data);

static void _RecordChar_pace2MinuteSec(float pace, int *minute, int *sec);
static void _init_struct();
static display_area_t *display_area = H_NULL;
static SportRecordDetailPresenter *sportRecordDetailPresenter = H_NULL;

#if HMI_COMP_STATISTICS
static void _ss_buried_point(uint8_t index, HM_STATIS_EVENT_ID_E value,
                             Param *parm)
{
    hm_statis_data_add_with_kv(value, parm,
                               index,
                               true);
}
#endif

// static void _releaseScreenLock()
// {
//     AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
//                                   AppData);

//     if (data->wakelock_handle != H_NULL)
//     {
//         H_Bool success = getPowerManager()->release(data->wakelock_handle);
//         data->wakelock_handle = H_NULL;
//         HLOG_I(TAG, "release current wakelock %d.\n\r", success);
//     }
// }

// static void _acquireScreenLock()
// {
//     AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
//                                   AppData);
//     data->wakelock_handle = getPowerManager()->acquire("SportRecordDetail",
//                             SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE);
// }

static int detail_multi_scroll_bar_create()
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    record_detail_page_info recordDetailPageInfo = RecordDetail_getPageInfo(
                data->recordParameterData.sport_type);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    data->viewHei = STORYBOARD_DISPLAY_WIDTH;
    data->totalHei = getDisplayMetrics()->height * recordDetailPageInfo.pageNumber;
    HLOG_I(TAG, "%s viewHei:%d totalHei:%d recordDetailPageInfo.pageNumber:%d\r\n",
           __func__, data->viewHei, data->totalHei, recordDetailPageInfo.pageNumber);
    RecordDetail_initScrollBar(getScreenContext(
                                   currentScreenInstance)
                               ->_grAppContext,
                               data->viewHei, data->totalHei);
    return 0;
};

static void detail_multi_layer_Scrolled(pageVScrollPager *scrollPager, int page)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    RecordDetail_scrollBar(getScreenContext(currentScreenInstance)->_grAppContext,
                           data->viewHei, data->totalHei, page);
};

static void detail_multi_layer_create()
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    record_detail_page_info recordDetailPageInfo = RecordDetail_getPageInfo(
                data->recordParameterData.sport_type);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    pageVScrollPager *pageVScrollPager = createPageVScrollPagerContainer(
            getScreenContext(currentScreenInstance)->_grAppContext);

    if (pageVScrollPager)
    {
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORT_RECORD_SCROLL,
                     pageVScrollPager);
        pageVScrollPager->create(getScreenContext(currentScreenInstance)->_grAppContext,
                                 pageVScrollPager, 480, "SportRecordDetailScreen", "SportRecordDetailLayer",
                                 recordDetailPageInfo.pageNumber, recordDetailPageInfo.showLayers,
                                 detail_multi_layer_Scrolled);
        pageVScrollPager->initPages(pageVScrollPager);
        pageVScrollPager->sync(pageVScrollPager, 0);
        pageVScrollPager->setDirection(pageVScrollPager, DRAG_VEL);
    }
};

static void onPageViewScrolled(pageScrollerMultiLayers *scroll, int page)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->hPageIndex = page;
    hSchedule *_hScedule = (hSchedule *)findContainer(getScreenContext(getScreen(
                               SCREEN_NAME)),
                           ID_SCHEDULE);

    if (_hScedule)
    {
        _hScedule->setScheduleCurrent(_hScedule, data->hPageIndex);
    }

#if HMI_COMP_STATISTICS
    _ss_buried_point(0, SPORT_REC_PROPORTION_EID, H_NULL);
#endif
}
static void detail_schedule_layer_create(int shceduleNum)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          "SportRecordDetailScreen.SportRecordScheduleLayer", 1);
    hSchedule *_hScedule = createHSchedulerContainer(getScreenContext(
                               currentScreenInstance)
                           ->_grAppContext);

    if (_hScedule)
    {
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SCHEDULE, _hScedule);
        _hScedule->create(getScreenContext(currentScreenInstance)->_grAppContext,
                          _hScedule, "SportRecordScheduleLayer", shceduleNum, 10);
        _hScedule->initShedule(_hScedule);
        _hScedule->setSchedulePostiton(_hScedule, SCREEN_ROUND);
        _hScedule->setScheduleCurrent(_hScedule, data->hPageIndex);
    }
}

// static void destory_timer()
// {
//     _releaseScreenLock();

//     if (H_NULL != timeHandle)
//     {
//         hmTime_stopTimer(timeHandle);
//         hmTime_destroyTimer(timeHandle);
//         timeHandle = H_NULL;
//     }
// }
// static inline bool _refPageInfo(EventData *eventData)
// {
//     AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
//                                   AppData);

//     if (data)
//     {
//         pageScrollerMultiLayers *scroll = (pageScrollerMultiLayers *)findContainer(
//                                               getScreenContext(getScreen(SCREEN_NAME)), ID_PAGESCROLLER_MULTILAYERS);

//         if (scroll)
//         {
//             scroll->scrollToPage(scroll, data->hPageIndex + 1, false);
//         }

//         hSchedule *_hScedule = (hSchedule *)findContainer(getScreenContext(getScreen(
//                                    SCREEN_NAME)),
//                                ID_SCHEDULE);

//         if (_hScedule)
//         {
//             _hScedule->setScheduleCurrent(_hScedule, data->hPageIndex + 1);
//         }

//         if ((data->hPageIndex + 1) == data->hPageNum - 1)
//         {
//             // data->hPageIndex = -1;
//             destory_timer();
//         }
//         else
//         {
//             data->hPageIndex = data->hPageIndex + 1;
//         }
//     }
//     else
//     {
//         destory_timer();
//     }

//     return true;
// }
// static void _timerCallBack(TimerHandle timer)
// {
//     message_sendEventDirect(getUIHandle(), NULL, _refPageInfo);
// }

// static void creat_auto_page()
// {
//     AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
//                                   AppData);

//     if (data->hPageNum > 1)
//     {
//         _acquireScreenLock();

//         if (H_NULL == timeHandle)
//         {
//             timeHandle = hmTime_createTimerWithName(HM_TIMER_PERIODIC, _timerCallBack,
//                                                     "record_detail_auto_page_time", H_NULL);
//             hmTime_startTimer(timeHandle, 4000);
//         }
//     }
// }

static void creat_inter_text()
{
    char buff[256] = {0};
    memset(buff, 0, sizeof(buff));
    snprintf(buff, sizeof(buff), "SportRecordCloseLayer.hite.close_hite");
    set_text_height("SportRecordCloseLayer.hite",
                    System_getLocaleText("id_sportrecord_close_hite"), "fonts/allfont-Medium.ttf",
                    36);
    setSBStringData(getAppInstance(), buff,
                    System_getLocaleText("id_sportrecord_close_hite"));
#ifdef HM_DISPLAY_IS_ROUND
    int hiteHeight = gre_get_control_h(getAppInstance(),
                                       "SportRecordCloseLayer.hite");
    gre_set_control_y(getAppInstance(), "SportRecordCloseLayer.button",
                      getDimension(142 + hiteHeight));
#endif
    memset(buff, 0, sizeof(buff));
    snprintf(buff, sizeof(buff), "SportRecordCloseLayer.button.closeButtontext");
    setSBStringData(getAppInstance(), buff,
                    System_getLocaleText("id_sportrecord_close_button_text"));
}

static void detail_h_multi_layer_create()
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    pageScrollerMultiLayers *scroll = createPageScrollerMultiLayers(
                                          getScreenContext(currentScreenInstance)->_grAppContext);

    if (scroll)
    {
        addContainer(getScreenContext(getScreen(SCREEN_NAME)),
                     ID_PAGESCROLLER_MULTILAYERS, scroll);
        scroll->init(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext, scroll,
                     STORYBOARD_DISPLAY_WIDTH, 0, 400, onPageViewScrolled);
        struct _pageScrollerPage *pages = RecordDetail_getHPageInfo(getScreenContext(
                                              getScreen(SCREEN_NAME))
                                          ->_grAppContext,
                                          data->recordHPageData);
        data->hPageNum = data->recordHPageData.pageNum;
        HLOG_I(TAG, "%s hPageNum:%d\r\n", __func__, data->hPageNum);
        scroll->initPages(scroll, pages, data->hPageNum);
        scroll->setOvershoot(scroll, 100);
        scroll->setCycle(scroll, false);
        scroll->setDirect(scroll, PSML_DIRECT_HOR);
        scroll->sync(scroll, 0);
    }
}
static void segment_onItemUpdate(unequalHeiList *list, int idx,
                                 const char *itemGroupPath)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (NULL == list || idx <= 0 || NULL == itemGroupPath)
    {
        return;
    }

    if (data->record_detail_segment_group_detail->grouping != 0
            || data->record_detail_segment_group_detail->segment != 0)
    {
        RecordSegment_setSegmentGroupDetailData(getScreenContext(getScreen(
                SCREEN_NAME))
                                                ->_grAppContext,
                                                idx - 2, data->recordParameterData.sport_type,
                                                itemGroupPath,
                                                data->record_detail_segment_group_detail->record_detail_segment_data[idx -
                                                            2]
                                                .detail);
    }

    if (data->record_detail_segment_group_detail->lesson != 0)
    {
        RecordSegment_setLessonDetailData(getScreenContext(getScreen(
                                              SCREEN_NAME))
                                          ->_grAppContext,
                                          idx - 2, data->recordParameterData.sport_type,
                                          itemGroupPath,
                                          data->record_detail_segment_group_detail->record_detail_segment_data[idx -
                                                      2]
                                          .detail);
    }
}

static void segment_onItemTouch(unequalHeiList *list, int idx,
                                const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
}

static void setSegmentHeadAndFoot()
{
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.SportRecordSegmentMore.item.morehite",
             "SportRecordSegmentItemLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_segment_more_hite"));
}

static void detail_segemnt_layer_creata()
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    unequalHeiList *segementList = createUnequalHeiListContainer(getScreenContext(
                                       currentScreenInstance)
                                   ->_grAppContext);

    if (segementList)
    {
        RecordSegment_refSegmentDetailTitle(getScreenContext(getScreen(
                                                SCREEN_NAME))
                                            ->_grAppContext,
                                            data->record_detail_segment_group_detail->grouping,
                                            data->record_detail_segment_group_detail->segment,
                                            data->record_detail_segment_group_detail->lesson,
                                            data->recordParameterData.sport_type);
        int itemNum = 0;

        if (data->record_detail_segment_group_detail->lesson != 0)
        {
            if (data->record_detail_segment_group_detail->lesson != 0)
            {
                if (data->record_detail_segment_group_detail->lesson > 50)
                {
                    itemNum = SEGMENT_MAX_VALUE + 2;
                }
                else
                {
                    itemNum = data->record_detail_segment_group_detail->lesson + 2;
                }
            }

            char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportRecordSegmentItemLayer.SportRecordLessonItem",
                "SportRecordSegmentItemLayer.SportRecordSegmentItemTitle",
                "SportRecordSegmentItemLayer.SportRecordSegmentTitle",
                "SportRecordSegmentItemLayer.SportRecordSegmentMore",
            };
            int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
            segementList->create(getScreenContext(currentScreenInstance)->_grAppContext,
                                 segementList, "SportRecordDetailScreen", "SportRecordSegmentLayer",
                                 itemNum, itemTemps, tempCnt,
                                 segment_onItemUpdate, segment_onItemTouch, "item");
        }
        else
        {
            if (data->record_detail_segment_group_detail->segment != 0)
            {
                if (data->record_detail_segment_group_detail->segment > 50)
                {
                    itemNum = SEGMENT_MAX_VALUE + 2;
                }
                else
                {
                    itemNum = data->record_detail_segment_group_detail->segment + 2;
                }
            }

            if (data->record_detail_segment_group_detail->grouping != 0)
            {
                if (data->record_detail_segment_group_detail->grouping > 50)
                {
                    itemNum = SEGMENT_MAX_VALUE + 2;
                }
                else
                {
                    itemNum = data->record_detail_segment_group_detail->grouping + 2;
                }
            }

            HLOG_I(TAG, "%s itemNum:%d\r\n", __func__, itemNum);
            char itemTemps1[][UNEQUALHEI_LIST_NAME_MAX] =
            {
                "SportRecordSegmentItemLayer.SportRecordSegmentItem",
                "SportRecordSegmentItemLayer.SportRecordSegmentItemTitle",
                "SportRecordSegmentItemLayer.SportRecordSegmentTitle",
                "SportRecordSegmentItemLayer.SportRecordSegmentMore",
            };
            int tempCnt1 = sizeof(itemTemps1) / sizeof(itemTemps1[0]);
            segementList->create(getScreenContext(currentScreenInstance)->_grAppContext,
                                 segementList, "SportRecordDetailScreen", "SportRecordSegmentLayer", itemNum,
                                 itemTemps1, tempCnt1, segment_onItemUpdate, segment_onItemTouch, "item");
        }

        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SEGMENT_LAYER,
                     segementList);
        segementList->setItemTemp(segementList, 0, 0, 2);
        segementList->setItemTemp(segementList, 1, 1, 1);
        segementList->setLastEmptySpace(segementList, getDimension(200));
        segementList->initItems(segementList);

        if (itemNum > 50)
        {
            setSegmentHeadAndFoot();
            segementList->addItem(segementList, 2);
        }
    }
}

static void detail_goal_layer_create(SportRecordDetailData
                                     sportRecordDetailData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    SportGoalContainer *sportGoalContainer = createSportGoalContainer(
                getScreenContext(currentScreenInstance)->_grAppContext);

    if (sportGoalContainer)
    {
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_GOAL_LAYER,
                     sportGoalContainer);
        sportGoalContainer->create(getScreenContext(
                                       currentScreenInstance)
                                   ->_grAppContext,
                                   sportGoalContainer,
                                   "SportRecordGoalLayer");
        sportGoalContainer->initSportGoalContainer(sportGoalContainer);

        switch (sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type)
        {
            case 2:
                sportGoalContainer->setLayerData(sportGoalContainer,
                                                 data->recordParameterData.sport_type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data,
                                                 (float)(sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime),
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                break;

            case 3:
            {
                sport_type_t sportTypet = (sport_type_t)data->recordParameterData.sport_type;
                sport_group_t group = sportGroup_get(sportTypet);
                float distance =
                    sportRecordDetailData.recordDetailDate.sport_summary_message.distance.distance;

                if (SPORT_GROUP_POOL_SWIMMING == group)
                {
                    sportGoalContainer->setLayerData(sportGoalContainer,
                                                     data->recordParameterData.sport_type,
                                                     sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                     sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data / 100,
                                                     distance / 100.0f,
                                                     sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                }
                else
                {
                    sportGoalContainer->setLayerData(sportGoalContainer,
                                                     data->recordParameterData.sport_type,
                                                     sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                     sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data / 1000,
                                                     ((int)(distance / 1000 * 100)) / 100.0f,
                                                     sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                }

                break;
            }

            case 4:
                sportGoalContainer->setLayerData(sportGoalContainer,
                                                 data->recordParameterData.sport_type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data,
                                                 (float)(sportRecordDetailData.recordDetailDate.sport_summary_message.calories.cal),
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                break;

            case 5:
                sportGoalContainer->setLayerData(sportGoalContainer,
                                                 data->recordParameterData.sport_type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data,
                                                 (float)(sportRecordDetailData.recordDetailDate.sport_summary_message.swim.trips),
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                break;

            case 6:
                sportGoalContainer->setLayerData(sportGoalContainer,
                                                 data->recordParameterData.sport_type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data,
                                                 (float)(sportRecordDetailData.recordDetailDate.sport_summary_message.number.totalNumberOfJumps),
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                break;

            case 7:
                sportGoalContainer->setLayerData(sportGoalContainer,
                                                 data->recordParameterData.sport_type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data,
                                                 (float)(sportRecordDetailData.recordDetailDate.sport_summary_message.other.aerobicTrainingEffect),
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                break;

            case 8:
                sportGoalContainer->setLayerData(sportGoalContainer,
                                                 data->recordParameterData.sport_type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type,
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data,
                                                 (float)(sportRecordDetailData.recordDetailDate.sport_summary_message.other.anaerobicTrainingEffect),
                                                 sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.data);
                break;

            default:
                break;
        }
    }
}

static void print_sport_record_detail_data(SportRecordDetailData
        sportRecordDetailData)
{
    HLOG_I(TAG, "MainData:%s AssistData:%s\r\n",
           sportRecordDetailData.recordDetailMainDataTitle.firstTitle,
           sportRecordDetailData.recordDetailAssisDataTitle.firstTitle);
};

#ifndef HM_DISPLAY_IS_ROUND
static H_VOID setFvalueAlignInt(H_Int8 value)
{
    setSBIntData(getAppInstance(), VARIABLE_FVALUE_HALIGN, value);
}

static H_VOID setFtypeAlignInt(H_Int8 value)
{
    setSBIntData(getAppInstance(), VARIABLE_FTYPE_HALIGN, value);
}

static H_VOID setSvalueAlignInt(H_Int8 value)
{
    setSBIntData(getAppInstance(), VARIABLE_SVALUE_HALIGN, value);
}

static H_VOID setStypeAlignInt(H_Int8 value)
{
    setSBIntData(getAppInstance(), VARIABLE_STYPE_HALIGN, value);
}

static void set_travel_pos(char *itemGroupPath, int postiton)
{
    if (NO_TRAVEL_X == postiton)
    {
        setFvalueAlignInt(GR_RENDER_HALIGN_CENTER);
        setFtypeAlignInt(GR_RENDER_HALIGN_CENTER);
        setSvalueAlignInt(GR_RENDER_HALIGN_CENTER);
        setStypeAlignInt(GR_RENDER_HALIGN_CENTER);
    }
    else if (HAVA_TRAVEL_X == postiton)
    {
        setFvalueAlignInt(GR_RENDER_HALIGN_LEFT);
        setFtypeAlignInt(GR_RENDER_HALIGN_LEFT);
        setSvalueAlignInt(GR_RENDER_HALIGN_LEFT);
        setStypeAlignInt(GR_RENDER_HALIGN_LEFT);
    }
}

#else
static void set_travel_pos(char *itemGroupPath, int postiton)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailTrackItem.item.fvx",
                itemGroupPath);
    setSBIntData(getAppInstance(), buffer, postiton);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailTrackItem.item.fhx",
                itemGroupPath);
    setSBIntData(getAppInstance(), buffer, postiton);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailTrackItem.item.shx",
                itemGroupPath);
    setSBIntData(getAppInstance(), buffer, postiton);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailTrackItem.item.svx",
                itemGroupPath);
    setSBIntData(getAppInstance(), buffer, postiton);
}
#endif

static void detail_refresh_main_data(char *itemGroupPath,
                                     SportRecordDetailData sportRecordDetailData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    sport_type_t sportTypet = (sport_type_t)data->recordParameterData.sport_type;
    sport_group_t group = sportGroup_get(sportTypet);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer),
             "%s.SportRecordDetailTrackItem.info.infoAlpha", itemGroupPath);

    if (group == SPORT_GROUP_STAIR_CLIMBING)
    {
        setSBIntData(getAppInstance(), buffer, 255);
    }
    else
    {
        setSBIntData(getAppInstance(), buffer, 0);
    }

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer),
             "%s.SportRecordDetailTrackItem.title.titleWidth", itemGroupPath);
    H_Int32 titleWidth = getSBIntData(getAppInstance(), buffer);
    gr_string_size_t sz = {0};
#ifdef HM_FEATURE_BLED_UI
    H_Int32 titleFontSize = 28;
#else
    H_Int32 titleFontSize = 40;
#endif
    gre_get_string_size(getAppInstance(),
                        sportType_getTypeNameChar(data->recordParameterData.sport_type),
                        "fonts/allfont-Medium.ttf", titleFontSize, titleWidth, &sz, false, 100);
    HLOG_I(TAG, "%s itemGroupPath:%s length:%d\r\n", __func__, itemGroupPath,
           strlen(itemGroupPath));
    H_Int32 tnLength = sz.width;
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer),
             "%s.SportRecordDetailTrackItem.title.titleAlign", itemGroupPath);

    if (tnLength > titleWidth)
    {
        setSBIntData(getAppInstance(), buffer, GRE_ALIGN_LEFT);
    }
    else
    {
        setSBIntData(getAppInstance(), buffer, GRE_ALIGN_CENTER);
    }

    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailTrackItem.title.titleValue", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportType_getTypeNameChar(data->recordParameterData.sport_type));
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailTrackItem.item.aiImage",
                itemGroupPath);

    if (data->recordParameterData.auto_reg == 1)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/icon_ai.png");
        memset(buffer, 0, sizeof(buffer));
        hm_snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailTrackItem.item.aiX",
                    itemGroupPath);

        if (sz.width > titleWidth)
        {
            int aix = (STORYBOARD_DISPLAY_WIDTH / 2) + (titleWidth / 2) + getDimension(3);
            setSBIntData(getAppInstance(), buffer, aix);
        }
        else
        {
            int aix = (STORYBOARD_DISPLAY_WIDTH / 2) + (sz.width / 2) + getDimension(3);
            setSBIntData(getAppInstance(), buffer, aix);
        }
    }
    else
    {
        setSBStringData(getAppInstance(), buffer, "images/record/icon_ai_bg.png");
    }

    if (sportRecordDetailData.track == NULL)
    {
        set_travel_pos(itemGroupPath, NO_TRAVEL_X);
    }
    else
    {
        set_travel_pos(itemGroupPath, HAVA_TRAVEL_X);
    }

    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailTrackItem.time.dataTime",
                itemGroupPath);
    RecordDate_getRecordDate(data->recordParameterData.track_id, buffer);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailTrackItem.item.firsttype",
                itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailMainDataTitle.firstTitle);
    //memset(buffer,0,sizeof(buffer));
    //hm_snprintf(buffer, sizeof(buffer), "%s.SportRecordDetailTrackItem.item.firstvalue", itemGroupPath);
    RecordMain_getRecordMainValue(sportRecordDetailData,
                                  data->recordParameterData.sport_type, itemGroupPath);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailTrackItem.item.secondtype", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailMainDataTitle.secondTitle);
};

static void detail_refresh_assist_first_data(char *itemGroupPath,
        SportRecordDetailData sportRecordDetailData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    HLOG_I(TAG, "%s itemGroupPath:%s length:%d\r\n", __func__, itemGroupPath,
           strlen(itemGroupPath));
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.firsttype", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailAssisDataTitle.firstTitle);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.secondtype", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailAssisDataTitle.secondTitle);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.thretype", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailAssisDataTitle.threeTitle);
    sport_type_t sportTypet = (sport_type_t)data->recordParameterData.sport_type;
    sport_group_t group = sportGroup_get(sportTypet);

#ifdef HM_FEATURE_BLED_UI
    const char* key_mainStyleFontConst = "mainStyleFont_const";
    const char* key_mainStyleSizeConst = "mainStyleSize_const";
    if (group == SPORT_GROUP_POOL_SWIMMING
            || group == SPORT_GROUP_OPEN_WATER_SWIMMING)
    {
        key_mainStyleFontConst = "mainStyleFont_const_swimming";
        key_mainStyleSizeConst = "mainStyleSize_const_swimming";
    }

    H_Int8 *mainStyleFont = H_NULL;
    H_Int32 mainStyleSize = 0;
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.%s", itemGroupPath,
                key_mainStyleFontConst);
    mainStyleFont = getSBStringData(getAppInstance(), buffer);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.%s", itemGroupPath,
                key_mainStyleSizeConst);
    mainStyleSize = getSBIntData(getAppInstance(), buffer);

    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.mainStyleFont", itemGroupPath);
    setSBStringData(getAppInstance(), buffer, mainStyleFont ? mainStyleFont : "fonts/huamifont.ttf");
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataFirstItem.item.mainStyleSize", itemGroupPath);
    setSBIntData(getAppInstance(), buffer, mainStyleSize ? mainStyleSize : 56);
    if (H_NULL != mainStyleFont)
    {
        H_FREE(mainStyleFont);
    }
#else
    if (group == SPORT_GROUP_POOL_SWIMMING
            || group == SPORT_GROUP_OPEN_WATER_SWIMMING)
    {
        memset(buffer, 0, sizeof(buffer));
        hm_snprintf(buffer, sizeof(buffer),
                    "%s.SportRecordDetailDataFirstItem.item.mainStyleFont", itemGroupPath);
        setSBStringData(getAppInstance(), buffer,
                        "fonts/allfont-Regular.ttf");
        memset(buffer, 0, sizeof(buffer));
        hm_snprintf(buffer, sizeof(buffer),
                    "%s.SportRecordDetailDataFirstItem.item.mainStyleSize", itemGroupPath);
        setSBIntData(getAppInstance(), buffer, 36);
    }
    else
    {
        memset(buffer, 0, sizeof(buffer));
        hm_snprintf(buffer, sizeof(buffer),
                    "%s.SportRecordDetailDataFirstItem.item.mainStyleFont", itemGroupPath);
        setSBStringData(getAppInstance(), buffer,
                        "fonts/huamifont.ttf");
        memset(buffer, 0, sizeof(buffer));
        hm_snprintf(buffer, sizeof(buffer),
                    "%s.SportRecordDetailDataFirstItem.item.mainStyleSize", itemGroupPath);
        setSBIntData(getAppInstance(), buffer, 80);
    }
#endif
    RecordFirst_getRecordFirstValue(sportRecordDetailData,
                                    data->recordParameterData.sport_type, itemGroupPath);
};
static void detail_refresh_assist_second_data(char *itemGroupPath,
        SportRecordDetailData sportRecordDetailData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    HLOG_I(TAG, "%s itemGroupPath:%s length:%d\r\n", __func__, itemGroupPath,
           strlen(itemGroupPath));
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataSecondItem.item.firsttype", itemGroupPath);
    HLOG_I(TAG, "path:%s\r\n", buffer);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailAssisDataTitle.fourTitle);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataSecondItem.item.secondtype", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailAssisDataTitle.fiveTitle);
    memset(buffer, 0, sizeof(buffer));
    hm_snprintf(buffer, sizeof(buffer),
                "%s.SportRecordDetailDataSecondItem.item.thretype", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportRecordDetailData.recordDetailAssisDataTitle.sixTitle);
    RecordSecond_getRecordSecondValue(sportRecordDetailData,
                                      data->recordParameterData.sport_type, itemGroupPath);
};

static void detail_refresh_HHZ_data(char *itemGroupPath,
                                    SportRecordDetailData sportRecordDetailData)
{
    RecordHHZ_getRecordHHZValue(getScreenContext(getScreen(
                                    SCREEN_NAME))
                                ->_grAppContext,
                                sportRecordDetailData, itemGroupPath);
};

static float record_speed(float speed)
{
    if(setting_getMileageUnit())
    {
        speed = sportEngine_utils()->km2mile(speed);
    }
    return speed;
}

static void _RecordChar_pace2MinuteSec(float pace, int *minute, int *sec)
{
    *minute = 0;
    *sec = 0;

    if (pace > 0.0f)
    {
        if (setting_getMileageUnit())
        {
            pace = sportEngine_utils()->mile2km(pace);
        }

        *minute = (int)(pace / 60.0f);
        *sec = (int)(pace - 60.0f * (*minute));
    }
};

static const char *chartLayer[2] =
{
    "SportRecordCharFirstLayer",
    "SportRecordCharSecondLayer",
};

static const char *chartID[2] =
{
    "SportRecordCharFirstLayer.firstCharType",
    "SportRecordCharSecondLayer.secondCharType",
};

static void detail_chart_create(uint32_t colorType,
                                sport_Chart_data_t sportChartData, SportRecord_ChartType_e charType,
                                int16_t *pos,
                                SportRecordDetailData sportRecordDetailData, uint8_t index)
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    sportChart_t *sportChart = createSportChart(getScreenContext(
                                   currentScreenInstance)
                               ->_grAppContext);
    sportChart->create(sportChart,
                       getScreenContext(currentScreenInstance)->_grAppContext, chartLayer[index], NULL,
                       NULL);
    chartInfo_t *chartInfo = (chartInfo_t *)H_MALLOC(sizeof(chartInfo_t));

    if (chartInfo == NULL)
    {
        HLOG_E(TAG, "%s : chartInfo MALLOC error", __func__);
        return;
    }

    chartDescrip_t *chartDescrip = (chartDescrip_t *)hm_mem_malloc(sizeof(
                                       chartDescrip_t));
    H_ASSERT(chartDescrip);
    hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%s",
                "--");
    hm_snprintf(chartDescrip->minString, sizeof(chartDescrip->maxString), "%s",
                "--");
    hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString), "%s",
                "--");
    hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->maxString), "%s",
                "--");
    hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->maxString), "%s",
                "--");
    hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->maxString), "%s",
                "--");

    switch (charType)
    {
        case SPORT_RECORD_CHART_HR:
            strncpy(chartInfo->title, System_getLocaleText("id_rt_chart_hr"),
                    sizeof(chartInfo->title));
            strncpy(chartInfo->avgNameString,
                    System_getLocaleText("id_sportrecord_char_avg_title"),
                    sizeof(chartInfo->avgNameString));
            strncpy(chartInfo->unitString, System_getLocaleText("id_rt_chart_hr_unit"),
                    sizeof(chartInfo->unitString));

            if ((int)sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.maximumHeartRate
                    > 0)
            {
                hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%d",
                            (int)sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.maximumHeartRate);
            }

            if ((int)sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.minimumHeartRate
                    > 0)
            {
                hm_snprintf(chartDescrip->minString, sizeof(chartDescrip->minString), "%d",
                            (int)sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.minimumHeartRate);
            }

            if ((int)sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate
                    > 0)
            {
                hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->avgString), "%d",
                            (int)sportRecordDetailData.recordDetailDate.sport_summary_message.heartRate.averageHeartRate);
            }

            if ((int)sportChartData.dataMax)
            {
                hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->topString), "%d",
                            (int)sportChartData.dataMax);
            }

            if ((int)(sportChartData.dataMax + sportChartData.dataMin) / 2 > 0)
            {
                hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->midString), "%d",
                            (int)(sportChartData.dataMax + sportChartData.dataMin) / 2);
            }

            if ((int)sportChartData.dataMin > 0)
            {
                hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->btmString), "%d",
                            (int)sportChartData.dataMin);
            }

            break;

        case SPORT_RECORD_CHART_SWIM_STROKE_SPEED:
            strncpy(chartInfo->title, System_getLocaleText("id_rt_chart_stroke_speed"),
                    sizeof(chartInfo->title));
            strncpy(chartInfo->avgNameString,
                    System_getLocaleText("id_sportrecord_char_avg_title"),
                    sizeof(chartInfo->avgNameString));
            strncpy(chartInfo->unitString,
                    System_getLocaleText("id_rt_chart_stroke_speed_unit"),
                    sizeof(chartInfo->unitString));
            hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.swim.maximumStrokeRate);
            hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.swim.averageStrokeRate);
            hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportChartData.dataMax);
            hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->maxString), "%d",
                        (int)(sportChartData.dataMax + sportChartData.dataMin) / 2);
            hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportChartData.dataMin);
            break;

        case SPORT_RECORD_CHART_PACE:
            strncpy(chartInfo->title, System_getLocaleText("id_rt_chart_pace"),
                    sizeof(chartInfo->title));
            strncpy(chartInfo->avgNameString,
                    System_getLocaleText("id_sportrecord_char_avg_title"),
                    sizeof(chartInfo->avgNameString));
            strncpy(chartInfo->unitString, System_getLocaleText("id_rt_chart_pace_unit"),
                    sizeof(chartInfo->unitString));
            int minute_max, sec_max, minute_min, sec_min, minute_avg, sec_avg;
            float pace1 = sportChartData.dataMin;
            float pace3 = sportChartData.dataMax;
            float pace2 = (sportChartData.dataMin + sportChartData.dataMax) / 2.0f;
            _RecordChar_pace2MinuteSec(pace1, &minute_max, &sec_max);
            _RecordChar_pace2MinuteSec(pace3, &minute_min, &sec_min);
            _RecordChar_pace2MinuteSec(pace2, &minute_avg, &sec_avg);

            if (minute_min > 0 || sec_min > 0)
            {
                hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->maxString),
                            "%d\'%02d", minute_min, sec_min);
            }

            if (minute_avg > 0 || sec_avg > 0)
            {
                hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->maxString),
                            "%d\'%02d", minute_avg, sec_avg);
            }

            if (minute_max > 0 || sec_max > 0)
            {
                hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->maxString),
                            "%d\'%02d", minute_max, sec_max);
            }

            float paceMax =
                sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace *
                1000.0f;
            float paceMin = 0;
            float paceAvg =
                sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace *
                1000.0f;
            _RecordChar_pace2MinuteSec(paceMax, &minute_max, &sec_max);
            _RecordChar_pace2MinuteSec(paceMin, &minute_min, &sec_min);
            _RecordChar_pace2MinuteSec(paceAvg, &minute_avg, &sec_avg);

            if (minute_max > 0 || sec_max > 0)
            {
                hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString),
                            "%d\'%02d", minute_max, sec_max);
            }

            if (minute_avg > 0 || sec_avg > 0)
            {
                hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString),
                            "%d\'%02d", minute_avg, sec_avg);
            }

            for (uint32_t n = 0; n < sportChartData.dataNum; n++)
            {
                if (pos[n] != -1)
                {
                    pos[n] = 1000 - pos[n];
                }
            }

            break;

        case SPORT_RECORD_CHART_SPEED:
            strncpy(chartInfo->title, System_getLocaleText("id_rt_chart_speed"),
                    sizeof(chartInfo->title));
            strncpy(chartInfo->avgNameString,
                    System_getLocaleText("id_sportrecord_char_avg_title"),
                    sizeof(chartInfo->avgNameString));
            strncpy(chartInfo->unitString,
                    System_getLocaleText("id_sport_chart_speed_matric_unit"),
                    sizeof(chartInfo->unitString));

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace > 0.0f)
            {
                float speed = (1 / sportRecordDetailData.recordDetailDate.sport_summary_message.pace.bestPace * 3.6);
                speed = record_speed(speed);
                hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%0.2f", speed);
            }
            else
            {
                hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%0.2f", 0.0f);
            }

            if (sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace > 0.0f)
            {
                float speed = (1 / sportRecordDetailData.recordDetailDate.sport_summary_message.pace.averagePace * 3.6);
                speed = record_speed(speed);
                hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString), "%0.2f", speed);
            }
            else
            {
                hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString), "%0.2f", 0.0f);
            }

            float topSpeed = sportChartData.dataMax / 100.0f;
            topSpeed = record_speed(topSpeed);
            hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->maxString), "%0.2f", topSpeed);

            float midSpeed = (sportChartData.dataMax + sportChartData.dataMin) / 200.0f;
            midSpeed = record_speed(midSpeed);
            hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->maxString), "%0.2f", midSpeed);

            float btmSpeed = sportChartData.dataMin / 100.0f;
            btmSpeed = record_speed(btmSpeed);
            hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->maxString), "%0.2f", btmSpeed);
            break;

        case SPORT_RECORD_CHART_ALTITUDE:
            strncpy(chartInfo->title, System_getLocaleText("id_rt_chart_altitude"),
                    sizeof(chartInfo->title));
            strncpy(chartInfo->avgNameString,
                    System_getLocaleText("id_sportrecord_char_avg_title"),
                    sizeof(chartInfo->avgNameString));
            strncpy(chartInfo->unitString,
                    System_getLocaleText("id_sport_chart_altitude_matric_unit"),
                    sizeof(chartInfo->unitString));
            hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.highestAltitude /
                        100);
            hm_snprintf(chartDescrip->minString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.lowestAltitude /
                        100);
            hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.altitude.averageAltitude /
                        100);
            hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportChartData.dataMax);
            hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->maxString), "%d",
                        (int)(sportChartData.dataMax + sportChartData.dataMin) / 2);
            hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportChartData.dataMin);
            break;

        case SPORT_RECORD_CHART_ROW_STROKE_FREQ:
            strncpy(chartInfo->title, System_getLocaleText("id_rt_chart_stroke_frep"),
                    sizeof(chartInfo->title));
            strncpy(chartInfo->avgNameString,
                    System_getLocaleText("id_sportrecord_char_avg_title"),
                    sizeof(chartInfo->avgNameString));
            strncpy(chartInfo->unitString,
                    System_getLocaleText("id_rt_chart_stroke_frep_unit"),
                    sizeof(chartInfo->unitString));
            hm_snprintf(chartDescrip->maxString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.rowing.highestRowingFrequency);
            hm_snprintf(chartDescrip->avgString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportRecordDetailData.recordDetailDate.sport_summary_message.rowing.averageRowingFrequency);
            hm_snprintf(chartDescrip->topString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportChartData.dataMax);
            hm_snprintf(chartDescrip->midString, sizeof(chartDescrip->maxString), "%d",
                        (int)(sportChartData.dataMax + sportChartData.dataMin) / 2);
            hm_snprintf(chartDescrip->btmString, sizeof(chartDescrip->maxString), "%d",
                        (int)sportChartData.dataMin);
            break;

        default:
            break;
    }

    chartInfo->lineWidth = CHART_LINE_WIDTH;
    chartInfo->attr = CHART_FOR_AFTER_SPORT;
    chartInfo->lineColor = chartColor[colorType][0];
    chartInfo->bgColor = chartColor[colorType][1];
    chartInfo->topColor = chartColor[colorType][0];
    chartInfo->btmColor = chartColor[colorType][1];
    sportChart->initInfo(sportChart, chartInfo);
    H_FREE(chartInfo);
    sportChart->initCanvas(sportChart, index);
    uint32_t interval =
        sportRecordDetailData.recordDetailDate.sport_summary_message.times.exerciseTime;

    if (interval / 3600)
    {
        hm_snprintf(chartDescrip->rightString, sizeof(chartDescrip->rightString),
                    "%d:%02d:%02d", (int)interval / 3600,
                    (int)(interval % 3600) / 60, (int)interval % 60);
    }
    else
    {
        hm_snprintf(chartDescrip->rightString, sizeof(chartDescrip->rightString),
                    "%d:%02d", (int)interval / 60,
                    (int)interval % 60);
    }

    hm_snprintf(chartDescrip->leftString, sizeof(chartDescrip->leftString), "0:00");
    sportChart->initDescrip(sportChart, chartDescrip);
    hm_mem_free(chartDescrip);
    addContainer(getScreenContext(getScreen(SCREEN_NAME)), (char *)chartID[index],
                 sportChart);
};

static void detail_char_draw(ScreenContext *screenContext, int16_t *pos,
                             uint32_t dataLength, float max, float min, uint8_t index)
{
    HLOG_I(TAG, "chart-%d, len : %d\r\n", index, dataLength);
    sportChart_t *sportChart = (sportChart_t *)findContainer(screenContext,
                               (char *)chartID[index]);
    point_t *array = sportChart->convertArray(sportChart, pos, dataLength,
                     dataLength > 0 ? true : false);
    sportChart->fresh(sportChart, array);
};

// static void log_char_info(SportRecordDetailCharData sportChartData){
//     for (int i = 0; i < 24; i++){
//         HLOG_I(TAG,"BBBBBBBBBB %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
//             " %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
//             " %d %d %d %d %d %d %d %d %d %d",
//             sportChartData.pos[50 * i + 0], sportChartData.pos[50 * i + 1], sportChartData.pos[50 * i + 2], sportChartData.pos[50 * i + 3], sportChartData.pos[50 * i + 4],
//             sportChartData.pos[50 * i + 5], sportChartData.pos[50 * i + 6], sportChartData.pos[50 * i + 7], sportChartData.pos[50 * i + 8], sportChartData.pos[50 * i + 9],
//             sportChartData.pos[50 * i + 10], sportChartData.pos[50 * i + 11], sportChartData.pos[50 * i + 12], sportChartData.pos[50 * i + 13], sportChartData.pos[50 * i + 14],
//             sportChartData.pos[50 * i + 15], sportChartData.pos[50 * i + 16], sportChartData.pos[50 * i + 17], sportChartData.pos[50 * i + 18], sportChartData.pos[50 * i + 19],
//             sportChartData.pos[50 * i + 20], sportChartData.pos[50 * i + 21], sportChartData.pos[50 * i + 22], sportChartData.pos[50 * i + 23], sportChartData.pos[50 * i + 24],
//             sportChartData.pos[50 * i + 25], sportChartData.pos[50 * i + 26], sportChartData.pos[50 * i + 27], sportChartData.pos[50 * i + 28], sportChartData.pos[50 * i + 29],
//             sportChartData.pos[50 * i + 30], sportChartData.pos[50 * i + 31], sportChartData.pos[50 * i + 32], sportChartData.pos[50 * i + 33], sportChartData.pos[50 * i + 34],
//             sportChartData.pos[50 * i + 35], sportChartData.pos[50 * i + 36], sportChartData.pos[50 * i + 37], sportChartData.pos[50 * i + 38], sportChartData.pos[50 * i + 39],
//             sportChartData.pos[50 * i + 40], sportChartData.pos[50 * i + 41], sportChartData.pos[50 * i + 42], sportChartData.pos[50 * i + 43], sportChartData.pos[50 * i + 44],
//             sportChartData.pos[50 * i + 45], sportChartData.pos[50 * i + 46], sportChartData.pos[50 * i + 47], sportChartData.pos[50 * i + 48], sportChartData.pos[50 * i + 49]);
//     }
// }

static void detail_draw_sport_chart(SportRecordDetailData sportRecordDetailData)
{
    uint32_t chartColor0 = 0;
    uint32_t chartColor1 = 0;
    uint32_t chartType0 = 0xFFFFFFFF;
    uint32_t chartType1 = 0xFFFFFFFF;
    SportRecordDetailCharData *chartData0 = NULL;
    SportRecordDetailCharData *chartData1 = NULL;
    void (*getRecordDetailCharData0)(SportRecordDetailPresenter * this,
                                     uint32_t charType, uint32_t track_id, uint32_t child_track_id) = NULL;
    void (*getRecordDetailCharData1)(SportRecordDetailPresenter * this,
                                     uint32_t charType, uint32_t track_id, uint32_t child_track_id) = NULL;
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    sport_type_t sportTypet = (sport_type_t)data->recordParameterData.sport_type;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING: /* 户外跑 */
        case SPORT_GROUP_TREADMILL:       /* 跑步机 */
        {
            getRecordDetailCharData0 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharHrData);
            chartType0 = SPORT_RECORD_CHART_HR;
            chartData0 = &data->hr_char_data;
            chartColor0 = CHART_COLOR_RED;
            getRecordDetailCharData1 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharPaceData);
            chartType1 = SPORT_RECORD_CHART_PACE;
            chartData1 = &data->pace_char_data;
            chartColor1 = CHART_COLOR_YELLOW;
            break;
        }

        case SPORT_GROUP_WALKING:              /* 健走 */
        case SPORT_GROUP_INDOOR_WALKING:       /* 室内步行 */
        case SPORT_GROUP_OUTDOOR_CYCLING:      /* 户外骑行 */
        case SPORT_GROUP_1:                    /* 类型1 */
        case SPORT_GROUP_CROSS_COUNTRY_SKIING: /* 越野滑雪 */
        case SPORT_GROUP_SOCCER:               /* 足球 */
        case SPORT_GROUP_DRIVING:              /* 驾车 */
        {
            getRecordDetailCharData0 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharHrData);
            chartType0 = SPORT_RECORD_CHART_HR;
            chartData0 = &data->hr_char_data;
            chartColor0 = CHART_COLOR_RED;
            getRecordDetailCharData1 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharSpeedData);
            chartType1 = SPORT_RECORD_CHART_SPEED;
            chartData1 = &data->speed_char_data;
            chartColor1 = CHART_COLOR_YELLOW;
            break;
        }

        case SPORT_GROUP_INDOOR_CYCLING:    /* 室内骑行 */
        case SPORT_GROUP_2:                 /* 类型2 */
        case SPORT_GROUP_3:                 /* 类型3 */
        case SPORT_GROUP_STRENGTH_TRAINING: /* 力量训练 */
        case SPORT_GROUP_INDOOR_FITNESS:    /* 室内健身 */
        case SPORT_GROUP_ROPE_SKIPPING:     /* 跳绳 */
        case SPORT_GROUP_STAIR_CLIMBING:    /* 爬楼 */
        {
            getRecordDetailCharData0 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharHrData);
            chartType0 = SPORT_RECORD_CHART_HR;
            chartData0 = &data->hr_char_data;
            chartColor0 = CHART_COLOR_RED;
            break;
        }

        case SPORT_GROUP_POOL_SWIMMING:       /* 泳池游泳 */
        case SPORT_GROUP_OPEN_WATER_SWIMMING: /* 公开水域游泳 */
        {
            getRecordDetailCharData0 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharHrData);
            chartType0 = SPORT_RECORD_CHART_HR;
            chartData0 = &data->hr_char_data;
            chartColor0 = CHART_COLOR_RED;
            getRecordDetailCharData1 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharSwimStrokeSpeedData);
            chartType1 = SPORT_RECORD_CHART_SWIM_STROKE_SPEED;
            chartData1 = &data->swim_stroke_speed_char_data;
            chartColor1 = CHART_COLOR_GREEN;
            break;
        }

        case SPORT_GROUP_MOUNTAINEERING: /* 登山 */
        case SPORT_GROUP_TRAIL_RUNNING:  /* 越野跑 */
        case SPORT_GROUP_SKIING:         /* 滑雪 */
        case SPORT_GROUP_PARASAILING:    /* 滑翔伞 */
        {
            getRecordDetailCharData0 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharHrData);
            chartType0 = SPORT_RECORD_CHART_HR;
            chartData0 = &data->hr_char_data;
            chartColor0 = CHART_COLOR_RED;
            getRecordDetailCharData1 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharAltituleData);
            chartType1 = SPORT_RECORD_CHART_ALTITUDE;
            chartData1 = &data->altitule_char_data;
            chartColor1 = CHART_COLOR_BLUE;
            break;
        }

        case SPORT_GROUP_ROWING_MACHINE: /* 划船机 */
        case SPORT_GROUP_OUTDOOR_ROWING: /* 户外划船 */
        {
            getRecordDetailCharData0 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharHrData);
            chartType0 = SPORT_RECORD_CHART_HR;
            chartData0 = &data->hr_char_data;
            chartColor0 = CHART_COLOR_RED;
            getRecordDetailCharData1 = CALL_FINAL_FUNCTION(sportRecordDetailPresenter,
                                       SportRecordDetailPresenter, getRecordDetailCharRowStrokeFreqData);
            chartType1 = SPORT_RECORD_CHART_ROW_STROKE_FREQ;
            chartData1 = &data->row_stroke_freq_char_data;
            chartColor1 = CHART_COLOR_GREEN;
            break;
        }

        case SPORT_GROUP_TENNIS: /* 网球 */
            break;

        case SPORT_GROUP_TRIATHLON: /* 铁人三项 （不支持实时数据配置） */
            break;

        case SPORT_GROUP_MULTISPORT: /* 复合运动 （不支持实时数据配置） */
            break;

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

    if (getRecordDetailCharData0)
    {
        getRecordDetailCharData0(sportRecordDetailPresenter, chartType0,
                                 data->recordParameterData.track_id, data->recordParameterData.child_trcak_id);
        detail_chart_create(chartColor0, chartData0->sportChartData, chartType0,
                            chartData0->pos,
                            sportRecordDetailData, 0);
        detail_char_draw(getScreenContext(getScreen(SCREEN_NAME)), chartData0->pos,
                         chartData0->sportChartData.dataNum, chartData0->sportChartData.dataMax,
                         chartData0->sportChartData.dataMin, 0);
    }

    if (getRecordDetailCharData1)
    {
        getRecordDetailCharData1(sportRecordDetailPresenter, chartType1,
                                 data->recordParameterData.track_id, data->recordParameterData.child_trcak_id);
        detail_chart_create(chartColor1, chartData1->sportChartData, chartType1,
                            chartData1->pos,
                            sportRecordDetailData, 1);
        detail_char_draw(getScreenContext(getScreen(SCREEN_NAME)), chartData1->pos,
                         chartData1->sportChartData.dataNum, chartData1->sportChartData.dataMax,
                         chartData1->sportChartData.dataMin, 1);
    }
}
// static void detail_refresh_fb_data(SportRecordDetailData sportRecordDetailData)
// {

// if (sportRecordDetailData.recordDetailDate.sport_summary_message.other.aerobicTrainingEffect > 0.0f || sportRecordDetailData.recordDetailDate.sport_summary_message.other.anaerobicTrainingEffect > 0.0f || sportRecordDetailData.recordDetailDate.sport_summary_message.other.trainingLoadOfThisActivity > 0)
// {
//     recordHPageData.pageNum += 1;
//     recordHPageData.isTrainingEff = true;
//     FB_refTe(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext, "SportRecordTrainingEffeLayer", sportRecordDetailData.recordDetailDate.sport_summary_message);
// }
// }

static void print_summary_data(SportRecordDetailData sportRecordDetailData)
{
    SportSummaryMessage sportSummaryMessage =
        sportRecordDetailData.recordDetailDate.sport_summary_message;
    HLOG_I(TAG,
           "TypeMessage t:%d s:%d DistanceMessage d:%f NumberMessage t:%d t:%d n:%d\r\n",
           sportSummaryMessage.type.type, sportSummaryMessage.type.subtype,
           sportSummaryMessage.distance.distance,
           sportSummaryMessage.number.totalNumberOfJumps,
           sportSummaryMessage.number.totalTimesOfRowing,
           sportSummaryMessage.number.numberOfBreak);
    HLOG_I(TAG,
           "PressureMessage a:%d m:%d m:%d DownhillMessage t:%d t:%d m:%d t:%d\r\n",
           sportSummaryMessage.pressure.averagePressure,
           sportSummaryMessage.pressure.maximumPressure,
           sportSummaryMessage.pressure.minimumPressure,
           sportSummaryMessage.downhill.timesOfDownhill,
           sportSummaryMessage.downhill.totalDistanceOfDownhill,
           sportSummaryMessage.downhill.maximumDownhillDrop,
           sportSummaryMessage.downhill.totalDownhillDrop);
    HLOG_I(TAG, "TimeMessage t:%d e:%d p:%d r:%d s:%d c:%d r:%d s:%d s:%d \r\n",
           sportSummaryMessage.times.totalTime, sportSummaryMessage.times.exerciseTime,
           sportSummaryMessage.times.pauseTime,
           sportSummaryMessage.times.restTimeBetweenGroups,
           sportSummaryMessage.times.swimmingTime, sportSummaryMessage.times.cyclingTime,
           sportSummaryMessage.times.runningTime, sportSummaryMessage.times.switchTime1,
           sportSummaryMessage.times.switchTime2);
    HLOG_I(TAG,
           "MarathonMessage t:%d t:%d d:%d FloorsMessage u:%d d:%d f:%f d:%f a:%f\r\n",
           sportSummaryMessage.marathon->type, sportSummaryMessage.marathon->timeUsage,
           sportSummaryMessage.marathon->distance,
           sportSummaryMessage.floors.upstairFloors,
           sportSummaryMessage.floors.downstairFloors,
           sportSummaryMessage.floors.upstairHeight,
           sportSummaryMessage.floors.downstairHeight,
           sportSummaryMessage.floors.averageFloorsClimbedPerMinute);
    HLOG_I(TAG, "SwimMessage m:%d t:%d t:%d a:%d s:%d a:%d m:%d p:%d u:%d\r\n",
           sportSummaryMessage.swim.mainSwimmingStyle, sportSummaryMessage.swim.trips,
           sportSummaryMessage.swim.totalStrokes,
           sportSummaryMessage.swim.averageDistancePerStroke,
           sportSummaryMessage.swim.swolf, sportSummaryMessage.swim.averageStrokeRate,
           sportSummaryMessage.swim.maximumStrokeRate, sportSummaryMessage.swim.poolLength,
           sportSummaryMessage.swim.unit);
    HLOG_I(TAG,
           "PaceMessage a:%f b:%f StepsMessage a:%f m:%f a:%d s:%d AltitudeMessage h:%d l:%d a:%d t:%d e:%d e:%d\r\n",
           sportSummaryMessage.pace.averagePace, sportSummaryMessage.pace.bestPace,
           sportSummaryMessage.steps.averageCadence,
           sportSummaryMessage.steps.maximumCadence,
           sportSummaryMessage.steps.averageStride, sportSummaryMessage.steps.steps,
           sportSummaryMessage.altitude.highestAltitude,
           sportSummaryMessage.altitude.lowestAltitude,
           sportSummaryMessage.altitude.averageAltitude,
           sportSummaryMessage.altitude.totalClimbDistance,
           sportSummaryMessage.altitude.elevationGain,
           sportSummaryMessage.altitude.elevationLoss);
    HLOG_I(TAG,
           "SlopeMessage a:%d b:%d SWingMessage t:%d t:%d t:%d s:%d f:%d b:%d p:%d\r\n",
           sportSummaryMessage.slope.averageSlope, sportSummaryMessage.slope.maximumSlope,
           sportSummaryMessage.swing.totalTimesOfSwing,
           sportSummaryMessage.swing.timesOfForehandSwing,
           sportSummaryMessage.swing.timesOfBackhandSwing,
           sportSummaryMessage.swing.serveTimes,
           sportSummaryMessage.swing.forehandPercentage,
           sportSummaryMessage.swing.backhandPercentage,
           sportSummaryMessage.swing.percentageOfServeTimes);
    HLOG_I(TAG,
           "CaloriesMessage c:%d RowingMessag a:%d h:%d a:%f a:%f FrequencyMessage a:%d h:%\r\n",
           sportSummaryMessage.calories.cal,
           sportSummaryMessage.rowing.averageRowingFrequency,
           sportSummaryMessage.rowing.highestRowingFrequency,
           sportSummaryMessage.rowing.averagePullingTime,
           sportSummaryMessage.rowing.averagePaddleTime,
           sportSummaryMessage.frequency.averageFrequency,
           sportSummaryMessage.frequency.highestFrequency);
    HLOG_I(TAG,
           "HeartRateMessage a:%d m:%d m:%d TempMessage a:%f h:%f l:%f OtherMessage t:%d m:%d m:%d a:%f a:%f t:%d v:%d\r\n",
           sportSummaryMessage.heartRate.averageHeartRate,
           sportSummaryMessage.heartRate.maximumHeartRate,
           sportSummaryMessage.heartRate.minimumHeartRate,
           sportSummaryMessage.temp.averageTemperature,
           sportSummaryMessage.temp.highestTemperature,
           sportSummaryMessage.temp.lowestTemperature,
           sportSummaryMessage.other.totalGroups, sportSummaryMessage.other.maximumSpO2,
           sportSummaryMessage.other.minimumSpO2,
           sportSummaryMessage.other.aerobicTrainingEffect,
           sportSummaryMessage.other.anaerobicTrainingEffect,
           sportSummaryMessage.other.trainingLoadOfThisActivity,
           sportSummaryMessage.other.VO2max);
    HLOG_I(TAG,
           "HeartrateZoneMessage t:%d h:%d z1:%d z2:%d z3:%d z4:%d z5:%d SlopeChartMessage u:%d d:%d u:%d d:%d\r\n",
           sportSummaryMessage.hrZone.type, sportSummaryMessage.hrZone.heart,
           sportSummaryMessage.hrZone.zone[1], sportSummaryMessage.hrZone.zone[2],
           sportSummaryMessage.hrZone.zone[3], sportSummaryMessage.hrZone.zone[4],
           sportSummaryMessage.hrZone.zone[5],
           sportSummaryMessage.slopeChart.uphillDistance,
           sportSummaryMessage.slopeChart.downhillDistance,
           sportSummaryMessage.slopeChart.uphillTime,
           sportSummaryMessage.slopeChart.downhillTime);
    HLOG_I(TAG, "GoalMessage t%d d:%f\r\n", sportSummaryMessage.Goal.type,
           sportSummaryMessage.Goal.data);
}

static void detail_refresh_data(SportRecordDetailData sportRecordDetailData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data == NULL)
    {
        HLOG_E(TAG, "%s : data is null", __func__);
        return;
    }

    print_summary_data(sportRecordDetailData);
    data->recordHPageData.pageNum = 2;
    data->recordHPageData.isDetail = true;
    data->recordHPageData.isClose = true;
    print_sport_record_detail_data(sportRecordDetailData);
    char buff[51];
    memset(buff, 0, sizeof(buff));
    strncpy(buff, refreshLayers[0], sizeof(buff));
    detail_refresh_main_data(buff, sportRecordDetailData);
    memset(buff, 0, sizeof(buff));
    strncpy(buff, refreshLayers[1], sizeof(buff));
    detail_refresh_assist_first_data(buff, sportRecordDetailData);
    memset(buff, 0, sizeof(buff));
    strncpy(buff, refreshLayers[2], sizeof(buff));
    detail_refresh_assist_second_data(buff, sportRecordDetailData);
    memset(buff, 0, sizeof(buff));
    strncpy(buff, refreshLayers[3], sizeof(buff));
    detail_refresh_HHZ_data(buff, sportRecordDetailData);
    detail_draw_sport_chart(sportRecordDetailData);
    // detail_refresh_char_data(sportRecordDetailData);
    // detail_refresh_fb_data(sportRecordDetailData);
    record_track_paint(sportRecordDetailData.track);

    if (sportRecordDetailData.recordDetailDate.sport_summary_message.Goal.type != 0)
    {
        data->recordHPageData.pageNum += 1;
        data->recordHPageData.isGoal = true;
        detail_goal_layer_create(sportRecordDetailData);
    }

    if (sportRecordDetailData.recordDetailDate.sport_summary_message.other.aerobicTrainingEffect
            > 0.0f || sportRecordDetailData.recordDetailDate.sport_summary_message.other.anaerobicTrainingEffect
            > 0.0f || sportRecordDetailData.recordDetailDate.sport_summary_message.other.trainingLoadOfThisActivity
            > 0.0f)
    {
        data->recordHPageData.pageNum += 1;
        data->recordHPageData.isTrainingEff = true;
        FB_refTe(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext,
                 "SportRecordTrainingEffeLayer",
                 sportRecordDetailData.recordDetailDate.sport_summary_message.other.aerobicTrainingEffect,
                 sportRecordDetailData.recordDetailDate.sport_summary_message.other.anaerobicTrainingEffect,
                 sportRecordDetailData.recordDetailDate.sport_summary_message.other.trainingLoadOfThisActivity);
    }
    if (data->recordParameterData.from_record_detail == FROM_SPORT_END_DETAIL && data->isDisplayVoice == false)
    {
        /* 语音播报：本次运动 */
        sportRecordVoicePlay(SPORT_VOICE_RECORD_THISSPORT, 0.0, 0, 0);
        /* 播放其它运动数据语音信息 */
        sportEndRedordVoiceDataSet(data->recordParameterData.sport_type);
        sportVoice_broadQueueEnd(); /* 多条语音播报结束 */
        data->isDisplayVoice = true;
    }
};

static void detail_char_hr_data_pos(SportRecordDetailCharData sportChartData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->hr_char_data = sportChartData;
};
static void detail_char_pace_data_pos(SportRecordDetailCharData sportChartData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->pace_char_data = sportChartData;
};
static void detail_char_speed_data_pos(SportRecordDetailCharData sportChartData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->speed_char_data = sportChartData;
};
static void detail_char_swim_stroke_speed_data_pos(SportRecordDetailCharData
        sportChartData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->swim_stroke_speed_char_data = sportChartData;
};
static void detail_char_altitule_data_pos(SportRecordDetailCharData
        sportChartData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->altitule_char_data = sportChartData;
};
static void detail_char_row_stroke_freq_data_pos(SportRecordDetailCharData
        sportChartData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->row_stroke_freq_char_data = sportChartData;
};
static void detail_segemnt_detail(record_detail_segment_group_data
                                  *recordDetailSegmentGroupData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->record_detail_segment_group_detail = recordDetailSegmentGroupData;

    if (data->record_detail_segment_group_detail->segment > 1
            || data->record_detail_segment_group_detail->grouping > 1
            || data->record_detail_segment_group_detail->lesson > 1)
    {
        data->recordHPageData.pageNum += 1;
        data->recordHPageData.isSegment = true;
        data->isHaveSegment = true;
        detail_segemnt_layer_creata();
    }
}

static void detail_pb_data(sport_record_detail_pb_data *sportRecordDetailPbData)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    data->sport_Record_Detail_Pb_Data = sportRecordDetailPbData;
    HLOG_I(TAG, "%s pbnum:%d\r\n", __func__,
           data->sport_Record_Detail_Pb_Data->pbNum);
    data->recordHPageData.pageNum = data->recordHPageData.pageNum +
                                    data->sport_Record_Detail_Pb_Data->pbNum;

    for (int i = 0; i < sportRecordDetailPbData->pbNum; i++)
    {
        switch (i)
        {
            case 0:
                data->recordHPageData.isPersonalRecordFirst = true;
                break;

            case 1:
                data->recordHPageData.isPersonalRecordSeocnd = true;
                break;

            case 2:
                data->recordHPageData.isPersonalRecordThree = true;
                break;

            case 3:
                data->recordHPageData.isPersonalRecordFour = true;
                break;

            case 4:
                data->recordHPageData.isPersonalRecordFive = true;
                break;

            case 5:
                data->recordHPageData.isPersonalRecordSix = true;
                break;

            case 6:
                data->recordHPageData.isPersonalRecordSenven = true;
                break;

            default:
                break;
        }
    }

    if(data->isDisplayVoice == false &&
    (data->recordHPageData.isPersonalRecordFirst == true ||
       data->recordHPageData.isPersonalRecordSeocnd == true ||
       data->recordHPageData.isPersonalRecordThree == true ||
       data->recordHPageData.isPersonalRecordFour == true ||
       data->recordHPageData.isPersonalRecordFive == true ||
       data->recordHPageData.isPersonalRecordSix == true ||
       data->recordHPageData.isPersonalRecordSenven == true ))
       {
            /* 语音播报：恭喜你创造了新的个人记录*/
            sportRecordVoicePlay(SPORT_VOICE_RECORD_PERSONALRECORD, 0.0, 0, 0);
            /* 语音播报：接下来放松一下吧*/
            sportRecordVoicePlay(SPORT_VOICE_RECORD_RELAXNEXT, 0.0, 0, 0);
       }
    RrcordPb_refPbData(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext,
                       sportRecordDetailPbData, data->recordParameterData.sport_type);
#if HMI_COMP_STATISTICS
    _ss_buried_point(0, SPORT_REC_NEW_PB_EID, H_NULL);
#endif
}
static bool is_have_pb(uint16_t sport_type)
{
    sport_type_t sportTypet = (sport_type_t)sport_type;
    sport_group_t group = sportGroup_get(sportTypet);

    switch (group)
    {
        case SPORT_GROUP_OUTDOOR_RUNNING:
        case SPORT_GROUP_OUTDOOR_CYCLING:
        case SPORT_GROUP_MOUNTAINEERING:
            return true;

        default:
            break;
    }

    return false;
}

static void detail_recovery_Time(SportEffectMessage
                                 *sportQuantificationResult)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->recordParameterData.from_record_detail == FROM_SPORT_END_DETAIL)
    {
        if (sportQuantificationResult->lastvalid_vo2max_fix_value > 0
                && (sportQuantificationResult->sport_end_utc_time ==
                    sportQuantificationResult->lastvalid_vo2max_utc_time))
        {
            data->recordHPageData.pageNum += 1;
            data->recordHPageData.isMaximalOxygenUptake = true;
            FB_refMaxOv2(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext,
                         "SportRecordVomaxLayer", sportQuantificationResult->lastvalid_vo2max_fix_value,
                         sportQuantificationResult->lastvalid_vo2max_utc_time,
                         sportQuantificationResult->if_valid_vo2max_raise);
        }

        if (_sport_calculate_recovery_time(sportQuantificationResult->Recovery_Time,
                                           sportQuantificationResult->sport_end_utc_time) > 0)
        {
            gre_set_layer_yoffset(getAppInstance(),
                                  "SportRecordDetailScreen.SportRecordDetailRecoveryTimeLayer",
                                  0);
            data->recordHPageData.pageNum += 1;
            data->recordHPageData.isRecoveryTime = true;
            FB_refRecoveryTime2(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext,
                                "SportRecordDetailRecoveryTimeLayer", sportQuantificationResult->Recovery_Time,
                                sportQuantificationResult->sport_end_utc_time);
        }
    }
}
static SportRecordDetailCallback sportRecordDetailCallback =
{
    .detail_refresh_data = detail_refresh_data,
    .detail_track_preload = record_track_preload,
    .detail_track_paint = record_track_paint,
    .detail_char_hr_data_pos = detail_char_hr_data_pos,
    .detail_char_pace_data_pos = detail_char_pace_data_pos,
    .detail_char_speed_data_pos = detail_char_speed_data_pos,
    .detail_char_swim_stroke_speed_data_pos = detail_char_swim_stroke_speed_data_pos,
    .detail_char_altitule_data_pos = detail_char_altitule_data_pos,
    .detail_char_row_stroke_freq_data_pos = detail_char_row_stroke_freq_data_pos,
    .detail_segemnt_detail = detail_segemnt_detail,
    .detail_pb_data = detail_pb_data,
    .detail_recovery_Time = detail_recovery_Time,
};
static void _initData(ScreenContext *screenContext)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    sportRecordDetailPresenter = SportRecordDetailPresenter_new();
    Presenter *basePresenter = SUPER_PTR(sportRecordDetailPresenter, Presenter);
    CALL_FUNCTION(basePresenter, Presenter, init)
    (basePresenter);
    CALL_FINAL_FUNCTION(sportRecordDetailPresenter, SportRecordDetailPresenter,
                        setCallback)
    (sportRecordDetailPresenter, &sportRecordDetailCallback);
    setScreenPresenter(screenContext, sportRecordDetailPresenter);

    if (is_have_pb(appData->recordParameterData.sport_type)
            && (appData->recordParameterData.from_record_detail == FROM_SPORT_END_DETAIL))
    {
        CALL_FINAL_FUNCTION(sportRecordDetailPresenter, SportRecordDetailPresenter,
                            getRecordPbData)
        (sportRecordDetailPresenter, appData->recordParameterData.track_id,
         appData->recordParameterData.child_trcak_id);
    }

    CALL_FINAL_FUNCTION(sportRecordDetailPresenter, SportRecordDetailPresenter,
                        getRecordDetail)
    (sportRecordDetailPresenter, appData->recordParameterData.track_id,
     appData->recordParameterData.child_trcak_id,
     appData->recordParameterData.sport_type);
    CALL_FINAL_FUNCTION(sportRecordDetailPresenter, SportRecordDetailPresenter,
                        getRecordSegmentDetail)
    (sportRecordDetailPresenter, appData->recordParameterData.track_id,
     appData->recordParameterData.child_trcak_id);

#if USE_HUAMI_SPORT_QUANTIFICATION
    CALL_FINAL_FUNCTION(sportRecordDetailPresenter, SportRecordDetailPresenter,
                        getRecordRecoverTime)
    (sportRecordDetailPresenter, appData->recordParameterData.track_id,
     appData->recordParameterData.child_trcak_id);
#endif
    detail_h_multi_layer_create();
    detail_multi_scroll_bar_create();
    detail_schedule_layer_create(appData->hPageNum);
    detail_multi_layer_create();
    // creat_auto_page();
    creat_inter_text();
}
static void destory(ScreenContext *screenContext)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->hr_char_data.pos != NULL)
    {
        H_FREE(data->hr_char_data.pos);
        data->hr_char_data.pos = NULL;
    }

    if (data->pace_char_data.pos != NULL)
    {
        H_FREE(data->pace_char_data.pos);
        data->pace_char_data.pos = NULL;
    }

    if (data->speed_char_data.pos != NULL)
    {
        H_FREE(data->speed_char_data.pos);
        data->speed_char_data.pos = NULL;
    }

    if (data->swim_stroke_speed_char_data.pos != NULL)
    {
        H_FREE(data->swim_stroke_speed_char_data.pos);
        data->swim_stroke_speed_char_data.pos = NULL;
    }

    if (data->altitule_char_data.pos != NULL)
    {
        H_FREE(data->altitule_char_data.pos);
        data->altitule_char_data.pos = NULL;
    }

    if (data->row_stroke_freq_char_data.pos != NULL)
    {
        H_FREE(data->row_stroke_freq_char_data.pos);
        data->row_stroke_freq_char_data.pos = NULL;
    }

    if (data->record_detail_segment_group_detail != NULL
            && data->record_detail_segment_group_detail->record_detail_segment_data != NULL)
    {
        H_FREE(data->record_detail_segment_group_detail->record_detail_segment_data);
        data->record_detail_segment_group_detail->record_detail_segment_data = NULL;
    }

    if (data->sport_Record_Detail_Pb_Data != NULL
            && data->sport_Record_Detail_Pb_Data->sportPbInfoData != NULL)
    {
        H_FREE(data->sport_Record_Detail_Pb_Data->sportPbInfoData);
        data->sport_Record_Detail_Pb_Data->sportPbInfoData = NULL;
    }

    HLOG_I(TAG, "%s\r\n", __func__);
    SportRecordDetailPresenter *presenter = (SportRecordDetailPresenter *)
                                            getScreenPresenter(screenContext);

    if (presenter)
    {
        //destory and free presenter
        Presenter *basePresenter = SUPER_PTR(presenter, Presenter);
        CALL_VOID(destory, basePresenter);
        freeScreenPresenter(screenContext, SportRecordDetailPresenter);
    }
    sportVoice_StopBroad(); /* 页面销毁时，强制终止语音播放 */
    //free screen
    removeAllContainers(screenContext);
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
#ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_setVisible(H_True);
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sportrecord_title"));
#endif
    HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));
    sport_voice_record_data voicedata; /* 语音播报操作类型数据 */

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }
    else
    {
        HLOG_I(TAG, "%s %s\r\n", __func__, "maclloc fail");
        return;
    }

    _init_struct();

    if (data != NULL)
    {
        memcpy(&appData->recordParameterData, (record_parameter_data *)data,
               sizeof(record_parameter_data));
        HLOG_I(TAG,
               "%s trackId:%d sportType:%d child_trcak_id:%d from_record_detail:%d auto_reg:%d\r\n",
               __func__, appData->recordParameterData.track_id,
               appData->recordParameterData.sport_type,
               appData->recordParameterData.child_trcak_id,
               appData->recordParameterData.from_record_detail,
               appData->recordParameterData.auto_reg);
        /* 多条语音播报开始 ,结束调用在所有语音播报结束后 */
        sportVoice_broadQueueStart();
        memset(&voicedata, 0, sizeof(sport_voice_record_data));
        voicedata.recordtype = SPORT_VOICE_RECORD_SPORTCOMPLETED;
        sportVoice_broadSportRecord(voicedata);  /* 语音播报：运动已完成*/

        _initData(screenContext);
#if HMI_COMP_STATISTICS
        Param parm = {0};
        parm.key = SPORT_TYPE_KEY_ID;
        parm.value =
            appData->recordParameterData.sport_type;
        _ss_buried_point(1, SPORT_REC_DETAIL_EID,  &parm);
#endif
    }
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    SaveData *saveData = (SaveData *)data;
    memcpy(&appData->recordParameterData, &saveData->recordParameterData,
           sizeof(record_parameter_data));
    HLOG_I(TAG,
           "%s trackId:%d sportType:%d child_trcak_id:%d from_record_detail:%d auto_reg:%d\r\n",
           __func__, appData->recordParameterData.track_id,
           appData->recordParameterData.sport_type,
           appData->recordParameterData.child_trcak_id,
           appData->recordParameterData.from_record_detail,
           appData->recordParameterData.auto_reg);
    appData->isDisplayVoice = saveData->isDisplayVoice;
    destory(screenContext);
    _initData(screenContext);
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    System_setKeyFilter(HM_KEY_CODE_SELECT,HM_KEY_ACT_FILTER_CLICK);
    System_setKeyFilter(HM_KEY_CODE_BACK, HM_KEY_ACT_FILTER_CLICK);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    // destory_timer();
    AppData *appData = getScreenDataByName(SCREEN_NAME, AppData);
    SaveData *saveData = H_MALLOC(sizeof(SaveData));
    memcpy(&saveData->recordParameterData, &appData->recordParameterData,
           sizeof(record_parameter_data));
    HLOG_I(TAG,
           "%s trackId:%d sportType:%d child_trcak_id:%d from_record_detail:%d auto_reg:%d\r\n",
           __func__, saveData->recordParameterData.track_id,
           saveData->recordParameterData.sport_type,
           saveData->recordParameterData.child_trcak_id,
           saveData->recordParameterData.from_record_detail,
           saveData->recordParameterData.auto_reg);
    saveData->isDisplayVoice = appData->isDisplayVoice;
    getScreenManager()->saveScreenData("SportRecordDetailScreen", saveData,
                                       sizeof(SaveData));
    H_FREE(saveData);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    // destory_timer();
    destory(screenContext);
    freeScreenData(screenContext);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
}
static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
}
static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    // destory_timer();
    pageScrollerMultiLayers *scroll = (pageScrollerMultiLayers *)findContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_PAGESCROLLER_MULTILAYERS);
    AppData *data = getScreenData(screenContext, AppData);
    if(info->code == HM_KEY_CODE_SELECT){
         if(data->hPageIndex == data->hPageNum){
            data->hPageIndex = 0;
            scroll->scrollToPage(scroll, data->hPageIndex, true);
        }else{
            data->hPageIndex ++;
            scroll->scrollToPage(scroll, data->hPageIndex, true);
        }
        return true;
    }
    if(info->code == HM_KEY_CODE_BACK){
        if (data->recordParameterData.from_record_detail == FROM_SPORT_END_DETAIL)
        {
            getScreenManager()->gotoHome();
        }
        else
        {
            getScreenManager()->finishCurrentScreen();
        }
        return true;
    }
    if(info->code == HM_KEY_CODE_UP || info->code == HM_KEY_CODE_DOWN){
        if (data->isHaveSegment)
        {
            if (data->hPageIndex == data->hPageNum - 3)
            {
                ScreenContainer *pageVScrollPager = findContainer(screenContext,
                                                    ID_SPORT_RECORD_SCROLL);

                if (pageVScrollPager)
                {
                    pageVScrollPager->handleKeyEvent(screenContext, pageVScrollPager,info);
                    return true;
                }
            }

            if (data->hPageIndex == data->hPageNum - 2)
            {
                ScreenContainer *segmentList = findContainer(screenContext, ID_SEGMENT_LAYER);

                if (segmentList)
                {
                    segmentList->handleKeyEvent(screenContext, segmentList, info);
                    return true;
                }
            }
        }
        else
        {
            if (data->hPageIndex == data->hPageNum - 2)
            {
                ScreenContainer *pageVScrollPager = findContainer(screenContext,
                                                    ID_SPORT_RECORD_SCROLL);

                if (pageVScrollPager)
                {
                    pageVScrollPager->handleKeyEvent(screenContext, pageVScrollPager, info);
                    return true;
                }
            }
        }
        return true;
    }
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    // destory_timer();
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->hPageIndex != 0)
    {
        return false;
    }
    else
    {
        if (eventId == GESTURE_RIGHT)
        {
#if HMI_COMP_STATISTICS
            _ss_buried_point(0, SPORT_REC_QUIT_EID, H_NULL);
#endif

            if (data->recordParameterData.from_record_detail == FROM_SPORT_END_DETAIL)
            {
                getScreenManager()->gotoHome();
            }
            else
            {
                getScreenManager()->finishCurrentScreen();
            }
        }
    }

    return true;
}
static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    // destory_timer();
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->isHaveSegment)
    {
        if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
        {
            if (data->hPageIndex == data->hPageNum - 3)
            {
                ScreenContainer *pageVScrollPager = findContainer(screenContext,
                                                    ID_SPORT_RECORD_SCROLL);

                if (pageVScrollPager)
                {
                    pageVScrollPager->handleEvent(screenContext, pageVScrollPager, id, event);
                    return true;
                }
            }

            if (data->hPageIndex == data->hPageNum - 2)
            {
                ScreenContainer *segmentList = findContainer(screenContext, ID_SEGMENT_LAYER);

                if (segmentList)
                {
                    segmentList->handleEvent(screenContext, segmentList, id, event);
                    return true;
                }
            }
        }
    }
    else
    {
        if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
        {
            if (data->hPageIndex == data->hPageNum - 2)
            {
                ScreenContainer *pageVScrollPager = findContainer(screenContext,
                                                    ID_SPORT_RECORD_SCROLL);

                if (pageVScrollPager)
                {
                    pageVScrollPager->handleEvent(screenContext, pageVScrollPager, id, event);
                    return true;
                }
            }
        }
    }

    return false;
}

static bool isTouchEventToScrollerPages(const char *id)
{
    if (strcmp(id, TAG))
    {
        return true;
    }

    pageScrollerMultiLayers *scroll = (pageScrollerMultiLayers *)findContainer(
                                          getScreenContext(getScreen(SCREEN_NAME)), ID_PAGESCROLLER_MULTILAYERS);

    if (scroll)
    {
        if (scroll->isLayerInclude(scroll, id))
        {
            return true;
        }
    }

    return false;
}

typedef enum
{
    PAGEDIRECTIONENUM_NONE,
    PAGEDIRECTIONENUM_HOR,
    PAGEDIRECTIONENUM_VEL,
} pagedirectionenum;
static int g_dragType = PAGEDIRECTIONENUM_NONE;
static int g_pressDown = false;
static SPoint g_pressPos = {.x = 0, .y = 0};

static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    // HLOG_I(TAG, "%s id:%s\r\n", __func__, id);
    bool result = true;
    // destory_timer();
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (eventId == TOUCH_SELECT)
    {
        if (!strcmp(id, ID_CLOSE) || !strcmp(id, ID_CLOSE_ICON))
        {
            if (data->recordParameterData.from_record_detail == FROM_SPORT_END_DETAIL)
            {
                getScreenManager()->gotoHome();
            }
            else
            {
                getScreenManager()->finishCurrentScreen();
            }
        }

        char buffer[256] = {0};
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer),
                 "%s.infoAlpha", id);

        if (!strcmp(id, ID_INDO) && (getSBIntData(getAppInstance(), buffer) == 255))
        {
            uint8_t status = 4;
            getScreenManager()->startScreenByFlag("SportHelpScreen",
                                                  FLAG_SCREEN_STANDAR,
                                                  (void *)&status, sizeof(uint8_t));
        }
    }

    if (isTouchEventToScrollerPages(id))
    {
        ScreenContainer *scroll = (ScreenContainer *)findContainer(getScreenContext(
                                      getScreen(SCREEN_NAME)),
                                  ID_PAGESCROLLER_MULTILAYERS);
        ScreenContainer *vPager = (ScreenContainer *)findContainer(getScreenContext(
                                      getScreen(SCREEN_NAME)),
                                  ID_SPORT_RECORD_SCROLL);

        if (eventId == TOUCH_DOWN)
        {
            g_pressDown = true;
            g_pressPos.x = info->x;
            g_pressPos.y = info->y;
            scroll->handleTouchEvent(getScreenContext(getScreen(SCREEN_NAME)), scroll, id,
                                     eventId, info);
            vPager->handleTouchEvent(getScreenContext(getScreen(SCREEN_NAME)), vPager, id,
                                     eventId, info);
        }
        else if (eventId == TOUCH_MOVE)
        {
            if (g_pressDown)
            {
                int deltaX = info->x - g_pressPos.x;
                int deltsY = info->y - g_pressPos.y;
                float x_abs = abs(deltaX);
                float y_abs = abs(deltsY);

                if (g_dragType == PAGEDIRECTIONENUM_NONE)
                {
                    if (x_abs > SCROLL_DRAG_THREHOLD)
                    {
                        g_dragType = PAGEDIRECTIONENUM_HOR;
                    }

                    if (y_abs > SCROLL_DRAG_THREHOLD)
                    {
                        g_dragType = PAGEDIRECTIONENUM_VEL;
                    }
                }
            }

            if (g_dragType == PAGEDIRECTIONENUM_HOR && data->hPageNum > 1)
            {
                result = false;
                scroll->handleTouchEvent(getScreenContext(getScreen(SCREEN_NAME)), scroll, id,
                                         eventId, info);
            }

            if (g_dragType == PAGEDIRECTIONENUM_VEL
                    && data->hPageIndex == (data->isHaveSegment ? data->hPageNum - 3 :
                                            data->hPageNum - 2))
            {
                result = false;
                vPager->handleTouchEvent(getScreenContext(getScreen(SCREEN_NAME)), vPager, id,
                                         eventId, info);
            }
        }
        else if (eventId == TOUCH_UP)
        {
            if (g_dragType == PAGEDIRECTIONENUM_HOR)
            {
                scroll->handleTouchEvent(getScreenContext(getScreen(SCREEN_NAME)), scroll, id,
                                         eventId, info);
            }

            if (g_dragType == PAGEDIRECTIONENUM_VEL)
            {
                vPager->handleTouchEvent(getScreenContext(getScreen(SCREEN_NAME)), vPager, id,
                                         eventId, info);
            }

            g_dragType = PAGEDIRECTIONENUM_NONE;
            g_pressDown = false;
        }

        // HLOG_I(TAG, "%s g_dragType:%d g_pressDown:%d\r\n", __func__, g_dragType, g_pressDown);
    }

    return result;
}

// ##
static void *record_track_preload(void)
{
    ScreenContext *screen = getScreenContext(getScreen(SCREEN_NAME));
    // ##
    display_area = H_MALLOC(sizeof(display_area_t));
    OS_ASSERT(display_area);
    display_area->x = 9;
    display_area->y = 36;
    display_area->w = gre_get_control_w(screen->_grAppContext,
                                        ID_TRACK_CONTAINER) -
                      9 * 2;
    display_area->h = gre_get_control_h(screen->_grAppContext,
                                        ID_TRACK_CONTAINER) -
                      36 * 2;
    return display_area;
}

// ##
static void record_track_paint(void *data)
{
    if (display_area != NULL)
    {
        H_FREE(display_area);
        display_area = NULL;
    }

    // ##
    multiPolyLine *track_line = createMultiPolyline(getScreenContext(getScreen(
                                    SCREEN_NAME))
                                ->_grAppContext);
    addContainer(getScreenContext(getScreen(SCREEN_NAME)), "track", track_line);
    track_line->create(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext,
                       track_line, ID_TRACK_CONTAINER, "track_canvas", 0);
    track_payload_t *track = (track_payload_t *)data;

    if (!track)
    {
        setSBIntData(getAppInstance(), IMG_TRACK_START_X_ID, -100);
        setSBIntData(getAppInstance(), IMG_TRACK_START_Y_ID, -100);
        setSBIntData(getAppInstance(), IMG_TRACK_END_X_ID, -100);
        setSBIntData(getAppInstance(), IMG_TRACK_END_Y_ID, -100);
        setSBIntData(getAppInstance(), ALPHA_ID, 0);
        HLOG_D(TAG, "track not found\r\n");
        return;
    }
    else
    {
        setSBIntData(getAppInstance(), ALPHA_ID, 255);
    }

    track_line->setLine(track_line,
                        0x000000, // #000000
                        0x17C06E, // #17C06E
                        TRACK_LINE_WIDTH);
    track_point_t start;
    track_point_t end;
    start.x = track->array[0].x;
    start.y = track->array[0].y;
    uint16_t track_cnt = 0;
    track_payload_t *mm = track;

    while (mm != NULL)
    {
        track_payload_t *nn = mm->next;
        track_line->addLine(track_line, (gr_render_float_point_t *)mm->array, mm->cnt,
                            mm == track);
        track_cnt++;
        end.x = mm->array[mm->cnt - 1].x;
        end.y = mm->array[mm->cnt - 1].y;
        HLOG_D(TAG, "track[%d] : %d points\r\n", track_cnt, mm->cnt);
        H_FREE(mm);
        mm = nn;
    }

#define IMG_PAD_OFFSET  getDimension(4)
    int startW = 0;
    int startH = 0;
    gre_get_image_size(getAppInstance(), "images/record/img_track_start.png",
                       &startW, &startH);
    startW = 0;
    int endW = 0;
    int endH = 0;
    gre_get_image_size(getAppInstance(), "images/record/img_track_end.png", &endW,
                       &endH);
    endW = 0;
    start.x += -IMG_PAD_OFFSET;
    start.y += abs(startH - endH);
    end.x += -IMG_PAD_OFFSET;
    end.y += -IMG_PAD_OFFSET;
    // ##
    setSBIntData(getAppInstance(), IMG_TRACK_START_X_ID, (int16_t)start.x);
    setSBIntData(getAppInstance(), IMG_TRACK_START_Y_ID, (int16_t)start.y);
    setSBIntData(getAppInstance(), IMG_TRACK_END_X_ID, (int16_t)end.x);
    setSBIntData(getAppInstance(), IMG_TRACK_END_Y_ID, (int16_t)end.y);
}

static void _init_struct()
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);
    memset(&data->recordParameterData, 0, sizeof(record_parameter_data));
    memset(&data->hr_char_data, 0, sizeof(SportRecordDetailCharData));
    memset(&data->pace_char_data, 0, sizeof(SportRecordDetailCharData));
    memset(&data->speed_char_data, 0, sizeof(SportRecordDetailCharData));
    memset(&data->swim_stroke_speed_char_data, 0,
           sizeof(SportRecordDetailCharData));
    memset(&data->altitule_char_data, 0, sizeof(SportRecordDetailCharData));
    memset(&data->row_stroke_freq_char_data, 0, sizeof(SportRecordDetailCharData));
    memset(&data->recordHPageData, 0, sizeof(record_h_page_data));

    if (data->record_detail_segment_group_detail != NULL)
    {
        memset(data->record_detail_segment_group_detail, 0,
               sizeof(record_detail_segment_group_data));
    }

    if (data->sport_Record_Detail_Pb_Data != NULL)
    {
        memset(data->sport_Record_Detail_Pb_Data, 0,
               sizeof(sport_record_detail_pb_data));
    }

    data->hPageIndex = 0;
    data->hPageNum = 0;
    data->viewHei = 0;
    data->totalHei = 0;
    data->isHaveSegment = false;
    data->isDisplayVoice = false;
}
