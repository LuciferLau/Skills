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

[R7：*bufferevent*进阶使用](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r7-bufferevents-advanced-topics-bufferevent%E8%BF%9B%E9%98%B6%E4%BD%BF%E7%94%A8)

[R8：*evbuffer*：缓存IO的高效且实用的方式](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r8-evbuffers-utility-functionality-for-buffered-io-evbuffer%E7%BC%93%E5%AD%98io%E7%9A%84%E9%AB%98%E6%95%88%E4%B8%94%E5%AE%9E%E7%94%A8%E7%9A%84%E6%96%B9%E5%BC%8F)

[R9：监听并接受TCP连接](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r9-connection-listeners-accepting-tcp-connections-%E7%9B%91%E5%90%AC%E5%B9%B6%E6%8E%A5%E5%8F%97tcp%E8%BF%9E%E6%8E%A5)

[R10：使用libevent的DNS功能](https://github.com/LuciferLau/Skills/blob/master/works/Libevent.md#r10-dns-for-libevent-%E4%BD%BF%E7%94%A8libevent%E7%9A%84dns%E5%8A%9F%E8%83%BD)

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
   - socket-based bufferevents 基于套接字的 bufferevent : 使用event_\*接口作为后端，通过底层流式套接字发送或者接收数据的bufferevent。
   - asynchronous-IO bufferevents 异步 IO bufferevent ：使用Windows IOCP接口，通过底层流式套接字发送或者接收数据的bufferevent（试验特性！仅用于 Windows）
   - filtering bufferevents 过滤型 bufferevent ：将数据传输到底层bufferevent对象之前，处理输入或者输出数据的bufferevent：比如说，为了压缩或者转换数据。
   - paired bufferevents 成对的 bufferevent : 相互传输数据的两个bufferevent。

### R7: Bufferevents: advanced topics (*bufferevent*进阶使用)
### R8: Evbuffers: utility functionality for buffered IO (*evbuffer*:缓存IO的高效且实用的方式)
### R9: Connection listeners: accepting TCP connections (监听并接受TCP连接)
### R10: DNS for Libevent (使用libevent的DNS功能)
