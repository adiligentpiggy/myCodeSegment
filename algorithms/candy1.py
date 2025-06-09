# 读取输入
n, k = map(int, input().split())
alpha = list(map(int, input().split()))

# 排序
alpha.sort()

# 计算
sum = 0
count = 0
for i in range(n):
    if sum + alpha[i] <= k:
        sum += alpha[i]
        count += 1
    else:
        break

# 输出
print(count)
