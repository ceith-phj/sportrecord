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
#ifndef RECORD_TEXT_AGLIN_UTILS_H
#define RECORD_TEXT_AGLIN_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grelib.h"
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <hlog.h>
#include "core/display_metrics.h"
#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _TextAttr
{
    uint8_t width;
    uint8_t height;
} TextAttr;

const void set_text_aglin(char *itemGroupPath, const char *title,
                          char *fontStyle,
                          int fontSize);
const void set_text_height(char *itemGroupPath, const char *title,
                           char *fontStyle,
                           int fontSize);
const void get_text_width(char *itemGroupPath, const char *title,
                          char *fontStyle,
                          int fontSize, uint8_t isWrap, TextAttr *textattr);
#ifdef __cplusplus
}
#endif

#endif