# LIBEVENT学习笔记
> 好久没在GIT上面写点东西了，菜鸡的主动学习能力不足，没多久就到了自己的性能瓶颈，适逢最近有新大佬带引擎方面的知识，于是开始学习libevent相关的知识。

![](https://camo.githubusercontent.com/911381096c669dd9fef3699539e6b9cf38b0ef4aac3a546f76741bc919d1bd9a/68747470733a2f2f6c69626576656e742e6f72672f6c69626576656e74332e706e67)

---

🤐顺带吐槽下，项目用的redmine感觉就是开单改进度关单的工具，公司也没有用git做项目管理，
最近倒是多了个叫TAPD的玩意，估计是企鹅那边的需求吧，项目管理真没劲👎

---
## 接下来会跟着官方文档步步走
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
### R2: Getting an event_base (使用event_base,重要基础)
当然，可以用简单的方式new一个最普通的base出来；
`struct event_base *event_base_new(void);`
也可以使用配置文件new一个你想要的base出来；
`struct event_base *event_base_new_with_config(const struct event_config *cfg);`
这个配置创建也有它的一套讲究，首先是先创建一个空的config；
`struct event_config *event_config_new(void);`
当然也有对应的析构函数;
`void event_config_free(struct event_config *cfg);`
然后就是配置这个配置了，说起来怪怪的🙇‍♂️
```
你可以选择你不想要的后端:(select, poll, epoll, kqueue, devpoll, evport, win32)
int event_config_avoid_method(struct event_config *cfg, const char *method);
不想用它就把它avoid掉吧，如果想知道什么后端可以用，就使用：
const char **event_get_supported_methods(void);
函数返回一个指针，指向 libevent 支持的方法名字数组，最后一个元素为NULL。

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

用配置虽然很爽，但你的OS不一定支持这些配置，如果不能满足你的要求，event_base_new_with_config只能返回NULL。
```

### R3: Running an event loop (使用事件循环)
### R4: Working with events (与事件一起工作)
### R5: Utility and portability functions (扩展和可移植函数)
### R6: Bufferevents: concepts and basics (*bufferevents*的概念与基础)
### R6: Bufferevents: advanced topics (*bufferevent*进阶使用)
### R7: Evbuffers: utility functionality for buffered IO (*evbuffer*:缓存IO的高效且实用的方式)
### R8: Connection listeners: accepting TCP connections (监听并接受TCP连接)
### R9: DNS for Libevent (使用libevent的DNS功能)
