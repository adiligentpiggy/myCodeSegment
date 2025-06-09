def bad_character_table(pattern):
    """生成坏字符表"""
    bc = [-1] * 256  # ASCII字符集大小
    for i in range(len(pattern)):
        bc[ord(pattern[i])] = i

    print('bad_character_table ' + str(bc))
    return bc

def good_suffix_table(pattern):
    """生成好后缀表"""
    m = len(pattern)
    # 初始化好后缀表
    gs = [m] * (m + 1)
    f = [0] * (m + 1)
    i = m
    j = m + 1
    f[i] = j
    while i > 0:
        while j <= m and pattern[i - 1] != pattern[j - 1]:
            if gs[j] == m:
                gs[j] = m - i
            j = f[j]
        i -= 1
        j -= 1
        f[i] = j
    j = f[0]
    for i in range(m + 1):
        if gs[i] == m:
            gs[i] = j
        if i == j:
            j = f[j]

    print('good_suffix_table ' + str(gs))
    return gs

def boyer_moore_search(text, pattern):
    """Boyer-Moore字符串搜索算法"""
    n = len(text)
    m = len(pattern)
    if m == 0:
        return 0
    bc = bad_character_table(pattern)
    gs = good_suffix_table(pattern)
    i = 0  # text的索引
    while i <= n - m:
        j = m - 1
        while j >= 0 and pattern[j] == text[i + j]:
            j -= 1
        if j < 0:
            return i  # 找到匹配，返回索引
        else:
            # 坏字符规则
            bad_char_shift = j - bc[ord(text[i + j])]
            # 好后缀规则
            good_suffix_shift = gs[j + 1]
            # 选择较大的移动步数
            i += max(bad_char_shift, good_suffix_shift)
    return -1  # 未找到匹配

# 测试
text = "HERE IS A SIMPLE EXAMPLE"
pattern = "EXAMPLE"
index = boyer_moore_search(text, pattern)
print(f"Pattern found at index {index}" if index != -1 else "Pattern not found")
