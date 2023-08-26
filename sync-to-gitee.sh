#!/usr/bin/env sh

# 确保脚本抛出遇到的错误
set -e

git config user.name "class-system-docs"
git config user.email "3501202786@qq.com"

git add -A
git commit -m 'deploy'

if [-z "$1"]
then
  echo "Done."
else
  git push -f https://$1@gitee.com/class-system-docs/ClassSystem.git main:master
  echo "Done."
fi