@echo off 

git status
echo �ύ��������� 
echo. & pause

echo �����ύ...... 
git add .
git commit -m "Update notes."
git pull --rebase
git push origin master

echo �ύ��ɣ� 
echo. & pause 

