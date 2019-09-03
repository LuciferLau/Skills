# Skynet框架
> 工作中服务器模型是类似这个开源框架的，是一个单进程多线程的服务器模型，基于C和lua。学习这个模型对项目的框架也能更好的理解。
## 网易大佬云风写的基于ACTOR模型的服务器框架
> [云风的blog-skynet](https://blog.codingnow.com/2012/09/the_design_of_skynet.html)

## Actor模型好处
1. 事件模型驱动： Actor之间的通信是异步的，即使Actor在发送消息后也无需阻塞或者等待就能够处理其他事情。  
2. 强隔离性： Actor中的方法不能由外部直接调用，所有的一切都通过消息传递进行的，从而避免了Actor之间的数据共享，想要观察到另一个Actor的状态变化只能通过消息传递进行询问。  
3. 位置透明： 无论Actor地址是在本地还是在远程机上对于代码来说都是一样的。  
4. 轻量性：Actor是非常轻量的计算单机，只需少量内存就能达到高并发。  

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
