#!/usr/bin/env sh

set -e

rm -rf .git

cd docs

GITEE_REPO=https://class-system:$1@gitee.com/class-system/docs.git

mkdir publish
git clone $GITEE_REPO publish

# 生成静态文件
yarn install
yarn global add vuepress
NODE_OPTIONS=--openssl-legacy-provider
yarn run docs:build

yes | cp -rf .vuepress/dist/. publish

cd publish

git config user.name "class-system"
git config user.email "3501202786@qq.com"

git add .
git commit -m 'deploy'

git push origin master
echo "Done."
