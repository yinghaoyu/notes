### 端口转发
```bat
netsh interface portproxy show all
netsh interface portproxy delete v4tov4 listenport=8000 listenaddress=0.0.0.0
netsh interface portproxy add v4tov4 listenport=12345 listenaddress=0.0.0.0 connectport=12345 connectaddress=wslhost
```
### 把wsl的ip写入windows的hosts

```sh
hostname="wslhost"

wslip=$(ip address show eth0 | grep 'inet ' | awk '{print $2}' | awk -F/ '{print $1}')

sed -i "/$hostname/d" /mnt/c/Windows/System32/drivers/etc/hosts

echo "$wslip $hostname" >> /mnt/c/Windows/System32/drivers/etc/hosts
```
