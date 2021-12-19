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

#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_context.h>

/* record */
importScreen(SportRecordList);
importScreen(SportRecordDetail);
importScreen(SportMultiple);
importScreen(SportPb);
/* --import end-- */

REGISTER_SCREEN(SportRecordApp)
{
    /* record */
    registerScreen(SportRecordList),
                   registerScreen(SportRecordDetail),
                   registerScreen(SportMultiple),
                   registerScreen(SportPb),
                   /* --regist end-- */
};

APPLICATION_DECLARE(SportRecordApp)
{
    ADD_SCREEN(SportRecordApp),
               ADD_NO_WIDGET(SportRecordApp),
};
