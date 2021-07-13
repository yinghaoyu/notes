## ArchLinux安装参考
设置控制台字体
```
setfont /usr/share/kbd/consolefonts/xxx.gz
```
设置键盘布局为colemak（一般不设置）
```
loadkeys colemak
```
改键位
```
vim keys.conf
//比如交换Caps_Lock与Escape
keycode 1 = Caps_Lock
keycode 58 = Escape
//加载键盘配置
loadkeys keys.conf
```
无网线，使用wifi
```
ip link//查看设备状态
ip link set wlan0//wlan0为系统列出来的设备名
iwlist wlan0 scan//扫描wifi
iwlist wlan0 scan | grep ESSID//输出wifi名
wifi-menu//可能支持一些安全协议，例如：可能上不了校园网，推荐使用wap_passphrase
wap_passphrase wifi名 wifi密码 > internet.conf//生成internet.conf配置文件
wpa_supplicant -c internet.conf -i wlan0 &//此时ping不通
dhcpcd &//通过动态分配ip地址后才能ping通
```
同步系统时间
```
timedatectl set-ntp true//更新系统时间
```
查看磁盘设备
```
fdisk -l//查看设备
fdisk /dev/mmcblk1//mmcblk1为磁盘设备名
//根据帮助文档
//按g创建新分区
//p打印
//w才会写入硬盘
查看你电脑支持以下哪种
BIOS with MBR
UEFI with GPT
支持UEFI的话
按n创建

```
