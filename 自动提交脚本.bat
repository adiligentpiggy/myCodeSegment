@echo off 

git status
echo 提交按任意键！ 
echo. & pause

echo 正在提交...... 
git add .
git commit -m "Update notes."
git pull --rebase
git push origin master

echo 提交完成！ 
echo. & pause 

