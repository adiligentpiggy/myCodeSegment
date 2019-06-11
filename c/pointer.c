#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint32;

typedef struct ST_UPLOAD_V_STATUS_ID {
    char * topic;
    char * id;
}st_UPLOAD_V_STATUS_ID;

typedef int (*PA_APP_CALL_BACK)(uint32 param1, uint32 param2);

int mqttUpload_vehicle_status(uint32 param1, uint32 param2)
{
    //st_RR_VEHICLE_STATUS vehicle_status = {"", "", "", "", "", "", "", 0};
    printf("param1:%ld\n", param1);
    st_UPLOAD_V_STATUS_ID * vehicle_status_mode = (st_UPLOAD_V_STATUS_ID *)param1;
	printf("vehicle_status_mode:%ld\n", vehicle_status_mode);
    if (vehicle_status_mode == NULL)
    {
        printf("vehicle_status_mode is NULL, return\n");
        return -1;
    }

    char * vehicle_status_id = vehicle_status_mode->id;
	printf("vehicle_status_id:%s\n", vehicle_status_id);

    free(vehicle_status_mode);

    return 0;
}

void test(uint32 param1, uint32 param2)
{
	PA_APP_CALL_BACK cb = (PA_APP_CALL_BACK)param1;
	if (cb != NULL) {
		(*cb)(param2, 0);
	}
}

void main(void)
{
    st_UPLOAD_V_STATUS_ID * v_status_id2 = malloc(sizeof(st_UPLOAD_V_STATUS_ID)*1000);
	st_UPLOAD_V_STATUS_ID * v_status_id = malloc(sizeof(st_UPLOAD_V_STATUS_ID));
	
	
	
    memset(v_status_id, 0, sizeof(st_UPLOAD_V_STATUS_ID));
    v_status_id->topic = "how_to_find_error";
    v_status_id->id = "123";
	//printf("uint32 size:%ld %lu\n", sizeof(uint32), 0xFFFFFFFF);
	printf("v_status_id:%ld v_status_id2:%ld\n", v_status_id, v_status_id2);
    //mqttUpload_vehicle_status((uint32)v_status_id, 0);
	test((uint32)mqttUpload_vehicle_status, (uint32)v_status_id);
}
