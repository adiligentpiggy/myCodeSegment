#include <stdio.h>
#include <stdbool.h>
#include "hashmap.h"

typedef unsigned short uint16;
typedef unsigned char uint8;

#define TCMS_STATION_TOTAL_NUM 32
#define TCMS_INVALID_VALUE     0
#define TCMS_VALID_VALUE     1

#define  U8toU16(A)  (uint16)((((uint16)(A)[0])<<8) | (A)[1])

enum {
    TRAIN_UP_GOING,
    TRAIN_DOWN_GOING,
    TRAIN_UP_DOWN_INVALID
};

static hmap_t s_jump_station_map = NULL;
static uint16 s_station_id_arr[TCMS_STATION_TOTAL_NUM] = {1,2,3,4,5,TCMS_INVALID_VALUE};


hmap_t get_jump_station_map(void)
{
    if (s_jump_station_map == NULL) {
        s_jump_station_map = hashmap_create();
    }
    return s_jump_station_map;
}

void destroy_jump_station_map(void)
{
    if (s_jump_station_map == NULL) return;
    hashmap_destroy(s_jump_station_map, NULL, 0);
    s_jump_station_map = NULL;
}

static void init_jump_station_map(void)
{
    hmap_t map = get_jump_station_map();
    uint8 i = 0;

    for (; i < TCMS_STATION_TOTAL_NUM; i++)
    {
        hashmap_put(map, (char *)&(s_station_id_arr[i]), (void_ptr)TCMS_INVALID_VALUE);
    }
}

bool set_bit_value(uint8 * p_byData, const uint8 bit_pos, const uint8 value)
{
    if (bit_pos < 0 || bit_pos > 7)
    {
        return false;
    }

    *p_byData = (*p_byData & ~(1 << bit_pos)) | (value << bit_pos);

    return true;
}

static int iter_print_jump_station_elems(void* elem, void *arg) {
  uint8 *el = (uint8 *) elem;
  printf("value=%d\n", *el);
  return HMAP_S_OK;
}

static void convert_manual_jump_station_to_pre_jump(
    const uint16 manual_jump_cur_sta_id,
    const uint16 manual_jump_next_sta_id,
    uint8 updown,
    uint16 * ptr_station_status_1_16,
    uint16 * ptr_station_status_17_32)
{
    hmap_t map = get_jump_station_map();
    uint16 start_station_id = updown == TRAIN_UP_GOING ?
        manual_jump_cur_sta_id : manual_jump_next_sta_id;
    uint16 terminate_station_id = updown == TRAIN_UP_GOING ?
        manual_jump_next_sta_id : manual_jump_cur_sta_id;

    //hashmap_put(map, &(s_station_id_arr[start_station_id]), (void_ptr)TCMS_INVALID_VALUE);

    uint16 i = start_station_id + 1;

    for (; i < terminate_station_id; i++)
    {
        hashmap_put(map, (char *)&(s_station_id_arr[i]), (void_ptr)TCMS_VALID_VALUE);
    }

    for (; i < TCMS_STATION_TOTAL_NUM; i++)
    {
        hashmap_put(map, (char *)&(s_station_id_arr[i]), (void_ptr)TCMS_INVALID_VALUE);
    }

    uint8 is_station_jump = TCMS_INVALID_VALUE;
    uint8 temp[4] = {TCMS_INVALID_VALUE};
    uint8 j;

    for (i = 0, j = 0; j < TCMS_STATION_TOTAL_NUM; j++)
    {
        if (j > 0 && j % 8 == 0)
        {
            i++;
        }
        if (HMAP_S_OK == hashmap_get(map, (const char *)&(s_station_id_arr[j]), (void_ptr *) &is_station_jump))
        {
            set_bit_value(&(temp[i]), j, is_station_jump);
        }
    }
    *ptr_station_status_1_16 = U8toU16(&(temp[0]));
    *ptr_station_status_17_32 = U8toU16(&(temp[2]));

    hashmap_iterate(map, iter_print_jump_station_elems, NULL);
}

void main()
{
    init_jump_station_map();

    uint16 station_status_1_16 = TCMS_INVALID_VALUE;
    uint16 station_status_17_32 = TCMS_INVALID_VALUE;

    convert_manual_jump_station_to_pre_jump(
        1,
        4,
        0,
        &station_status_1_16,
        &station_status_17_32);
    
    destroy_jump_station_map();
}
