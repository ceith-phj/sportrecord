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

#include <sb_screen_context.h>
#include <presenter/record_list_presenter.h>
#include <record/record_parameter.h>
#include <hlog.h>
#include <widget/unequalHeiList/unequalHeiList.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include <record/record_unit_utils.h>
#include <record/record_time_utils.h>
#include "core/display_metrics.h"
#include "sport_type_auto.h"
#include <hm_system_status_bar.h>

#if HMI_COMP_STATISTICS
#include "hmstatistics.h"
#endif

#define ID_PB_INFO_LIST "SportPbScreen.unequalHeiList"
#define TAG "SportPbScreen"
#define SCREEN_NAME SportPb
SCREEN_DECLARE(SCREEN_NAME);

typedef struct
{
    //传递参数
    pb_list pb_list_data;
    sport_record_list_pb_data *_sport_record_list_pb_data;
} AppData;

typedef struct
{
    pb_list pb_list_data;
    sport_record_list_pb_data *_sport_record_list_pb_data;
} SaveData;


static SportRecordListPresenter *sportRecordListPresenter = NULL;

#if HMI_COMP_STATISTICS
static void _ss_buried_point(uint8_t index, HM_STATIS_EVENT_ID_E value,
                             Param *parm)
{
    hm_statis_data_add_with_kv(value, parm,
                               index,
                               true);
}
#endif

static void set_km_time(int kmIndex, int yiIndex, char value[])
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    char values[256] = {0};
    memset(values, 0, sizeof(values));

    if (getMileageUnit() != 1)
    {
        if (appData->_sport_record_list_pb_data->sportPbInfoData[kmIndex].value != 0)
        {
            Time_getExerciseTime(
                appData->_sport_record_list_pb_data->sportPbInfoData[kmIndex].value, value);
        }
        else
        {
            snprintf(values, sizeof(values), "--:--:--");
            setSBStringData(getAppInstance(), value, values);
        }
    }
    else
    {
        if (appData->_sport_record_list_pb_data->sportPbInfoData[yiIndex].value != 0)
        {
            Time_getExerciseTime(
                appData->_sport_record_list_pb_data->sportPbInfoData[yiIndex].value, value);
        }
        else
        {
            snprintf(values, sizeof(values), "--:--:--");
            setSBStringData(getAppInstance(), value, values);
        }
    }
}

static void init_outdoor_running(int idx, const char *itemGroupPath)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.icon", itemGroupPath);
    char value[256] = {0};
    memset(value, 0, sizeof(value));
    snprintf(value, sizeof(value), "%s.item.text", itemGroupPath);
    char hite[256] = {0};
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.item.hite", itemGroupPath);
    char km[256] = {0};
    memset(km, 0, sizeof(km));
    char values[256] = {0};
    memset(values, 0, sizeof(values));

    switch (idx)
    {
        case 0:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[6].value != 0)
            {
                float showData = RecordMile_to_km(
                                     appData->_sport_record_list_pb_data->sportPbInfoData[6].value / 1000.0);
                snprintf(values, sizeof(values), System_getLocaleText("id_sportrecord_pb_dis"),
                         (((int)(showData * 100)) / 100.0f),
                         (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                          System_getLocaleText("id_sportrecord_pb_uint_mile")));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                          System_getLocaleText("id_sportrecord_pb_uint_mile")));
            }

            setSBStringData(getAppInstance(), buffer, "images/record/pb_list_icon_km.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_dis_hite"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        case 1:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[7].value != 0)
            {
                Time_getExerciseTime(
                    appData->_sport_record_list_pb_data->sportPbInfoData[7].value, value);
            }
            else
            {
                snprintf(values, sizeof(values), "--:--:--");
                setSBStringData(getAppInstance(), value, values);
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_time.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_time_hite"));
            break;
        }

        case 2:
        {
            set_km_time(0, 3, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 3,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast3.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        case 3:
        {
            set_km_time(1, 4, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 5,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast5.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        case 4:
        {
            set_km_time(2, 5, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 10,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast10.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        case 5:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[8].value != 0)
            {
                Time_getExerciseTime(
                    appData->_sport_record_list_pb_data->sportPbInfoData[8].value, value);
            }
            else
            {
                snprintf(values, sizeof(values), "--:--:--");
                setSBStringData(getAppInstance(), value, values);
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fasthm.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_fhm"));
            break;
        }

        case 6:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[9].value != 0)
            {
                Time_getExerciseTime(
                    appData->_sport_record_list_pb_data->sportPbInfoData[9].value, value);
            }
            else
            {
                snprintf(values, sizeof(values), "--:--:--");
                setSBStringData(getAppInstance(), value, values);
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fastm.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_fm"));
            break;
        }

        default:
            break;
    }
}

static void init_outdoor_cycling(int idx, const char *itemGroupPath)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.icon", itemGroupPath);
    char value[256] = {0};
    memset(value, 0, sizeof(value));
    snprintf(value, sizeof(value), "%s.item.text", itemGroupPath);
    char hite[256] = {0};
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.item.hite", itemGroupPath);
    char km[256] = {0};
    memset(km, 0, sizeof(km));
    char values[256] = {0};
    memset(values, 0, sizeof(values));

    switch (idx)
    {
        case 0:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[22].value != 0)
            {
                float showData = RecordMile_to_km(
                                     appData->_sport_record_list_pb_data->sportPbInfoData[22].value / 1000.0);
                snprintf(values, sizeof(values), System_getLocaleText("id_sportrecord_pb_dis"),
                         (((int)(showData * 100)) / 100.0f),
                         (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                          System_getLocaleText("id_sportrecord_pb_uint_mile")));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                          System_getLocaleText("id_sportrecord_pb_uint_mile")));
            }

            setSBStringData(getAppInstance(), buffer, "images/record/pb_list_icon_km.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_dis_hite"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        case 1:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[23].value != 0)
            {
                Time_getExerciseTime(
                    appData->_sport_record_list_pb_data->sportPbInfoData[23].value, value);
            }
            else
            {
                snprintf(values, sizeof(values), "--:--:--");
                setSBStringData(getAppInstance(), value, values);
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_time.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_time_hite"));
            break;
        }

        // case 2:
        // {
        //     set_km_time(1, 4, value);
        //     snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 3, (getMileageUnit() != 1 ? System_getLocaleText("id_id_sportrecord_pb_km_uint") : System_getLocaleText("id_sportrecord_pb_uint_mile")));
        //     setSBStringData(getAppInstance(), buffer, "images/record/pb_list_icon_fast3.png");
        //     setSBStringData(getAppInstance(), hite, km);
        //     break;
        // }
        case 2:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[24].value != 0)
            {
                snprintf(values, sizeof(values),
                         System_getLocaleText("id_sportrecord_pb_altitude"),
                         appData->_sport_record_list_pb_data->sportPbInfoData[24].value,
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_altitude_1.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_mc"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        case 3:
        {
            set_km_time(14, 18, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 5,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast5.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        case 4:
        {
            set_km_time(15, 19, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 10,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast10.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        case 5:
        {
            set_km_time(16, 20, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 20,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast20.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        case 6:
        {
            set_km_time(17, 21, value);
            snprintf(km, sizeof(km), System_getLocaleText("id_sportrecord_pb_km"), 40,
                     (getMileageUnit() != 1 ? System_getLocaleText("id_sportrecord_pb_km_uint") :
                      System_getLocaleText("id_sportrecord_pb_uint_mile")));
            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_fast40.png");
            setSBStringData(getAppInstance(), hite, km);
            break;
        }

        default:
            break;
    }
}
static void init_mountaineering(int idx, const char *itemGroupPath)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.item.icon", itemGroupPath);
    char value[256] = {0};
    memset(value, 0, sizeof(value));
    snprintf(value, sizeof(value), "%s.item.text", itemGroupPath);
    char hite[256] = {0};
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.item.hite", itemGroupPath);
    char values[256] = {0};
    memset(values, 0, sizeof(values));

    switch (idx)
    {
        case 0:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[10].value != 0)
            {
                snprintf(values, sizeof(values),
                         System_getLocaleText("id_sportrecord_pb_altitude"),
                         appData->_sport_record_list_pb_data->sportPbInfoData[10].value,
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_altitude.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_ha"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        case 1:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[11].value != 0)
            {
                snprintf(values, sizeof(values),
                         System_getLocaleText("id_sportrecord_pb_altitude"),
                         appData->_sport_record_list_pb_data->sportPbInfoData[11].value,
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_altitudeup.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_mr"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        case 2:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[12].value != 0)
            {
                snprintf(values, sizeof(values),
                         System_getLocaleText("id_sportrecord_pb_altitude"),
                         appData->_sport_record_list_pb_data->sportPbInfoData[12].value,
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_altitudedown.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_md"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        case 3:
        {
            if (appData->_sport_record_list_pb_data->sportPbInfoData[13].value != 0)
            {
                snprintf(values, sizeof(values),
                         System_getLocaleText("id_sportrecord_pb_altitude"),
                         appData->_sport_record_list_pb_data->sportPbInfoData[13].value,
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }
            else
            {
                snprintf(values, sizeof(values), "-- %s",
                         System_getLocaleText("id_sportrecord_pb_uint_meter"));
            }

            setSBStringData(getAppInstance(), buffer,
                            "images/record/pb_list_icon_altitude_1.png");
            setSBStringData(getAppInstance(), hite,
                            System_getLocaleText("id_sportrecord_pb_mc"));
            setSBStringData(getAppInstance(), value, values);
            break;
        }

        default:
            break;
    }
}

static void onItemUpdate(unequalHeiList *list, int idx,
                         const char *itemGroupPath)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);

    if (NULL == list || idx <= 0 || NULL == itemGroupPath
            || appData->pb_list_data.item_num == 0)
    {
        return;
    }

    switch (appData->pb_list_data.sportType)
    {
        case SPORT_TYPE_OUTDOOR_RUNNING:
            init_outdoor_running(idx - 1, itemGroupPath);
            break;

        case SPORT_TYPE_OUTDOOR_CYCLING:
            init_outdoor_cycling(idx - 1, itemGroupPath);
            break;

        case SPORT_TYPE_MOUNTAINEERING:
            init_mountaineering(idx - 1, itemGroupPath);
            break;

        default:
            break;
    }

#if HMI_COMP_STATISTICS
    Param parm = {0};
    parm.key = SPORT_PERSON_REC_KEY_ID;
    parm.value = idx - 1;
    _ss_buried_point(1, SPORT_REC_OUT_RUN_PERSON_REC_EID, &parm);
#endif
}

static void onItemTouch(unequalHeiList *list, int idx, const char *idOnly,
                        const SBTouchEvent eventId, const SBEventInfo *info)
{
}

static void getPbShowData(sport_record_list_pb_data *_sport_record_list_pb_data)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    appData->_sport_record_list_pb_data = _sport_record_list_pb_data;
}

static SportRecordListCallback sportRecordListCallback =
{
    .getPbShowData = getPbShowData,
};
static void initTitle()
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    char buffer[256] = {0};
    snprintf(buffer, sizeof(buffer), "%s.item.title",
             "SportPbInfoItemLayer.PbTitle");
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText(appData->pb_list_data.title));
}

static void create_pb_list()
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    Screen *currentScreenInstance = getScreen(SCREEN_NAME);
    unequalHeiList *pbInfoList = createUnequalHeiListContainer(getScreenContext(
                                     currentScreenInstance)->_grAppContext);

    if (pbInfoList)
    {
        initTitle();
        char itemTemps[][UNEQUALHEI_LIST_NAME_MAX] =
        {
            "SportPbInfoItemLayer.PbItem",
            "SportPbInfoItemLayer.PbTitle",
        };
        int tempCnt = sizeof(itemTemps) / sizeof(itemTemps[0]);
        pbInfoList->create(getScreenContext(currentScreenInstance)->_grAppContext,
                           pbInfoList, "SportPbScreen", "SportPbInfoLayer",
                           appData->pb_list_data.item_num + 1, itemTemps, tempCnt, onItemUpdate,
                           onItemTouch, "item");
        addContainer(getScreenContext(getScreen(SCREEN_NAME)), ID_PB_INFO_LIST,
                     pbInfoList);
        pbInfoList->setItemTemp(pbInfoList, 0, 0, 1);
        pbInfoList->setLastEmptySpace(pbInfoList, getDimension(200));
        pbInfoList->initItems(pbInfoList);
    }
}

static void onScreenPreCreate(ScreenContext *screenContext, void *data)
{
#ifndef HM_DISPLAY_IS_ROUND
    HmSysStatusBar_setVisible(H_True);
    HmSysStatusBar_updateTitle(System_getLocaleText("id_sportrecord_pb_title"));
#endif
    AppData *appData = (AppData *)H_MALLOC(sizeof(AppData));

    if (appData != H_NULL)
    {
        memset(appData, 0, sizeof(AppData));
        setScreenData(screenContext, appData);
    }

    if (data != NULL)
    {
        memcpy(&appData->pb_list_data, data, sizeof(pb_list));
    }

    HLOG_I(TAG, "%s icon:%s title:%s type:%d item:%d\r\n", __func__,
           appData->pb_list_data.icon, appData->pb_list_data.title,
           appData->pb_list_data.sportType, appData->pb_list_data.item_num);
    sportRecordListPresenter = SportRecordListPresenter_new();
    Presenter *basePresenter = SUPER_PTR(sportRecordListPresenter, Presenter);
    CALL_FUNCTION(basePresenter, Presenter, init)
    (basePresenter);
    CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                        setCallback)
    (sportRecordListPresenter, &sportRecordListCallback);
    setScreenPresenter(screenContext, sportRecordListPresenter);
    CALL_FINAL_FUNCTION(sportRecordListPresenter, SportRecordListPresenter,
                        getPbData)
    (sportRecordListPresenter);
    create_pb_list();
}
static void onNewScreenData(ScreenContext *screenContext, void *data)
{
}
static void onDataRestore(ScreenContext *screenContext, void *data)
{
    AppData *appData = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                     AppData);
    SaveData *saveData = (SaveData *)data;

    if (&saveData->pb_list_data)
    {
        memcpy(&appData->pb_list_data, &saveData->pb_list_data, sizeof(pb_list));
    }

    removeAllContainers(screenContext);
    create_pb_list();
}
static void onScreenPostCreate(ScreenContext *screenContext)
{
}
static void onScreenPreDestory(ScreenContext *screenContext)
{
}
static void onDataSave(ScreenContext *screenContext)
{
    AppData *appData = getScreenDataByName(SCREEN_NAME, AppData);
    SaveData *saveData = H_MALLOC(sizeof(SaveData));

    if (&appData->pb_list_data)
    {
        memcpy(&saveData->pb_list_data, &appData->pb_list_data, sizeof(pb_list));
    }

    getScreenManager()->saveScreenData("SportPbScreen", saveData,
                                       sizeof(SaveData));
    H_FREE(saveData);
}
static void onScreenPostDestory(ScreenContext *screenContext)
{
    AppData *data = getScreenData(getScreenContext(getScreen(SCREEN_NAME)),
                                  AppData);

    if (data->_sport_record_list_pb_data != NULL
            && data->_sport_record_list_pb_data->sportPbInfoData != NULL)
    {
        H_FREE(data->_sport_record_list_pb_data->sportPbInfoData);
        data->_sport_record_list_pb_data->sportPbInfoData = NULL;
    }

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
static bool handleTouchEvent(ScreenContext *screenContext, const char *id,
                             const SBTouchEvent eventId, const SBEventInfo *info)
{
    ScreenContainer *list = findContainer(screenContext, ID_PB_INFO_LIST);

    if (list)
    {
        list->handleTouchEvent(screenContext, list, id, eventId, info);
        return true;
    }

    return false;
}
static bool handleKeyEvent(ScreenContext *screenContext, const HmKeyEvent *info)
{
    return false;
}
static bool handleGestureEvent(ScreenContext *screenContext, const char *id,
                               const SBGestureEvent eventId)
{
    if (eventId == GESTURE_RIGHT)
    {
        getScreenManager()->finishCurrentScreen();
    }

    return true;
}
static bool handleEvent(ScreenContext *screenContext, const char *id,
                        const char *event)
{
    if (strcmp(event, GR_EVENT_KEY_SPIN) == 0)
    {
        ScreenContainer *list = findContainer(screenContext, ID_PB_INFO_LIST);

        if (list)
        {
            list->handleEvent(screenContext, list, id, event);
        }
    }

    return true;
}
