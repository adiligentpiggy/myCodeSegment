import os
import shutil

 
def ensure_directory(path):
    if not os.path.exists(path):
        os.makedirs(path)

def copy_files(src, dst):
    # 检查源路径是否存在
    if not os.path.exists(src):
        print(f"Source path {src} does not exist.")
        return
        
    # 如果目标目录不存在，则创建它
    if not os.path.exists(dst):
        os.makedirs(dst)    

    for item in os.listdir(src):
        src_path = os.path.join(src, item)
        dst_path = os.path.join(dst, item)

        # 检查是否为目录
        if os.path.isdir(src_path):
            # 如果目标目录不存在，则创建它
            if not os.path.exists(dst_path):
                os.makedirs(dst_path)
            copy_files(src_path, dst_path)  # 递归调用
        else:
            # 如果目标文件已存在，则跳过复制
            if not os.path.exists(dst_path):
                shutil.copy2(src_path, dst_path)  # 拷贝文件 





# 指定源目录和目标目录的路径
root_dir = r'Z:\code\di6_dev\ap_qssi'
target_dir = r'D:\code\di6_dev\ap_qssi'

child_dirs = [r'\frameworks\av\media', 
r'\frameworks\av\services',
r'\frameworks\base\core\java\android\os',
r'\frameworks\base\media',
r'\frameworks\native',
r'\frameworks\rs',
r'\system\hwservicemanager',
r'\system\media',
r'\vendor\qcom\proprietary\commonsys\mm-parser'
]

# 执行复制操作
# for item in child_dirs:
    # copy_files(root_dir + item, target_dir + item)    


copy_files(r'D:\doc\di-new', r'D:\code\multimedia\dilink6')

