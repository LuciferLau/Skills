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
> BSON：是一种类json的一种二进制形式的存储格式，简称Binary JSON，它和JSON一样，支持内嵌的文档对象和数组对象，但是BSON有JSON没有的一些数据类型，如Date和BinData类型。是MongoDB主要的数据类型。  

ISODate()格林尼治时间GMT+0，Date()JS中的日期方法。  
#### 对数据库的操作
方法|介绍
:-:|-
use db | 切换数据库，切换到不存在的数据库中并插入数据，相当于创建新库
show db | 显示所有数据库
db.dropDatabase() | 删库
#### 对集合(表)的操作
方法|介绍
:-:|-
show | 查看所有集合
db.cerateCollection(name,options) |name：集合名称。options：(bool capped, bool autoIndexId, number size, numebr max)
db.COLLECTION_NAME.insert(...) | 和数据库一样，可以通过插入数据自动创建COLLECTION_NAME这个集合
db.COLLECTION_NAME.drop() | 删除集合
db.COLLECTION_NAME.update(query, update, (upsert, multi, writeConcern)) | 更新集合，query是查询条件；update类似set子句；后三个参数可选：upsert默认为false，不存在update的记录则不插入；multi默认为false，只更新符合条件的第一条数据，否则更新全部；最后一个是抛出异常的级别
db.COLLECTION_NAME.save(document, writeConcern) | 用传入文档替换已有文档
db.COLLECTION_NAME.remove(query, justOne, writeConcern) | 删除文档，query是查询条件，justOne默认为false，和multi一个意思
db.COLLECTION_NAME.find(query, projection) | 查询文档，query是查询条件，projection是指定返回的键
db.COLLECTION_NAME.find().pretty() | 格式化输出文档
#### 关系运算
操作|格式|MongoDB|RDBMS
:-:|-|-|-
等于	{<key>:<value>}	db.col.find({"by":"菜鸟教程"}).pretty()	where by = '菜鸟教程'
小于	{<key>:{$lt:<value>}}	db.col.find({"likes":{$lt:50}}).pretty()	where likes < 50
小于或等于	{<key>:{$lte:<value>}}	db.col.find({"likes":{$lte:50}}).pretty()	where likes <= 50
大于	{<key>:{$gt:<value>}}	db.col.find({"likes":{$gt:50}}).pretty()	where likes > 50
大于或等于	{<key>:{$gte:<value>}}	db.col.find({"likes":{$gte:50}}).pretty()	where likes >= 50
不等于	{<key>:{$ne:<value>}}	db.col.find({"likes":{$ne:50}}).pretty()	where likes != 50
