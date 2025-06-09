def boyer_moore(text, pattern):
    def bad_character_table(pattern):
        """生成坏字符表"""
        table = {}
        length = len(pattern)
        for i in range(length):
            table[pattern[i]] = i
        
        print('bad_character_table ' + str(table))
        return table

    def good_suffix_table(pattern):
        """生成好后缀表"""
        length = len(pattern)
        table = [0] * (length + 1)
        last_prefix_position = length

        for i in range(length):
            if is_prefix(pattern, i + 1):
                last_prefix_position = i + 1
            table[length - i] = last_prefix_position - i + length - 1

        for i in range(length - 1):
            slen = suffix_length(pattern, i)
            table[slen] = length - 1 - i + slen

        print('good_suffix_table ' + str(table))
        return table

    def is_prefix(pattern, p):
        """判断子串pattern[p:]是否是pattern的前缀"""
        length = len(pattern)
        for i in range(p, length):
            if pattern[i] != pattern[i - p]:
                return False
        return True

    def suffix_length(pattern, p):
        """返回pattern[p:]的后缀与pattern的最长匹配长度"""
        length = len(pattern)
        i = 0
        while (p - i >= 0) and (pattern[p - i] == pattern[length - 1 - i]):
            i += 1
        return i

    # 预处理阶段
    bad_char_table = bad_character_table(pattern)
    good_suffix_table = good_suffix_table(pattern)

    # 搜索阶段
    m, n = len(pattern), len(text)
    i = 0

    while i <= n - m:
        j = m - 1
        while j >= 0 and pattern[j] == text[i + j]:
            j -= 1
        if j < 0:
            return i
        else:
            bc_shift = j - bad_char_table.get(text[i + j], -1)
            gs_shift = good_suffix_table[j + 1]
            i += max(bc_shift, gs_shift)

    return -1

# 示例使用
text = "HERE IS A SIMPLE EXAMPLE"
pattern = "EXAMPLE"
result = boyer_moore(text, pattern)

if result != -1:
    print(f"Pattern found at index {result}")
else:
    print("Pattern not found")
