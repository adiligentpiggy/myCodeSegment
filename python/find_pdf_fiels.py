import os

def find_pdf_files(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.pdf'):
                print(os.path.join(root, file))

# 替换为你想要遍历的目录路径
directory_path = r'D:\books\economic'
find_pdf_files(directory_path)