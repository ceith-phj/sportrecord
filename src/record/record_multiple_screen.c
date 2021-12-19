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

#define TAG "sport_multiple_screen"
#include <sb_screen_context.h>
#include <hlog.h>
#include <packages/apps/sports/include/presenter/record_multiple_presenter.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <record/record_parameter.h>
#include <sport_type_utils.h>
#include <record/record_time_utils.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <record/record_unit_utils.h>
#include <uiframework/ui_system/screenmanager/include/screen_manager.h>
#include <hm_system_status_bar.h>

#define ID_MULTIPLE_LIST "SportMultipleListLayer.MultipleList"
#define SCREEN_NAME SportMultiple
SCREEN_DECLARE(SCREEN_NAME);

static SportRecordMultiplePresenter *sportRecordMultiplePresenter = NULL;
static record_parameter_data *recordParameterData = NULL;
static child_sports_manager_t _child_sport_manager = {0};
typedef struct
{
    //传递参数
} AppData;

static void sportList_onItemUpdate(unequalHeiList *list, int idx,
                                   const char *itemGroupPath)
{
    if (NULL == list || idx <= 0 || NULL == itemGroupPath
            || _child_sport_manager.child_num == 0)
    {
        return;
    }

    idx -= 1;
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.icon", itemGroupPath);
    setSBStringData(getAppInstance(), buffer,
                    sportType_getTypeIconId(_child_sport_manager.child_info[idx].sport_type));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.exercisetime", itemGroupPath);
    //setSBIntData(getAppInstance(), buffer, sport_record_list_Data.list_info[idx].time);
    Time_getExerciseTime(_child_sport_manager.child_info[idx].sport_time, buffer);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.data", itemGroupPath);
    //setSBStringData(getAppInstance(), buffer, _RecordData_getRecordMainData(sport_record_list_Data.list_info[idx].sport_type,sport_record_list_Data.list_info[idx].main_data));
    //需要添加判断取值
    RecordData_getRecordMainData(_child_sport_manager.child_info[idx].sport_type,
                                 _child_sport_manager.child_info[idx].distance, buffer, 0);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.time", itemGroupPath);
    //setSBIntData(getAppInstance(), buffer, sport_record_list_Data.list_info[idx].track_id);
    RecordTime_getRecordTime(_child_sport_manager.child_info[idx].track_id, buffer);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.unit", itemGroupPath);
    //setSBStringData(getAppInstance(), buffer, _RecordUnit_getRecordMainDataUnit(sport_record_list_Data.list_info[idx].sport_type));
    RecordUnit_getRecordMainDataUnit(
        _child_sport_manager.child_info[idx].sport_type, buffer, 0);
};
static void sportList_onItemTouch(unequalHeiList *list, int idx,
                                  const char *idOnly, const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_I(TAG, "%s\r\n", __func__);

    if (NULL == list || idx <= 0)
    {
        return;
    }

    if (eventId != TOUCH_SELECT)
    {
        return;
    }

    record_parameter_data _record_paramete_data = {0};
    _record_paramete_data.sport_type = _child_sport_manager.child_info[idx -
                                           1].sport_type;
    _record_paramete_data.track_id = recordParameterData->track_id;
    _record_paramete_data.child_trcak_id = _child_sport_manager.child_info[idx -
                                               1].track_id;
    _record_paramete_data.auto_reg = _child_sport_manager.child_info[idx -
                                         1].auto_reg;
    getScreenManager()->startScreen("SportRecordDetailScreen",
                                    (void *)&_record_paramete_data, sizeof(record_parameter_data));
};

static void creatMultipleListView(int itemNumber)
{
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    unequalHeiList *multipleList = createUnequalHeiListContainer(getScreenContext(
                                       currentScreenInstance)->_grAppContext);

    if (multipleList)
    {
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
        {
            "SportMultipleListItemLayer.SportMultipleListItem",
            "SportMultipleListItemLayer.SportMultipleListHead",
            "SportMultipleListItemLayer.SportMultipleListBottom",
        };
        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
        multipleList->create(getScreenContext(currentScreenInstance)->_grAppContext,
                             multipleList, "SportMultipleScreen", "SportMultipleListLayer", itemNumber,
                             itemTemps, tempCnt, sportList_onItemUpdate, sportList_onItemTouch, "item");
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_MULTIPLE_LIST,
                     multipleList);
        multipleList->setItemTemp(multipleList, 0, 0, 1);
        multipleList->initItems(multipleList);

        if (itemNumber == 10)
        {
            multipleList->addItem(multipleList, 2);
        }
    }
};

static void multiple_list_refresh_main_data()
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportMultipleListHead.item.title",
             "SportMultipleListItemLayer");
    setSBStringData(getAppInstance(), buffer,
                    sportType_getTypeNameChar(recordParameterData->sport_type));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportMultipleListHead.item.time",
             "SportMultipleListItemLayer");
    Time_getRecordTime(recordParameterData->track_id, buffer);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportMultipleListHead.item.firstValue",
             "SportMultipleListItemLayer");
    char strff[21];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%d", (int)_child_sport_manager.child_num);
    setSBStringData(getAppInstance(), buffer, strff);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.SportMultipleListHead.item.secondValue",
             "SportMultipleListItemLayer");
    Time_getExerciseTime(_child_sport_manager.total_time, buffer);
};

static void showMultipleListViewItem(child_sports_manager_t childSportManager)
{
    _child_sport_manager = childSportManager;
    multiple_list_refresh_main_data();
};

static SportRecordMultipleCallback sportRecordMultipleCallback =
{
    .creatMultipleListView = creatMultipleListView,
    .showMultipleListViewItem = showMultipleListViewItem,
};

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
#ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_setVisible(H_True);
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sportrecord_title"));
#endif
    HLOG_I(TAG, "%s\r\n", __func__);
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }

    // historyId = *((uint32_t *)data);
    recordParameterData = (record_parameter_data *)data;
    sportRecordMultiplePresenter = SportRecordMultiplePresenter_new();
    Presenter *basePresenter = SUPER_PTR(sportRecordMultiplePresenter, Presenter);
    CALL_FUNCTION(basePresenter, Presenter, init)(basePresenter);
    CALL_FINAL_FUNCTION(sportRecordMultiplePresenter, SportRecordMultiplePresenter,
                        setCallback)(sportRecordMultiplePresenter, &sportRecordMultipleCallback);
    setScreenPresenter(screenContext, sportRecordMultiplePresenter);
    CALL_FINAL_FUNCTION(sportRecordMultiplePresenter, SportRecordMultiplePresenter,
                        obtainSportRecordMultipleList)(sportRecordMultiplePresenter,
                                recordParameterData->track_id);
}
static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}
static void onDataRestore(ScreenContext *screenContext, void *data)
{
}
static void onScreenPostCreate(ScreenContext *screenContext)
{
}
static void onScreenPreDestory(ScreenContext *screenContext)
{
}
static void onDataSave(ScreenContext *screenContext)
{
}
static void onScreenPostDestory(ScreenContext *screenContext)
{
}
static void onAnimationComplete(ScreenContext *screenContext, const char *id)
{
}
static void onAnimationStop(ScreenContext *screenContext, const char *id)
{
}
static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    ScreenContainer *settingList = findContainer(screenContext, ID_MULTIPLE_LIST);

    if (settingList)
    {
        settingList->handleTouchEvent(screenContext, settingList, id, eventId, info);
        return true;
    }

    return false;
}
static bool handleKeyEvent(ScreenContext *screenContext,
                           const HmKeyEvent *info)
{
    return true;
}
static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    HLOG_I(TAG, "%s\r\n", __func__);
    getScreenManager()->finishCurrentScreen();
    return true;
}
static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    return true;
}
