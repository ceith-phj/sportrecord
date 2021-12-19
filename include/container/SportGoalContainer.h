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

#ifndef SPORT_GOAL_CONTAINER_H
#define SPORT_GOAL_CONTAINER_H
#include <gre/gre.h>
#include <widget/base/sb_container.h>
#include <gre/sdk/grelib.h>
#include <hlog.h>
#include "core/display_metrics.h"
#include <gre/sdk/greal_alloc.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include <record/record_unit_utils.h>

#define SPORT_GOAL_PAGE_MAX 128

typedef struct _SportGoalContainer SportGoalContainer;

#ifdef __cplusplus
extern "C"
{
#endif

struct _SportGoalContainer
{
    ScreenContainer container;
    bool (*create)(gr_application_t *app, SportGoalContainer *sportGoalContainer,
                   const char *Layer);
    bool (*initSportGoalContainer)(SportGoalContainer *sportGoalContainer);
    bool (*setLayerData)(SportGoalContainer *sportGoalContainer, uint16_t sportType,
                         uint32_t goalType, float goalData, float currentData, float disUnit);
    char layer[SPORT_GOAL_PAGE_MAX];
    gr_application_t *app;
};

SportGoalContainer *createSportGoalContainer(gr_application_t *app);

#ifdef __cplusplus
}
#endif

#endif