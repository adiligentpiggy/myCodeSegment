import os

def remove_suffix_1_from_files(directory):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            # 检查文件是否以.1结尾
            if filename.endswith('.1'):
                # 构造原文件的完整路径
                old_file = os.path.join(root, filename)
                # 构造新文件名（去掉.1后缀）
                new_filename = filename[:-2]
                # 构造新文件的完整路径
                new_file = os.path.join(root, new_filename)
                # 重命名文件
                os.rename(old_file, new_file)
                print(f'Renamed "{old_file}" to "{new_file}"')

# 替换为你想要遍历的目录路径
your_directory = r'D:\doc\di-new'
remove_suffix_1_from_files(your_directory)