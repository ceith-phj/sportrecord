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

#ifndef RECORD_PARAMETER_H
#define RECORD_PARAMETER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    FROM_SPORT_END_DETAIL = 0,
    FROM_SPORT_RECORD_LIST_DETAIL = 1,
} Record_Detail_Come_From;

typedef struct _record_parameter_data
{
    /* data */
    uint16_t sport_type;
    uint32_t track_id;
    uint16_t auto_reg;
    uint32_t child_trcak_id;
    Record_Detail_Come_From from_record_detail;
} record_parameter_data;

typedef struct _record_h_page_data
{
    uint8_t pageNum;
    bool isPersonalRecordFirst;
    bool isPersonalRecordSeocnd;
    bool isPersonalRecordThree;
    bool isPersonalRecordFour;
    bool isPersonalRecordFive;
    bool isPersonalRecordSix;
    bool isPersonalRecordSenven;
    bool isRecoveryTime;
    bool isMaximalOxygenUptake;
    bool isTrainingEff;
    bool isDetail;
    bool isSegment;
    bool isGoal;
    bool isClose;
} record_h_page_data;

typedef struct
{
    char icon[40];
    char title[40];
    uint8_t sportType;
    uint8_t item_num;
} pb_list;

#ifdef __cplusplus
}
#endif

#endif