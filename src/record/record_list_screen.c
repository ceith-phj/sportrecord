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

#define TAG "sport_record_list"
#include <sb_screen_context.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <presenter/record_list_presenter.h>
#include <hlog.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <gre/sdk/grelib.h>
#include <sport_type_utils.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <record/record_unit_utils.h>
#include <record/record_parameter.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include <record/record_detail_jump_utils.h>
#include <widget/pageScrollerMultiLayers/pageScrollerMultiLayers.h>
#include "core/display_metrics.h"
#include <record/record_parameter.h>
#include <widget/hSchedule/hScedule.h>
#include <widget/alertDialog/alertDialog.h>
#include "sport_type_auto.h"
#include <record/record_text_aglin_utils.h>
#include <hm_system_status_bar.h>
#if HMI_COMP_STATISTICS
#include "hmstatistics.h"
#endif

#define ID_SPORTRECORD_LIST "SportRecordListLayer.SportRecordlist"
#define ID_RECORDPB_LIST "SportRecordPBLayer.Pblist"
#define ID_RECORD_MULTILAYERS "SportRecordListScreen.pageScrollerMultiLayers"
#define ID_LIST_SCHEDULE "SportRecordListScreen.SportListScheduleLayer"
#define PATH_RECORD_NO_LIST "SportRecordListScreen.SportRecordNoListLayer"
#define PATH_RECORD_HAVE_LIST "SportRecordListScreen.SportRecordListLayer"
#define PATH_RECORD_HAVE_PB "SportRecordListScreen.SportRecordPBLayer"
#define PATH_RECORD_HAVE_RESULT_PRED "SportRecordListScreen.SportRecordResultPredLayer"
#define ID_SPORT_RECORD_DIALOG "SportRecordListScreen.AlertDialog"
#define RECORD_SCROLL_DRAG_THREHOLD 20

#define SCREEN_NAME SportRecordList
SCREEN_DECLARE(SCREEN_NAME);

static SportRecordListPresenter *sportRecordListPresenter = NULL;

typedef struct
{
    int x;
    int y;
} SPoint;

typedef struct
{
    int h_pageIndex;
} SaveData;

typedef struct
{
    sport_record_list_new_t sport_record_list_Data;
    child_sports_manager_t child_sports_manager_data;
    int item_number;
    int item_sync_number;
    int h_pageIndex;
    AlertDialog *abandonDialog;
    int deleteId;
} AppData;

static pb_list pb_list_data[] =
{
    {"images/record/pb_list_icon_run.png", "id_sport_type_outdoor_running", SPORT_TYPE_OUTDOOR_RUNNING, 7},
    {"images/record/pb_list_icon_cycle.png", "id_sport_type_outdoor_cycling", SPORT_TYPE_OUTDOOR_CYCLING, 7},
    {"images/record/pb_list_icon_mount.png", "id_sport_type_mountaineering", SPORT_TYPE_MOUNTAINEERING, 4},
};

#if HMI_COMP_STATISTICS
static void _ss_buried_point(uint8_t index, HM_STATIS_EVENT_ID_E value,
                             Param *parm)
{
    hm_statis_data_add_with_kv(value, parm,
                               index,
                               true);
}
#endif

static void sportList_onItemUpdate(unequalHeiList *list, int idx,
                                   const char *itemGroupPath)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);

    if (NULL == list || idx <= 0 || NULL == itemGroupPath
            || appData->item_number == 0)
    {
        return;
    }

    idx = idx - 1;

    if (idx < 0)
    {
        return;
    }

    // HLOG_I(TAG, "%d \r\n", idx);
    char buffer[256] = {0};
    HLOG_I(TAG,
           "%s,exercisetime:%d data:%f time:%d auto_reg:%d sportType:%d sync:%d\r\n",
           __func__, appData->sport_record_list_Data.list_info[idx].time,
           appData->sport_record_list_Data.list_info[idx].main_data,
           appData->sport_record_list_Data.list_info[idx].track_id,
           appData->sport_record_list_Data.list_info[idx].sport_type,
           appData->sport_record_list_Data.list_info[idx].sync);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.imageai", itemGroupPath);

    if (appData->sport_record_list_Data.list_info[idx].auto_reg == 1)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/icon_ai.png");
    }
    else
    {
        setSBStringData(getAppInstance(), buffer, "images/record/icon_ai_bg.png");
    }

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.sync", itemGroupPath);

    if (appData->sport_record_list_Data.list_info[idx].sync == 1)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/sync.png");
    }
    else
    {
        setSBStringData(getAppInstance(), buffer, "images/record/sync_bg.png");
    }

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.icon", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportType_getTypeIconId(
                        appData->sport_record_list_Data.list_info[idx].sport_type));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.exercisetime", itemGroupPath);
    RecordExercisetime_getRecordExercisetime(
        appData->sport_record_list_Data.list_info[idx].time, buffer);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.data", itemGroupPath);
    RecordData_getRecordMainData(
        appData->sport_record_list_Data.list_info[idx].sport_type,
        appData->sport_record_list_Data.list_info[idx].main_data, buffer,
        appData->sport_record_list_Data.list_info[idx].pool_unit);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.time", itemGroupPath);
    RecordTime_getRecordTime(
        appData->sport_record_list_Data.list_info[idx].track_id, buffer);
    // memset(buffer, 0, sizeof(buffer));
    // snprintf(buffer, sizeof(buffer), "%s.item.unit", itemGroupPath);
    // RecordUnit_getRecordMainDataUnit(appData->sport_record_list_Data.list_info[idx].sport_type, buffer);
}
static void sportList_onItemTouch(unequalHeiList *list, int idx,
                                  const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    // HLOG_I(TAG, "%s\r\n", __func__);
    if (NULL == list || idx <= 0)
    {
        return;
    }

    if (eventId != TOUCH_SELECT)
    {
        return;
    }

    singleSportjumpDetail(idx - 1, FLAG_SCREEN_STANDAR,
                          FROM_SPORT_RECORD_LIST_DETAIL);
}
static void delete_record_dialog()
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    uint32_t track_id =
        appData->sport_record_list_Data.list_info[appData->deleteId].track_id;
    HLOG_I(TAG, "%s delete id:%d\r\n", __func__, track_id);
    CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                        deleteSportRecord)
    (sportRecordListPresenter, track_id);
    removeAllContainers(getScreenContext(getScreen(SCREEN_NAME)));
    memset(appData, 0, sizeof(AppData));
    CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                        obtainSportRecordList)
    (sportRecordListPresenter);
#if HMI_COMP_STATISTICS
    Param parm = {0};
    parm.key = SPORT_TYPE_KEY_ID;
    parm.value =
        appData->sport_record_list_Data.list_info[appData->deleteId].sport_type;
    _ss_buried_point(1, SPORT_REC_DELETE_EID, &parm);
#endif
}
static void sportList_longPressed(unequalHeiList *list, int idx,
                                  const char *itemGroupPath)
{
    HLOG_I(TAG, "%s \r\n", __func__);
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);

    if (NULL == list || idx <= 0)
    {
        return;
    }

    appData->abandonDialog->showDialog(appData->abandonDialog);
    appData->deleteId = idx - 1;
}

static void pbList_onItemUpdate(unequalHeiList *list, int idx,
                                const char *itemGroupPath)
{
    if (NULL == list || idx <= 0 || NULL == itemGroupPath)
    {
        return;
    }

    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.icon", itemGroupPath);
    setSBStringData(getAppInstance(), buffer, pb_list_data[idx - 1].icon);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.text", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText(pb_list_data[idx - 1].title));
}

static void pbList_onItemTouch(unequalHeiList *list, int idx,
                               const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    // HLOG_I(TAG, "%s\r\n", __func__);
    if (NULL == list || idx <= 0)
    {
        return;
    }

    if (eventId != TOUCH_SELECT)
    {
        return;
    }

    void *addr = (void *)&pb_list_data[idx - 1];
    getScreenManager()->startScreenByFlag("SportPbScreen", FLAG_SCREEN_STANDAR,
                                          addr, sizeof(pb_list));
}

static void refSyncStatus()
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    char buffer[256] = {0};
    char strff[256];
    memset(strff, 0, sizeof(strff));

    if (appData->item_sync_number > 0)
    {
        snprintf(strff, sizeof(strff), System_getLocaleText("id_sportrecord_list_sync"),
                 appData->item_sync_number);
    }

    HLOG_I(TAG, "%s strff:%s", __func__, strff);
    // set_text_aglin("SportRecordListItemLayer.SportRecordListSynStatus.item", strff,
    //                "fonts/allfont-Regular.ttf", 28);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.synstatus",
             "SportRecordListItemLayer.SportRecordListTitle");
    setSBStringData(getAppInstance(), buffer, strff);
}

static void setHaveRecordHeadAndFoot()
{
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.SportRecordListTitle.item.title",
             "SportRecordListItemLayer");
    set_text_aglin("SportRecordListItemLayer.SportRecordListTitle.item",
                   System_getLocaleText("id_sportrecord_title"),
                   "fonts/allfont-Medium.ttf", 40);
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.SportRecordListTitle.item.title",
             "SportRecordListItemLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_title"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.SportRecordListMore.item.morehite",
             "SportRecordListItemLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_more_record"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.SportPBTitle.item.title",
             "SportRecordPBItemLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_pb_title"));
}

static void create_record_list(int itemNumber)
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    unequalHeiList *sportRecordList = createUnequalHeiListContainer(
                                          getScreenContext(currentScreenInstance)->_grAppContext);

    if (sportRecordList)
    {
        setHaveRecordHeadAndFoot();
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
        {
            "SportRecordListItemLayer.SportRecordListItem",
            "SportRecordListItemLayer.SportRecordListTitle",
            "SportRecordListItemLayer.SportRecordListMore",
            // "SportRecordListItemLayer.SportRecordListSynStatus"
        };
        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
        sportRecordList->create(getScreenContext(currentScreenInstance)->_grAppContext,
                                sportRecordList, "SportRecordListScreen", "SportRecordListLayer",
                                itemNumber + 1, itemTemps, tempCnt, sportList_onItemUpdate,
                                sportList_onItemTouch, "item");
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_SPORTRECORD_LIST,
                     sportRecordList);
        sportRecordList->setSupportLongPress(sportRecordList, sportList_longPressed,
                                             true);
        sportRecordList->setItemTemp(sportRecordList, 0, 0, 1);
        // sportRecordList->setItemTemp(sportRecordList, 1, 1, 3);
        refSyncStatus();
        sportRecordList->setLastEmptySpace(sportRecordList, getDimension(200));
        sportRecordList->setSupportPressEffect(sportRecordList, true);
        sportRecordList->initItems(sportRecordList);

        if (itemNumber == MAX_DISPLAY_RECORD_NUM)
        {
            sportRecordList->addItem(sportRecordList, 2);
        }
    }
}

static void create_pb_list()
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    unequalHeiList *pbList = createUnequalHeiListContainer(getScreenContext(
                                 currentScreenInstance)->_grAppContext);

    if (pbList)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
        {
            "SportRecordPBItemLayer.SportPBItem",
            "SportRecordPBItemLayer.SportPBTitle",
        };
        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
        pbList->create(getScreenContext(currentScreenInstance)->_grAppContext, pbList,
                       "SportRecordListScreen", "SportRecordPBLayer", 4, itemTemps, tempCnt,
                       pbList_onItemUpdate, pbList_onItemTouch, "item");
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_RECORDPB_LIST,
                     pbList);
        pbList->setItemTemp(pbList, 0, 0, 1);
        pbList->setLastEmptySpace(pbList, getDimension(200));
        pbList->setSupportPressEffect(pbList, true);
        pbList->initItems(pbList);
    }
}

static void setBarTitle(int page)
{
#ifndef HM_DISPLAY_IS_ROUND


    switch (page)
    {
        case 0:
            HmSysStatusBar_updateTitle(System_getLocaleText("id_sportrecord_title"));
            break;

        case 1:
            HmSysStatusBar_updateTitle(System_getLocaleText("id_sportrecord_pb_title"));
            break;

        default:
            break;
    }

#endif
}

static void onPageViewScrolled(pageScrollerMultiLayers *scroll, int page)
{
    setBarTitle(page);
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    appData->h_pageIndex = page;
    hSchedule *_hScedule = (hSchedule *)findContainer(getScreenContext(getScreen(
                               SCREEN_NAME)), ID_LIST_SCHEDULE);

    if (_hScedule)
    {
        _hScedule->setScheduleCurrent(_hScedule, appData->h_pageIndex);
    }
}

static void creat_h_record_layer(int itemNumber)
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    pageScrollerMultiLayers *scroll = createPageScrollerMultiLayers(
                                          getScreenContext(currentScreenInstance)->_grAppContext);

    if (scroll)
    {
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_RECORD_MULTILAYERS,
                     scroll);
        scroll->init(getScreenContext(getScreen(SCREEN_NAME))->_grAppContext, scroll,
                     getDisplayMetrics()->width, 0, 400, onPageViewScrolled);

        if (itemNumber == 0)
        {
            struct _pageScrollerPage pages[] =
            {
                {1, {"SportRecordListScreen.SportRecordNoListLayer"}},
                {1, {"SportRecordListScreen.SportRecordPBLayer"}},
                // {1, {"SportRecordListScreen.SportRecordResultPredLayer"}},
            };
            scroll->initPages(scroll, pages, sizeof(pages) / sizeof(pages[0]));
        }
        else
        {
            struct _pageScrollerPage pages[] =
            {
                {1, {"SportRecordListScreen.SportRecordListLayer"}},
                {1, {"SportRecordListScreen.SportRecordPBLayer"}},
                // {1, {"SportRecordListScreen.SportRecordResultPredLayer"}},
            };
            scroll->initPages(scroll, pages, sizeof(pages) / sizeof(pages[0]));
        }

        scroll->setOvershoot(scroll, 100);
        scroll->setCycle(scroll, false);
        scroll->setDirect(scroll, PSML_DIRECT_HOR);
        scroll->sync(scroll, 0);
    }
}

static void creatScheduleView()
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          "SportRecordListScreen.SportListcheduleLayer", 1);
    hSchedule *_hScedule = createHSchedulerContainer(getScreenContext(
                               currentScreenInstance)->_grAppContext);

    if (_hScedule)
    {
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_LIST_SCHEDULE,
                     _hScedule);
        _hScedule->create(getScreenContext(currentScreenInstance)->_grAppContext,
                          _hScedule, "SportListcheduleLayer", 2, 10);
        _hScedule->initShedule(_hScedule);
        _hScedule->setSchedulePostiton(_hScedule, SCREEN_ROUND);
        _hScedule->setScheduleCurrent(_hScedule, appData->h_pageIndex);
    }
}
static void dialogCallBackListener(ScreenContext *contenxt, AlertDialog *self,
                                   AlertButtonType type)
{
    switch (type)
    {
        case BUTTON_LEFT_CLICK:
            self->dismissDialog(self);
            break;

        case BUTTON_RIGHT_CLICK:
            self->dismissDialog(self);
            delete_record_dialog();
            break;

        default:
            break;
    }
}

static void status_dialog_create()
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    AlertDialog *abandonDialog = creatAlertDialog(getScreenContext(
                                     currentScreenInstance), "SportRecordListScreen");
    addContainer(getScreenContext(currentScreenInstance), ID_SPORT_RECORD_DIALOG,
                 abandonDialog);
    abandonDialog->setTitleAndContent(abandonDialog,
                                      System_getLocaleText("id_sportrecord_delete_title"),
                                      System_getLocaleText("id_sportrecord_delete_content"));
    abandonDialog->setNegativeButton(abandonDialog, NULL);
    abandonDialog->setPositiveButton(abandonDialog, NULL);
    abandonDialog->setClickListener(abandonDialog, dialogCallBackListener);
    appData->abandonDialog = abandonDialog;
}
static void initNoRecordListView(int itemNumber)
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_NO_LIST, 1);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_HAVE_LIST, 0);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_HAVE_PB, 1);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_HAVE_RESULT_PRED, 0);
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.title.title",
             "SportRecordNoListLayer");
    set_text_aglin("SportRecordNoListLayer.title",
                   System_getLocaleText("id_sportrecord_title"),
                   "fonts/allfont-Medium.ttf", 40);
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.title.title", "SportRecordNoListLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_title"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.item1.hite", "SportRecordNoListLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_no_record"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.SportPBTitle.item.title",
             "SportRecordPBItemLayer");
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportrecord_pb_title"));
    create_pb_list();
    creat_h_record_layer(itemNumber);
    creatScheduleView();
}

static void initHaveRecordListView(int itemNumber)
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_HAVE_LIST, 1);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_HAVE_PB, 1);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_HAVE_RESULT_PRED, 1);
    gre_set_layer_visible(getScreenContext(currentScreenInstance)->_grAppContext,
                          PATH_RECORD_NO_LIST, 0);
    create_record_list(itemNumber);
    create_pb_list();
    creat_h_record_layer(itemNumber);
    creatScheduleView();
    status_dialog_create();
}

static void showRecordListData(sport_record_list_new_t sportRecordListData,
                               int itemNumber)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    appData->sport_record_list_Data = sportRecordListData;
    appData->item_number = itemNumber;
}

static void getChildSportInfo(child_sports_manager_t childSportsManager)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    appData->child_sports_manager_data = childSportsManager;
}

static void createSportRecordList(int itemNumber)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (itemNumber == 0)
    {
        initNoRecordListView(itemNumber);
    }
    else
    {
        CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                            getNoSyncSportRecordNum)
        (sportRecordListPresenter);
        CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                            showSportRecordListData)
        (sportRecordListPresenter);
        initHaveRecordListView(itemNumber);
    }
}

static void getNoSyncRecordData(uint16_t noSyncNum)
{
    HLOG_I(TAG, "%s no sync num%d\r\n", __func__, noSyncNum);
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    appData->item_sync_number = noSyncNum;
}

static SportRecordListCallback sportRecordListCallback =
{
    .createSportRecordList = createSportRecordList,
    .showRecordListData = showRecordListData,
    .getChildSportInfo = getChildSportInfo,
    .getNoSyncRecordData = getNoSyncRecordData,
};

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }

    setBarTitle(0);
    sportRecordListPresenter = SportRecordListPresenter_new();
    Presenter *basePresenter = SUPER_PTR(sportRecordListPresenter, Presenter);
    CALL_FUNCTION(basePresenter, Presenter, init)
    (basePresenter);
    CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                        setCallback)
    (sportRecordListPresenter, &sportRecordListCallback);
    setScreenPresenter(screenContext, sportRecordListPresenter);
    CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                        obtainSportRecordList)
    (sportRecordListPresenter);
#if HMI_COMP_STATISTICS
    _ss_buried_point(0, SPORT_REC_HOME_PAGE_EID, H_NULL);
#endif
}

static void onNewScreenData(ScreenContext *screenContext, void *data)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onDataRestore(ScreenContext *screenContext, void *data)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    SaveData *saveData = (SaveData *)data;
    memcpy(&appData->h_pageIndex, &saveData->h_pageIndex, sizeof(int));
    pageScrollerMultiLayers *scroll = (pageScrollerMultiLayers *)findContainer(
                                          getScreenContext(getScreen(SCREEN_NAME)), ID_RECORD_MULTILAYERS);

    if (scroll)
    {
        setBarTitle(appData->h_pageIndex);
        scroll->scrollToPage(scroll, appData->h_pageIndex, false);
    }

    hSchedule *_hScedule = (hSchedule *)findContainer(getScreenContext(getScreen(
                               SCREEN_NAME)), ID_LIST_SCHEDULE);

    if (_hScedule)
    {
        _hScedule->setScheduleCurrent(_hScedule, appData->h_pageIndex);
    }
}

static void onScreenPostCreate(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onScreenPreDestory(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
}

static void onDataSave(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = getScreenDataByName(SCREEN_NAME, AppData);
    SaveData *saveData = H_MALLOC(sizeof(SaveData));
    memcpy(&saveData->h_pageIndex, &appData->h_pageIndex, sizeof(int));
    getScreenManager()->saveScreenData("SportRecordListScreen", saveData,
                                       sizeof(SaveData));
    H_FREE(saveData);
}

static void onScreenPostDestory(ScreenContext *screenContext)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    SportRecordListPresenter *presenter = (SportRecordListPresenter *)
                                          getScreenPresenter(screenContext);

    if (presenter)
    {
        //destory and free presenter
        Presenter *basePresenter = SUPER_PTR(presenter, Presenter);
        CALL_VOID(destory, basePresenter);
        freeScreenPresenter(screenContext, SportRecordListPresenter);
    }

    //free screen
    removeAllContainers(screenContext);
    freeScreenData(screenContext);
}

static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
}
static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
}
typedef enum
{
    RLNONE,
    RLHOR,
    RLVEL,
} rlpagedirectionenum;
static int g_dragType = RLNONE;
static int g_pressDown = false;
static SPoint g_pressPos = {.x = 0, .y = 0};

static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->abandonDialog != H_NULL
            && strstr(id, data->abandonDialog->getAlertDialogId(data->abandonDialog)))
    {
        bool intercepted = data->abandonDialog->container.handleTouchEvent(
                               screenContext, &data->abandonDialog->container, id, eventId, info);

        if (intercepted)
        {
            return true;
        }
    }

    // HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);

    if (eventId == TOUCH_SELECT)
    {
        if (appData->h_pageIndex == 1)
        {
            ScreenContainer *pb1 = (ScreenContainer *)findContainer(screenContext,
                                   ID_RECORDPB_LIST);

            if (pb1)
            {
                pb1->handleTouchEvent(screenContext, pb1, id, eventId, info);
                return true;
            }
        }

        if (appData->h_pageIndex == 0 && appData->item_number > 0)
        {
            ScreenContainer *list = (ScreenContainer *)findContainer(screenContext,
                                    ID_SPORTRECORD_LIST);

            if (list)
            {
                list->handleTouchEvent(screenContext, list, id, eventId, info);
                return true;
            }
        }
    }

    if (eventId == TOUCH_MOVE && g_pressDown)
    {
        int deltaX = abs(info->x - g_pressPos.x);
        int deltaY = abs(info->y - g_pressPos.y);
        if (deltaX + deltaY > 20 && appData->h_pageIndex == 0 && appData->item_number > 0)
        {
            unequalHeiList *recordList = (unequalHeiList *)findContainer(screenContext,
                                         ID_SPORTRECORD_LIST);

            if (recordList)
            {
                recordList->cancelLongPress(recordList);
            }
        }
    }

    // if (!strcmp(id, "SportRecordListScreen"))
    // {
    ScreenContainer *hscroll = (ScreenContainer *)findContainer(screenContext,
                               ID_RECORD_MULTILAYERS);
    ScreenContainer *list = NULL;

    if (appData->item_number > 0)
    {
        list = (ScreenContainer *)findContainer(screenContext, ID_SPORTRECORD_LIST);
    }

    ScreenContainer *pb = (ScreenContainer *)findContainer(screenContext,
                          ID_RECORDPB_LIST);

    if (eventId == TOUCH_DOWN)
    {
        g_pressDown = true;
        g_pressPos.x = info->x;
        g_pressPos.y = info->y;
        if(hscroll){
            hscroll->handleTouchEvent(screenContext, hscroll, id, eventId, info);
        }

        if (list && appData->h_pageIndex == 0 && appData->item_number > 0)
        {
            list->handleTouchEvent(screenContext, list, id, eventId, info);
        }

        if (pb && appData->h_pageIndex == 1)
        {
            pb->handleTouchEvent(screenContext, pb, id, eventId, info);
        }
    }
    else if (eventId == TOUCH_MOVE)
    {
        if (g_pressDown)
        {
            int deltaX = info->x - g_pressPos.x;
            int deltsY = info->y - g_pressPos.y;
            float x_abs = abs(deltaX);
            float y_abs = abs(deltsY);

            if (g_dragType == RLNONE)
            {
                if (x_abs > RECORD_SCROLL_DRAG_THREHOLD)
                {
                    g_dragType = RLHOR;
                }

                if (y_abs > RECORD_SCROLL_DRAG_THREHOLD)
                {
                    g_dragType = RLVEL;
                }
            }
        }

        if (hscroll && g_dragType == RLHOR)
        {
            hscroll->handleTouchEvent(screenContext, hscroll, id, eventId, info);
            gre_set_layer_attr(screenContext->_grAppContext, "SportRecordListLayer",
                               "scroll_enabled", 0);
            gre_set_layer_attr(screenContext->_grAppContext, "SportRecordPBLayer",
                               "scroll_enabled", 0);
        }

        if (g_dragType == RLVEL)
        {
            if (list && appData->h_pageIndex == 0 && appData->item_number > 0)
            {
                list->handleTouchEvent(screenContext, list, id, eventId, info);
            }

            if (pb && appData->h_pageIndex == 1)
            {
                pb->handleTouchEvent(screenContext, pb, id, eventId, info);
            }
        }
    }
    else if (eventId == TOUCH_UP)
    {
        if (hscroll && g_dragType == RLHOR)
        {
            hscroll->handleTouchEvent(screenContext, hscroll, id, eventId, info);
        }

        if (g_dragType == RLVEL)
        {
            if (list && appData->item_number > 0)
            {
                list->handleTouchEvent(screenContext, list, id, eventId, info);
            }
            if(pb){
                pb->handleTouchEvent(screenContext, pb, id, eventId, info);
            }
        }

        gre_set_layer_attr(screenContext->_grAppContext, "SportRecordListLayer",
                           "scroll_enabled", 1);
        gre_set_layer_attr(screenContext->_grAppContext, "SportRecordPBLayer",
                           "scroll_enabled", 1);
        g_dragType = RLNONE;
        g_pressDown = false;
    }

    // }
    return true;
}
static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    return true;
}
static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->h_pageIndex == 0)
    {
        if (eventId == GESTURE_RIGHT)
        {
            getScreenManager()->finishCurrentScreen();
        }
    }

    return true;
}
static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                      AppData);

        if (data->h_pageIndex == 0 && data->item_number > 0)
        {
            ScreenContainer *list = findContainer(screenContext, ID_SPORTRECORD_LIST);

            if (list)
            {
                list->handleEvent(screenContext, list, id, event);
            }
        }

        if (data->h_pageIndex == 1)
        {
            ScreenContainer *pb = findContainer(screenContext, ID_RECORDPB_LIST);

            if (pb)
            {
                pb->handleEvent(screenContext, pb, id, event);
            }
        }
    }

    return true;
}
