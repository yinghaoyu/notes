`git init`初始化仓库

`git status`查看文件的情况
没有提交的文件，丢失就找不到。

`git add filename` 添加文件
`git add .` 添加该目录下所有文件
`git diff` 显示没有add的内容是什么
`git reset` 撤销上一步的add操作

`git config --global user.name "rain"`告诉git是谁进行了提交操作
`git config --global user.email "20520@126.com"`

`git commit -m "add files"` 提交

`vim .gitignore` 创建`.gitignore`，在这个文件下可以放想让git忽略的文件名，换行分隔，如果**git曾经追踪过该文件，则需要手动取消追踪，不然gitignore不生效**

`git rm --cached filename` 取消追踪该文件

`git branch` 查看分支
`git branch branchname` 新建分支
`git checkout branchname`切换分支

`git merge branchname` 将当前分支与branchname分支合并，但不删除branchname，需要手动删除

`git branch -d branchname`删除当前分支
`git branch -D branchname`强制删除当前分支

先在github创建一个仓库
`git add remote origin git@github.com:raining888/myproject.git`添加远端仓库地址
`git push --set-upstream origin master`推送当前到远端的主分支，会提示你输密码

`git config crediential.helper store`保存你输入的密码

`git clone git@github.com:raining888/myproject.git`复制仓库

`git commit -a`不会提交你未追踪的文件`git commit -m`会

`git push`克隆仓库修改并提交后，推送到远端

`git pull`将远端仓库最新的文件拉到本地仓库
