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
#include <record/record_detail_fb_utils.h>
#define TAG "record_detail_fb_utils"
#define FLOAT_PRECISION 1e-6

static void _setLeftImage(char *itemPath, float aerobicTe);
static void _setRightImage(char *itemPath, float anAerobicTe);
static void _setLeftValue(char *itemPath, float value);
static void _setRightValue(char *itemPath, float value);
static void _setLeftArrow(char *itemPath, float value);
static void _setRightArrow(char *itemPath, float value);
static void _setCurrentEx(char *itemPath, uint32_t value);
static void _setMaxArrowAttr(gr_application_t *app, char *itemPath);
static void _setTime(char *itemPath, float time);
#ifdef  HM_DISPLAY_IS_ROUND
static void getTextHeight(const char *info, char *itemPath,char *controlName);
#endif
static uint32_t recovery_time = 0;

const void FB_refTe(gr_application_t *app, char *itemPath, float aerobic,
                    float anAerobic, float EPOC)
{
    char hite[256] = {0};
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teTitle.title", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_effe_title"));
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAerobicHite", itemPath);
    set_text_aglin(hite, System_getLocaleText("id_sportrecord_tr_eff_aer"),
                   "fonts/allfont-Regular.ttf", 28);
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAerobicHite.aerHite", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_eff_aer"));
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAnaerobicHite", itemPath);
    set_text_aglin(hite, System_getLocaleText("id_sportrecord_tr_eff_anaer"),
                   "fonts/allfont-Regular.ttf", 28);
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAnaerobicHite.anaerHite", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_eff_anaer"));
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teCurrentExHite", itemPath);
    set_text_aglin(hite, System_getLocaleText("id_sportrecord_tr_eff_curr_exer"),
                   "fonts/allfont-Regular.ttf", 28);
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teCurrentExHite.curexhite", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_eff_curr_exer"));
    _setMaxArrowAttr(app, itemPath);
    _setLeftImage(itemPath, aerobic);
    _setRightImage(itemPath, anAerobic);
    _setCurrentEx(itemPath, (uint32_t)EPOC);
}


#ifndef  HM_DISPLAY_IS_ROUND



const void FB_refTrainingLoad(gr_application_t *app, char *itemPath,
                              float vlaue, uint16_t minValue, uint16_t maxValue, uint16_t bigMaxValue)
{
    // vlaue = 10;
    // minValue = 80;
    // maxValue = 260;
    // bigMaxValue = 300;

    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff),
             "%s.track_area_last.track_area_last_text", itemPath);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportstatus_tr_last"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff),
             "%s.track_area_tr.track_area_tr_text", itemPath);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportstatus_tr_hite"));
    char bg_visible[256] = {0};
    memset(bg_visible, 0, sizeof(bg_visible));
    snprintf(bg_visible, sizeof(bg_visible), "%s.tr_bg", itemPath);
    char first_visible[256] = {0};
    memset(first_visible, 0, sizeof(first_visible));
    snprintf(first_visible, sizeof(first_visible), "%s.track_area_first", itemPath);
    char second_visible[256] = {0};
    memset(second_visible, 0, sizeof(second_visible));
    snprintf(second_visible, sizeof(second_visible), "%s.track_area_second",
             itemPath);
    char three_visible[256] = {0};
    memset(three_visible, 0, sizeof(three_visible));
    snprintf(three_visible, sizeof(three_visible), "%s.track_area_three", itemPath);
    char startbuffer[256] = {0};
    char endbuffer[256] = {0};
    char areColor[256] = {0};
    memset(startbuffer, 0, sizeof(startbuffer));
    snprintf(startbuffer, sizeof(startbuffer),
             "%s.track_area_first.pstion_first_start",
             itemPath);
    memset(endbuffer, 0, sizeof(endbuffer));
    snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_first.pstion_first_end",
             itemPath);

#ifdef HM_FEATURE_BLED_UI
    float track_bar_max_width = gre_get_control_w(getAppInstance(), first_visible);
#else
    float track_bar_max_width = 342.0f;
#endif

    if (bigMaxValue > 0)
    {

        gre_set_control_visible(getAppInstance(), bg_visible, 0);
        gre_set_control_visible(getAppInstance(), first_visible, 1);
        int w = (((float)minValue / (float)bigMaxValue) * track_bar_max_width) - 10.0f;
        setSBFloatData(getAppInstance(), endbuffer, w);

        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_first.firstColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x229DBE);
    }
    else
    {
        setSBFloatData(getAppInstance(), startbuffer, 5.0f);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_first.firstColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x333333);
        gre_set_control_visible(getAppInstance(), first_visible, 0);

    }

    memset(startbuffer, 0, sizeof(startbuffer));
    snprintf(startbuffer, sizeof(startbuffer),
             "%s.track_area_second.pstion_second_start", itemPath);



    if (bigMaxValue > 0)
    {
        int w = (((float)minValue / (float)bigMaxValue) * track_bar_max_width)+2.0f;
        setSBFloatData(getAppInstance(), startbuffer, w);
    }
    else
    {
        setSBFloatData(getAppInstance(), startbuffer, 5.0f);

    }

    memset(endbuffer, 0, sizeof(endbuffer));
    snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_second.pstion_second_end",
             itemPath);

    // memset(endbuffer, 0, sizeof(endbuffer));
    // snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_second.grd_width" ,
    //          itemPath);
    if (bigMaxValue > 0)
    {
        gre_set_control_visible(getAppInstance(), second_visible, 1);
        int w = (((float)maxValue / (float)bigMaxValue - (float)minValue / (float)bigMaxValue) * track_bar_max_width) - 4.0f;
        setSBFloatData(getAppInstance(), endbuffer, w);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_second.sceondColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x1BC371);
    }
    else
    {
        setSBFloatData(getAppInstance(), endbuffer, 0.0f);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_second.sceondColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x333333);
        gre_set_control_visible(getAppInstance(), second_visible, 0);
    }

    memset(startbuffer, 0, sizeof(startbuffer));
    snprintf(startbuffer, sizeof(startbuffer),
             "%s.track_area_three.pstion_three_start", itemPath);
    // memset(startbuffer, 0, sizeof(startbuffer));
    // snprintf(startbuffer, sizeof(startbuffer), "%s.track_area_three.grd_x",
    //          itemPath);

    if (bigMaxValue > 0)
    {
        gre_set_control_visible(getAppInstance(), three_visible, 1);
        int w = (((float)maxValue / (float)bigMaxValue) * track_bar_max_width)+2.0f;
        setSBFloatData(getAppInstance(), startbuffer, w);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_three.threeColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0xD86A32);
    }
    else
    {
        setSBFloatData(getAppInstance(), startbuffer, 6.0f);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_three.threeColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x333333);
        gre_set_control_visible(getAppInstance(), three_visible, 0);
    }

    memset(endbuffer, 0, sizeof(endbuffer));
    snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_three.pstion_three_end",
             itemPath);
    //     memset(endbuffer, 0, sizeof(endbuffer));
    // snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_three.grd_width",
    //          itemPath);
    setSBFloatData(getAppInstance(), endbuffer, track_bar_max_width - (((float)maxValue / (float)bigMaxValue) * track_bar_max_width)-9.0f);
    char color_buffer[256] = {0};
    memset(color_buffer, 0, sizeof(color_buffer));
    snprintf(color_buffer, sizeof(color_buffer),
             "%s.track_area_hite.traning_load_color", itemPath);
    char hitBuffer[256] = {0};
    memset(hitBuffer, 0, sizeof(hitBuffer));
    snprintf(hitBuffer, sizeof(hitBuffer), "%s.track_area_hite.hite_value",
             itemPath);
    char alpha[256] = {0};
    char openarrow_visible[256] = {0};
    memset(openarrow_visible, 0, sizeof(openarrow_visible));
    snprintf(openarrow_visible, sizeof(openarrow_visible), "%s.arrow", itemPath);

    if (vlaue >= 0)
    {
        char image_buffer[256];
        memset(image_buffer, 0, sizeof(image_buffer));
        snprintf(image_buffer, sizeof(image_buffer), "%s.tr_bg.trbg",
                 itemPath);

        if (vlaue == 0)
        {
            gre_set_control_visible(getAppInstance(), image_buffer, 0);
            gre_set_control_visible(getAppInstance(), alpha, 1);
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_one"));
            setSBIntData(getAppInstance(), color_buffer, 0x45B5C8); //#45B5C8
        }
        else if (vlaue > 0 && vlaue <= minValue)
        {
            gre_set_control_visible(getAppInstance(), openarrow_visible, 1);
            gre_set_control_visible(getAppInstance(), image_buffer, 0);
            gre_set_control_visible(getAppInstance(), alpha, 1);
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_one"));
            setSBIntData(getAppInstance(), color_buffer, 0x45B5C8); //#45B5C8
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_first.first_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 255);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_second.second_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_three.three_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
        }

        if (vlaue > minValue && vlaue <= maxValue)
        {
            gre_set_control_visible(getAppInstance(), openarrow_visible, 1);
            gre_set_control_visible(getAppInstance(), image_buffer, 0);
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_two"));
            setSBIntData(getAppInstance(), color_buffer, 0x1BC371); //#1BC371
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_first.first_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_second.second_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 255);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_three.three_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
        }

        if ((vlaue > maxValue && vlaue <= bigMaxValue) || (vlaue > bigMaxValue))
        {
            gre_set_control_visible(getAppInstance(), openarrow_visible, 1);
            gre_set_control_visible(getAppInstance(), image_buffer, 0);
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_three"));
            setSBIntData(getAppInstance(), color_buffer, 0xD86A32); //##D86A32
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_first.first_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_second.second_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_three.three_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 255);
        }
    }

    char train_value[256] = {0};
    memset(train_value, 0, sizeof(train_value));
    snprintf(train_value, sizeof(train_value),
             "%s.track_area_value.training_load_value", itemPath);
    char valueStrff[256];
    memset(valueStrff, 0, sizeof(valueStrff));
    snprintf(valueStrff, sizeof(valueStrff), "%d", (int)vlaue);
    setSBStringData(getAppInstance(), train_value, valueStrff);
    char arrow_buffer[256] = {0};
    memset(arrow_buffer, 0, sizeof(arrow_buffer));
    snprintf(arrow_buffer, sizeof(arrow_buffer), "%s.arrow.arrow_posx", itemPath);
    HLOG_I(TAG, "%s -------arrow:%f\r\n", "%d", vlaue);
    uint16_t maxArrow = bigMaxValue;

    if (vlaue >= bigMaxValue)
    {
        vlaue = bigMaxValue;
    }

#ifdef HM_FEATURE_BLED_UI
    float arrow_max_pos = gre_get_control_w(getAppInstance(), openarrow_visible) - 26/*width of arrow.png*/;
#else
    float arrow_max_pos = 368.0f;
#endif
    // float max_arrow = (float)(-122.0f + (vlaue / (maxArrow == 0 ? 1 : maxArrow) *
    //                                      244.0F));
    float max_arrow = (float)(0.0f + ((vlaue / maxArrow) * arrow_max_pos));
    HLOG_I(TAG, "%s max_arrow:%f\r\n", __func__, max_arrow);
    setSBFloatData(getAppInstance(), arrow_buffer, max_arrow);
}

#else
const void FB_refTrainingLoad(gr_application_t *app, char *itemPath,
                              float vlaue, uint16_t minValue, uint16_t maxValue, uint16_t bigMaxValue)
{
    // vlaue = 300;
    // minValue = 82;
    // maxValue = 220;
    // bigMaxValue = 260;
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff),
             "%s.track_area_last.track_area_last_text", itemPath);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportstatus_tr_last"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff),
             "%s.track_area_tr.track_area_tr_text", itemPath);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sportstatus_tr_hite"));
    _setMaxArrowAttr(app, itemPath);
    char startbuffer[256] = {0};
    char endbuffer[256] = {0};
    char areColor[256] = {0};
    memset(startbuffer, 0, sizeof(startbuffer));
    snprintf(startbuffer, sizeof(startbuffer),
             "%s.track_area_first.first_startAngle", itemPath);
    setSBFloatData(getAppInstance(), startbuffer, 0.0f);
    memset(endbuffer, 0, sizeof(endbuffer));
    snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_first.first_endAngle",
             itemPath);

    if (minValue != 0 || maxValue != 0 || bigMaxValue != 0)
    {
        char arcRound[256] = {0};
        memset(arcRound, 0, sizeof(arcRound));
        snprintf(arcRound, sizeof(arcRound), "%s.track_area_first", itemPath);
        gre_control_set_arc_roundcorner(getAppInstance(), arcRound, 1);
        memset(arcRound, 0, sizeof(arcRound));
        snprintf(arcRound, sizeof(arcRound), "%s.track_area_second", itemPath);
        gre_control_set_arc_roundcorner(getAppInstance(), arcRound, 3);
        memset(arcRound, 0, sizeof(arcRound));
        snprintf(arcRound, sizeof(arcRound), "%s.track_area_three", itemPath);
        gre_control_set_arc_roundcorner(getAppInstance(), arcRound, 2);
    }

    if (bigMaxValue > 0)
    {
        setSBFloatData(getAppInstance(), endbuffer,
                       (((float)minValue / (float)bigMaxValue) * 244.0f));
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_first.firstColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x45B5C8);
    }
    else
    {
        setSBFloatData(getAppInstance(), startbuffer, 0.0f);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_first.firstColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x333333);
    }

    memset(startbuffer, 0, sizeof(startbuffer));
    snprintf(startbuffer, sizeof(startbuffer),
             "%s.track_area_second.second_startAngle", itemPath);

    if (bigMaxValue > 0)
    {
        setSBFloatData(getAppInstance(), startbuffer,
                       (((float)minValue / (float)bigMaxValue) * 244.0f));
    }
    else
    {
        setSBFloatData(getAppInstance(), startbuffer, 0.0f);
    }

    memset(endbuffer, 0, sizeof(endbuffer));
    snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_second.second_endAngle",
             itemPath);

    if (bigMaxValue > 0)
    {
        setSBFloatData(getAppInstance(), endbuffer,
                       (((float)maxValue / (float)bigMaxValue) * 244.0f));
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_second.sceondColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x1BC371);
    }
    else
    {
        setSBFloatData(getAppInstance(), endbuffer, 0.0f);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_second.sceondColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x333333);
    }

    memset(startbuffer, 0, sizeof(startbuffer));
    snprintf(startbuffer, sizeof(startbuffer),
             "%s.track_area_three.three_startAngle", itemPath);

    if (bigMaxValue > 0)
    {
        setSBFloatData(getAppInstance(), startbuffer,
                       (((float)maxValue / (float)bigMaxValue) * 244.0f));
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_three.threeColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0xD86A32);
    }
    else
    {
        setSBFloatData(getAppInstance(), startbuffer, 0.0f);
        memset(areColor, 0, sizeof(areColor));
        snprintf(areColor, sizeof(areColor), "%s.track_area_three.threeColor",
                 itemPath);
        setSBFloatData(getAppInstance(), areColor, 0x333333);
    }

    memset(endbuffer, 0, sizeof(endbuffer));
    snprintf(endbuffer, sizeof(endbuffer), "%s.track_area_three.three_endAngle",
             itemPath);
    setSBFloatData(getAppInstance(), endbuffer, 244.0f);
    char color_buffer[256] = {0};
    memset(color_buffer, 0, sizeof(color_buffer));
    snprintf(color_buffer, sizeof(color_buffer),
             "%s.track_area_hite.traning_load_color", itemPath);
    char hitBuffer[256] = {0};
    memset(hitBuffer, 0, sizeof(hitBuffer));
    snprintf(hitBuffer, sizeof(hitBuffer), "%s.track_area_hite.hite_value",
             itemPath);
    char alpha[256] = {0};

    if (vlaue >= 0)
    {
        memset(alpha, 0, sizeof(alpha));
        snprintf(alpha, sizeof(alpha), "%s.tr_bg",
                 itemPath);

        if (vlaue == 0)
        {
            gre_set_control_visible(getAppInstance(), alpha, 1);
            getTextHeight(System_getLocaleText("id_sportstatus_train_load_one"),itemPath,"track_area_hite");
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_one"));
            setSBIntData(getAppInstance(), color_buffer, 0x45B5C8); //#45B5C8
        }
        else if (vlaue > 0 && vlaue <= minValue)
        {
            gre_set_control_visible(getAppInstance(), alpha, 0);
            getTextHeight(System_getLocaleText("id_sportstatus_train_load_one"),itemPath,"track_area_hite");
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_one"));
            setSBIntData(getAppInstance(), color_buffer, 0x45B5C8); //#45B5C8
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_first.first_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 255);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_second.second_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_three.three_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
        }

        if (vlaue > minValue && vlaue <= maxValue)
        {
            getTextHeight(System_getLocaleText("id_sportstatus_train_load_two"),itemPath,"track_area_hite");
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_two"));
            setSBIntData(getAppInstance(), color_buffer, 0x1BC371); //#1BC371
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_first.first_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_second.second_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 255);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_three.three_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
        }

        if ((vlaue > maxValue && vlaue <= bigMaxValue) || (vlaue > bigMaxValue))
        {
            getTextHeight(System_getLocaleText("id_sportstatus_train_load_three"),itemPath,"track_area_hite");
            setSBStringData(getAppInstance(), hitBuffer,
                            System_getLocaleText("id_sportstatus_train_load_three"));
            setSBIntData(getAppInstance(), color_buffer, 0xD86A32); //##D86A32
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_first.first_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_second.second_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 80);
            memset(alpha, 0, sizeof(alpha));
            snprintf(alpha, sizeof(alpha), "%s.track_area_three.three_alpha",
                     itemPath);
            setSBIntData(getAppInstance(), alpha, 255);
        }
    }

    char train_value[256] = {0};
    memset(train_value, 0, sizeof(train_value));
    snprintf(train_value, sizeof(train_value),
             "%s.track_area_value.training_load_value", itemPath);
    char valueStrff[256];
    memset(valueStrff, 0, sizeof(valueStrff));
    snprintf(valueStrff, sizeof(valueStrff), "%d", (int)vlaue);
    setSBStringData(getAppInstance(), train_value, valueStrff);
    char arrow_buffer[256] = {0};
    memset(arrow_buffer, 0, sizeof(arrow_buffer));
    snprintf(arrow_buffer, sizeof(arrow_buffer), "%s.arrow.arrow_angle", itemPath);
    uint16_t maxArrow = bigMaxValue;

    if (vlaue >= bigMaxValue)
    {
        vlaue = bigMaxValue;
    }

    float max_arrow = (float)(-122.0f + (vlaue / (maxArrow == 0 ? 1 : maxArrow) *244.0F));
    HLOG_I(TAG, "%s max_arrow:%f\r\n", __func__, max_arrow);
    setSBFloatData(getAppInstance(), arrow_buffer, max_arrow);
}
#endif


#ifndef HM_DISPLAY_IS_ROUND


const void FB_refMaxOv2(gr_application_t *app, char *itemPath,
                        float Vo2MaxValue, float Vo2MaxTime, float Vo2MaxStatus)
{
    // Vo2MaxValue = 65;
    // Vo2MaxTime = 1628564664;
    // Vo2MaxStatus = 1;
    HLOG_I(TAG,
           "%s ————————————————VO2max:%f Vo2MaxTime:%f Vo2MaxStatus:%f itemPath:%s\r\n",
           __func__, Vo2MaxValue,
           Vo2MaxTime, Vo2MaxStatus, itemPath);
    _setMaxArrowAttr(app, itemPath);
    char image_buffer[256] = {0};
    memset(image_buffer, 0, sizeof(image_buffer));
    snprintf(image_buffer, sizeof(image_buffer), "%s.track_area.image_bg",
             itemPath);
    char status_buffer[256] = {0};
    memset(status_buffer, 0, sizeof(status_buffer));
    snprintf(status_buffer, sizeof(status_buffer),
             "%s.track_area_three.status_hite", itemPath);
    char arrow_buffer[256] = {0};
    memset(arrow_buffer, 0, sizeof(arrow_buffer));
    snprintf(arrow_buffer, sizeof(arrow_buffer), "%s.arrow.grd_x", itemPath);
    char arrow_visible[256] = {0};
    memset(arrow_visible, 0, sizeof(arrow_visible));
    snprintf(arrow_visible, sizeof(arrow_visible), "%s.arrow", itemPath);

    if (Vo2MaxValue > 65.0f)
    {
        Vo2MaxValue = 65.0f;
    }

    if (Vo2MaxValue > 0 && Vo2MaxValue < 15.0f)
    {
        Vo2MaxValue = 15.0f;
    }

#ifdef HM_FEATURE_BLED_UI
    float max_arrow = 273.0f;
    float max_arrow_avg = 252.0f / 8;
    float start = 3.0f;
    float end = 299.0f;
#else
    float max_arrow = 348.0f;
    float max_arrow_avg = 322.0f / 8;
    float start = 8.0f;
    float end = 382.0f;
#endif
    //float proportion = 0.0f;
    uint8_t proportionMax = 0;
    const sport_quantification_service_getData_t *record_instance =
        sportQuantification_getData();

    if ((int)Vo2MaxValue > 0)
    {
        gre_set_control_visible(getAppInstance(), arrow_visible, 1);
        HLOG_I(TAG, "_________Vo2MaxValue:%f   VO2MAX_LEVEL_VERY_POOR  %f \r\n ",
               Vo2MaxValue, VO2MAX_LEVEL_VERY_POOR);

        switch (record_instance->getVO2MaxLevel(Vo2MaxValue, &proportionMax))
            // switch (6)
        {
            case VO2MAX_LEVEL_VERY_POOR:
            {
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_bg.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_novice"));
                start = (0 + max_arrow_avg * 0);
                end = (0 + max_arrow_avg * 2);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + abs(end - start));
                HLOG_I(TAG, "_________start:%fend %f max_arrow_avg %f max_arrow  %f\r\n ",
                       start, end, max_arrow_avg, max_arrow);
                break;
            }

            case VO2MAX_LEVEL_POOR:
            {
                //getVO2MaxLevel(150, &proportionMax);
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_one.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_getting_started"));
                start = (0 + max_arrow_avg * 2 + 3);
                end = (0 + max_arrow_avg * 3);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start +  abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_FAIR:
            {
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_two.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_general"));
                start = (0 + max_arrow_avg * 3 + 3);
                end = (0 + max_arrow_avg * 4);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start +  abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_AVERAGE:
            {
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_three.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_medium"));
                start = (0 + max_arrow_avg * 4 + 3);
                end = (0 + max_arrow_avg * 5);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start +  abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_GOOD:
            {
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_four.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_good"));
                start = (0 + max_arrow_avg * 5 + 3);
                end = (0 + max_arrow_avg * 6);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start +  abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_VERR_GOOD:
            {
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_five.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_excellent"));
                start = (0 + max_arrow_avg * 6 + 3);
                end = (0 + max_arrow_avg * 7);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start +  abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_EXCELLENT:
            {
                setSBStringData(getAppInstance(), image_buffer,
                                "images/sportstatus/max_six.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_outstanding"));
                start = (0 + max_arrow_avg * 7 + 3);
                end = (0 + max_arrow_avg * 8);
                //proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start +  abs(end - start));
                break;
            }

            default:
                break;
        }
    }
    else
    {
        gre_set_control_visible(getAppInstance(), arrow_visible, 0);
        setSBStringData(getAppInstance(), image_buffer,
                        "images/sportstatus/max_bg_one.png");
    }

    HLOG_I(TAG, "%s max_arrow:%f\r\n", __func__, max_arrow);
    setSBFloatData(getAppInstance(), arrow_buffer, max_arrow);
    char value_buffer[256] = {0};
    memset(value_buffer, 0, sizeof(value_buffer));
    snprintf(value_buffer, sizeof(value_buffer), "%s.track_area_value.value",
             itemPath);
    char valueStrff[256];
    memset(valueStrff, 0, sizeof(valueStrff));

    if ((int)Vo2MaxValue > 0)
    {
        snprintf(valueStrff, sizeof(valueStrff), "%d", (int)Vo2MaxValue);
    }
    else
    {
        snprintf(valueStrff, sizeof(valueStrff), "%s", "--");
    }

    setSBStringData(getAppInstance(), value_buffer, valueStrff);
    _setTime(itemPath, Vo2MaxTime);
    char buff[256] = {0};
    memset(buff, 0, sizeof(buff));
    snprintf(buff, sizeof(buff), "%s.track_area_status.up_or_down", itemPath);
    char buffsAlpha[256] = {0};
    memset(buffsAlpha, 0, sizeof(buffsAlpha));
    snprintf(buffsAlpha, sizeof(buffsAlpha),
             "%s.track_area_status.statusAlpha",
             itemPath);
    char buffs[256] = {0};
    memset(buffs, 0, sizeof(buffs));
    snprintf(buffs, sizeof(buffs), "%s.track_area_status_hite.status_hites",
             itemPath);

    switch ((uint8_t)Vo2MaxStatus)
    {
        case 1:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 255);
            setSBStringData(getAppInstance(), buff, "images/sportstatus/up.png");
            setSBStringData(getAppInstance(), buffs,
                            System_getLocaleText("id_sportstatus_maxVo_status_hite_first"));
            break;
        }

        case 2:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 255);
            setSBStringData(getAppInstance(), buff, "images/sportstatus/down.png");
            setSBStringData(getAppInstance(), buffs,
                            System_getLocaleText("id_sportstatus_maxVo_status_hite_second"));
            break;
        }

        case 3:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 255);
            setSBStringData(getAppInstance(), buff, "images/sportstatus/same.png");
            setSBStringData(getAppInstance(), buffs,
                            System_getLocaleText("id_sportstatus_maxVo_status_hite_three"));
            break;
        }

        default:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 0);
            break;
        }
        break;
    }

    char inter[256] = {0};
    memset(inter, 0, sizeof(inter));
    snprintf(inter, sizeof(inter), "%s.track_area_second.track_area_second_text",
             itemPath);
    setSBStringData(getAppInstance(), inter,
                    System_getLocaleText("id_sportstatus_max_vo_title"));
    memset(inter, 0, sizeof(inter));
    snprintf(inter, sizeof(inter), "%s.track_area_first.track_area_first_text",
             itemPath);
    setSBStringData(getAppInstance(), inter,
                    System_getLocaleText("id_sportstatus_maxVo_unit"));
}

const void FB_refRecoveryTime2(gr_application_t *app, char *itemPath,
                               uint16_t RecoveryTime, uint32_t finishTime)
{
    int recoveryTime = (int)_sport_calculate_recovery_time(RecoveryTime,
                       finishTime);
    //  recoveryTime= 40;
    //  finishTime = 1626166163;
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_value.value", itemPath);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%d", recoveryTime);
    setSBStringData(getAppInstance(), buffer, strff);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_middle_hite.rvt", itemPath);
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText("id_sportstatus_rv_title"));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_uint.rvu", itemPath);
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText("id_sportstatus_rv_unit"));
    char color_buffer[256] = {0};
    memset(color_buffer, 0, sizeof(color_buffer));
    snprintf(color_buffer, sizeof(color_buffer), "%s.track_area.image_backbg",
             itemPath);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_bottom_hite.rvh", itemPath);
    char endbuffer[256] = {0};
    //char areColor[256] = {0};
    char widthbuffer[256] = {0};
    memset(widthbuffer, 0, sizeof(widthbuffer));
    snprintf(widthbuffer, sizeof(widthbuffer), "%s.track_area.grd_width", itemPath);
    char widthstrff[256];
    memset(widthstrff, 0, sizeof(widthstrff));
#ifdef HM_FEATURE_BLED_UI
    snprintf(widthstrff, sizeof(widthstrff), "%f",
             (((float)recoveryTime / 96) * 270));
#else
    snprintf(widthstrff, sizeof(widthstrff), "%f",
             (((float)recoveryTime / 96) * 340));
#endif
    setSBStringData(getAppInstance(), widthbuffer, widthstrff);
    char fill_visible[256] = {0};
    memset(fill_visible, 0, sizeof(fill_visible));
    snprintf(fill_visible, sizeof(fill_visible), "%s.track_area", itemPath);

//recoveryTime = 150;

    if (recoveryTime >= 0 && recoveryTime <= 18)
    {
        gre_set_control_visible(getAppInstance(), fill_visible, 1);
        // char progress_buffer[256] = {0};
        // memset(progress_buffer, 0, sizeof(progress_buffer));
        // snprintf(progress_buffer, sizeof(progress_buffer), "%s.track_area.endwidth",
        //          itemPath);
        //     setSBFloatData(getAppInstance(), endbuffer,
        //                    (((float)recoveryTime /96) * 340));
        // char buffer[256] = {0};
        // memset(buffer, 0, sizeof(buffer));
        // snprintf(buffer, sizeof(buffer), "%s.track_area.grd_width", itemPath);
        memset(endbuffer, 0, sizeof(endbuffer));
        snprintf(endbuffer, sizeof(endbuffer), "%s.track_area.color_fill",
                 itemPath);
        setSBFloatData(getAppInstance(), endbuffer, 0x17C06E);
        // char greenbg_buffer[256] = {0};
        // memset(greenbg_buffer, 0, sizeof(greenbg_buffer));
        // snprintf(greenbg_buffer, sizeof(greenbg_buffer), "%s.track_area.image_green", itemPath);
        //setSBStringData(getAppInstance(), color_buffer, "images/sportstatus/hf-A.png");
        // setSBIntData(getAppInstance(), color_buffer, 0x17C06E); //#17C06E
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_one"));
    }

    if (recoveryTime >= 19 && recoveryTime <= 35)
    {
        gre_set_control_visible(getAppInstance(), fill_visible, 1);
        // setSBFloatData(getAppInstance(), endbuffer,
        //                (((float)recoveryTime /96) * 340));
        memset(endbuffer, 0, sizeof(endbuffer));
        snprintf(endbuffer, sizeof(endbuffer), "%s.track_area.color_fill",
                 itemPath);
        setSBFloatData(getAppInstance(), endbuffer, 0xDFA032);
        // char yellowbg_buffer[256] = {0};
        // memset(yellowbg_buffer, 0, sizeof(yellowbg_buffer));
        // snprintf(yellowbg_buffer, sizeof(yellowbg_buffer), "%s.track_area.image_yellow", itemPath);
        //setSBStringData(getAppInstance(), color_buffer, "images/sportstatus/hf-B.png");
        // setSBIntData(getAppInstance(), color_buffer, 0xDFA032); //#DFA032
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_two"));
    }

    if (recoveryTime >= 36 && recoveryTime <= 53)
    {
        gre_set_control_visible(getAppInstance(), fill_visible, 1);
        // setSBFloatData(getAppInstance(), endbuffer,
        //                (((float)recoveryTime /96) * 340));
        memset(endbuffer, 0, sizeof(endbuffer));
        snprintf(endbuffer, sizeof(endbuffer), "%s.track_area.color_fill",
                 itemPath);
        setSBFloatData(getAppInstance(), endbuffer, 0xCE581A);
        // char orangebg_buffer[256] = {0};
        // memset(orangebg_buffer, 0, sizeof(orangebg_buffer));
        // snprintf(orangebg_buffer, sizeof(orangebg_buffer), "%s.track_area.image_orange", itemPath);
        //setSBStringData(getAppInstance(), color_buffer, "images/sportstatus/hf-C.png");
        //  setSBIntData(getAppInstance(), color_buffer, 0xCE581A); //#CE581A
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_thre"));
    }

    if (recoveryTime >= 54 && recoveryTime <= 96)
    {
        gre_set_control_visible(getAppInstance(), fill_visible, 1);
        // setSBFloatData(getAppInstance(), endbuffer,
        //                (((float)recoveryTime /96) * 340));
        memset(endbuffer, 0, sizeof(endbuffer));
        snprintf(endbuffer, sizeof(endbuffer), "%s.track_area.color_fill",
                 itemPath);
        setSBFloatData(getAppInstance(), endbuffer, 0xC42742);
        // char redbg_buffer[256] = {0};
        // memset(redbg_buffer, 0, sizeof(redbg_buffer));
        // snprintf(redbg_buffer, sizeof(redbg_buffer), "%s.track_area.image_red", itemPath);
        //setSBStringData(getAppInstance(), color_buffer, "images/sportstatus/hf-D.png");
        //setSBIntData(getAppInstance(), color_buffer, 0xC42742); //#C42742
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_four"));
    }

    char progress_buffer[256] = {0};
    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.track_area.endwidth",
             itemPath);
    H_Float progress = (recoveryTime / 96.0f) * 340.0f;
    HLOG_I(TAG, "%s progress:%f\r\n", __func__, progress);
    setSBFloatData(getAppInstance(), progress_buffer, progress);
}

#else



const void FB_refMaxOv2(gr_application_t *app, char *itemPath,
                        float Vo2MaxValue, float Vo2MaxTime, float Vo2MaxStatus)
{
    HLOG_I(TAG, "%s VO2max:%f Vo2MaxTime:%f Vo2MaxStatus:%f itemPath:%s\r\n",
           __func__, Vo2MaxValue,
           Vo2MaxTime, Vo2MaxStatus, itemPath);
    _setMaxArrowAttr(app, itemPath);
    char image_buffer[256] = {0};
    memset(image_buffer, 0, sizeof(image_buffer));
    snprintf(image_buffer, sizeof(image_buffer), "%s.track_area.image_bg",
             itemPath);
    char status_buffer[256] = {0};
    memset(status_buffer, 0, sizeof(status_buffer));
    snprintf(status_buffer, sizeof(status_buffer),
             "%s.track_area_three.status_hite", itemPath);
    char arrow_buffer[256] = {0};
    memset(arrow_buffer, 0, sizeof(arrow_buffer));
    snprintf(arrow_buffer, sizeof(arrow_buffer), "%s.arrow.arrow_angle", itemPath);
    char arrow_visible[256] = {0};
    memset(arrow_visible, 0, sizeof(arrow_visible));
    snprintf(arrow_visible, sizeof(arrow_visible), "%s.arrow", itemPath);

    if (Vo2MaxValue > 65.0f)
    {
        Vo2MaxValue = 65.0f;
    }

    if (Vo2MaxValue > 0 && Vo2MaxValue < 15.0f)
    {
        Vo2MaxValue = 0.0f;
    }

    float max_arrow = 0.0f;
    float max_arrow_avg = 244.0f / 8;
    float start = 0.0f;
    float end = 0.0f;
    float proportion = 0.0f;
    uint8_t proportionMax = 0;
    const sport_quantification_service_getData_t *record_instance =
        sportQuantification_getData();

    if ((int)Vo2MaxValue > 0)
    {
        gre_set_control_visible(getAppInstance(), arrow_visible, 1);

        switch (record_instance->getVO2MaxLevel(Vo2MaxValue, &proportionMax))
        {
            case VO2MAX_LEVEL_VERY_POOR:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_bg.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_novice"));
                start = (-122.0f + max_arrow_avg * 0);
                end = (-122.0f + max_arrow_avg * 2);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_POOR:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_one.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_getting_started"));
                start = (-122.0f + max_arrow_avg * 2);
                end = (-122.0f + max_arrow_avg * 3);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_FAIR:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_two.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_general"));
                start = (-122.0f + max_arrow_avg * 3);
                end = (-122.0f + max_arrow_avg * 4);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_AVERAGE:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_three.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_medium"));
                start = (-122.0f + max_arrow_avg * 4);
                end = (-122.0f + max_arrow_avg * 5);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_GOOD:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_four.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_good"));
                start = (-122.0f + max_arrow_avg * 5);
                end = (-122.0f + max_arrow_avg * 6);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_VERR_GOOD:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_five.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_excellent"));
                start = (-122.0f + max_arrow_avg * 6);
                end = (-122.0f + max_arrow_avg * 7);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            case VO2MAX_LEVEL_EXCELLENT:
            {
                setSBStringData(getAppInstance(), image_buffer, "images/record/max_six.png");
                setSBStringData(getAppInstance(), status_buffer,
                                System_getLocaleText("id_sportstatus_maxVo_outstanding"));
                start = (-122.0f + max_arrow_avg * 7);
                end = (-122.0f + max_arrow_avg * 8);
                proportion = ((float)proportionMax) / 100;
                max_arrow = (float)(start + proportion * abs(end - start));
                break;
            }

            default:
                break;
        }
    }
    else
    {
        gre_set_control_visible(getAppInstance(), arrow_visible, 0);
        setSBStringData(getAppInstance(), image_buffer, "images/record/max_bg_one.png");
    }

    HLOG_I(TAG, "%s max_arrow:%f\r\n", __func__, max_arrow);
    setSBFloatData(getAppInstance(), arrow_buffer, max_arrow);
    char value_buffer[256] = {0};
    memset(value_buffer, 0, sizeof(value_buffer));
    snprintf(value_buffer, sizeof(value_buffer), "%s.track_area_value.value",
             itemPath);
    char valueStrff[256];
    memset(valueStrff, 0, sizeof(valueStrff));

    if ((int)Vo2MaxValue > 0)
    {
        snprintf(valueStrff, sizeof(valueStrff), "%d", (int)Vo2MaxValue);
    }
    else
    {
        snprintf(valueStrff, sizeof(valueStrff), "%s", "--");
    }

    setSBStringData(getAppInstance(), value_buffer, valueStrff);
    _setTime(itemPath, Vo2MaxTime);
    char buff[256] = {0};
    memset(buff, 0, sizeof(buff));
    snprintf(buff, sizeof(buff), "%s.track_area_status.up_or_down", itemPath);
    char buffsAlpha[256] = {0};
    memset(buffsAlpha, 0, sizeof(buffsAlpha));
    snprintf(buffsAlpha, sizeof(buffsAlpha),
             "%s.track_area_status.statusAlpha",
             itemPath);
    char buffs[256] = {0};
    memset(buffs, 0, sizeof(buffs));
    snprintf(buffs, sizeof(buffs), "%s.track_area_status_hite.status_hites",
             itemPath);

    switch ((uint8_t)Vo2MaxStatus)
    {
        case 1:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 255);
            setSBStringData(getAppInstance(), buff, "images/sportstatus/up.png");
            setSBStringData(getAppInstance(), buffs,
                            System_getLocaleText("id_sportstatus_maxVo_status_hite_first"));
            break;
        }

        case 2:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 255);
            setSBStringData(getAppInstance(), buff, "images/sportstatus/down.png");
            setSBStringData(getAppInstance(), buffs,
                            System_getLocaleText("id_sportstatus_maxVo_status_hite_second"));
            break;
        }

        case 3:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 255);
            setSBStringData(getAppInstance(), buff, "images/sportstatus/same.png");
            setSBStringData(getAppInstance(), buffs,
                            System_getLocaleText("id_sportstatus_maxVo_status_hite_three"));
            break;
        }

        default:
        {
            setSBIntData(getAppInstance(), buffsAlpha, 0);
            break;
        }
        break;
    }

    char inter[256] = {0};
    memset(inter, 0, sizeof(inter));
    snprintf(inter, sizeof(inter), "%s.track_area_second.track_area_second_text",
             itemPath);
    setSBStringData(getAppInstance(), inter,
                    System_getLocaleText("id_sportstatus_max_vo_title"));
    memset(inter, 0, sizeof(inter));
    snprintf(inter, sizeof(inter), "%s.track_area_first.track_area_first_text",
             itemPath);
    setSBStringData(getAppInstance(), inter,
                    System_getLocaleText("id_sportstatus_maxVo_unit"));
}

const void FB_refRecoveryTime2(gr_application_t *app, char *itemPath,
                               uint16_t RecoveryTime, uint32_t finishTime)
{
    int recoveryTime = (int)_sport_calculate_recovery_time(RecoveryTime,
                       finishTime);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_value.value", itemPath);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%d", recoveryTime);
    setSBStringData(getAppInstance(), buffer, strff);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_middle_hite", itemPath);
    set_text_aglin(buffer, System_getLocaleText("id_sportstatus_rv_title"),
                   "fonts/allfont-Medium.ttf", 40);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_middle_hite.rvt", itemPath);
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText("id_sportstatus_rv_title"));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_uint.rvu", itemPath);
    setSBStringData(getAppInstance(), buffer,
                    System_getLocaleText("id_sportstatus_rv_unit"));
    char color_buffer[256] = {0};
    memset(color_buffer, 0, sizeof(color_buffer));
    snprintf(color_buffer, sizeof(color_buffer), "%s.track_area.color", itemPath);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_bottom_hite.rvh", itemPath);

    if (recoveryTime >= 0 && recoveryTime <= 18)
    {
        setSBIntData(getAppInstance(), color_buffer, 0x17C06E); //#17C06E
        getTextHeight(System_getLocaleText("id_sportstatus_rv_hite_one"), itemPath,"track_area_bottom_hite");
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_one"));
    }

    if (recoveryTime >= 19 && recoveryTime <= 35)
    {
        setSBIntData(getAppInstance(), color_buffer, 0xDFA032); //#DFA032
        getTextHeight(System_getLocaleText("id_sportstatus_rv_hite_two"), itemPath,"track_area_bottom_hite");
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_two"));
    }

    if (recoveryTime >= 36 && recoveryTime <= 53)
    {
        setSBIntData(getAppInstance(), color_buffer, 0xCE581A); //#CE581A
        getTextHeight(System_getLocaleText("id_sportstatus_rv_hite_thre"), itemPath,"track_area_bottom_hite");
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_thre"));
    }

    if (recoveryTime >= 54 && recoveryTime <= 96)
    {
        setSBIntData(getAppInstance(), color_buffer, 0xC42742); //#C42742
        getTextHeight(System_getLocaleText("id_sportstatus_rv_hite_four"), itemPath,"track_area_bottom_hite");
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sportstatus_rv_hite_four"));
    }

    char progress_buffer[256] = {0};
    memset(progress_buffer, 0, sizeof(progress_buffer));
    snprintf(progress_buffer, sizeof(progress_buffer), "%s.track_area.endAngle",
             itemPath);
    H_Float progress = (recoveryTime / 96.0f) * 244.0f;
    HLOG_I(TAG, "%s progress:%f\r\n", __func__, progress);
    setSBFloatData(getAppInstance(), progress_buffer, progress);
}

#endif
static void _setLeftImage(char *itemPath, float aerobicTe)
{
    HLOG_I(TAG, "%s aerobicTe:%f itemPath:%s\r\n", __func__, aerobicTe, itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teleft.te_left_image", itemPath);

    if ((aerobicTe < 0.000001f) && (aerobicTe > -0.000001f))
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_bg.png");
    }

    if (aerobicTe > 0.0f && aerobicTe <= 0.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_one.png");
    }

    if (aerobicTe >= 1.0f && aerobicTe <= 1.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_two.png");
    }

    if (aerobicTe >= 2.0f && aerobicTe <= 2.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_three.png");
    }

    if (aerobicTe >= 3.0f && aerobicTe <= 3.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_four.png");
    }

    if (aerobicTe >= 4.0f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_five.png");
    }

    _setLeftValue(itemPath, aerobicTe);
    _setLeftArrow(itemPath, aerobicTe);
}
static void _setRightImage(char *itemPath, float anAerobicTe)
{
    HLOG_I(TAG, "%s anAerobicTe:%f itemPath:%s\r\n", __func__, anAerobicTe,
           itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teright.te_right_image", itemPath);

    if ((anAerobicTe < 0.000001f) && (anAerobicTe > -0.000001f))
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_bg.png");
    }

    if (anAerobicTe > 0.0f && anAerobicTe <= 0.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_one.png");
    }

    if (anAerobicTe >= 1.0f && anAerobicTe <= 1.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_two.png");
    }

    if (anAerobicTe >= 2.0f && anAerobicTe <= 2.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_three.png");
    }

    if (anAerobicTe >= 3.0f && anAerobicTe <= 3.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_four.png");
    }

    if (anAerobicTe >= 4.0f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_five.png");
    }

    _setRightValue(itemPath, anAerobicTe);
    _setRightArrow(itemPath, anAerobicTe);
}
static void _setLeftValue(char *itemPath, float value)
{
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%.1f", value);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teAerobic.aeronic", itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

static void _setRightValue(char *itemPath, float value)
{
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%.1f", value);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teAnaerobic.anaerobic", itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

static void _setLeftArrow(char *itemPath, float value)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teleftarrow.left_angle", itemPath);

    if (value > 5.0f)
    {
        value = 5.0f;
    }

    H_Float left_arrow = 0.0f;

    if ((value < 0.000001f) && (value > -0.000001f))
    {
        left_arrow = -141;
    }
    else
    {
        left_arrow = (0.0f - (value / 5.0f) * 104.0f);
        left_arrow = -141 - left_arrow;
    }

    HLOG_I(TAG, "%s left_arrow:%f\r\n", __func__, left_arrow);
    setSBFloatData(getAppInstance(), buffer, left_arrow);
}

static void _setRightArrow(char *itemPath, float value)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.terightarrow.right_angle", itemPath);

    if (value > 5.0f)
    {
        value = 5.0f;
    }

    H_Float rihjt_arrow = 0.0f;

    if ((value < 0.000001f) && (value > -0.000001f))
    {
        rihjt_arrow = 141;
    }
    else
    {
        rihjt_arrow = (0.0f + (value / 5.0f) * 104.0f);
        rihjt_arrow = 141 - rihjt_arrow;
    }

    HLOG_I(TAG, "%s rihjt_arrow:%f\r\n", __func__, rihjt_arrow);
    setSBFloatData(getAppInstance(), buffer, rihjt_arrow);
}

static void _setCurrentEx(char *itemPath, uint32_t value)
{
    HLOG_I(TAG, "%s currentEx:%d itemPath:%s\r\n", __func__, value, itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teCurrentEx.currentEx", itemPath);
    setSBIntData(getAppInstance(), buffer, value);
}


#ifndef HM_DISPLAY_IS_ROUND

static void _setMaxArrowAttr(gr_application_t *app, char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_grdx", itemPath);
    //setSBIntData(getAppInstance(), buffer,
    //((getDisplayMetrics()->width / 2) - (42 / 2)));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_posy", itemPath);
    //setSBIntData(getAppInstance(), buffer, 22);
    // memset(buffer, 0, sizeof(buffer));
    // snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_centerX", itemPath);
    // setSBFloatData(getAppInstance(), buffer, getDisplayMetrics()->width / 2.0f);
    // memset(buffer, 0, sizeof(buffer));
    // snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_centerY", itemPath);
    // setSBFloatData(getAppInstance(), buffer, getDisplayMetrics()->height / 2.0f);
}

#else

static void _setMaxArrowAttr(gr_application_t *app, char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_posx", itemPath);
    setSBIntData(getAppInstance(), buffer,
                 ((getDisplayMetrics()->width / 2) - (42 / 2)));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_posy", itemPath);
    setSBIntData(getAppInstance(), buffer, 22);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_centerX", itemPath);
    setSBFloatData(getAppInstance(), buffer, getDisplayMetrics()->width / 2.0f);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_centerY", itemPath);
    setSBFloatData(getAppInstance(), buffer, getDisplayMetrics()->height / 2.0f);
}
#endif


static void _setTime(char *itemPath, float time)
{
    HLOG_I(TAG, "%s Vo2MaxTime:%f\r\n", __func__, time);
    char strff[256] = {0};

    if (time > FLOAT_PRECISION)
    {
        memset(strff, 0, sizeof(strff));
        HmTimeDate timeDate = {0};
        HmTime rspTime = {0};
        rspTime.sec = (uint32_t)time;
        // hmTime_getTime(&rspTime);
        hmTime_getLocalTime(&rspTime, &timeDate);
        HmTime currentTime = {0};
        hmTime_getTime(&currentTime);
        // ##
        uint32_t current_sec = currentTime.sec;
        int64_t diff_sec = current_sec - time;
        int32_t diff_hour = diff_sec / 3600;
        int32_t diff_min = diff_sec / 60;

        if ((diff_hour > 0 && diff_hour <= (24 - diff_hour)) || (diff_hour == 0
                && diff_min > 0) || (diff_sec > 0 && diff_sec < 60))
        {
            snprintf(strff, sizeof(strff),
                     System_getLocaleText("id_sportstatus_maxVo_update_today"));
        }
        else if (diff_hour >= (24 - diff_hour) &&  diff_hour <= (48 - diff_hour))
        {
            snprintf(strff, sizeof(strff),
                     System_getLocaleText("id_sportstatus_maxVo_update_yesterday"));
        }
        else
        {
            snprintf(strff, sizeof(strff) / sizeof(strff[0]),
                     System_getLocaleText("id_sportstatus_maxVo_time"), timeDate.year,
                     timeDate.month, timeDate.day, timeDate.hour, timeDate.minute);
        }
    }

    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.track_area_hite.updata_time", itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

const uint32_t _sport_calculate_recovery_time(uint32_t recv_time,
        uint32_t finish_time)
{
#define RECOVERY_TIME_FULL 97
    HmTime rspTime = {0};
    hmTime_getTime(&rspTime);
    // ##
    uint32_t current_sec = rspTime.sec;
    int64_t diff_sec = current_sec - finish_time;
    int32_t diff_recovery_sec = (recv_time * 60) - diff_sec;
    int32_t recovery_min = (diff_recovery_sec + 30) / 60;

    if (recovery_min > 0 && recovery_min <= (RECOVERY_TIME_FULL * 60))
    {
        recovery_time = recovery_min / 60;
    }
    else
    {
        recovery_time = 0;
    }

    return recovery_time;
}

#ifdef  HM_DISPLAY_IS_ROUND
static void getTextHeight(const char *info, char *itemPath,char *controlName)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer),
             "%s.%s", itemPath, controlName);
    H_Int32 titleWidth = gre_get_control_w(getAppInstance(), buffer);
    gr_string_size_t sz = {0};
    gre_get_string_size(getAppInstance(),
                        info,
                        "fonts/allfont-Regular.ttf", 32, titleWidth, &sz, true, 100);
    gre_set_control_h(getAppInstance(), buffer, (sz.height + getDimension(100)));
}
#endif
