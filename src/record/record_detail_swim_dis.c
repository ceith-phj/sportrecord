#include <record/record_detail_swim_dis.h>
#include "hlog.h"

const float _SwimPoolLenght(float dis){
    SwimPoolLength swim_pool = {0};
    uint32_t pool_length = 25;
    if (sportSetting_getSettingManager()->poolLength->get(SPORT_TYPE_SWIMMING_POOL, &swim_pool)) {
        if (swim_pool.choice.choice == 0 ||
            swim_pool.choice.choice == 2)
            pool_length = 25;
        else if (swim_pool.choice.choice == 1 ||
                  swim_pool.choice.choice == 3)
            pool_length = 50;
        else if (swim_pool.choice.choice == 4)
            pool_length = (uint32_t)swim_pool.length.data;
    }
    HLOG_I("_SwimPoolLenght","RecordSwimLength %d %d\r\n", swim_pool.choice.choice,pool_length);
    if (((uint32_t)dis % pool_length) == 0)
    {
        return dis;
    }else{
        return (((uint32_t)dis / pool_length) + 1) * pool_length;
    }
    return 0;
}