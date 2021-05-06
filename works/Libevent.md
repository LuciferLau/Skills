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
