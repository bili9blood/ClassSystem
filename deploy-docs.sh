#!/usr/bin/env sh

# 确保脚本抛出遇到的错误
set -e

# 生成静态文件
yarn install
yarn run docs:build

# 进入生成的文件夹
cd .vuepress/dist

# 如果是发布到自定义域名
# echo 'www.example.com' > CNAME

git init


git config user.name "class-system-docs"
git config user.email "3501202786@qq.com"


git add -A
git commit -m 'deploy'

if [-z "$1"]
then
  echo "Done."
else
  git push -f https://$1@gitee.com/class-system-docs/class-system-docs.git master
  echo "Done."
fi