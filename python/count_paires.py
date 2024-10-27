def count_pairs(n, students):
    # 统计四类学生的数量
    count = [0] * 4  # [偶,偶], [偶,奇], [奇,偶], [奇,奇]
 
    for a, b in students:
        a_parity = a % 2
        b_parity = b % 2
        count[a_parity * 2 + b_parity] += 1
 
    # 计算可以组成的对数
    total_pairs = 0
    for k in count:
        total_pairs += k * (k - 1) // 2
 
    return total_pairs
 
# 输入处理
n = int(input())
students = [tuple(map(int, input().split())) for _ in range(n)]
 
# 输出结果
print(count_pairs(n, students))