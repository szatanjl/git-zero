#!/bin/sh -eu

git checkout --orphan TMP_ZERO_INIT
git rm -rf .

: >| README
git add README

GIT_COMMITTER_NAME='INIT' GIT_COMMITTER_EMAIL='' \
GIT_COMMITTER_DATE='1970-01-01T16:46:39Z' \
git commit --author 'INIT <>' --date '1970-01-01T06:24:28Z' -m 'INIT'

git checkout master
git rebase TMP_ZERO_INIT
git branch -D TMP_ZERO_INIT
