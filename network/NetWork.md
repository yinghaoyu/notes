## 计算机网络体系结构

### 具有五层协议的体系结构

- OSI七层协议体系概念清楚，理论也较完整，但它既浮渣又不实用。

- 五层协议体系结构综合OSI和TCP/IP优点，简洁又能将概念阐释清楚。
 
<div align="center">

![network_1](/images/network_1.png)

</div>

1. **应用层** (application layer)

	- **任务：** 通过应用进程间的交互来完成特定网络应用。应用层定义的是应用进程间通信和交互的规则。
	
	- **协议：** 域名系统 DNS，支持万维网应用的 HTTP 协议，支持电子邮件的 SMTP 协议等。
	
	- **数据单元：** 应用层交互的数据单元称为 **报文**(message)。

2. **运输层** (transport layer)

	- **任务：** 负责向**两台主机中进程间**的通信提供**通用数据传输**服务。运输层有复用和分用的功能，**复用**就是多个应用层进程可同时使用下面运输层的服务，**分用**是运输层把收到的信息分别交付到上面应用层的相应进程。

	- **协议：**
	
	- **传输控制协议 TCP** (Transmission Control Protocol) —— 提供面向连接、可靠的数据传输服务，其数据传输单位是 **报文段** 。
	
	- **用户数据报协议 UDP** (User Datagram Protocol) —— 提供无连接、尽最大努力的数据传输服务（不保证数据传输可靠性），其数据传输单位是 **用户数据报** 。

3. **网络层** (network layer)
	
	- **任务：** 负责为分组交换网上**不同主机**提供通信服务。互联网是由大量**异构网络**通过路由器相互连接起来的。
	
	- **协议：** 无连接的网际协议 IP(Internet Protocol)。
	
	- **数据单元：** 分组 或 IP 数据报。

4. **数据链路层** (data link layer)
	
	- **任务：** 两台主机之间的数据传输，总是在一段段的链路上传送的。链路层将网络层交下来的数据报**组装成帧**，并在两个相邻节点间传输。每一帧包括数据和必要的控制信息（同步信息、地址信息、**差错控制**等）。
	
	- **数据单元：** 帧。
	
5. **物理层** (physical layer)

	- **任务：** 发送方发送 1 或 0 ，接收方应当接收到对应数字。因此物理层要考虑用多大电压代表 1 和 0 ；物理层还要确定连接电缆的插头应当有多少根引脚以及其连接方式。
	
		> 注意： 传递信息所利用的物理媒体，如双绞线、同轴电缆、光缆、无线信道等，并不在物理层协议内，而是在物理层下面。
	
	- **数据单元：** 比特。
	
	<div align="center">
	
	![network_2](/images/network_2.png)
	
	</div>
	
### TCP/IP 体系结构

- 实际上，现在的互联网使用的 TCP/IP 体系结构有时已经演变成下图所示那样，即某些应用程序可以**直接使用 IP 层**，或甚至**直接使用最下面的网络接口层**。

<div align="center">

![network_3](/images/network_3.png)

</div>

- **TCP/IP 协议** 可以为**各式各样的应用程序**提供服务，同时也允许 IP 协议在**各式各样的网络**构成的互联网上运行。

<div align="center">

![network_4](/images/network_4.png)

</div>

## 数据链路层

### **三个基本问题：** 封装成帧、透明传输、差错检测

#### **封装成帧：**

- 在一段数据分别添加首部和尾部，作用是**帧定界**，以便接收方定位数据边界。
	
- 每种链路层协议规定了帧的数据部分长度上限——**最大长度单元 MTU**。

<div align="center">

![network_5](/images/network_5.png)

</div>

#### **透明传输：**

> 表示无论什么比特组合的数据，都能按照原样没有差错的通过这个数据链路层正确传输。

- 当数据是非 ASCII 码文件时，数据链路层可能会在数据中 **错误地找到帧的边界** SOH/EOT ，此时不满足透明传输。

<div align="center">

![network_6](/images/network_6.png)

</div>

- **字节填充法：** 在数据中出现 SOH/EOT/ESC 前加入一个转义字符 ESC ，接收端把数据送往网络层之前删除 ESC 转义字符。这样就可以解决错误帧边界问题。

<div align="center">

![network_7](/images/network_7.png)

</div>
	
#### **差错检测**：

> 分为比特差错和传输差错。

- **循环冗余检验 CRC ——比特差错：** 在长度为k的数据 M 后加 n 位冗余码，一共发 (k+n) 位。
	
- 计算方法： 在M后添加 n 个 0 ，得到的 (k+n) 位数除以双方事先商定的 (n+1) 位除数 P ，得到余数 R ，作为冗余码拼接在数据 M 后发送出去。如果传输过程中无差错，经过 CRC 检验后的余数R肯定是 0 。
	
	- **FCS 和 CRC 区别：** CRC是一种检错方法，而 FCS 是添加在数据后面的冗余码。检错方法可以选择 CRC 之外的方法。
	
	<div align="center">
	
	![network_8](/images/network_8.png)
	
	</div>
	
	- **帧编号、确认、重传机制——传输差错 ：** 可靠传输指的是数据链路层发送端发送什么，接收端就接收到什么。**帧丢失、帧重复、帧失序**属于传输差错。
	
	- 对于通信质量良好的传输链路，改正传输差错的任务由上层协议（如TCP协议）完成。
	
	- 对于通信质量较差的传输链路，数据链路层使用确认和重传机制，向上层提供可靠传输服务，可以提升传输效率。
	
	<div align="center">
	
	![network_9](/images/network_9.png)
	
	</div>

### 点对点协议 PPP

#### **PPP 协议特点**

- 简单、封装成帧 + MTU 、透明性、差错检测；
 
- 支持多种网络层协议、多种类型链路、自动检测连接状态。
 
#### **PPP 协议组成**
 
1. 将IP数据报封装到串行链路的方法。
 
2. 用来建立、配置和测试数据链路链接的 **链路控制协议 LCP**；
 
3. 一套**网络控制协议 NCP**。
 
#### PPP 协议帧格式
 
1. 各字段意义：

	- **标志字段 F ：** 首部和尾部字段都为 0x7E ，标志一个帧的开始或结束，标志字段是 PPP 帧定界符。
	
	- **地址字段 A 和控制字段 C ：** 规定为 0xFF 和 0x03 ，至今没有实际定义。
	
	- **协议字段：** 0x0021 对应 IP 数据报，0xC021 对应链路控制协议 LCP 数据，0x8021 对应网络层控制数据。
	
	<div align="center">
	
	![network_10](/images/network_10.png)
	
	</div>
   
1. **零比特填充：**

	- **发送端：** 发现有 5 个连续 1 ，就立即填入一个 0 ，这样可以保证信息字段中不会出现 6 个连续 1 。
	
	- **接收端：** 每当发现 5 个连续 1 时，就把这 5 个连续 1 后一个 0 删除，还原成原来的信息比特流，保证了透明传输。
	
	<div align="center">
	
	![network_11](/images/network_11.png)
	
	</div>
  
#### **PPP协议工作状态**

1. PPP链路起始终止状态永远是**链路静止**状态；

2. PC通过调制调节器呼叫路由器，建立物理层连接后，PPP就进入**链路建立**状态；

3. LCP开始协商一些配置选项，包括链路最大帧长，不使用的PPP帧中的地址和控制字段；协商结束后就进入**鉴别**状态；

4. 经过口令鉴别协议 PAP 或更复杂的口令握手鉴别协议 CHAP，若鉴别成功，则进入**网络层协议**状态；

5. PPP 链路两端的网络控制协议 NCP 根绝网络层不同协议互相交换网络层特定的网络控制分组。若在 PPP 链路上运行的是 IP 协议，则要使用 **IP 控制协议 IPCP** 。网络层配置完毕后，进入可数据通信的**链路打开**状态；

6. 数据传输结束后，链路一端发出**终止请求**，在收到对方发来的**终止确认**后，转到**链路终止**状态。如果链路出现故障，也会从链路打开转到链路终止状态。当调制调节器载波终止后，回到**链路静止**状态。

<div align="center">

![network_12](/images/network_12.png)

</div>

### 使用广播信道的数据链路层

#### **以太网的 MAC 层**

1. **MAC 层的硬件地址**

	- 又称物理地址或 MAC 地址（因为这种地址用在 MAC 帧中）；
	
	- 指局域网上每一台计算机中固化在适配器的 ROM 中的 48 位地址。

2. **适配器过滤功能：** 适配器从网络上收到一个 MAC 帧，先用硬件检查帧中的目的地址，若是本站的则收下，然后再做其他处理，否则就丢弃。

	- 单播： 收到的帧的 MAC 地址与本站地址相同；
	
	- 广播： 发送给本局域网上所有站点的帧（全 1 地址）；
	
	- 多播： 发送给本局域网上一部分站点的帧。
 
#### MAC帧的格式

| 位置 | 名称 | 长度 | |
|:--|:--:|:--:|:--:|
| 头部 | 目的地址 | 6 字节 | 48 位长地址 |
| 头部 | 源地址 | 6 字节 | 48 位长地址 |
| 头部 | 类型字段 | 2 字节 | 标志上一层使用的是什么协议 |
| 尾部 | 帧检验序列 FCS | 4 字节 | 使用 CRC 检验 |

<div align="center">

![network_13](/images/network_13.png)

</div>

- 实际传送比 MAC 帧还**多 8 个字节**。这是因为当一个站刚开始接受 MAC 帧时，适配器时钟尚未与比特流同步，则导致MAC帧最前面若干位无法接收，结果使 MAC 帧失效。

	- **前 7 个字节——前同步码：** 使接收端适配器调整时钟频率，使它和发送端时钟同步，实现“位同步”（比特同步）。
	
	- **后 1 个字节——帧开始定界符：** 定义为 10101011。
   
### 扩展以太网

#### **在物理层扩展以太网**

1. 使用多个**集线器**，连接成覆盖更大范围的多级星型结构的以太网。

<div align="center">

![network_14](/images/network_14.png)

</div>

2. 主机和集线器之间**扩展距离**的一种简单方法就是使用**光纤**。

	- 光纤作用是执行**电信号**和**光信号**转换；
 
	- 光纤时延很小，带宽很宽。
	
<div align="center">

![network_15](/images/network_15.png)

</div>
 
#### **在数据链路层扩展以太网**

> 交换式集线器1990年问世，常称为以太网交换机，工作在数据链路层。

1. 以太网交换机特点：

	- 实质上就是一个多接口**网桥**；
	
	- 相互通信的主机都是**独占**传输媒体，无碰撞地传输数据；（多个用户独占而不是和其他网络用户共享传输媒体带宽，对于 10Mbit/s 的共享式以太网，10 个接口的交换机总容量为 100Mbit/s）。
	
	- 内部的**帧交换表**（又称**地址表**）使通过**自学习算法**自动地逐渐建立起来的。
 
2. 以太网交换机的**自学习**功能：

	- 接收到帧时，将帧的**源地址—接口**记录在**交换表**中；
	
	- 地址表中找不到对应目的地址时，则**广播**此帧，只有对应主机会收下此帧，其余主机会丢弃（过滤）；
	
	- 经过多次互相发送帧，最终交换表将记录所有主机**源地址—接口**。
	
	<div align="center">
	
	![network_16](/images/network_16.png)
	
	</div>
	
	- 使用交换机组网时，往往会增加一些冗余链路。可能会形成**环**导致帧在网络的某个环路中**无限兜圈子**。
	
	- 为了解决此问题，IEEE 的 802.1D 标准制定了**生成树协议 STP 。可以在不改变网络实际拓扑结构下，在逻辑上切断某些链路，使得所有主机间的路径是无环路的树状结构。**
	
	<div align="center">
	
	![network_17](/images/network_17.png)
	
	</div>

## 网络层

### 网络层提供的两种服务

> 网络层应该向运输层提供怎样的服务面向连接还是无连接曾引起长期争论。争论焦点：可靠交付应当由谁负责？网络还是端系统？

1. **思路一：** 计算机应该仿照打电话使用的面向连接的通信方式。先建立连接，预留双方通信所需的一切网络资源，然后双方沿着已建立的虚电路发送分组。

2. **思路二：** 与电话不同的是，计算机网络的端系统是有智能的计算机，有很强的差错处理能力，因此网络层无需建立链接。

3. **互联网采用的设计：** 网络层只向上提供**简单灵活、无连接、尽最大努力交付**的数据报服务。

	- 网络层不提供服务质量承诺，有可能出现数据出错、丢失、重复和失序。
	
	- 如果需要可靠传输，则由主机中的运输层负责（包括差错处理、流量控制等）。
	
	- **优点：** 网络造价大大降低，运行方式灵活，能够适应多种应用。
	
	<div align="center">
	
	![network_18](/images/network_18.png)
	
	</div>
 
### 网际协议 IP（IPv4）
 
- 与 IP 协议配套使用的 3 个协议：
 
	- 地址解析协议 ARP
	
	- 网际控制报文协议 ICMP
	
	- 网际组管理协议 IGMP
	
	<div align="center">
	
	![network_19](/images/network_19.png)
	
	</div>
 
#### **分类的 IP 地址**

- **IP 地址编址方法**共经过 3 个历史阶段：

	1. 分类的IP地址（1981 年）
	
	2. 子网的划分（1985 年）
	
	3. 构成超网（1993 年）
	
- **两级 IP 地址：**

	1. 第一个字段是**网络号**，标志主机（或路由器）连接到的网络；
	
	2. 第二个字段是**主机号**，标志该主机（或路由器）。
	
	<div align="center">
	
	![network_20](/images/network_20.png)
	
	</div>
	
	- 一个 IP 地址在整个互联网范围是唯一的。
 
 - **各类 IP 地址：**
 
	- **A/B/C 类地址**网络号字段分别为 1/2/3 字节长，而在网络号最前面有 1-3 位的**类别位**，分别规定为 0/10/110 。
	
	- **D 类地址**用于多播（一对多通信）。
	
	- **E 类地址**保留为以后用。
	
	<div align="center">
	
	![network_21](/images/network_21.png)
	
	</div>
	
- **常用的三种类别 IP 地址：**

|名称 | 网络号字段 | 首固定位 | 可指派网络号数量 | 保留地址|
|:--|:--:|:--:|:--:|:--:|
|A类 | 占 1 字节 | 0 |2<sup>7</sup>-2 | 00000000 本网络；01111111 软件环回测试|
|B类 | 占 2 字节 | 10 |2<sup>14</sup>- 1 | 128.0.0.0 不指派|
|C类 | 占 3 字节 | 110 |2<sup>21</sup>-1 | 192.0.0.0 不指派|

- **点分十进制记法**

	- 每 8 位插入一个 '.' ，方便书写。
	
	<div align="center">
	
	![network_22](/images/network_22.png)
	
	</div>

#### **IP 地址与硬件地址**

- **硬件地址与 IP 地址区别：** 物理地址是数据链路层和物理层使用的地址， IP 地址是网络层和以上各层使用的地址，是一种逻辑地址。

	- 在 IP 层抽象的互联网上，只能看到 IP 数据报。经过路由多次转发的 IP 数据报首部中的源地址和目的地址始终分别为 IP1 和 IP2 。 路由器只根据目的地址进行路由选择。
	
	- 在局域网的链路层，只能看见 MAC 帧。在不同的硬件和网络上传送时， MAC 帧首部的源地址和目的地址（硬件地址）会发生变化。
	
	<div align="center">
	
	![network_23](/images/network_23.png)
	
	</div>

#### **地址解析协议 ARP**

- **APR 协议作用：** 已经知道一个机器的 **IP 地址**，需要找出对应的**硬件地址**。

	- **RARP 协议**：现在的 DHCP 协议包含逆地址解析协议功能，即从**硬件地址**找出其 **IP 地址**。
	
- **ARP 协议工作原理：** 网络层使用 IP 地址，但实际网络链路上传送数据帧时，最终必须使用该网络的硬件地址。ARP 解决方法是在**主机 ARP 高速缓存** 中存放一个从 IP 地址到硬件地址的映射表，并实时动态更新（新增或超时删除）。

	- 当主机 A 要向主机 B 发送 IP 数据报时，就在其 ARP 高速缓存中查看有无主机 B 的 IP 地址，如有就查找并写入 MAC 帧，并传输。
	
	- 有可能查不到主机 B 的 IP 地址，可能主机 B 刚入网，可能主机 A 刚加电（高速缓存为空）。在这种情况下，主机 A 就自动运行 ARP，通过广播请求分组找出主机 B 的硬件地址。

- **ARP 协议工作流程：**

	1. 主机 A 在本局域网上发送一个 ARP 请求分组，在本局域网上所有主机上运行的 ARP 进程都收到此请求分组。
	
	2. 主机 B 收下这个 ARP 请求，并向主机 A 发送响应分组（普通的单播，发送给主机 A ）。同时，把请求分组里 A 的 IP 地址到硬件地址的映射写入主机 B 的 ARP 高速缓存中。
	
	3. 主机 A 收到主机 B 的响应分组后，就在 ARP 高速缓存中写入主机 B 的 IP 地址到硬件地址的映射。
	
	<div align="center">
	
	![network_24](/images/network_24.png)
	
	</div>
	
- **ARP生存时间：** 超过生存时间（例如 10 ~ 20 分钟）的项目从高速缓存中删掉。这样如果 B 更换了设备， A 无法找到原主机 B ，但过了一段时间， A 的 ARP 高速缓存中已经删除了 B 原先硬件地址，于是 A 重新广播发送 ARP请求分组，找到新的 B 硬件地址。

#### **IP 数据报的格式**

<div align="center">

![network_25](/images/network_25.png)

</div>

1. **首部固定字段**

	1. **版本：** 占 4 位；指 IP 协议版本。通信双方使用的 IP 协议版本需一致。
	
	2. **首部长度：** 占 4 位；单位是 32 位字（4 字节）。
	
	3. **区分服务：** 占 8 位；只有在使用区分服务时才使用，一般情况不使用。
	
	4. **总长度：** 占 16 位；指首部和数据长度之和；单位为字节，因此数据报最大长度为 2<sup>16</sup>-1=655362字节。
	
		- 数据链路层规定了**数据帧中数据字段的最大长度 MTU** ，最常用的以太网规定 MTU = 1500 字节，若长度超过 MTU 值，就必须把数据报分片处理。
		
		- 虽然尽可能长的 IP 数据报会使传输效率变高（首部长度占比小），但数据报短路由器转发速度也更快。
	
	5. **标识：** 占 16 位。 每产生一个 IP 数据报，计数器 + 1+1 。
	
		- 这个标识并不是序号（网络层不负责按序接收）；
		
		- 在由于长度超过 MTU 而被分片时，标识字段值被复制到所有分片的标识字段，使各数据报片最后能正确地组装成原来的数据报。
		
	6. **标志：** 占 3 位，目前仅前 2 位有意义。
	
		- 最低位记为 MF (More Fragment) ，MF = 1 表示后面“还有分片”数据报，MF = 0 表示这是若干数据报片中的最后一个。
		
		- 中间一位记为 DF (Don't Fragment) ， DF = 1 表示不能分片，DF = 0 才允许分片。
		
	7. **片偏移：** 占 13 位；表示数据报在分片后，某片在原分组中的相对位置；单位是 8 字节。
	
		- 每个分片的长度一定是 8 字节（64 位）的整数倍。
		
		<div align="center">
		
		![network_26](/images/network_26.png)
		
		</div>
		
	8. **生存时间（TTL）：** 占 8 位；表示数据报在网络中的**寿命**。防止数据报在互联网中无限制兜圈子。
	
		- 最初设计 TTL 以秒为单位，消耗时间小于 1s 则将 TTL 减 1 。
		
		- 目前路由器处理数据报时间一般远远小于 1s ，后来 TTL 字段功能改为**跳数限制**，路由器每次转发数据之前把 TTL 减 1 ，当 TTL = 0 就丢失这个数据报。
		
		- 数据报能在互联网中经过路由器的最大数值是 255(2<sup>8</sup> - 1)。若设置 TTL = 1 ，则表示此数据报只能在本局域网中传送。
		
	9. **协议：** 占 8 位；表示此数据报中携带的数据使用何种协议，以便接收方主机 IP 层直到应将数据上交给哪个协议处理。
	
	<div align="center">
	
	![network_27](/images/network_27.png)
	
	</div>
	
	10. **首部校验和：** 占 16 位；只检验数据报的首部，不包括数据部分（减少计算量）。
	
		- 检验和不采用复杂的 CRC 检验，而换做以下简单方法：
		
			1. 先把 IP 数据报划分为许多 16 位字的序列，并把检验和字段置零；
			
			2. 用**反码算术运算**把所有 16 位字相加，将**和的反码**写入检验和字段；
			
				> 反码算术运算： 若最高位相加产生进位，则最后得到的结果再加 1 。
			
			3. 接收方收到数据报后，将首部所有的 16 位字使用**反码算数运算**相加，将得到的**和取反码**，若首部未发生任何变化，则结果必为 0 ；若结果不为 0 ，则认为首部出现差错，将数据报丢弃。
			
			<div align="center">
			
			![network_28](/images/network_28.png)
			
			</div>
			
	11. **源地址：** 占 32 位；
	
	12. **目的地址：** 占 32 位。

2. **首部可变部分**

	- IP 数据报首部的可变部分就是一个**选项字段**，用来支持**排错、测量、安全措施**等。
	
	- 长度 1 - 40 字节，取决于选择的项目。
	
	- 可变部分是用来增加 IP 数据报的功能，实际上这些选项很少被使用。很多路由器都不考虑 IP 首部的选项字段，因此新的 IP 版本 **IPv6 就将 IP 数据报的首部长度做成固定的。**

#### **IP 层转发分组流程**

- **路由表：** （目的网络地址， 下一跳地址）；

	- 整个网络拓扑简化为一条链路，强调了在互联网转发分组时，是**从一个路由器转发到下一个路由器**。
	
	<div align="center">
	
	![network_29](/images/network_29.png)
	
	</div>

- **特定主机路由：** 对特定的目的主机指明一个路由。

	- 可使网络管理人员更方便地控制网络和测试网络；
	
	- 可在考虑某种安全问题时采用这种特定主机路由；
	
	- 网络连接或路由表排错时，特定路由十分有用。
	
- **默认路由：** 减少路由表占用的空间和搜索路由表所用时间；

	- 在一个网络只有很少的对外连接时很有用。
	
	<div align="center">
	
	![network_30](/images/network_30.png)
	
	</div>

- **分组转发算法：**

	1. 从数据报首部提取目的主机 IP 地址 D，得出目的网络地址为 **N**。

	2. 当 N 就是与此路由器直接相连的某个网络地址，则**直接交付**（包括把目的主机地址转换为具体的硬件地址，把数据封装为 MAC 帧）；

	3. 若路由表中有目的地址为 D 的**特定主机路由**，则把数据报传送给路由表中所指明的**下一跳路由器**；否则执行 4.；
	
	4. 路由表中有到达网络 N 的路由，则把数据报传送给路由表中所指明的**下一跳路由器**；否则执行 5.；
	
	5. 若路由表中有一个默认路由，则把数据报传送给路由表中指明的**默认路由器**；否则执行 6.；
	
	6. 报告转发分组**出错**。
  
### 划分子网和构造超网

#### **划分子网**

1. **两级 IP 地址** → **三级 IP 地址**

	- **两级 IP 地址不合理：**
	
		1. **IP 地址空间利用率有时很低：** 有的单位申请到一个 B 类地址网络，但所连主机不多，造成 IP地址的浪费；
		
		2. **网络号使路由表变得太大：** 互联网中的网络号越多，路由器路由表的项目也就越多，使查找路由时耗费更多时间；
		
		3. **两级 IP 地址不够灵活：** 必须事先到互联网管理机构申请新的网络号，才能开通新的网络。
		
	- **子网寻址 & 子网路由选择：**
	
	- 单位可将所属**物理网络**划分为若干**子网**，对外**仍然表现为一个网络**。
	
	- **三级 IP 地址** :: = { <网络号>， <子网号>， <主机号> }
	
	<div align="center">
	
	![network_31](/images/network_31.png)
	
	</div>
   
2. **子网掩码**
 
> 假定有一个数据报已经到达了路由器，那么此路由器如何把它转发到子网 145.13.3.0 呢？

- IP 数据报上没有子网的划分信息，因此需要**子网掩码**。
	
	- 无论是否划分子网，只要将 IP 地址和子网掩码进行逐位**与运算**，就可以立即得到**网络地址**。
		
	<div align="center">
		
	![network_32](/images/network_32.png)
		
	</div>
  
- 如果一个网络不划分子网，则使用**默认子网掩码**。

	- A 类地址默认子网掩码为 255.0.0.0；
	
	- B 类地址默认子网掩码为 255.255.0.0；
	
	- C 类地址默认子网掩码为 255.255.255.0。
	
	<div align="center">
	
	![network_33](/images/network_33.png)
	
	</div>
  
#### **使用子网时分组的转发**

- 使用子网划分后，路由表必须包括**三项内容**：目的网络地址、子网掩码、下一跳地址。

- 在划分子网的情况下，**路由器转发分组算法：**

	1. 获取数据报的目标 IP 地址 D；
	
	2. 对路由器每个直接相连网络的子网掩码和 D 与运算，若有匹配的网络地址，则**直接交付**；
	
	3. 若路由表中有目的地址为 D 的**特定主机路由**，则把数据报传送给路由表中指明的下一跳路由器；
	
	4. 对路由表中每一行的子网掩码与D与运算，结果为N。若N与某行目的网络地址匹配，则传送给对应的下一跳路由器；
	
	5. 若路由表中有一个**默认路由**，则将数据报传送给指明的默认路由器；
	
	6. 报告转发分组出错。
	
	<div align="center">
	
	![network_34](/images/network_34.png)
	
	</div>
 
### 网际控制报文协议 ICMP

#### **ICMP 报文种类**

- ICMP 差错报告报文和 ICMP 询问报文。

<div align="center">

![network_35](/images/network_35.png)

</div>

#### **ICMP 应用举例**

- **分组网间探测 PING (Packet Internet Groper)**：使用了 ICMP 的回送请求和回送回答报文。

### 互联网的路由选择协议

### IPv6

### IP 多播

## 运输层

### 运输层协议概述

#### **进程间通信**
 
- **通信端点：** 从**网络层**来说，通信的两端是**两台主机**；而从**运输层**的角度看，通信的真正端点不是主机而是**主机的进程**。

- **逻辑通信：** 数据好像是水平方向直接传送数据，但事实上是沿着下图虚线方向（经过多个层次）传送的。

	- **网络层**为主机间提供逻辑通信，**运输层**则为应用进程间提供端到端的逻辑通信。
	
	<div align="center">
	
	![network_36](/images/network_36.png)
	
	</div>

#### **运输层的两个主要协议**

- **用户数据报协议 UDP** 和 **传输控制协议 TCP**

<div align="center">

![network_37](/images/network_37.png)
 
</div>

- 下表给出了一些应用和应用层协议主要使用的 **运输层协议**

<div align="center">

![network_38](/images/network_38.png)

</div>

#### **运输层的端口**

- **复用与分用：** 应用层所有应用进程都可以通过运输层传输到 IP 层，称为**复用**；运输层从 IP 层收到发送给各应用进程的数据后，必须分别交付给指明的各应用进程，称为**分用**。

- **端口(port)：** 只要把报文交送到目的主机的某个合适的端口，剩下的工作就由 TCP 或 UDP 来完成。

	> 此协议端口是软件端口，和路由器或交换机上的硬件端口是完全不同的概念。

	- **服务端**使用的端口号：
	
		- **系统端口号**（熟知端口号）： 数值为 0 ~ 1023 ；
		
		- **登记端口号**： 数值为 1024 ~ 49151 ；
		
	- **客户端**使用的端口号（**短暂端口号**）：数值为 49152 ~ 65536 。

<div align="center">

![network_39](/images/network_39.png)

</div>

### 用户数据报协议 UDP

#### **UDP 概述**

1. **无连接：** 减少了开销和发送前时延；
 
2. **尽最大努力交付：** 不保证可靠交付；

3. **面向报文：** 对应用层交下来的报文既不合并，也不拆分，保留这些报文的边界。

4. **没有拥塞控制：** 网络拥塞不会使源主机发送速率降低，允许在网络拥塞时丢失一些数据，却不允许数据有太大的时延。

5. **支持一对一、一对多、多对一、多对多的交互通信**。

6. **首部开销小：** 首部长度只有 8 字节，比 TCP 的 20 字节短。
 
#### **UDP 首部格式**

- 由 4 个字段组成，每个字段 2 字节，共 8 字节：

<div align="center">

![network_40](/images/network_40.png)

</div>

- **源端口：** 需要对方回信时选用，不需要时可全 0 ；

- **目的端口：** 终点交付报文时必须使用；

- **长度：** 用户数据报长度，仅有首部时最小长度为 8；

- **检验和：** 检测 UDP 用户数据报传输中是否有错，有错就丢弃。

	- **伪首部：** 计算检验和时，要在用户数据报前增加 12 个字节的**伪首部**；伪首部既不向下传送也不向上递交。
	
	- **UDP 检验和方法：** 和 IP 数据报方法相似；
	
	- **UDP 检验范围：** 把**首部**和**数据**部分一起都检验（**IP 数据报**检验只检验 IP 数据报首部）。
	
		> 同时检验了：源端口、目的端口、数据部分、源 IP 地址、目的 IP 地址。
   
### 传输控制协议 TCP

#### **TCP 协议概述**

1. **面向连接：** 传输前需建立连接，传输完成需释放连接；TCP 连接是一条**逻辑连接**。

2. **点对点：** 每一条 TCP 连接只能有两个端点；

3. **提供可靠交付：** 无差错、不丢失、不重复、不失序；

4. **全双工通信：** 设有发送缓存和接收缓存，允许双方随时发送数据。

5. **面向字节流：** TCP 把应用进程交下来的数据看作一连串的无结构字节流；

	- 多个字节加上 TCP 首部，构成 TCP 报文段。
	
	<div align="center">
	
	![network_41](/images/network_41.png)
	
	</div>
   
#### **TCP 的连接**

- **连接端点：** 套接字(socket)或插口。套接字由端口号拼接到 IP 地址构成。

<div align="center">

![network_42](/images/network_42.png)

</div>
 
### 可靠传输的工作原理

#### **停止等待 ARQ 协议**

1. **无差错情况：** 无差错情况最简单，A 发送 + B 确认，交替进行。

<div align="center">

![network_43](/images/network_43.png)

</div>

2. **出现差错（超时重传）：** B 接收 M1 时检测出了差错，就丢弃 M1，其他什么也不做（不通知 A 收到差错）。A 只要超过一段时间仍然没有收到确认，就认为刚才发送的分组丢失了，因而重传前面的分组，称为**超时重传**。

	- **应注意以下三点：**
	
		1. A 发送完一个分组后，必须暂时保留已发送**分组的副本**，以供超时重传使用。
		
		2. 分组和确认分组都必须有**编号**，用来确认哪个分组收到、哪个没有收到。
		
		3. **超时计时器**设置的重传时间应比数据在分组传输的**平均往返时间**长一些。
		
<div align="center">

![network_44](/images/network_44.png)

</div>
   
3. **确认丢失：** B 发送的对 M1 的确认丢失了，导致 A 没有收到确认并重传了 M1 。

	1. B 丢弃这个重复的分组 M1 ，不向上层交付；

	2. B 向 A 再次发送确认（不能认为已经发送过一次就不再发送，因为 A 并没有收到确认）。
	
<div align="center">

![network_45](/images/network_45.png)

</div>
 
4. **确认迟到：** B 发送的对 M1 的确认迟到了，导致 A 重传了 M1 并收到了两次 M1 的确认。

	1. A 将收到的重复确认直接丢弃；

	2. B 将收到的重复分组直接丢弃，并重传确认分组。（因为并不能确定是确认丢失还是确认迟到导致的收到了重复分组）

	<div align="center">

	![network_46](/images/network_46.png)

	</div>

- **信道利用率**

	- 停止等待协议优点是简单，缺点是信道利用率低。
	
	<div align="center">
	
	![network_47](/images/network_47.png)
	
	</div>
	
	- 为了提高传输效率，可以采用**流水线传输**，发送方可连续发送多个分组。连续发送使用的是**连续 ARQ 协议**和**滑动窗口协议**。
	
	<div align="center">
	
	![network_48](/images/network_48.png)
	
	</div>

#### **连续 ARQ 协议**

- **发送窗口：** 位于发送窗口内的 5 个分组都可以连续发送出去，而不需等待对方的确认，这样信道利用率就提高了。

<div align="center">

![network_49](/images/network_49.png)

</div>

- **累计确认：** 接收方不必对收到的分组逐个确认，而是在收到几个分组后，对**按序到达的最后一个分组**发送确认，表示此分组前的所有分组都正确收到。

- **缺点：** 无法反映此分组的后面分组的情况（可能后面分组全部正确）。
  
### TCP 报文段首部格式

- TCP 虽然是**面向字节流**的，但传送的数据单元却是**报文段**。一个报文段分为首部和数据两部分。

- TCP 报文段首部前 20 字节固定，后面有 4n 字节可根据需要加选项。
 
1. **源端口和目的端口：** 各占 2 字节；TCP 的分用功能通过端口实现。

2. **序号：** 占 4 字节（序号范围 [0,2<sup>32</sup>-1]）；TCP 传送的**字节流**中每一个字节都**按顺序编号**；序号字段代表本报文段发送数据的**第一个字节的序号**，例如一报文段序号301，最后一个字节序号400，那么下一个报文段的数据序号应当从401开始。

3. **确认号：** 占 4 字节；代表**期望收到对方下一个报文段第一个数据字节的序号**；例如 B 正确收到一个报文段，序号字段是 501，数据长度是 200 字节，则 B 期望收到下一个数据序号是 701 ，即将发送给 A 的确认报文段中的确认号置为 701 。

	> 若确认号 = N，代表直到序号 N-1 的所有数据都已正确收到。

4. **数据偏移：** 占 4 位；单位 4 字节；指出 TCP 报文段**数据起始处**距离 TCP 报文段起始处的距离；能表示的最大十进制数字为 15，因此数据偏移最大值是 60 字节，也是 TCP 首部的最大长度。

5. **保留：** 占 6 位，保留今后使用，应设置为 0；
 
6. **紧急 URG (urgent)：** 当 URG = 1 时，表明紧急指针字段有效。发送方 TCP 把紧急数据插入到本报文段数据的**最前面**。

7. **确认 ACK (acknowledgment)：** 仅当 ACK = 1 时确认号字段有效。TCP 规定，在连接建立后所有传送的报文段应将 ACK 置 1 。

8. **推送 PSH (push)：** 当两个应用进程交互式通信时，一段应用进程希望键入一个命令后立即收到对方响应；推送操作很少使用。

9. **复位 RST (reset)：** RST = 1 代表 TCP 出现严重差错（例如主机崩溃），必须释放连接； RST 还用来拒绝一个非法的报文段或拒绝打开一个连接。

10. **同步 SYN (synchronization)：** 连接建立时用来同步序号。当 SYN = 1 而 ACK = 0 时，表明这是一个**连接请求**报文段。SYN = 1 和 ACK = 1 就表示**连接接受**。

11. **终止 FIN (finish)：** 用来释放一个连接。当 FIN = 1 时，表明发送方数据已经发送完毕，并要求**释放连接**。

12. **窗口：** 占 2 字节；指发送本报文段的一方的**接收窗口**（窗口大小是由接收方定的，作为发送方设置其发送窗口的依据）。

	> 窗口字段明确指出了现在允许对方发送的数据量。窗口值是动态变化的。

13. **检验和：** 占 2 字节；检验范围包括**首部和数据**两部分。计算检验和前需要在 TCP 报文段前面加上 12 字节的伪首部，格式与 UDP 伪首部一样，但应把第 4 个字段中的 17 改为 6 （TCP 协议号为 6 ），把第 5 个字段中 UDP 长度改为 TCP 长度。

14. **紧急指针：** 占 2 字节；仅在 URG = 1 时才有意义，指出本报文段中紧急数据的字节数（紧急数据后都是普通数据），紧急指针指出了紧急数据的末尾所在报文段中的位置。

	- 窗口为 0 时也可发紧急数据。
  
15. **选项：** 长度可变，最长 40 字节。无选项时， TCP 首部长度为 20 字节。
 
### TCP 可靠传输实现

#### **以字节为单位的滑动窗口**

- 假定 A 收到来自 B 的确认报文段，其中窗口是 20 字节，确认号是 31 （表明 B 期望收到的下一个序号是 31）。根据这两个数据， A 构造出自己的发送窗口。

<div align="center">

![network_50](/images/network_50.png)

</div>

- 发送窗口由**前沿**和**后沿**共同确定。发送窗口通常**不断向前移动**，也有**可能不动**（一是没有收到新的确认；二是收到了新的确认但对方通知窗口缩小了，因此只有后沿缩小，前沿正好不动）。

- 描述一个发送窗口的状态需要**三个指针**：P<sub>1</sub>,P<sub>2</sub>,P<sub>3</sub>，指针指向字节序号。

	1. 小于 P<sub>1</sub> 是已经发送并**收到确认**的部分
	
	2. P<sub>1</sub> - P<sub>2</sub> 是已经发送但**未收到确认**的部分
	
	3. P<sub>2</sub> - P<sub>3</sub> 是**窗口内允许发送**但**尚未发送**的部分
	
	4. 大于 P<sub>3</sub> 是**窗口外不允许发送**的部分
	
	<div align="center">
	
	![network_51](/images/network_51.png)
	
	</div>
  
#### **超时重传时间选择**

- **重传时间选择**是 TCP 最复杂的问题之一：

	- **设置太短**： 引起很多报文段不必要的重传；
	
	- **设置太长**： 网络空闲时间增大，降低传输效率。
	
- TCP 采用**加权平均往返时间** RTT<sub>S</sub>：

	- 报文段收到确认与发出的时间差为往返时间；
	
	- RFC 6298 建议标准推荐的 α 值为 0.125 。（ α 接近 0 表示受新的往返时间样本影响小；接近 1 表示受新的样本影响大）。
	
	<div align="center">
	
	![network_52](/images/network_52.png)
	
	</div>
	
- **超时重传时间 RTO：**

	- 应略大于上面公式得出的 RTT<sub>S</sub> ，RFC 6298 建议使用下式计算：
	
	<div align="center">
	
	![network_53](/images/network_53.png)
	
	</div>
	
	- 其中 RTT<sub>D</sub> 是 RTT 的**偏差**加权平均值，RFC 6298 建议使用下式计算：
	
	- β 的推荐值是 0.25 。
	
	<div align="center">
	
	![network_54](/images/network_54.png)
	
	</div>
   
- **忽略重传样本：**

	- 发送出一个报文段，设定的重传时间到了还没有收到确认，因此重传，经过一段时间后收到了确认。**无法判断的问题：** 此确认是对第一次报文段的确认，还是第二次报文段的确认？
	
	<div align="center">
	
	![network_55](/images/network_55.png)
	
	</div>
	
	- **修正的 Karn 算法：** 报文段每重传一次，就把 RTO 增大为原来的 2 倍，当不再发生重传时，再根据上面的公式计算 RTO 。实践证明，这种策略较为合理。

#### **选择确认 SACK**

- 假设收到的报文段无差错，只是未按序号，中间还缺少一些序号的数据，那么时候能设法只传送缺少的数据而不重传已经正确到达接收方的数据？选择确认 SACK 就是一种可行的处理方法。

<div align="center">

![network_56](/images/network_56.png)

</div>

- 如果要使用选择确认 SACK ，那么在建立 TCP 连接时，就要在首部选项中加“允许 SACK”选项。

	- 首部选项长度最多 40 字节，指定一个边界需要用掉 4 字节（因为序号是 4 字节），因此选项中最多可以指明 4 个字节块的边界信息（8个边界共用掉 32 字节，还有 2 字节分别用来指明 SACK 选项和这个选项要占用多少字节）。

### TCP 流量控制

- 流利用滑动窗口可以实现流量控制。流量控制是让对方发送速率不要太快，要让接收方来得及接收。

<div align="center">

![network_57](/images/network_57.png)

</div>
 
- **持续计时器：** 当 rwnd = 0 时即不允许发送方再发送数据；过段时间 B 向 A 发送了 rwnd = 400 的报文段，但在传输过程丢失了，此时 A 仍然不发送数据，A, B 构成互相等待的局面。解决方案是 TCP 连接一方收到对方的零窗口通知，就启动**持续计时器**，若时间到期就发送一个**零窗口探测报文段**，对方则会确认这个探测报文段，死锁的僵局就被打破了。

### TCP 拥塞控制

#### **拥塞控制的一般原理**

- **拥塞定义：** 若网络中某一资源的需求超过了该资源所能提供的可用部分，网络性能就会变坏，称为拥塞。

<div align="center">

![network_58](/images/network_58.png)

</div>
 
- **拥塞控制：** 防止过多的数据注入到网络中，使网络中路由器或链路不至过载。

	- 拥塞控制是一个**全局性过程**；流量控制往往指**点对点通信量的控制**。
	
	- 拥塞控制很难设计，因为是一个**动态问题**。
	
	<div align="center">
	
	![network_59](/images/network_59.png)
	
	</div>
  
#### **TCP 的拥塞控制方法**

- 发送方窗口的上限值应取**接收方窗口 rwnd 和拥塞窗口 cwnd 两个变量中的较小值**：

<div align="center">

![network_60](/images/network_60.png)

</div>
 
- **拥塞控制流程图：**

<div align="center">

![network_61](/images/network_61.png)

</div>

1. **慢开始：**当主机开始发送数据时，由于不清楚网络负载情况，因此由**小到大逐渐增大发送窗口**。

	- 开始时设置 cwnd = 1 ；每经过一个传输轮次，拥塞窗口 cwnd 就加倍。（一个传输轮次所经历的时间其实就是往返时间 RTT ）。
	
	<div align="center">
	
	![network_62](/images/network_62.png)
	
	</div>

2. **拥塞避免：** 让拥塞窗口 cwnd 缓慢增大，即没经过一个往返时间 RTT 就把发送方的拥塞窗口 cwnd 加 1 ，而不是像慢开始阶段那样加倍增长，使网络比较不容易出现拥塞。

	- **当 cwnd < ssthresh 时**，使用慢开始算法；**当 cwnd > ssthresh 时**，使用拥塞避免算法。
	
	- 网络出现超时，发送方判断网络拥塞，于是调整门限值 **ssthresh = cwnd / 2，cwnd = 1**，并进入慢开始阶段；当逐渐增大到 **ssthresh = cwnd** 时，进入拥塞避免阶段。
	
	<div align="center">
	
	![network_63](/images/network_63.png)
	
	</div>
	
3. **快重传：** 让发送方尽早**知道个别报文段的丢失**，以至于不需超时重传。使用快重传可以使整个网络吞吐量提升 20% 。

	- 快重传要求接收方不要等待自己发送数据时才捎带确认，而是**立即发送确认**；即使收到了失序的报文段也要立即发出对已收到的报文段的**重复确认**。
	
	- 如下图所示，发送方只要一**连收到 3 个重复确认**，就知道接收方没有收到 M3 ，而**立即重传 M3** 。
	
	<div align="center">
	
	![network_64](/images/network_64.png)
	
	</div>
  
4. **快恢复：** 当接收到连续 3 个重复确认时，知道只是丢失了个别报文段，于是不启动慢开始，而是执行快恢复。

	- 发送方调整门限值 **ssthresh = cwnd / 2**；同时设置 **cwnd = ssthresh** ，立即进入**拥塞避免**。

### TCP 的运输连接管理

#### **TCP 连接建立 —— 三报文握手**

- **三报文握手解决的问题：** 假设 A 发出的第一个连接请求报文段没有丢失，而是在网络某处长时间滞留了，本来这是一个失效的连接请求；但后续 B 收到了它，误认为是 A 的连接请求，就向 A 发出确认报文段。若不采用三报文握手，此时一个已经失效的连接已经被建立，网络资源被白白浪费了。

| 握手次数 | A 确认 | B 确认|
|:--|:--:|:--:|--:|
|第一次 | | A 发送正常，B 接收正常|
|第二次 |A 发送正常， B 接收正常；A接收正常，B发送正常 | |
|第三次 | |A 发送正常， B 接收正常；A接收正常，B发送正常|

<div align="center">

![network_65](/images/network_65.png)

</div>

#### **TCP 连接释放 —— 四报文握手**

1. A 发送 TCP 连接释放报文，进入 **FIN-WAIT-1** 状态，**等待 B 的确认**；

2. B 收到报文段后立即发出确认，并进入 **CLOSE-WAIT** 状态；此时 A 到 B 的单向连接已经被释放， TCP 连接处于**半关闭状态**；B 继续向 A 传输未完成的数据。

3. A 收到 B 的确认后，进入 **FIN-WAIT-2** 状态，**等待 B 的连接释放报文段**；

4. 若 B 已经将所有数据向 A 发送完毕，则向 A 发送 TCP 连接释放报文，并进入 **LAST-ACK** 状态，**等待 A 的确认**；

5. A 收到 B 的连接释放报文段后，立即发出确认，然后进入 **TIME-WAIT** 状态，必须经过**时间等待计时器** 设置的时间 2MSL(4 min) 后，A 才进入 **CLOSED** 状态，这是因为：

	- **第一：** 由于 A 到 B 的确认报文段有可能丢失，B 可能回超时重传释放连接报文段，而 A 就能在 2MSL 时间内收到这个重传的连接释放报文段，并重传确认且重新启动 2MSL 计时器。
	
	- **第二：** 防止**已失效的连接请求报文段**出现在本连接中， A 等待 2MSL 就可以使本链接持续时间内所产生的所有报文段在网络中小时。
	
	<div align="center">
	
	![network_66](/images/network_66.png)
	
	</div>

- **TCP 保活计时器：** 服务器每收到一次客户端的数据后，就重新设置保活计时器（通常为 **2h** ），若保活计时器期限内没有收到客户的数据，服务器就每隔 **75s** 发出一个探测报文段，若连续 **10** 个探测报文端后客户仍无响应，则**直接关闭连接**。
 
#### **TCP 有限状态机**

- 为了更清晰地看出 TCP 连接各种状态之间的关系，下图给出了 TCP 的有限状态机。图中每一个方框即 TCP 可能**具有的状态**；状态之间的箭头表示**可能发生的状态变迁**；箭头旁边的文字表明引起这种**变迁的原因**：

	- 粗实线箭头表示**对客户进程正常变迁**；

	- 粗虚线箭头表示**对服务器进程的正常变迁**；

	- 细线箭头表示**异常变迁**。
	
	<div align="center">
	
	![network_67](/images/network_67.png)
	
	</div>
	
## 参考文献
1. 谢希仁. [计算机网络（第7版）. 电子工业出版社, 2017](https://book.douban.com/subject/26960678/)
2. James F. Kurose / Keith W. Ross. [计算机网络（原书第7版）-自顶向下方法. 机械工业出版社, 2018](https://book.douban.com/subject/30280001/)
