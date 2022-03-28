# 每天学一个vim技巧

## 一、基础操作
i=insert
shift + i 从行首进入insert模式

a=append
shift + a 从行尾进入insert模式

o 从下一新行进入insert模式
shift + o 从上一新行进入insert模式

x 剪切光标字符（不进入insert模式），x最像剪刀
s 删除光标字符并进入inser模式，slash

左移h 下移j 上移k 右移l

ctrl + o 和ctrl+i切换历史光标，**可以跨文件**

## 二、Operation + motion

d+方向右，往右边删除字符，delete

d+方向左，往左边删除字符

d + 3 + 方向左，往左边删除3字符

重复操作表示对行操作，d + d 对行进行剪切 ，p粘贴

相同操作c(change)会进入写入模式

y复制操作，yy复制当前行（0行头$行尾）

```txt
<hello> <text me> you
```
w(word)操作定位到每个词

c(change)+i(in)+w(word)表示在某个词中删除并开始编辑

光标在hello，删除hello并写入

光标在<text me>内。输入c + i + <会把text me全部删除并写入

同理y+i+w 复制hello, y+i+<复制text me

```txt
this is vim : the best ...
```

f(find) v可以把光标移到vim开头find查找当前行，再按0可以回到行首

d+f+：删除到冒号

y+f+: 复制到冒号

## 三、标签tabe

:tabc关闭当前的tabe

:tabn切换下一个tabe

:tabp切换前一个tabe

:tabs查看共有几个tabe

1gt快速切换到第1个tabe

2gt快速切换到第2个tabe

## 四、标记mark

ma文件内设置标记，名称为a

mA跨文件设置标记，名称为A

mb文件内设置标记，名称为b

`a定位到标记a处（撇号+a）

'a定位到标记a处的行首（单引号+a）

:marks 显示所有的标记

:delmarks 删除指定的标记

:delmarks! 删除所有标记（大些字母的标记除外）

## 五、寄存器reg

双引号）加上一个 a-z 的字母来给定义一个有名字的寄存器

""表示无名寄存器

"ayy 表示的是复制当前行内容到 a 寄存器当中
  
"ap 表示的是从 a 寄存器当中取出内容
  
:reg a列出a寄存器的内容

## 六、提高操作

noremap E 5j 按一下E=5个j

set hlsearch（hight search）查找高亮

set incsearch边输入边高亮

set ignorecase忽略大小写

set smartcase搜索Max至显示Max，搜索max大小写MAX都显示

syntax on 语法高亮

set number/nonumber 显示行号

set relativenumber 显示分段set行号

set cursorline 显示鼠标线

set warp 显示不超过屏幕，自动换行显示

set showcmd 命令行显示指令
set wildmenu tab指令提示

noremap n h 改键位
map Q  :q!<CR>
map s <nop>
map R :source $MYVIMRC<CR>

:set splitright/nosplitleft可设置分屏左右光标
set nosplitbelow上下

:split上下分屏
:vsplit左右分屏
:q退出分屏
:e(edit) +路径 分屏打开文件
ctrl + w + h分屏光标移动至左边

:res+-5更改上下分屏大小
:vertical resize+-5更改左右分屏大小

:tabe 打开标签页
:-tabnext上一页
:+tabnext下一页

ctrl+w+t+ctrl+w+H交换分屏方式
ctrl+w+t+ctrl+w+K

:color default 设置背景色

## 七、Version模式

v进入version模式，可以v后，上下左右选中+y复制

:normal Imy在选中行头I加my

:normal A.png在选中行尾A加.png

shift+v+g选中所有（normal 模式下gg会把光标移动至开头G移动最后）

ctrl+v进入可视块 选中可以d操作，shift+i插入，按esc就生效

set mouse=a设置鼠标可用

set scrolloff = 5浏览时设置光标预留5行，保证到文件尾，看起来不舒服

set spell拼写检查
:z=会出现可供参考的词
或者ctrl +x +s

在normal模式gf+路径，打开文件

**没有sudo权限可用**

:w+路径 可以保存进一个新文件
  
w !sudo tee % !表示终端指令， %表示当前文件

<++>占位符
map <LEADER><LEADER> <ESC>/<++><CR>:nohlsearch<CR>c4l

figlet安装

:%TOhtml转HTML

set clipboard=unnamedplus剪贴板变为系统剪贴板

“ 、+号、y将vim剪贴板内容复制到系统剪贴板

vnoremap Y "+y可视模式下将Y绑定

把内容复制到buffer
"b yy

"a yy

"A yy 把内容补充到a

"a p

:help 打开帮助
：checkhealth查看安装情况
  

## 八、段首位添加字符

比如有一个txt文档：

```txt
This is a test
This is a test, oh my god !!!
This is a test, oh no !
This is a test, wow !
This is a test, what ?
This is a test, really ?
This is a test, seriously ?

no, you are right
yes, you are right
no, you are wrong
```

假如我们想要在段首加上`<begin>`，在段尾加`<end>`，然后在每行尾部加上`\\`，变成下面这样。

```txt
<begin>
This is a test \\
This is a test, oh my god !!! \\
This is a test, oh no ! \\
This is a test, wow ! \\
This is a test, what ? \\
This is a test, really ? \\
This is a test, seriously ? \\
<end>

<begin>
no, you are right \\
yes, you are right \\
no, you are wrong \\
<end>
```

怎么办呢？我们可以先录制一个宏：

`esc q a { O <begin> esc } o <end> esc k V { :norm A \\ q`

`{`表示到段首，`}`表示到段尾， `V {`其中`V`表示选中整个句子， `V {`表示选中从当前句子直到段首,`:norm A \\`表示在所有选中行尾部添加`\\`

然后`@a`执行，这一段就会被格式化了。
