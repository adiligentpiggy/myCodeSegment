def max_candies(n, k, alpha):
    # 初始化dp数组
    dp = [[0] * (k + 1) for _ in range(n + 1)]
    
    # 动态规划计算
    for i in range(1, n + 1):
        for j in range(1, k + 1):
            if j >= alpha[i - 1]:
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - alpha[i - 1]] + 1)
            else:
                dp[i][j] = dp[i - 1][j]
    
    # 返回结果
    return dp[n][k]

# 读取输入
n, k = map(int, input().split())
alpha = list(map(int, input().split()))

# 调用函数并输出结果
print(max_candies(n, k, alpha))
