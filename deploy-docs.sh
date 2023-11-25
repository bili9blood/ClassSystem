#!/usr/bin/env sh

# 确保脚本抛出遇到的错误
set -e

rm -rf .git

cd docs

GITEE_REPO=https://class-system:$1@gitee.com/class-system/docs.git

mkdir .vuepress/dist
git clone $GITEE_REPO .vuepress/dist

cd .vuepress/dist

echo "status before"
git status

cd -

# 生成静态文件
yarn install
yarn global add vuepress
NODE_OPTIONS=--openssl-legacy-provider
yarn run docs:build

# 进入生成的文件夹
cd .vuepress/dist

echo "status after"
echo $PWD
git status

# git config user.name "class-system"
# git config user.email "3501202786@qq.com"

# unset http_proxy
# unset https_proxy

# git add .
# git commit -m 'deploy'

# git push $GITEE_REPO master
# echo "Done."
