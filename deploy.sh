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


git config user.name "bili9Blood"
git config user.email "3501202786@qq.com"


git add -A
git commit -m 'deploy'


# 如果发布到 https://<USERNAME>.github.io
# git push -f git@github.com:<USERNAME>/<USERNAME>.github.io.git master

# 如果发布到 https://<USERNAME>.github.io/<REPO>
git push -f https://$1@github.com/bili9blood/class-system-docs.git master:gh-pages