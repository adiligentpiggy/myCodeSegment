from datetime import datetime

# 时间点数据
# timestamps = [
    # "17:52:08.360",
    # "17:52:08.666",
    # "17:52:16.291",
    # "17:52:16.425",
    # "17:52:23.775",
    # "17:52:23.939",
    # "17:52:30.860",
    # "17:52:31.000",
    # "17:52:35.625",
    # "17:52:35.800",
    # "17:52:40.573",
    # "17:52:40.692",
    # "17:52:46.993",
    # "17:52:47.453",
    # "17:52:53.624",
    # "17:52:54.026",
    # "17:53:01.295",
    # "17:53:01.478",
    # "17:53:07.789",
    # "17:53:07.918",
    # "17:53:14.975",
    # "17:53:15.181",
    # "17:53:19.046",
    # "17:53:19.220",
    # "17:53:23.806",
    # "17:53:23.942",
    # "17:53:28.297",
    # "17:53:28.852",
    # "17:53:34.166",
    # "17:53:34.642",
    # "17:53:38.942",
    # "17:53:39.184",
    # "17:53:45.443",
    # "17:53:45.552"
# ]

timestamps = [
"12:02:24.996",
"12:02:25.105",
"12:02:34.482",
"12:02:34.682",
"12:02:44.695",
"12:02:44.837",
"12:02:53.403",
"12:02:53.418",
"12:03:01.370",
"12:03:01.564",
"12:03:15.585",
"12:03:15.781",
"12:03:26.467",
"12:03:26.617",
"12:03:35.668",
"12:03:35.839",
"12:03:44.467",
"12:03:44.664",
"12:03:52.882",
"12:03:53.075",
"12:04:01.200",
"12:04:01.285"
]

# 将字符串时间转换为datetime对象
timestamps = [datetime.strptime(time, "%H:%M:%S.%f") for time in timestamps]

# 计算每组时间的差值
for i in range(0, len(timestamps) - 1, 2):
    start_time = timestamps[i]
    end_time = timestamps[i + 1]
    time_difference = end_time - start_time
    #print(f"Time difference between {start_time} and {end_time}: {time_difference}")
    print(f"Time difference: {time_difference}")
