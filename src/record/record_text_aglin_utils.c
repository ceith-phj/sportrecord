#include <record/record_text_aglin_utils.h>
#define TAG "record_text_aglin_utils"

const void set_text_aglin(char *itemGroupPath, const char *title,
                          char *fontStyle,
                          int fontSize)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s", itemGroupPath);
    H_Int32 titleWidth = gre_get_control_w(getAppInstance(), buffer);
    gr_string_size_t sz = {0};
    gre_get_string_size(getAppInstance(), title, fontStyle, fontSize, titleWidth,
                        &sz,
                        0, 100);
    HLOG_I(TAG, "%s itemGroupPath:%s length:%d\r\n", __func__, itemGroupPath,
           strlen(itemGroupPath));
    H_Int32 tnLength = sz.width;
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer),
             "%s.align", itemGroupPath);

    if (tnLength > titleWidth)
    {
        setSBIntData(getAppInstance(), buffer, GRE_ALIGN_LEFT);
    }
    else
    {
        setSBIntData(getAppInstance(), buffer, GRE_ALIGN_CENTER);
    }
}

const void set_text_height(char *itemGroupPath, const char *title,
                           char *fontStyle,
                           int fontSize)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s", itemGroupPath);
    H_Int32 textWidth = gre_get_control_w(getAppInstance(), buffer);
    H_Int32 textHeight = gre_get_control_h(getAppInstance(), buffer);
    gr_string_size_t sz = {0};
    gre_get_string_size(getAppInstance(), title, fontStyle, fontSize, textWidth,
                        &sz,
                        1, 100);
    HLOG_I(TAG, "%s itemGroupPath:%s length:%d\r\n", __func__, itemGroupPath,
           strlen(itemGroupPath));

    if (textHeight > sz.height)
    {
        gre_set_control_h(getAppInstance(), buffer, getDimension(textHeight));
    }
    else
    {
        gre_set_control_h(getAppInstance(), buffer, getDimension(sz.height));
    }
}

const void get_text_width(char *itemGroupPath, const char *title,
                          char *fontStyle,
                          int fontSize, uint8_t isWrap, TextAttr *textattr)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s", itemGroupPath);
    H_Int32 textWidth = gre_get_control_w(getAppInstance(), buffer);
    H_Int32 textHeight = gre_get_control_h(getAppInstance(), buffer);
    gr_string_size_t sz = {0};
    gre_get_string_size(getAppInstance(), title, fontStyle, fontSize, textWidth,
                        &sz,
                        isWrap, 100);
    HLOG_I(TAG, "%s itemGroupPath:%s length:%d\r\n", __func__, itemGroupPath,
           strlen(itemGroupPath));

    if (textHeight > sz.height)
    {
        textattr->height = textHeight;
    }
    else
    {
        textattr->height = sz.height;
    }

    if (textWidth > sz.width)
    {
        textattr->width = textWidth;
    }
    else
    {
        textattr->width = sz.width;
    }
}