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

#include <container/SportGoalContainer.h>
#define CLONE_TEMPLATE "SportRecordGoalTemplateLayer.GoalComponent"
#define CLONE_NAME "Goal"
#define TAG "SportGoalContainer"

static void _setTime(float time, char *strff, size_t strffLen);
static void _setFloat(float dis, char *strff, size_t strffLen);
static void _setOneFloat(float dis, char *strff, size_t strffLen);
static void _setInt(float num, char *strff, size_t strffLen);
static void _setPercentage(float num, char *strff, size_t strffLen);

static bool _create(gr_application_t *app,
                    SportGoalContainer *sportGoalContainer, const char *Layer)
{
    sportGoalContainer->app = app;
    strncpy(sportGoalContainer->layer, Layer, sizeof(sportGoalContainer->layer));
    return true;
}

static bool _initSportGoalContainer(SportGoalContainer *sportGoalContainer)
{
    char strff[256];
    #ifdef HM_DISPLAY_IS_ROUND
    gr_integer_kv_t data[2] =
    {
        {"x", 16},
        {"y", 16},
    };
    #else
    gr_integer_kv_t data[2] =
    {
        {"x", 0},
        {"y", 0},
    };
    #endif
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_NAME);
    char *fqn = NULL;
    gre_clone_object(sportGoalContainer->app, CLONE_TEMPLATE, strff,
                     sportGoalContainer->layer, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
    return true;
}

static bool _setLayerData(SportGoalContainer *sportGoalContainer,
                          uint16_t sportType, uint32_t goalType, float goalData, float currentData,
                          float disUnit)
{
    HLOG_I(TAG, "%s sportType:%d goalType:%d goalData:%f,currentData:%f\r\n",
           __func__, sportType, goalType, goalData, currentData);
    char returnStrff[256] = {0};
    char strff[256] = {0};
    char valueStrff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.goalFirstHite", sportGoalContainer->layer,
             CLONE_NAME);

    switch (goalType)
    {
        case 2:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setTime(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setTime(currentData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_time"));
            break;
        }

        case 3:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setFloat(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setFloat(RecordMile_to_km(currentData), returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_distance"));
            break;
        }

        case 4:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setInt(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setInt(currentData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_consumption"));
            break;
        }

        case 5:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setInt(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setInt(currentData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_number_of_trips"));
            break;
        }

        case 6:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setInt(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setInt(currentData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_count"));
            break;
        }

        case 7:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setOneFloat(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setOneFloat(currentData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_aerobic_TE"));
            break;
        }

        case 8:
        {
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalFirstValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setOneFloat(goalData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            memset(valueStrff, 0, sizeof(valueStrff));
            snprintf(valueStrff, sizeof(valueStrff), "%s.%s.goalSecondValue",
                     sportGoalContainer->layer, CLONE_NAME);
            memset(returnStrff, 0, sizeof(returnStrff));
            _setOneFloat(currentData, returnStrff, sizeof(returnStrff));
            setSBStringData(getAppInstance(), valueStrff, returnStrff);
            setSBStringData(getAppInstance(), strff,
                            System_getLocaleText("id_sportgoal_first_target_anaerobic_TE"));
            break;
        }

        default:
            break;
    }

    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.goalSecondHite",
             sportGoalContainer->layer, CLONE_NAME);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportgoal_second_hite"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.goalThreeHite", sportGoalContainer->layer,
             CLONE_NAME);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportgoal_three_hite"));
    #ifdef HM_DISPLAY_IS_ROUND
    char progress_buffer[256] = {0};
    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.%s.goalEndAngle",
             sportGoalContainer->layer, CLONE_NAME);
    H_Float progress = (currentData / goalData) * 244.0f;

    if (progress >= 244.0f)
    {
        progress = 244.0f;
    }

    setSBFloatData(getAppInstance(), progress_buffer, progress);
    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.%s.goalThreeValue",
             sportGoalContainer->layer, CLONE_NAME);
    H_Float progressValue = (currentData / goalData) * 100.0f;
    // if (progressValue >= 100.0f)
    // {
    //     progressValue = 100.0f;
    // }
    _setPercentage(progressValue, returnStrff, sizeof(returnStrff));
    setSBStringData(getAppInstance(), progress_buffer, returnStrff);
    #else
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.goalTitle", sportGoalContainer->layer,
             CLONE_NAME);
    setSBStringData(getAppInstance(), strff,
                System_getLocaleText("id_sportgoal_title"));
    char progress_buffer[256] = {0};
    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.%s.bgwidth",
             sportGoalContainer->layer, CLONE_NAME);

    int progress = (currentData / goalData) * getSBIntData(getAppInstance(), progress_buffer);
    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.%s.goalW",
             sportGoalContainer->layer, CLONE_NAME);

    setSBIntData(getAppInstance(), progress_buffer, progress);

    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.%s.goalThreeValue",
             sportGoalContainer->layer, CLONE_NAME);
    H_Float progressValue = (currentData / goalData) * 100.0f;
    _setPercentage(progressValue, returnStrff, sizeof(returnStrff));
    setSBStringData(getAppInstance(), progress_buffer, returnStrff);
    #endif
    return true;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    SportGoalContainer *sportGoalContainer = (SportGoalContainer *)container;

    if (sportGoalContainer)
    {
        char strff[256];
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", sportGoalContainer->layer, CLONE_NAME);
        gre_delete_object(sportGoalContainer->app, strff);
    }
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    return true;
}
static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container, const HmKeyEvent *info)
{
    return true;
}
static bool handleGestureEvent(ScreenContext *screenContext,
                               ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return true;
}
static bool handleEvent(ScreenContext *screenContext,
                        ScreenContainer *container, const char *id, const char *event)
{
    return true;
}

SportGoalContainer *createSportGoalContainer(gr_application_t *app)
{
    SportGoalContainer *sportGoalContainer = H_NULL;
    createContainer(sportGoalContainer, SportGoalContainer,
                    sportGoalContainer->container);

    if (sportGoalContainer)
    {
        sportGoalContainer->create = _create;
        sportGoalContainer->initSportGoalContainer = _initSportGoalContainer;
        sportGoalContainer->setLayerData = _setLayerData;
    }

    return sportGoalContainer;
}

static void _setTime(float time, char *strff, size_t strffLen)
{
    int hour = time / 3600;
    int minute = (time - hour * 3600) / 60;
    int sec = (time - hour * 3600 - minute * 60);

    if (hour == 0 && minute == 0)
    {
        snprintf(strff, strffLen, "0:%02d", sec);
    }
    else if (hour == 0 && minute > 0)
    {
        snprintf(strff, strffLen, "%d:%02d", minute, sec);
    }
    else if (hour > 0)
    {
        snprintf(strff, strffLen, "%d:%02d:%02d", hour, minute, sec);
    }
}
static void _setFloat(float dis, char *strff, size_t strffLen)
{
    snprintf(strff, strffLen, "%0.2f", dis);
}
static void _setOneFloat(float dis, char *strff, size_t strffLen)
{
    snprintf(strff, strffLen, "%0.1f", dis);
}
static void _setInt(float num, char *strff, size_t strffLen)
{
    snprintf(strff, strffLen, "%d", (int)num);
}

static void _setPercentage(float num, char *strff, size_t strffLen)
{
    snprintf(strff, strffLen, "%d%%", (int)num);
}