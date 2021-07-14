## ArchLinux安装参考
设置控制台字体（一般不设置）
```
setfont /usr/share/kbd/consolefonts/xxx.gz
```
设置键盘布局为colemak（一般不设置）
```
loadkeys colemak
```
改键位（一般不设置）
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
timedatectl status // 确保设置成功
```
查看磁盘设备
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

格式化磁盘格式
```
mkfs.fat -F32 /dev/sda1 // UEFI引导分区，用于boot， mkfs= make filesystem, fat为文件格式
mkfs.ext4 /dev/sda2// 引导主分区，这个格式有多种，比如ext4，xfs等
mkswap /dev/sda3 // 交换分区
swapon /dev/sda3 // 开启交换分区
```

pacman配置（根据个人配置，也可以不配置）
```
vim /etc/pacman.conf // 没有编译器可以pycman -S vim下一个
里面的color选项推荐打开，以后系统提示就会变成彩色字体而不是黑白
选项Include=/etc/pacman.d/mirrorlist //这里保存pacman的源
按gf进入/etc/pacman.d/mirrorlist  //mirrorlist是从上往下找的，因此第一个可以填离你最近的源，提升更新网速
你可以加入清华大学源Server = https://mirrors.tuna.tsinghua.edu.cn/archlinux/$repo/os/$arch
或者可以ping一下源地址，看哪个快
```
挂载分区
```
//我们目前是位于u盘下，或者是虚拟机的虚拟u盘下，如果要安装，必须挂载，因为你u盘等下是要拔掉的!
monut /dev/sda2 /mnt // 首先挂载主分区
ls /mnt //查看挂载情况，如果为lost+found表示成功
mkdir /mnt/boot      //创建一个用于boot的路径
mount /dev/nvme0n1p1 /mnt/boot // 挂载引导分区
挂载后，/mnt就相当与你安装的主分区了
```
运行安装脚本
```
//pacstrap是archlinux官方提供的一个安装脚本
//base是基础软件，linux是linux内核，linux-firmware是linux的基础框架
pacstrap /mnt base linux linux-firmware
```
生成fstab文件
```
genfstab -U /mnt >> /mnt/etc/fstab
```
到此archlinux基本安装完毕

进入到安装的系统
```
arch-chroot /mnt  //后面就相当于在你的新系统里执行命令了
```

时区设置
```
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime //建立一个软连接 localtime需要手动敲
hwclock --systohc  //使用hwclock生成/etc/adjtime文件
```

本地化设置
本地化的程序与库若要本地化文本，都依赖Locale，后者明确规定地域、货币、时区日期的格式、字符排列方式和其他本地化标准等等。在下面两个文件设置：locale.gen与locale.conf。

```
// 记住/mnt是系统挂载到你u盘的路径，你u盘是已经有vim或者刚刚已经装过vim的
// 这个时候由于你的系统里没装vim，所以你必须退出你的系统

输入exit+回车

vim /mnt/etc/locale.gen  //将en_US.UTF-8 UTF-8取消注释
arch-chroot /mnt  //再进入系统
locale-gen  //根据刚刚修改的文件生成locale
输入exit+回车 //再退出系统
vim /mnt/etc/locale.conf  //locale.conf需要手动输入
添加LANG=en_US.UTF-8并保存  //系统的Log就会用英文显示，这样更容易问题的判断和处理
```

网络设置
```
vim /mnt/etc/hostname //需手动敲，将你的主机名加进入假设为yyh
vim /mnt/etc/hosts  //需手动敲
加入
127.0.0.1 localhost
::1 localhost
127.0.0.1 yyh.localdomain yyh
```

更改root密码
```
arch-chroot /mnt
passwd
```

安装及配置引导程序
```
// 安装引导程序之后才能进入系统，用pacman安装必要的软件包
// grub作为系统引导
// intel-ucode是更新驱动用的
pacman -S grub efibootmgr intel-ucode os-prober // AMD请装amd-ucode

UEFI系统安装配置grub

mkdir /boot/grub
grub-mkconfig > /boot/grub/grub.cfg  //需手敲
uname -m //查看当前系统架构，x86_64
grub-install --target=x86_64-efi --efi-directory=/boot //提示无错误则安装完毕
```

安装基本软件（可根据自己喜好安装）

```
//其中wpa_supplicant和dhcpcd建议安装，联网用的
pacman -S neovim vi zsh wpa_supplicant dhcpcd
```

退出重启
```
exit
reboot
拔掉usb，或者移除虚拟usb
```
