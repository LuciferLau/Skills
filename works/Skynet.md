# Skynet框架
> 工作中服务器模型是类似这个开源框架的，是一个单进程多线程的服务器模型，基于C和lua。学习这个模型对项目的框架也能更好的理解。
## 前网易现ejoy大佬云风写的基于ACTOR模型的Skynet框架
> [云风的blog-skynet](https://blog.codingnow.com/2012/09/the_design_of_skynet.html)

## Actor模型好处
1. 事件模型驱动： Actor之间的通信是异步的，即使Actor在发送消息后也无需阻塞或者等待就能够处理其他事情。  
2. 强隔离性： Actor中的方法不能由外部直接调用，所有的一切都通过消息传递进行的，从而避免了Actor之间的数据共享，想要观察到另一个Actor的状态变化只能通过消息传递进行询问。  
3. 位置透明： 无论Actor地址是在本地还是在远程机上对于代码来说都是一样的。  
4. 轻量性：Actor是非常轻量的计算单机，只需少量内存就能达到高并发。  
> 作者：简单说，Skynet 只负责把一个数据包从一个服务内发送出去，让同一进程内的另一个服务收到，调用对应的 callback 函数处理。它保证，模块的初始化过程，每个独立的 callback 调用，都是相互线程安全的。编写服务的人不需要特别的为多线程环境考虑任何问题。专心处理发送给它的一个个数据包。

## 服务
preload会预加载服务对应的函数，环境变量也被加载进去，且只增不减。  
环境变量设置完成后，当前节点上所有服务都可以访问到，即使服务退出了，所以要慎用。  
服务分为普通服务和全局唯一服务。  

### 分类
普通服务通过：*newservice(servicename)* 创建  

唯一服务通过：*uniqueservice(true,servicename)* 创建，如果带了true参数表示服务在所有节点唯一，否则在本节点唯一  

查询服务使用：*queryservice(true,servicename)* true含义同上   

### 别名
别名：本地别名以'.'开头，如.alias，全局别名不能以点开头，通过:*name("(.)alias",servicename)设置  

注意杀死别名服务后，本地别名变为nil，但全局的别名依然存在，使用它会导致不可预知的错误。  
还有，注意区分全局唯一服务名和全局别名。
### 调度
> 因为这一块底层封装了，所以快速浏览一遍，暂不深究  

Name | Description
:-:|-
skynet.sleep(time) | 让当前的任务等待 time * 0.01s。
skynet.fork(func, ...) | 虽然你也可以使用原生的coroutine.create来创建协程，但是会打乱skynet的工作流程;启动一个新的任务去执行函数 func , 其实就是开了一个协程，函数调用完成将返回线程句柄。
skynet.yield() | 让出当前的任务执行流程，使本服务内其它任务有机会执行，随后会继续运行。
skynet.wait() | 让出当前的任务执行流程，直到用 wakeup 唤醒它。
skynet.wakeup(co) | 唤醒用 wait 或 sleep 处于等待状态的任务。
skynet.timeout(time, func) | 设定一个定时触发函数 func ，在 time * 0.01s 后触发。
skynet.starttime() | 返回当前进程的启动 UTC 时间（秒）。
skynet.now()| 返回当前进程启动后经过的时间 (0.01 秒) 。
skynet.time() | 通过 starttime 和 now 计算出当前 UTC 时间（秒）。

### 服务间消息通信
> 项目使用的是REST API是restful风格的消息传递，rfc和传统RPC有一定区别。  
所谓 RPC 调用，就是一个远程对象加一个方法名，加上若干参数。通过序列化方法，打包成一个数据包，查询到远程对象所在的服务地址，发送过去即可。  
  
每种类型的服务，都在start处用dispatch进行绑定（注册），而参数的传递，由pack()打包；unpack()；解包trash()释放。

#### 消息处理
    1.不需响应的消息
send(addr, type, ...) --发送未打包的消息，函数自动打包...参数列表
rawsend(addr, tpye, msg, sz) --发送打包好的消息
redirect(dst, src, type, session, msg, sz) --伪造其它服务地址发送信息，可以用于白盒测试？

    2.必须响应的消息
call(addr, type, ...) --发送未打包的消息并等待响应，函数自动打包...参数列表并解包  
rawcall(addr, tpye, msg, sz) --发送打包好的消息，需要自行解包
session --保证本服务中发出的消息是唯一的

    3.响应消息
ret --用于返回服务处理消息的结果
retpack(...) --将消息用pack打包后再调用ret
response --不想接受请求和响应请求再同一个协程中完成时，用它代替ret

#### 消息队列
> 如果某个服务中有阻塞API，它挂起后仍能处理别的消息，可能得到未预计的结果，解决方法就是消息队列。  
> *本质上它就是个存放服务的临界区，项目中一般cs就是指消息队列。*

skynet维护了两级消息队列，全局消息队列里面放了所有不为空的次级消息队列。  
通过将消息先存入消息队列，再从消息队列中严格按照先后次序取出执行对应服务，保证了伪线程安全（可重入）。

#### 代理服务
所谓代理，就是讲请求的消息先发给代理服务，再由代理服务转发给真正处理这个请求的服务；  
同样的，处理的结果也是先返回给代理服务，再返回给请求。项目中一般用*proxy*表示代理。  
forward_type和start类似，前者还需要一张消息映射转换表，其它方法和start一样[skynet.PTYPE_OLDNAME]= skynet.PTYPE_NEWNAME

## 集群
虽然说skynet是单进程的，但是也可以建立集群。  
单进程的优点是数据都在内存中，通过传递C结构体指针，不同服务可以快速访问相同内存块的内容而无需建立TCP连接而产生额外的开销。甚至实现零拷贝，但这有风险。  
如果使用上了集群，不同的skynet节点就要使用服务来进行进程间通信了；  
在skynet中这个服务名叫harbor，它通过在两个/多个节点之间建立TCP连接完成数据交换的工作；  
还有一个master服务广播告知集群内所有节点新加入的节点地址。  

### 节点间消息通信
前面讲了单进程内的服务通信，不同skynet进程间也可以通信，方法有组播和广播。

#### 组播 multicast
就是类似发布与订阅系统，在创建了一个频道后，向里面发送消息，订阅了这个频道的人都能收到这个消息。  
在游戏中，管理员给全体用户或者部分用户发送邮件的时候能用上？  
单节点情况下，组播就是传C结构体指针；多节点情况下，将整个数据包传过去并让其它节点中的接受者传递这个C结构体指针。

## 套接字 SOCKET
> 项目也有封装，大体浏览一遍了解

Name | Description
:-:|-
socket.open(address, port) | 建立一个 TCP 连接。返回一个数字 id，底层使用linux socket api 的connect。
socket.close(id) | 关闭一个连接，这个 API 有可能阻塞住执行流。因为如果有其它 coroutine正在阻塞读这个 id 对应的连接，会先驱使读操作结束，close 操作才返回。
socket.close_fd(id) | 在极其罕见的情况下，需要粗暴的直接关闭某个连接，而避免 socket.close 的阻塞等待流程，可以使用它。
socket.shutdown(id) | 强行关闭一个连接。和 close 不同的是，它不会等待可能存在的其它 coroutine 的读操作。一般不建议使用这个API，但如果你需要在 \_\_gc 元方法中关闭连接的话，shutdown 是一个比 close 更好的选择（因为在 gc 过程中无法切换 coroutine）与close_fd类似。
socket.read(id, sz) | 从一个 socket 上读 sz 指定的字节数。如果读到了指定长度的字符串，它把这个字符串返回;如果连接断开导致字节数不够，将返回一个 false 加上读到的字符串;如果 sz 为 nil ，则返回尽可能多的字节数，但至少读一个字节（若无新数据，会阻塞）。
socket.readall(id) | 从一个 socket 上读所有的数据，直到 socket 主动断开，或在其它 coroutine 用 socket.close 关闭它。
socket.readline(id, sep) | 从一个 socket 上读一行数据。sep 指行分割符。默认的 sep 为 "\n"。读到的字符串是不包含这个分割符的。如果另外一端就关闭了，那么这个时候会返回一个nil，如果buffer中有未读数据则作为第二个返回值返回。
socket.block(id) | 等待一个 socket 可读。
socket.write(id, str) | 把一个字符串置入正常的写队列，skynet 框架会在 socket 可写时发送它。
socket.lwrite(id, str) | 把字符串写入低优先级队列。如果正常的写队列还有写操作未完成时，低优先级队列上的数据永远不会被发出。只有在正常写队列为空时，才会处理低优先级队列。但是，每次写的字符串都可以看成原子操作。不会只发送一半，然后转去发送正常写队列的数据。
socket.listen(address, port, backlog) |监听一个端口，返回一个 id ，供 start 使用。
socket.start(id , accept) | accept 是一个函数。每当一个监听的 id 对应的 socket 上有连接接入的时候，都会调用 accept 函数。这个函数会得到接入连接的 id 以及 ip 地址。你可以做后续操作。每当 accept 函数获得一个新的 socket id 后，并不会立即收到这个 socket 上的数据。这是因为，我们有时会希望把这个 socket 的操作权转让给别的服务去处理。accept(id, addr)
socket.start(id) | 任何一个服务只有在调用 socket.start(id) 之后，才可以读到这个 socket 上的数据。向一个 socket id 写数据也需要先调用 start 。socket 的 id 对于整个 skynet 节点都是公开的。也就是说，你可以把 id 这个数字通过消息发送给其它服务，其他服务也可以去操作它。skynet 框架是根据调用 start 这个api 的位置来决定把对应 socket 上的数据转发到哪里去的。
socket.abandon(id) | 清除 socket id 在本服务内的数据结构，但并不关闭这个 socket 。这可以用于你把 id 发送给其它服务，以转交 socket 的控制权。
socket.warning(id, callback) | 当 id 对应的 socket 上待发的数据超过 1M 字节后，系统将回调 callback 以示警告。function callback(id, size) 回调函数接收两个参数 id 和 size ，size 的单位是 K 。如果你不设回调，那么将每增加 64K 利用 skynet.error 写一行错误信息。

## 框架内一些server
> 水平不足，先不深入了解这些底层实现。
### DNS Server
用域名获取IP时，底层使用系统调用 getaddrinfo()可能导致阻塞。
### Login Server
其实就是通过各种加密算法对账号密码进行处理。
### Gateway Server
控制客户端连接数，接受socket和lua的消息。
#### Agent服务
一般网关服务登陆完毕后，会由agent服务处理客户端请求（登入登出）
### Msg Server
接受客户请求的数据包，并给出回应。
### HTTP Server
服务器可以像web server一样接受并响应HTTP请求，实现某些功能。

## 使用Protobuf服务
使用云风的[pbc](https://github.com/cloudwu/pbc)，对protobuf进行了一定封装使其更加方便。
注册.proto文件后，直接使用encode/decode方法进行序列化/反序列化。

## Skynet框架
 ← agent →  ← ← ← ← ←   
 ↓        ↑↓         ↑  
client → gate ←→ watchdog ← main
## 参考文献
[Skynet——Github](https://github.com/cloudwu/skynet/wiki)
