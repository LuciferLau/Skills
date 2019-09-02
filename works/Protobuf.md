# Protocol Buffers
> 官方定义：Protocol buffers are Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data – think XML, but smaller, faster, and simpler. 
## 工作上会用这个工具进行前后端的数据交互，因此记录下学习过程
标题是protobuf全名，文件后缀是.proto，是谷歌发明的一款十分强大的工具  
它有平台无关，语言无关，优良扩展性，且解析速度快，操作序列化（Serialization）数据简单轻量。  
    
    1、协议号
目前protobuf最新的版本是proto3，与proto2的旧API不能完全兼容。
一般的，会在文件的头部进行协议版本的声明，若无，则默认为proto2，代码格式如下：  
' syntax = "proto3" '
