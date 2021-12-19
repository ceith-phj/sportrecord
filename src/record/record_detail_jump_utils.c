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

#include <record/record_detail_jump_utils.h>
#define TAG "record_detail_jump_utils"

static record_parameter_data recordParameterData = {0};

const void singleSportjumpDetailUtiles(uint32_t item_id, ScreenFlag flag)
{
    HLOG_I(TAG, "%s clickItemId:%d\r\n", __func__, item_id);
    record_list_item_Data recordListItemDate =
        getRecordListManager()->getRecordListData();
    memset(&recordParameterData, 0, sizeof(record_parameter_data));
    recordParameterData.sport_type =
        recordListItemDate.record_list_Data.list_info[item_id].sport_type;
    recordParameterData.track_id =
        recordListItemDate.record_list_Data.list_info[item_id].track_id;
    recordParameterData.child_trcak_id =
        recordListItemDate.record_list_Data.list_info[item_id].track_id;
    recordParameterData.auto_reg =
        recordListItemDate.record_list_Data.list_info[item_id].auto_reg;
    if (recordListItemDate.record_list_Data.list_info[item_id].multi == 2)
    {
        getScreenManager()->startScreenByFlag("SportMultipleScreen", flag,
                                              (void *)&recordParameterData, sizeof(record_parameter_data));
    }
    else
    {
        getScreenManager()->startScreenByFlag("SportRecordDetailScreen", flag,
                                              (void *)&recordParameterData, sizeof(record_parameter_data));
    }
}

const void singleSportjumpDetail(uint32_t item_id, ScreenFlag flag,
                                 Record_Detail_Come_From recordDetailComeFrom)
{
    HLOG_I(TAG, "%s clickItemId:%d\r\n", __func__, item_id);
    record_list_item_Data recordListItemDate =
        getRecordListManager()->getRecordListData();
    memset(&recordParameterData, 0, sizeof(record_parameter_data));
    recordParameterData.sport_type =
        recordListItemDate.record_list_Data.list_info[item_id].sport_type;
    recordParameterData.track_id =
        recordListItemDate.record_list_Data.list_info[item_id].track_id;
    recordParameterData.child_trcak_id =
        recordListItemDate.record_list_Data.list_info[item_id].track_id;
    recordParameterData.auto_reg =
        recordListItemDate.record_list_Data.list_info[item_id].auto_reg;
    recordParameterData.from_record_detail = recordDetailComeFrom;
    if (recordListItemDate.record_list_Data.list_info[item_id].multi == 2)
    {
        getScreenManager()->startScreenByFlag("SportMultipleScreen", flag,
                                              (void *)&recordParameterData, sizeof(record_parameter_data));
    }
    else
    {
        getScreenManager()->startScreenByFlag("SportRecordDetailScreen", flag,
                                              (void *)&recordParameterData, sizeof(record_parameter_data));
    }
}