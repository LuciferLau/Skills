# Protocol Buffers
> 官方定义：Protocol buffers are Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data – think XML, but smaller, faster, and simpler. 
## 工作上会用这个工具进行前后端的数据交互，因此记录下学习过程
标题是protobuf全名，文件后缀是.proto，是谷歌发明的一款十分强大的工具  
它有平台无关，语言无关，优良扩展性，且解析速度快，操作序列化（Serialization）数据简单轻量。  
    
### 1、协议号
目前protobuf最新的版本是proto3，与proto2的旧API不能完全兼容。
一般的，会在文件的头部进行协议版本的声明，若无，则默认为proto2，代码格式如下：  
```
syntax = "proto3"; //使用proto3
syntax = "proto2"; //使用proto2，default
```
### 2、字段
每个字段（Field）都有相关的编号，1到15中字段编号需用1个byte编码，16到2047中字段需用2个byte编码，所以前者一般用于保存非常频繁出现的消息；同时19000~19999中的编号是保留的，不可使用。  
#### 保留字段和编号  
假如某次更新中某些字段或编号被遗弃，不再使用，可以将它们指定为reserved的，保证将来任何用户不会使用到它们。  
```
enum Foo {
    reserved 2, 15, 9 to 11, 40 to max; //注：保留字段和编码必须分开写
    reserved "FOO", "BAR";
}
```
#### 字段修饰  
required：表示这个字段有且只有1个。  
optional：表示这个字段可以为0或1个。
repeated：表示这个字段可以为0或N个。
```
message myMSG{
    required int32 id = 1;
    required string name = 2;
    optional string email = 3; [defaukt = 0]
    repeated int32 something = 4; 
}
```
> Required Is Forever You should be very careful about marking fields as required.   
If at some point you wish to stop writing or sending a required field, it will be problematic to change the field to an optional field – old readers will consider messages without this field to be incomplete and may reject or drop them unintentionally.   
You should consider writing application-specific custom validation routines for your buffers instead.   
Some engineers at Google have come to the conclusion that using required does more harm than good; they prefer to use only optional and repeated. However,   
this view is not universal.
官方对于require修饰符的一些建议，大概意思就是慎用require，除非你能确保不修改这个字段，否则容易导致兼容性错误↑
test
