# MongoDB 第二个接触的NoSQL
> 菜鸟教程定义：MongoDB 是一个基于分布式文件存储的数据库。  
由 C++ 语言编写。旨在为 WEB 应用提供可扩展的高性能数据存储解决方案。  
MongoDB 是一个介于关系数据库和非关系数据库之间的产品，是非关系数据库当中功能最丰富，最像关系数据库的。
## 学到的Redis没能在第一份工作用上，但NoSQL的思想应该是相近的。
### MongoDB术语（后者为SQL术语）
数据库：database（database）  
数据表：collection（table）  
行：document（row）  
字段：field（column）  
索引：index（index）  
连接：不支持（join）  
主键：primary key（primary key）  
### 数据类型
Type | Description
:-:|-
String	|字符串。存储数据常用的数据类型。在 MongoDB 中，UTF-8 编码的字符串才是合法的。
Integer	|整型数值。用于存储数值。根据你所采用的服务器，可分为 32 位或 64 位。
Boolean	|布尔值。用于存储布尔值（真/假）。
Double	|双精度浮点值。用于存储浮点值。
Min/Max keys	|将一个值与 BSON（二进制的 JSON）元素的最低值和最高值相对比。
Array	|用于将数组或列表或多个值存储为一个键。
Timestamp	|时间戳。记录文档修改或添加的具体时间。
Object	|用于内嵌文档。
Null	|用于创建空值。
Symbol	|符号。该数据类型基本上等同于字符串类型，但不同的是，它一般用于采用特殊符号类型的语言。
Date	|日期时间。用 UNIX 时间格式来存储当前日期或时间。你可以指定自己的日期时间：创建 Date 对象，传入年月日信息。
Object ID	|对象 ID。用于创建文档的 ID。
Binary Data	|二进制数据。用于存储二进制数据。
Code	|代码类型。用于在文档中存储 JavaScript 代码。
Regular expression	|正则表达式类
### 常用方法
> 额外的一个知识点：BSON是一种类json的一种二进制形式的存储格式，简称Binary JSON，它和JSON一样，支持内嵌的文档对象和数组对象，但是BSON有JSON没有的一些数据类型，如Date和BinData类型。
ISODate()格林尼治时间GMT+0，Date()JS中的日期方法。  
