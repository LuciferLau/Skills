# LIBEVENT学习笔记
> 好久没在GIT上面写点东西了，菜鸡的主动学习能力不足，没多久就到了自己的性能瓶颈，适逢最近有新大佬带引擎方面的知识，于是开始学习libevent相关的知识。
---

顺带吐槽下，项目用的redmine感觉就是开单改进度关单的工具，公司也没有用git做项目管理，
最近倒是多了个叫TAPD的玩意，估计是企鹅那边的需求吧，项目管理真没劲。

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
bufferevent|为 libevent 基于事件的核心提供使用更方便的封装。除了通知程序套接字已经准备好读写之外，还让程序可以请求缓冲的读写操作，可以知道何时IO已经真正发生。（bufferevent接口有多个后
端，可以采用系统能够提供的更快的非阻塞 IO 方式，如 Windows 中的 IOCP。）
evbuffer|在 bufferevent 层之下实现了缓冲功能，并且提供了方便有效的访问函数。
evhttp|一个简单的 HTTP 客户端/服务器实现。
evdns|一个简单的 DNS 客户端/服务器实现。
evrpc|一个简单的 RPC 实现。
#### libevent中的库
库名|描述
--|:--:
ibevent_core|所有核心的事件和缓冲功能，包含了所有的 event_base、evbuffer、bufferevent 和工具函数。
ibevent_extra|定义了程序可能需要，也可能不需要的协议特定功能，包括 HTTP、DNS 和 RPC。
libevent|这个库因为历史原因而存在，它包含 libevent_core 和 libevent_extra 的内容。不应该使用这个库，未来版本的 libevent 可能去掉这个库。
---
某些平台上可能安装下列库
libevent_pthreads|添加基于 pthread 可移植线程库的线程和锁定实现。它独立于libevent_core，这样程序使用 libevent 时就不需要链接到 pthread，除非是以多线程方式使用 libevent。
libevent_openssl|这个库为使用 bufferevent 和 OpenSSL 进行加密的通信提供支持。它独立于 libevent_core，这样程序使用 libevent 时就不需要链接到 OpenSSL，除非是进行加密通信。
### R1: Setting up the Libevent library (设置libevent)
### R2: Getting an event_base (使用event_base)
### R3: Running an event loop (使用事件循环)
### R4: Working with events (与事件一起工作)
### R5: Utility and portability functions (扩展和可移植函数)
### R6: Bufferevents: concepts and basics (*bufferevents*的概念与基础)
### R6: Bufferevents: advanced topics (*bufferevent*进阶使用)
### R7: Evbuffers: utility functionality for buffered IO (*evbuffer*:缓存IO的高效且实用的方式)
### R8: Connection listeners: accepting TCP connections (监听并接受TCP连接)
### R9: DNS for Libevent (使用libevent的DNS功能)
