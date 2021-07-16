## ArchLinux安装参考

虚拟机安装须在设置-系统-勾选**启用EFI**

### 设置控制台字体（一般不设置）
```
setfont /usr/share/kbd/consolefonts/xxx.gz
```
### 设置键盘布局为colemak（一般不设置）
```
loadkeys colemak
```
### 改键位（一般不设置）
```
vim keys.conf
//比如交换Caps_Lock与Escape
keycode 1 = Caps_Lock
keycode 58 = Escape
//加载键盘配置
loadkeys keys.conf
```
### 联网
```
ip link//查看设备状态
ip link set wlan0//wlan0为系统列出来的设备名
iwlist wlan0 scan//扫描wifi
iwlist wlan0 scan | grep ESSID//输出wifi名
wifi-menu//可能支持一些安全协议，例如：可能上不了校园网，推荐使用wap_passphrase
wap_passphrase wifi名 wifi密码 > internet.conf//生成internet.conf配置文件
wpa_supplicant -c internet.conf -i wlan0 &//此时ping不通
dhcpcd &//通过动态分配ip地址后才能ping通，如果出错了，请用root用户执行
```
### 同步系统时间
```
timedatectl set-ntp true//更新系统时间
timedatectl status // 确保设置成功
```
### 查看磁盘设备
```
fdisk -l//查看设备
fdisk /dev/sda1//sda1为磁盘设备名
//根据帮助文档
//按g把所有分区归为1个分区
//p打印
查看你电脑支持以下哪种
BIOS with MBR
UEFI with GPT
按n创建分区，根据提示创建

//一般创建3块分区就可以
Device      Start  End  Sectors  Size    Type
/dev/sda1                         2G    filesystem
/dev/sda2                         16G   filesystem
/dev/sda3                         2G    filesystem

sda1用与bootable
sda2用于主文件系统
sda3用于swap分区

//按w才会写入硬盘
```

### 格式化磁盘格式
```
mkfs.fat -F32 /dev/sda1 // UEFI引导分区，用于boot， mkfs= make filesystem, fat为文件格式
mkfs.ext4 /dev/sda2// 引导主分区，这个格式有多种，比如ext4，xfs等
mkswap /dev/sda3 // 交换分区
swapon /dev/sda3 // 开启交换分区
```

### pacman配置（根据个人配置，也可以不配置）
```
vim /etc/pacman.conf // 没有编译器可以pycman -S vim下一个
里
```

### 图形界面服务器
```
pacman -S xorg xorg-server

pacman -S xorg-xint
sudo pacman -S xorg-xinit
在/etc/X11/xinit/xinitrc加入exec dwm，输入startx就可以进入dwm了

pacman -Syyu后，root用户执行startx正常，而非root用户执行startx出现缺少模块错误。这个是为啥？
```
### yay包管理 tlp电池管理
```
sudo pacman -S yay tlp

compton窗口渲染器(可选安装)
```
### pacman基本用法
```
(sync)pacman -S
pacman -Sy(yank)//更新软件源
pacman -Syy//强行更新更新软件源

pacman -Syu(update)//更新软件源更新安装的软件
pacman -Syyu(update)//强行更新软件源更新安装的软件

pacman -Ss(search)//查找

pacman -Sc//删除缓存的软件包

(remove)pacman -Rs + name//删除软件和依赖包
pacman -Rns + name//删除软件、依赖包和全局文件
(query)pacman -Q + name//带参查询指定软件，不带参查询所有软件
pacman -Q | wc -l//查询你装了多少个软件
pacman -Qe | wc -l//查询你手动装了多少个软件
pacman -Qeq//查询你手动装了什么软件

pacman -Qs + name//查询你手动装了什么软件

pacman -Qdt（dependences）//查询孤儿软件，不再需要的软件
pacman -R $ (pacman -Qdtq) //删除所有孤儿软件

/etc/pacman.conf//pacman的配置文件

可以手动添加源（非必须）
[archlinuxcn]
SigLevel = Optional TrustedOnly
Include = /etc/pacman.d/archlinuxcn //这个文件需要手动创建

/etc/pacman.d/archlinuxcn
添加Server=http地址
```
