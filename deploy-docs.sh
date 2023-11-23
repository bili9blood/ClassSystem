#!/usr/bin/env sh

# 确保脚本抛出遇到的错误
set -e

cd docs

# 生成静态文件
yarn install
yarn global add vuepress
export NODE_OPTIONS=--openssl-legacy-provider
yarn run docs:build

# 进入生成的文件夹
cd .vuepress/dist

# 如果是发布到自定义域名
# echo 'www.example.com' > CNAME

git init


git config user.name "class-system"
git config user.email "3501202786@qq.com"


git add -A
git commit -m 'deploy'

git push -f https://class-system:$1@gitee.com/class-system/docs.git master
echo "Done."
