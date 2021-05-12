# LIBEVENT学习笔记
> 好久没在GIT上面写点东西了，菜鸡的主动学习能力不足，没多久就到了自己的性能瓶颈，适逢最近有新大佬带引擎方面的知识，于是开始学习libevent相关的知识。
> 
> ~~🤐顺带吐槽下，项目用的redmine感觉就是开单改进度关单的工具，公司也没有用git做项目管理，最近倒是多了个叫TAPD的玩意，估计是企鹅那边的需求吧，项目管理真没劲👎~~

![](https://camo.githubusercontent.com/911381096c669dd9fef3699539e6b9cf38b0ef4aac3a546f76741bc919d1bd9a/68747470733a2f2f6c69626576656e742e6f72672f6c69626576656e74332e706e67)
---
# 目录
[R0：概要](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r0-preliminaries-%E7%AC%AC%E4%B8%80%E6%AD%A5%E4%B8%87%E4%B8%88%E9%AB%98%E6%A5%BC%E5%B9%B3%E5%9C%B0%E8%B5%B7)

[R1：设置libevent](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r1-setting-up-the-libevent-library-%E8%AE%BE%E7%BD%AElibevent)

[R2：创建event_base](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r2-getting-an-event_base-%E4%BD%BF%E7%94%A8event_base%E9%87%8D%E8%A6%81%E5%9F%BA%E7%A1%80)

[R3：使用事件循环](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r3-running-an-event-loop-%E4%BD%BF%E7%94%A8%E4%BA%8B%E4%BB%B6%E5%BE%AA%E7%8E%AF)

[R4：与事件一起工作](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r4-working-with-events-%E4%B8%8E%E4%BA%8B%E4%BB%B6%E4%B8%80%E8%B5%B7%E5%B7%A5%E4%BD%9C)

[R5：扩展和可移植函数](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r5-utility-and-portability-functions-%E6%89%A9%E5%B1%95%E5%92%8C%E5%8F%AF%E7%A7%BB%E6%A4%8D%E5%87%BD%E6%95%B0)

[R6：*bufferevents*的概念与基础](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r6-bufferevents-concepts-and-basics-bufferevents%E7%9A%84%E6%A6%82%E5%BF%B5%E4%B8%8E%E5%9F%BA%E7%A1%80)

[R6a：*bufferevent*进阶使用](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r6a-bufferevents-advanced-topics-bufferevent%E8%BF%9B%E9%98%B6%E4%BD%BF%E7%94%A8)

[R7：*evbuffer*：缓存IO的高效且实用的方式](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r7-evbuffers-utility-functionality-for-buffered-io-evbuffer%E7%BC%93%E5%AD%98io%E7%9A%84%E9%AB%98%E6%95%88%E4%B8%94%E5%AE%9E%E7%94%A8%E7%9A%84%E6%96%B9%E5%BC%8F)

[R8：监听并接受TCP连接](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r8-connection-listeners-accepting-tcp-connections-%E7%9B%91%E5%90%AC%E5%B9%B6%E6%8E%A5%E5%8F%97tcp%E8%BF%9E%E6%8E%A5)

[R9：使用libevent的DNS功能](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r9-dns-for-libevent-%E4%BD%BF%E7%94%A8libevent%E7%9A%84dns%E5%8A%9F%E8%83%BD)

---
## 跟着官方文档步步走
> [官方文档传送门]http://www.wangafu.net/~nickm/libevent-book/ 当前笔记对应文档版本：Last updated 2012-01-31 09:28:39 EDT
> 
> 当前使用libevent版本libevent-2.1.12-stable Released 2020-07-05.
### R0: Preliminaries (第一步，万丈高楼平地起)
#### libevent 由下列组件构成：
组件名|描述
--|:--:
evutil|用于抽象不同平台网络实现差异的通用功能。
event && event_base|libevent 的核心，为各种平台特定的、基于事件的非阻塞 IO 后端提供抽象 API，让程序可以知道套接字何时已经准备好，可以读或者写，并且处理基本的超时功能，检测 OS 信号。
bufferevent|为 libevent 基于事件的核心提供使用更方便的封装。除了通知程序套接字已经准备好读写之外，还让程序可以请求缓冲的读写操作，可以知道何时IO已经真正发生；bufferevent接口有多个后端，可以采用系统能够提供的更快的非阻塞 IO 方式，如 Windows 中的 IOCP。
evbuffer|在 bufferevent 层之下实现了缓冲功能，并且提供了方便有效的访问函数。
evhttp|一个简单的 HTTP 客户端/服务器实现。
evdns|一个简单的 DNS 客户端/服务器实现。
evrpc|一个简单的 RPC 实现。
#### libevent中的库:
库名|描述
--|:--:
ibevent_core|所有核心的事件和缓冲功能，包含了所有的 event_base、evbuffer、bufferevent 和工具函数。
ibevent_extra|定义了程序可能需要，也可能不需要的协议特定功能，包括 HTTP、DNS 和 RPC。
libevent|这个库因为历史原因而存在，它包含 libevent_core 和 libevent_extra 的内容。不应该使用这个库，未来版本的 libevent 可能去掉这个库。
---
某些平台上可能安装下列库:
库名|描述
--|:--:
libevent_pthreads|添加基于 pthread 可移植线程库的线程和锁定实现。它独立于libevent_core，这样程序使用 libevent 时就不需要链接到 pthread，除非是以多线程方式使用 libevent。
libevent_openssl|这个库为使用 bufferevent 和 OpenSSL 进行加密的通信提供支持。它独立于 libevent_core，这样程序使用 libevent 时就不需要链接到 OpenSSL，除非是进行加密通信。
#### 关于版本：
①1.3之前是BUG版本，1.4之前只有一个libevent库，2.0之前线程不安全（无lock，condition机制），新的头文件都在event2文件夹中，为了兼容旧的头文件，开发时尽可能不要用到旧的玩意，毕竟没有人维护是最大的问题。

②版本号的坑：libevent版本号会在编译时确定宏定义的，但如果使用动态库，就不能保证接口返回的版本号了。

```
#define LIBEVENT_VERSION_NUMBER ~= ev_uint32_t event_get_version_number(void)
int check_version_match(void) //判断当前运行版本号是否为编译时版本号
{
    ev_uint32_t v_compile, v_run;
    v_compile = LIBEVENT_VERSION_NUMBER;
    v_run = event_get_version_number();
    if ((v_compile & 0xffff0000) != (v_run & 0xffff0000)) {
        printf("运行时版本号(%s)与编译时版本号(%s)不同.\n", event_get_version(), LIBEVENT_VERSION);
        return -1;
    }
    return 0;
}
```

---
### R1: Setting up the Libevent library (设置libevent)
#### 日志相关：Debug用，暂略
```
*日志等级，分四级*
#define EVENT_LOG_DEBUG 0
#define EVENT_LOG_MSG   1
#define EVENT_LOG_WARN  2
#define EVENT_LOG_ERR   3
*对应四个定义*
#define _EVENT_LOG_DEBUG EVENT_LOG_DEBUG
#define _EVENT_LOG_MSG   EVENT_LOG_MSG
#define _EVENT_LOG_WARN  EVENT_LOG_WARN
#define _EVENT_LOG_ERR   EVENT_LOG_ERR
```
#### 错误处理：类似tryCatch机制，暂略
#### 内存管理：重载malloc、realloc、free，优化用，水平不足暂略
`在释放先前分配的event_base等结构体时，内存并不能完全释放干净，可能会有部分全局结构体遗留，通常这无关紧要，但这会导致部分监察工具觉得程序出现了内存泄漏，这时候可以调用：void libevent_global_shutdown(void)，这个函数不会帮你释放你先前创造的结构体，只是起到回收垃圾，即那些残留结构体的作用，具体可以参考event2/event.h中定义与.c中的实现`
#### 锁和线程：理解pthread即可
---
### R2: Getting an event_base (创建event_base,重要基础)
当然，可以用简单的方式new一个最普通的base出来；

`struct event_base *event_base_new(void);`

+ 对应的析构函数就是(不会释放相关联的任何事件，或者关闭他们的套接字，或者释放任何指针)；

`void event_base_free(struct event_base *base);`

+ 也可以使用配置文件new一个你想要的base出来；

`struct event_base *event_base_new_with_config(const struct event_config *cfg);`

+ 这个配置创建也有它的一套讲究，首先是先创建一个空的config；

`struct event_config *event_config_new(void);`

+ 当然也有对应的析构函数;

`void event_config_free(struct event_config *cfg);`

+ 然后就是配置这个配置了，说起来怪怪的🙇‍♂️
```
你可以选择你不想要的后端:(select, poll, epoll, kqueue, devpoll, evport, win32)
int event_config_avoid_method(struct event_config *cfg, const char *method);
不想用它就把它avoid掉吧，如果想知道什么后端可以用，就使用：
const char **event_get_supported_methods(void);
函数返回一个指针，指向 libevent 支持的方法名字数组，最后一个元素为NULL。
----------------------------------------------------------------------------------------------------
你可以选择你想要的特性：
enum event_method_feature {
    EV_FEATURE_ET = 0x01, //支持边缘触发
    EV_FEATURE_O1 = 0x02, //O(1)完成增删查操作
    EV_FEATURE_FDS = 0x04,//支持多句柄控制
};
int event_config_require_features(struct event_config *cfg, enum event_method_feature feature);
同样的，想知道当前的base是用什么特性，就使用：
enum event_method_feature event_base_get_features(const struct event_base *base);
返回值和枚举做‘&’操作，就能判断是否为当前特性。
----------------------------------------------------------------------------------------------------
你可以让libevent根据你的喜好工作：
enum event_base_config_flag {
    EVENT_BASE_FLAG_NOLOCK = 0x01,              //不给用锁，线程不安全
    EVENT_BASE_FLAG_IGNORE_ENV = 0x02,          //不检查EVENT_*环境，更难调试
    EVENT_BASE_FLAG_STARTUP_IOCP = 0x04,        //仅Windows
    EVENT_BASE_FLAG_NO_CACHE_TIME = 0x08,       //不要缓存时间，每次超时回调时再获取时间检测，更耗时
    EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST = 0x10,//若使用epoll，可选更快的epoll-changeList，但会有小bug
    EVENT_BASE_FLAG_PRECISE_TIMER = 0x20        //使用更慢但可能更精确的计时机制，默认是使用最快但不那么精确的
};
int event_config_set_flag(struct event_config *cfg, enum event_base_config_flag flag);
----------------------------------------------------------------------------------------------------
用配置虽然很爽，但你的OS不一定支持这些配置，如果不能满足你的要求，event_base_new_with_config只能返回NULL。
```
+ 可以让你的base有不同的优先级，**event2/event.h:1440** 中定义了 **EVENT_MAX_PRIORITIES** 值为 256。
最好在new出这个base之后立刻调用，否则必须在所有事件active前调用；

`int event_base_priority_init(struct event_base *base, int n_priorities);`

+ 同样的，获取这个base当前的优先级;

`int event_base_get_npriorities(struct event_base *base);`

+ 在 *fork()* 之后，后端不一定可以正常的工作，所以需要重新初始化这个event_base；

`int event_reinit(struct event_base *base);`

for example:
```
struct event_base *base = event_base_new();
/* ... add some events to the event_base ... */
if (fork())     //parent
{
    continue_running_parent(base);
} 
else            //child 
{ 
    event_reinit(base);
    continue_running_child(base);
}
```
---
### R3: Running an event loop (使用事件循环🌟)
事件循环，顾名思义就是让base loop起来，处理里面的event，这里base充当一个controller的角色；

🅰️:开始事件循环
```
#define EVLOOP_ONCE             0x01
#define EVLOOP_NONBLOCK         0x02
#define EVLOOP_NO_EXIT_ON_EMPTY 0x04 
int event_base_loop(struct event_base *base, int flags);
int event_base_dispatch(struct event_base *base);
event_base_dispatch == (没有flags的)event_base_loop

LOOP工作方式的伪代码：
while (any events are registered with the loop, or EVLOOP_NO_EXIT_ON_EMPTY was set) {
    if (EVLOOP_NONBLOCK was set, or any events are already active)
        如果有事件被触发，标记他们为active的
    else
        等待至少一个事件被触发，并标记为active的
        
    for (p = 0; p < n_priorities; ++p) {
       if (当前p优先级有active事件) {
          执行当前p优先级的所有active事件
          break; /* Do not run any events of a less important priority */
       }
    }
    
    if (EVLOOP_ONCE was set or EVLOOP_NONBLOCK was set)
       break;
}

无flags的情况下，event_base_loop（）函数默认运行 event_base 直到其中没有已经注册的事件为止。

有flags的情况下，由伪代码可以看出，循环中如果没有注册事件，但设置了 EVLOOP_NO_EXIT_ON_EMPTY ，这个循环也将无限做下去；
否则在循环执行一次后，将break这个while，返回循环结果，succ 0，fatal/err -1；
ONCE和NONBLOCK的区别也比较简单，
NONBLOCK会不断尝试检测注册事件的状态，并将它们标记为active，如果没事件触发，直接就返回了；
ONCE则阻塞在else处，等待至少一个事件被触发，并标记为active处理完返回。
```
🅱️:结束事件循环

event_base_loopexit可以在指定时间之后停止循环，若tv为NULL则立刻停止；

`int event_base_loopexit(struct event_base *base, const struct timeval *tv);`

event_base_loopbreak也可以立刻停止循环，区别是前者执行完forLoop中所有active事件的回调后停止，后者只执行完当前active事件的回调(break forLoop)。

`int event_base_loopbreak(struct event_base *base);`

若果想知道base是因为哪种方式停止的，可通过这两个函数；

`int event_base_got_exit(struct event_base *base);`

`int event_base_got_break(struct event_base *base);`

⏲️时间的小坑；
```
int event_base_gettimeofday_cached(struct event_base *base, struct timeval *tv_out);
int event_base_update_cache_time(struct event_base *base); //2.1.1-alpha新增，base若没有在运行循环，则调用无效
如果想避免系统调用 gittimeofday() 带来的额外开销，可以调用event_base_gettimeofday_cached，这个函数会将回调开始时的时间设进tv_out里。
但如果你的回调执行的时间越长，这个时间就会愈发不准确，当然你可以update它（个人见解：好像没啥意义，还不如直接拿系统当前时间）。
```

🐛DEBUG相关：

如果想获得base里面所有的事件及其状态，可以用它写进文件里；

`void event_base_dump_events(struct event_base *base, FILE *f);`

☢️新特性:可以通过类似foreach的方法遍历base里面绑定的事件，但千万不能进行增删改操作！

`int event_base_foreach_event(struct event_base *base, event_base_foreach_event_cb fn, void *arg); //2.1.2-alpha新增`

void \*arg 作为callback的参数传入，且callback函数必须返回0，才能继续让迭代器往下走，否则迭代器停止，回调函数的返回值也就是 event_base_foreach_event() 的返回值。

`typedef int (*event_base_foreach_event_cb)(const struct event_base *, const struct event *, void *);`

这个操作会给当前的event_base上锁，其他线程可能进行的某些有益操作可能会被拒绝，所以确保foreach里面callback不要做太多事，占用这个event_base太久。

---
### R4: Working with events (与事件一起工作🌟)
事件是libevent的基本操作单元，它拥有以下的状态变化。
状态名|描述
--|:--:
初始态 initialized|事件刚刚new出来的状态
未决态 pending|事件刚刚添加进event_base的状态
激活态 active|事件触发的条件达成时进入的状态
持久态 persistent|可将事件配置为这个状态，事件将一直保持未决
非未决态|非持续态事件执行完一次回调后的状态

#define 条件名|描述
--|:--:
EV_TIMEOUT      0x01 |表示超时发生时触发这个事件
EV_READ         0x02 |表示fd可读时触发这个事件
EV_WRITE        0x04 |表示fd可写时触发这个事件
EV_SIGNAL       0x08 |表示收到对应信号(POSIX)时触发这个事件
EV_PERSIST      0x10 |表示事件是持久的
EV_ET           0x20 |表示fd可读写时，应该边缘触发(如果event_base后端支持的话)

1️⃣:创建一个事件
```
typedef void (*event_callback_fn)(evutil_socket_t, short, void *); //回调函数指针
struct event *event_new(struct event_base *base, evutil_socket_t fd, short what, event_callback_fn cb, void *arg);
其中what参数是上面定义条件的flags的集合，除了cb自己，其它参数都会传给cb做参数使用。
event通过_add()放入event_base，通过_del()从event_base移除；

For Example:
void cb_func(evutil_socket_t fd, short what, void *arg)
{
        const char *data = arg;
        printf("Got an event on socket %d:%s%s%s%s [%s]",
            (int) fd,
            (what&EV_TIMEOUT) ? " timeout" : "",
            (what&EV_READ)    ? " read" : "",
            (what&EV_WRITE)   ? " write" : "",
            (what&EV_SIGNAL)  ? " signal" : "",
            data);
}

void main_loop(evutil_socket_t fd1, evutil_socket_t fd2)
{
        struct event *ev1, *ev2;
        struct timeval five_seconds = {5,0};
        struct event_base *base = event_base_new();

        /* The caller has already set up fd1, fd2 somehow, and make them nonblocking. */
        // 都是持久态事件，前者读就绪触发，后者写就绪触发，前者还能超时触发
        ev1 = event_new(base, fd1, EV_TIMEOUT|EV_READ|EV_PERSIST, cb_func, (char*)"Reading event"); 
        ev2 = event_new(base, fd2, EV_WRITE|EV_PERSIST, cb_func, (char*)"Writing event");

        event_add(ev1, &five_seconds);
        event_add(ev2, NULL);
        event_base_dispatch(base); //默认方式开启event_base_loop
}
这段示例代码可以在事件触发回调时打印出他们所需要的部分条件，并将参数打印到标准I/O
```
2️⃣:关于持久态

一般的事件，在event_base中触发过变成激活态后，执行完回调函数，就会回到非未决态(event_base将无视这个事件)，如果想让它重新回到未决态(可以被触发激活)，需要在回调函数中对它进行 *event_add()* ；

而EV_PERSIST的设置，表示了事件可以多次触发，多次激活->回调->未决；而想让它终止下来，则需要对其调用 *event_del()* 。

PS:如果想要将自己作为参数传入回调，可以使用 `void *event_self_cbarg();` 不可直接传入自己，因为自己此刻还没初始化(2.1.1-alpha新增)。

3️⃣:关于触发条件

libevent提供了超时事件的宏定义，同时也提供了信号事件的宏定义，如下：
#define 宏|等价于
--|:--:
evtimer_new(base, callback, arg) | event_new((base), -1, 0, (callback), (arg))
evtimer_add(ev, tv) | event_add((ev),(tv))
evtimer_del(ev) | event_del(ev)
evtimer_pending(ev, tv_out) | event_pending((ev), EV_TIMEOUT, (tv_out))
evtimer_assign(event, base, callback, arg) | event_assign(event, base, -1, 0, callback, arg)
evsignal_new(base, signum, cb, arg) | event_new(base, signum, EV_SIGNAL\|EV_PERSIST, cb, arg)
evsignal_add(ev, tv) | event_add((ev),(tv))
evsignal_del(ev) | event_del(ev)
evsignal_pending(ev, what, tv_out) | event_pending((ev), (what), (tv_out))
evsignal_assign(event, base, signum, callback, arg) | event_assign(event, base, signum, EV_SIGNAL\|EV_PERSIST, callback, arg)

For Example:

定义一个挂起信号事件pointer，假设base已经创建，可以这样子绑定这个事件到base;
`struct event *evt = evsignal_new(base, SIGHUP, cb_hubfunc, NULL);`

PS:event_base的后端，除了**kqueue**，每个进程中只能有一个信号事件可以捕获信号，所以多个信号事件通常是不正常工作的，且不要设置**TIMEOUT**条件，*因为可能不被支持(待定)* 。

```
int event_assign(struct event *event, struct event_base *base, evutil_socket_t fd, short what,
                  void (*callback)(evutil_socket_t, short, void *), void *arg);
```
该函数比起event_new多了#1参数，用于将这个事件在一个大的结构体里初始化，

可以省去：1、对新event结构体指针取值的开销，2、内存分配器在堆分配小对象的开销(可能考虑碎片？)，3、缓存中没有事件时获取事件的开销，

最好别用这个接口，除非你的结构体能保证在不同OS下的空间都足够容纳这个event，可用*event_get_struct_event_size()* 获取事件结构体大小。

WARNING：不要对已经在base中未决的事件调用 *event_assign()* ，这可能会导致难以诊断的错误。如果已经初始化和成为未决的，调用 *event_assign()* 之前需要调用 *event_del()* 。

4️⃣:关于状态变更
函数名|原状态|终状态|描述
--|:--:|:--:|--
int event_add(struct event \*ev, const struct timeval \*tv); | 初始态 | 未决态 | 如为超时事件，tv内为超时所需要经过的时间而非时间戳
int event_del(struct event \*ev); | 未决态 | 非未决态 | 事件即使激活，此时删除事件，回调也不会执行
int event_remove_timer(struct event \*ev); | 未决态 | (非)未决态 | 仅对超时事件使用，其余无效；移除超时事件的tv，如果时间只有**EV_TIMEOUT**,那么它的效果等同于 *event_del()*
int event_priority_set(struct event \*event, int priority); | \ | \ | 设置事件的优先级
int event_get_priority(const struct event \*ev); | \ | \ | 返回事件的优先级(2.1.2-alpha新增)
int event_pending(const struct event \*ev, short what, struct timeval \*tv_out); | \ | \ | 函数确定给定的事件是否是未决的或者激活的。如果是，而且*what*参数设置了标志，则函数会返回事件当前为之未决或者激活的所有标志(flags)。如果提供了*tv_out*参数，并且*what*参数中设置了 **EV_TIMEOUT** 标志，而事件当前正因超时事件而未决或者激活，则*tv_out*会返回事件的超时值
evutil_socket_t event_get_fd(const struct event \*ev); | \ | \ | 返回事件配置的文件描述符
struct event_base \*event_get_base(const struct event \*ev); | \ | \ | 返回事件绑定的event_base
short event_get_events(const struct event \*ev); | \ | \ | 返回事件配置的条件
event_callback_fn event_get_callback(const struct event \*ev); | \ | \ | 返回事件的回调函数
void \*event_get_callback_arg(const struct event \*ev); | \ | \ | 返回事件回调函数及其参数指针
void event_get_assignment(const struct event \*event, struct event_base \*\*base_out, evutil_socket_t \*fd_out, short \*events_out, event_callback_fn \*callback_out, void \*\*arg_out); | \ | \ | 将对应参数赋予#1的event中

5️⃣:关于优化的一些内容

+ 可获取当前正在运行的事件，仅在当前base循环里有用！仅在base运行线程有用！

`struct event *event_base_get_running_event(struct event_base *base);`

+ 如果想要事件做一次就直接自动释放(那么当然不支持SIGNAL和PERSIST)，就可以用:

`int event_base_once(struct event_base *, evutil_socket_t, short, void (*)(evutil_socket_t, short, void *), void *, const struct timeval *);`

缺点是该事件无法用*event_del()* 删除，无法手动激活使用，如果希望能够取消事件，应该使用*event_new()* 或者*event_assign()* ，且2.1.2-alpha前，这类事件如果一直未触发，不会自动释放，会导致内存泄露，即便是该版本后能随base释放，如果它的回调函数关联到某些地方，仍然不会自动释放🈲

+ 极限情况下，如果想手动激活事件(不考虑这么变态的情况⚡):

`void event_active(struct event *ev, int what, short ncalls);`

+ 大量同时超时事件的触发(很多时间一样的计时器？项目无需求，略)

+ 从已清除的内存识别事件(从memset/bzero这些清楚的内存中获取事件，我是不敢用这种接口的，略)

---
### R5: Utility and portability functions (扩展和可移植函数)
1️⃣:基本定义
WIN32的socket比较特殊，所以timeval对其进行了处理
```
#ifdef WIN32
#define evutil_socket_t intptr_t
#else
#define evutil_socket_t int
#endif
```
还有一些整数类型，兼容性类型基本根据C99标准指定。

2️⃣:工具函数
+ ⏲️时间相关

函数|描述(这里的参数都是timeval结构)
--|:--:
#define evutil_timeradd(tvp, uvp, vvp) | vvp = tvp + uvp
#define evutil_timersub(tvp, uvp, vvp) | vvp = tvp - uvp
#define evutil_timerclear(tvp) | tvp清零
#define evutil_timerisset(tvp) | 判断tvp是否为0(空)
#define evutil_timercmp(tvp, uvp, cmp) | tvp cmp(>, >=, <, <=, ==, !=) uvp ? true : false
int evutil_gettimeofday(struct timeval \*tv, struct timezone \*tz) | #1是给tv赋值，#2暂时无用

+ 🧦套接字兼容(因为Windows不兼容Berkeley的套接字API，暂时忽略，项目在Linux开发，稳)

+ ✏️字符串相关

函数|描述
--|:--:
ev_int64_t evutil_strtoll(const char \*s, char \*\*endptr, int base) | 字符串中数字部分转为长整型返回，字符部分放入endptr
int evutil_snprintf(char \*buf, size_t buflen, const char \*format, ...) | 将可变参数按照format格式化写入buf中，超过buflen-1截断，末尾补\0
int evutil_vsnprintf(char \*buf, size_t buflen, const char \*format, va_list ap) | 同上，'v'一般表示安全版函数
int evutil_ascii_strcasecmp(const char \*str1, const char \*str2) | 比较字符串，大小写不敏感，返回同0，大于>0，小于<0
int evutil_ascii_strncasecmp(const char \*str1, const char \*str2, size_t n) | 同上，表示只比较前n个字符

+ 🗺️网络相关

函数|描述
--|:--:
const char \*evutil_inet_ntop(int af, const void \*src, char \*dst, size_t len) | 网络序转字节序 AF_INET||AF_INET6
int evutil_inet_pton(int af, const char \*src, void \*dst) | 字节序转网络序
int evutil_parse_sockaddr_port(const char \*str, struct sockaddr \*out, int \*outlen) | 解析来自str的地址，将结果写入到out中
int evutil_sockaddr_cmp(const struct sockaddr \*sa1, const struct sockaddr \*sa2, int include_port) | 比较两个地址，如果sa1在sa2前面，返回负数；相等返回0；否则返回正数

+ 🥕其他(offsetof宏，安全随机数发生器，略，不需要用这个库里面的)

---
### R6: Bufferevents: concepts and basics (*bufferevents*的概念与基础🌟)
> 概念：bufferevent由一个底层的传输端口（如套接字），一个读取缓冲区和一个写入缓冲区组成。(缓存I/O)
>
> 与通常的事件在底层传输端口已经就绪，可以读取或者写入的时候执行回调不同的是，bufferevent 在读取或者写入了足够量的数据之后调用用户提供的回调。
+ bufferevent分类：
   - socket-based bufferevents (基于套接字的 bufferevent)
      + 使用event_\*接口作为后端，通过底层流式套接字发送或者接收数据的bufferevent。
   - asynchronous-IO bufferevents (异步 IO bufferevent)
      + 使用Windows IOCP接口，通过底层流式套接字发送或者接收数据的bufferevent（试验特性！仅用于 Windows）
   - filtering bufferevents (过滤型 bufferevent)
      + 将数据传输到底层bufferevent对象之前，处理输入或者输出数据的bufferevent：比如说，为了压缩或者转换数据。
   - paired bufferevents (成对的 bufferevent)
      + 相互传输数据的两个bufferevent。

#### 回调(Callbacks)与水位(watermarks)
> 每个bufferevent有2个数据相关的回调：读取回调，写入回调。
> 
> 从底层传输端口读取了任意量的数据后（默认不为0时），会调用读取回调；
> 向底层传输端口输出了足够量的数据后（默认为全部输出），会调用写入回调；
> 
> 调整读取水位，写入水位，可以覆盖默认的函数行为。

- 读取低水位：读取操作使得输入缓冲区的数据量在此级别或者更高时，读取回调将被调用。默认值为0，所以每个读取操作都会导致读取回调被调用。

- 读取高水位：输入缓冲区中的数据量达到此级别后，bufferevent 将停止读取，直到输入缓冲区中足够量的数据被抽取，使得数据量低于此级别。默认值是无限，所以永远不会因为输入缓冲区的大小而停止读取。

- 写入低水位：写入操作使得输出缓冲区的数据量达到或者低于此级别时，写入回调将被调用。默认值是0，所以只有输出缓冲区空的时候才会调用写入回调。

- 写入高水位：bufferevent 没有直接使用这个水位。它在 bufferevent 用作另外一个bufferevent 的底层传输端口时有特殊意义。请看后面关于过滤型 bufferevent 的介绍。

相应的，bufferevent也有标志位通知non-data-oriented(非面向数据)事件，有下列标志位：

标志|描述
--|:--
BEV_EVENT_READING | 读取操作时发生某事件，具体是哪种事件请看其他标志。
BEV_EVENT_WRITING | 写入操作时发生某事件，具体是哪种事件请看其他标志。
BEV_EVENT_ERROR | 操作时发生错误，用EVUTIL_SOCKET_ERROR()获取错误的更多信息。
BEV_EVENT_TIMEOUT | 发生超时。
BEV_EVENT_EOF | 遇到文件结束指示。
BEV_EVENT_CONNECTED | 请求的连接过程已经完成。

💡延迟调用：指回调不在条件达成立即调用，在依赖关系复杂时，进行排队调用可以确定回调安全进行。


##### 1️⃣创建bufferevent：其中fd是表示套接字的文件描述符，options是下面表格中任意标志。

`struct bufferevent *bufferevent_socket_new(struct event_base *base, evutil_socket_t fd, enum bufferevent_options options);`

选项|描述
--|:--
BEV_OPT_CLOSE_ON_FREE | 释放bufferevent时关闭底层传输端口。这将关闭底层套接字，释放底层bufferevent等。上文中提到，释放这些结构体全局不一定能释放干净，调用这个选项能够部分改善（待验证）
BEV_OPT_THREADSAFE | 自动为bufferevent分配锁，这样就可以安全地在多个线程中使用bufferevent。（看着就很强）
BEV_OPT_DEFER_CALLBACKS | 设置这个标志时，bufferevent延迟所有回调，如上所述。
BEV_OPT_UNLOCK_CALLBACKS | 默认情况下，如果设置bufferevent为线程安全的，则bufferevent会在调用用户提供的回调时进行锁定。设置这个选项会让libevent在执行回调的时候不进行锁定。

##### 2️⃣使用bufferevnet连接服务器：与标准调用的connect()参数几乎一致，参考UNP；值得注意的是，connect函数告知bufferevent连接未成功，需要等待accept。

`int bufferevent_socket_connect(struct bufferevent *bev, struct sockaddr *address, int addrlen);`

还可以连接到指定的主机:

`int bufferevent_socket_connect_hostname(struct bufferevent *bev, struct evdns_base *dns_base, int family, const char *hostname, int port);`

##### 3️⃣释放bufferevent：bufferevent内部具有引用计数，如果释放时还有未决的延迟回调，则在回调完成之bufferevent不会被删除
`void bufferevent_free(struct bufferevent *bev);`

##### 4️⃣回调函数的修改，获取：readcb、writecb、eventcb；要禁用回调，传递NULL作为修改参数。
```
typedef void (*bufferevent_data_cb)(struct bufferevent *bev, void *ctx); //读写回调函数定义
typedef void (*bufferevent_event_cb)(struct bufferevent *bev, short events, void *ctx); //事件回调函数定义
void bufferevent_setcb(struct bufferevent *bufev, bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg);
void bufferevent_getcb(struct bufferevent *bufev, bufferevent_data_cb *readcb_ptr, bufferevent_data_cb *writecb_ptr, bufferevent_event_cb *eventcb_ptr, void **cbarg_ptr);
//bufferevent_getcb(2.1.1-alpha新增)，将bufferevent当前cb指针赋值到对应的*xxxcb_ptr上，参数放入**cbarg_ptr

//控制bufferevent能否进行EV_READ，EV_WRITE
void bufferevent_enable(struct bufferevent *bufev, short events); //启用事件
void bufferevent_disable(struct bufferevent *bufev, short events); //正常情况下不要禁用事件，输出缓存无数据bufferevent会自动停止写入。
short bufferevent_get_enabled(struct bufferevent *bufev); //获取bufferevent当前启用的事件
```
##### 5️⃣缓冲区的操作：其实和系统调用read()，write()类似，数据类型为evbuffer，详见R7。
```
struct evbuffer *bufferevent_get_input(struct bufferevent *bufev); //获取输入缓冲区
struct evbuffer *bufferevent_get_output(struct bufferevent *bufev); //获取输出缓冲区
int bufferevent_write(struct bufferevent *bufev, const void *data, size_t size); //添加size字节数据到输出缓冲区末尾
int bufferevent_write_buffer(struct bufferevent *bufev, struct evbuffer *buf); //移除buf内所有内容，放置bufev末尾（移动）
size_t bufferevent_read(struct bufferevent *bufev, void *data, size_t size); //读取最多size字节数据，并存入data，返回实际读取字节数
int bufferevent_read_buffer(struct bufferevent *bufev, struct evbuffer *buf); //读取bufev所有内容，放入buf（移动）
//清空bufferevent，有数据清空1，无则0，错误-1。iotype从EV_READ/WRITE选择组合。
int bufferevent_flush(struct bufferevent *bufev, short iotype, enum bufferevent_flush_mode state);
enum bufferevent_flush_mode {
	BEV_NORMAL = 0, //通常在处理数据时设置这个state
	BEV_FLUSH = 1, /** want to checkpoint all data sent. */
	BEV_FINISHED = 2, //读到EOF或发送完数据
};
```
##### 6️⃣其他相关操作：

⏰超时回调：规定时间内没有读取或写入数据时候调用。只有在读取或者写入的时候才会计算超时，超时发生时，相应的读取/写入操作被禁止，回调调用。

`void bufferevent_set_timeouts(struct bufferevent *bufev, const struct timeval *timeout_read, const struct timeval *timeout_write);`

🧦仅能用于套接字bufferevent的函数：
```
优先级相关：
int bufferevent_priority_set(struct bufferevent *bufev, int pri); //设置优先级，回看 event_priority_set()
int bufferevent_get_priority(struct bufferevent *bufev); //获取当前bufev的优先级（2.1.2-alpha新增）
文件描述符：
int bufferevent_setfd(struct bufferevent *bufev, evutil_socket_t fd);
evutil_socket_t bufferevent_getfd(struct bufferevent *bufev);
获取event_base：
struct event_base *bufferevent_get_base(struct bufferevent *bev);
锁相关：
void bufferevent_lock(struct bufferevent *bufev); //如果没设置BEV_OPT_THREADSAFE标记，锁无效
void bufferevent_unlock(struct bufferevent *bufev);
其他：
struct bufferevent *bufferevent_get_underlying(struct bufferevent *bufev); //返回作为bufferevent底层传输端口的另一个bufferevent，有待研究。
```
---
### R6a: Bufferevents: advanced topics (*bufferevent*进阶使用)
> 入门请跳过此章节，本章主要描述bufferevent中那些平时不必须的高级特性

#### 1️⃣使用成对的bufferevent:(项目参考了这个概念，但未使用这个结构)
> 有时候网络程序需要与自身通信，但使用套接字会徒增消耗，所以我们可以创建一对成对的bufferevent。这样，写入到一个bufferevent的字节都被另一个接收(反过来也是)。

*1-1:* 创建一对pair，相互connect，**BEV_OPT_CLOSE_ON_FREE**无效、**BEV_OPT_DEFER_CALLBACKS**总是打开，其它选项照常。

`int bufferevent_pair_new(struct event_base *base, int options, struct bufferevent *pair[2]);`

*1-2:* 至于为什么延迟回调强制打开呢？通常一边会通知另一边回调调用执行，并且循环这个过程多次，如果不延迟回调，其它连接可能饿死(全部CPU用在了互相通知上)，并且要求回调函数是可重入(线程安全，重复执行不影响外部数据)的。

*1-3:* 支持*bufferevent_flush()* ，**BEV_NORMAL**或者**BEV_FLUSH**会强制要求所有相关数据传输到对端bufferevent中，**BEV_FINISHED**还会让对端的产生EOF事件。

💡PS：释放对中某一个bufferevent，另一个不会自动释放，也不会有EOF；只会connect断开，无法进行读写数据或产生事件了。

*1-4:* 获取pair中对端的bufferevent，若果对端还存在，且当前bev是在pair中，返回对端，否则NULL。

`struct bufferevent *bufferevent_pair_get_partner(struct bufferevent *bev)`

#### 2️⃣数据处理中间件——bufferevent过滤器：(了解概念，暂用不上这种功能)
> 所有通过底层bufferevent接收的数据在到达过滤bufferevent之前都会经过“输入”过滤器的转换；
> 
> 所有通过底层bufferevent发送的数据在被发送到底层bufferevent之前都会经过“输出”过滤器的转换。

#### 3️⃣速率限制，控制bufferevent的传输带宽：(了解概念，暂用不上这种功能)
> 要创建速率限制组，使用一个 event_base 和一个已经初始化的 ev_token_bucket_cfg 作为参数调用*bufferevent_rate_limit_group_new* 函数 。 
> 
> 使用*bufferevent_add_to_rate_limit_group* 将bufferevent添加到组中； 
> 
> 使用*bufferevent_remove_from_rate_limit_group* 从组中删除bufferevent。
>
> 可以通过修改组的配置更改速率限制，速率限制实现仅计算TCP分组传输的数据。

#### 4️⃣安全传输，使用OpenSSL：(⚠️项目使用了SSL，主要是HTTP通信时保护数据报) 
> 因为很多应用不需要或者不想链接OpenSSL，这部分功能在单独的**libevent_openssl** 库中实现。未来版本可能会添加其他SSL/TLS库，如NSS/GnuTLS，当前只有OpenSSL。

*4-1:* SSL对象及其状态，SSL对象有3种状态，在openssl_bufferevent对象CLOSE_ON_FREE时，对应SSL对象也同时关闭。
```
enum bufferevent_ssl_state {
        BUFFEREVENT_SSL_OPEN = 0, //SSL握手已经完成
        BUFFEREVENT_SSL_CONNECTING = 1, //SSL当前作为客户端在进行协商
        BUFFEREVENT_SSL_ACCEPTING = 2, //SSL当前作为服务器在进行协商
};
```

*4-2:* 基于过滤器和套接字的bufferevent。

`struct bufferevent *bufferevent_openssl_filter_new(struct event_base *base, struct bufferevent *underlying, SSL *ssl, enum bufferevent_ssl_state state, int options);`

`struct bufferevent *bufferevent_openssl_socket_new(struct event_base *base, evutil_socket_t fd, SSL *ssl, enum bufferevent_ssl_state state, int options);`

💡PS：创建基于套接字的bufferevent时，如果SSL对象已经设置了套接字，fd只要传递-1就可以。也可以随后调用*bufferevent_setfd()* 来设置。

*4-3:* SSL相关的一些操作。
```
SSL *bufferevent_openssl_get_ssl(struct bufferevent *bev); //返回使用的SSL对象
unsigned long bufferevent_get_openssl_error(struct bufferevent *bev); //返回第一个未决的OpenSSLU错误
int bufferevent_ssl_renegotiate(struct bufferevent *bev); //要求SSL重新协商(有些 SSL 版本具有与重新协商相关的安全问题)

所有SSL协议的好版本（比如SSLv3，以及所有的TLS版本）都支持关闭认证操作，这可以在底层缓冲区中区分出到底是偶然的关闭还是恶意的终止。
默认情况下，将除了正确关闭之外的所有关闭都视为链接错误。如果allow_dirty_shutdown标志为1，则将连接中的关闭视为 BEV_EVENT_EOF。
int bufferevent_openssl_get_allow_dirty_shutdown(struct bufferevent *bev); //2.1.1-alpha新增
void bufferevent_openssl_set_allow_dirty_shutdown(struct bufferevent *bev, int allow_dirty_shutdown); //2.1.1-alpha新增
```

---
### R7: Evbuffers: utility functionality for buffered IO (*evbuffer*:缓存IO的高效且实用的方式)
> libevent 的 evbuffer 实现了为向后面添加数据和从前面移除数据而优化的字节队列。简单理解，就是SOCKET编程中常见的buff[]。

#### 1️⃣创建和释放evbuffer

`struct evbuffer *evbuffer_new(void); //创建evbuffer`

`void evbuffer_free(struct evbuffer *buf); //释放evbuffer`

#### 2️⃣使用evbuffer
*2-1:* 线程安全，由于是缓冲区，多个线程同时操作如果不加锁肯定是不安全的，所以有配套的方法

`int evbuffer_enable_locking(struct evbuffer *buf, void *lock); //lock参数为空时，使用evthread_set_lock_creation_callback创建锁`

`void evbuffer_lock(struct evbuffer *buf); //加锁`

`void evbuffer_unlock(struct evbuffer *buf); //释放锁`

*2-2:* 获取大小

`size_t evbuffer_get_length(const struct evbuffer *buf); //返回buf中存储的字节数`

`size_t evbuffer_get_contiguous_space(const struct evbuffer *buf); //获取buf中第一块内存连续地存储前面的字节数。`

*2-3:* 缓存的增删改查
```
struct evbuffer_iovec {
        void *iov_base;
        size_t iov_len;
}; //结构比较迷惑，项目未用到，暂略

➕尾增：
int evbuffer_add(struct evbuffer *buf, const void *data, size_t datlen); //向buf尾添加data中datlen字节数据
int evbuffer_add_printf(struct evbuffer *buf, const char *fmt, ...); //向buf尾格式化添加数据到
int evbuffer_add_vprintf(struct evbuffer *buf, const char *fmt, va_list ap); //参照vprintf

➕前增：（千万不要对bufferevent共享的evbuffer使用）
int evbuffer_prepend(struct evbuffer *buf, const void *data, size_t size); //除向buf前增加，同evbuffer_add
int evbuffer_prepend_buffer(struct evbuffer *dst, struct evbuffer* src); //除移动到dst前，同evbuffer_add_buffer

➕直接增，无需const void *data：
/* 函数会扩展缓冲区以至少提供size字节的空间，到扩展空间的指针存放在vec，n_vecs是vec长度*/
int evbuffer_reserve_space(struct evbuffer *buf, ev_ssize_t size, struct evbuffer_iovec *vec, int n_vecs);
/* 写入iovec的数据不会在buf，直至调用commit */
int evbuffer_commit_space(struct evbuffer *buf, struct evbuffer_iovec *vec, int n_vecs);

➖前删：
int evbuffer_drain(struct evbuffer *buf, size_t len); //从buf前开始移除len字节
int evbuffer_remove(struct evbuffer *buf, void *data, size_t datlen); //同上，只不过将移除的数据复制进data

🌌改：
/* 修改缓冲区最后一块或添加新块，使其能够容纳datlen且不需分配内存 */
int evbuffer_expand(struct evbuffer *buf, size_t datlen);
/* 线性化buf前面size字节，如果存放内存不连续，可能需要复制或移动，非常耗时 */
unsigned char *evbuffer_pullup(struct evbuffer *buf, ev_ssize_t size);

🚀移动：
int evbuffer_add_buffer(struct evbuffer *dst, struct evbuffer *src); //src数据全部移到dst末尾，成功0否则-1
int evbuffer_remove_buffer(struct evbuffer *src, struct evbuffer *dst, size_t datlen); //移动src中datlen字节到dst，返回实际移动字节数
/* 嫌evbuffer_copyout()慢, 用evbuffer_peek()代替 */
ev_ssize_t evbuffer_copyout(struct evbuffer *buf, void *data, size_t datlen); //类似evbuffer_remove，但不移除数据，而是复制到data中
/* 从buf前方pos指针所指位置开始，复制datlen字节数据到data_out中 */
ev_ssize_t evbuffer_copyout_from(struct evbuffer *buf, const struct evbuffer_ptr *pos, void *data_out, size_t datlen);

🔍查：
/* 查找len个字符的字符串what，如果提供start，从start位置开始查找，返回指针指向what的位置 */
struct evbuffer_ptr evbuffer_search(struct evbuffer *buffer, const char *what, size_t len, const struct evbuffer_ptr *start);
/* 类似上面，但只查找end位置前的结果 */
struct evbuffer_ptr evbuffer_search_range(struct evbuffer *buffer, const char *what, size_t len, const struct evbuffer_ptr *start, const struct evbuffer_ptr *end);
/* 像evbuffer_readln()一样检测行结束，返回指向行结束符的ptr */
struct evbuffer_ptr evbuffer_search_eol(struct evbuffer *buffer, struct evbuffer_ptr *start, size_t *eol_len_out, enum evbuffer_eol_style eol_style);
/* 通过vec_out数组，数组的长度是n_vec。函数会让每个结构体包含指向evbuffer内部内存块的指针(iov_base)和块中数据长度 */
int evbuffer_peek(struct evbuffer *buffer, ev_ssize_t len, struct evbuffer_ptr *start_at, struct evbuffer_iovec *vec_out, int n_vec);
💡PS：多线程编程时，用peek先上锁，使用完返回内容后要解锁
```
*2-4:* 面向行的输入，从evbuffer前面取出一行，用一个新分配的空字符结束的字符串返回这一行

`char *evbuffer_readln(struct evbuffer *buffer, size_t *n_read_out, enum evbuffer_eol_style eol_style);`
eol_style | 描述
-- | --
EVBUFFER_EOL_LF | 行尾是单个换行符（也就是\n，ASCII 值是0x0A）
EVBUFFER_EOL_CRLF_STRICT | 行尾是一个回车符，后随一个换行符（也就是\r\n，ASCII值是0x0D 0x0A）
EVBUFFER_EOL_CRLF| 行尾是一个可选的回车，后随一个换行符（也就是说，可以是\r\n 或者\n）。这种格式对于解析基于文本的互联网协议很有用，因为标准通常要求\r\n 的行结束符，而不遵循标准的客户端有时候只使用\n。
EVBUFFER_EOL_ANY | 行尾是任意数量、任意次序的回车和换行符。这种格式不是特别有用。它的存在主要是为了向后兼容
EVBUFFER_EOL_NUL | 行尾是一个字节的\0(也就是ASCII的NUL)2.1.1-alpha新增

*2-5:* 网络IO
✍️

`int evbuffer_write(struct evbuffer *buffer, evutil_socket_t fd); //尽量向fd写入buf所有内容`

`int evbuffer_write_atmost(struct evbuffer *buffer, evutil_socket_t fd, ev_ssize_t howmuch); //将buffer前面至多howmuch字节写入到fd，若-1则==write`

📖

`int evbuffer_read(struct evbuffer *buffer, evutil_socket_t fd, int howmuch); //从fd读取至多howmuch字节到buffer`

*2-6:* 回调相关，在向evbuffer添加/移除数据时，回调执行。
```
struct evbuffer_cb_info {
        size_t orig_size; //缓冲区改变大小前的字节数
        size_t n_added; //添加字节数
        size_t n_deleted; //移除字节数
};
回调函数定义:
typedef void (*evbuffer_cb_func)(struct evbuffer *buffer, const struct evbuffer_cb_info *info, void *arg); 
增加回调：
struct evbuffer_cb_entry;
struct evbuffer_cb_entry *evbuffer_add_cb(struct evbuffer *buffer, evbuffer_cb_func cb, void *cbarg);
删除回调：
int evbuffer_remove_cb_entry(struct evbuffer *buffer, struct evbuffer_cb_entry *ent);
int evbuffer_remove_cb(struct evbuffer *buffer, evbuffer_cb_func cb, void *cbarg);
回调标志位： #define EVBUFFER_CB_ENABLED 1 清楚这个标志则不会触发回调
int evbuffer_cb_set_flags(struct evbuffer *buffer,  struct evbuffer_cb_entry *cb, ev_uint32_t flags);
int evbuffer_cb_clear_flags(struct evbuffer *buffer, struct evbuffer_cb_entry *cb, ev_uint32_t flags);
延迟回调：如果回调被延迟，则最终执行时，它可能是多个操作结果的总和；但能避免栈崩溃
int evbuffer_defer_callbacks(struct evbuffer *buffer, struct event_base *base);
```

*2-7:* 性能调优与其它

`int evbuffer_add_reference(struct evbuffer *outbuf, const void *data, size_t datlen, evbuffer_ref_cleanup_cb cleanupfn, void *extra);`

这个函数通过**引用**向 evbuffer 末尾添加一段数据。不会进行复制：evbuffer只会存储一个到data处的datlen字节的指针。

`typedef void (*evbuffer_ref_cleanup_cb)(const void *data, size_t datalen, void *extra);`

因此，在evbuffer使用这个指针期间，必须保持指针是有效的。evbuffer会在不再需要这部分数据的时候调用用户提供的*cleanupfn* 。

通过文件写evbuffer, add_by_file

`int evbuffer_add_file(struct evbuffer *output, int fd, ev_off_t offset, size_t length); //将fd中offset开始处length字节添加到output末尾`

---
### R8: Connection listeners: accepting TCP connections (监听并接受TCP连接)
### R9: DNS for Libevent (使用libevent的DNS功能)
