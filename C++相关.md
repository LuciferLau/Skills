# C++常见问题汇总
•	指针和引用的区别  
指针在创建的时候需要分配内存空间，引用不用，因为引用指向的对象是已经存在的。  
正因如此，引用在创建的时候就要指定引用对象，并且无法修改它所引用的对象，而指针不同，指针可以修改他所指的对象，并且可以指向空。  

•	堆和栈的区别  
堆是由程序员动态分配的内存空间，栈是存放函数局部变量和参数的内存空间。
堆的内存地址生长方向由低到高，栈的内存地址生长方向由高到低。  
堆的内存都是程序员动态分配并回收的，栈可动可静，但它的动态是OS来分配并回收，程序员不干涉。  

•	new和delete是如何实现的，new 与 malloc的异同处  
异：new和delete都是运算符，所以是可运算符重载的（oprator new），底层是基于malloc和free函数（STL里的东西）实现的，
通过内存池技术(memory pool)，将不同大小的内存用链表分成块，再根据申请内存的大小从不同的位置拿出一块大小最合适的内存给新的对象。  
malloc需要手动计算需要申请的内存并填入参数，new不需要，自动根据类型计算。  
malloc返回(void*)万能指针，所以需要强转回自己需要的类型，new直接按你申请的类型返回。   
同：是都在堆上分配内存，且都要配套使用。不同点是new可以重载，且有new[]专门给数组分配内存，new不一定要指定内存大小。   

**TIPS**：new[]和delete[]必须配套使用（代码规范来说），实际上，new[]基本类型时，delete也可以正确释放，
但如果都有自定义析构函数的类，ERROR！必须用delete[]才能正确释放。
（因为不连续，只释放数组头指针指向的内存？待求证，在MinGW下实验得出临时结论，实际上应该是未定义行为）
```
int* A = new int[3]();
delete[] A; // OK
//delete A; // still OK

class myClass {
public:
  myClass(){ printf("%s\n", "myClass::myClass()"); }
  ~myClass(){ printf("%s\n", "myClass::~myClass()"); } 
}
myClass* B = new myClass[6]();
//delete B; // unpredictable behaviour, depend on os/compiler
delete[] B; // much better choice
```

**TIPS2**：
观察以下代码，差距是什么？  
int* arr = new int[3];  //not_init 11735192,11735896,0  
int* arr = new int[3]\(); //0,0,0  
后者多了括号，作用是初始化数组内int的默认值，否者该地址的值是未定义的。

•	C和C++的区别  
C++是面向对象的，C是面向过程的。C++相较于C最大的区别就是加入了继承性与多态性。  
继承通过类与对象来实现，多态则通过类中的虚函数来实现，虚函数实现了动态联编，与静态联编共同形成了多态。  
同时C++还加入了STL，异常处理，泛型编程，运算符重载，命名空间，以及lambda等新特性。  

•	Struct和class的区别  
在定义类的时候，struct默认成员是public的，class默认成员是private的。  

•	define和const的区别（编译阶段、安全性、内存占用等）   
①：作用时机，define在预处理阶段替换代码段，const在编译阶段和程序运行阶段生效;   
（以gcc为例，使用g++ -E x.cc -o x.i，可生成预处理后的文件）  
②：define无语法检查，const会进行语法检测;  
③：define无强类型，const有，#define MAX 123可以，但const (int) MAX 123肯定不行;  
因此，const可以用于修饰变量，形参，类的成员函数，而define不行。  
④：define可以用于#ifdef #endif这类防止头文件重复引用的操作，const不行;  
⑤：define因为只是代码段替换，不存在占用内存的概念，而const修饰的变量会占用内存中的常量存储区。  
（当然运行时函数内的局部const也可能存在于栈中）  

•	在C++中const和static的用法（定义，用途）  
const和static都可以修饰函数和变量，static将变量/函数声明为静态的，存储在静态存储区，且只在本文件中看见。  
const修饰变量/指针，表示这是一个常量；在类内修饰函数，表示这个函数不会改变类的数据成员。  

•	const和static在类中使用的注意事项（定义、初始化和使用）  
const可以修饰函数，也可以定义常量，一般在构造函数的初始化列表设置初始值。  
static将函数/变量声明为静态的，只在本文件可见，static变量需要在类外初始化。  

•	C++中的const类成员函数（用法和意义），以及和非const成员函数的区别  
const类成员函数表示这个函数不对类的成员变量进行修改。  

•	C++的顶层const和底层const  
声明指向常量的指针也就是底层const，声明常量指针就是顶层const。  
底层：const int* elem。 顶层：int* const elem。  
在作为形参传入时，顶层const属性时常会被抛弃，使用auto关键字的时候也是。  

•	final和override关键字  
类的声明后加入final关键字说明该类不可以被继承，不可以成为父类。  
final修饰函数同样可以使子类不可以覆写这个函数。  
函数后面加入override，说明该函数是覆写了父类中的虚函数，实现不同的功能。  

•	拷贝初始化和直接初始化，初始化和赋值的区别  
两个问题可整合成初始化和赋值的区别，拷贝初始化先用默认构造函数创建一个类的对象，再给他赋值，  
直接初始化就是在创建这个对象的同时给他赋值，虽然达到的效果是一致的，但是后者要更加节省时间，效率更高。  

•	extern "C"的用法  
extern“C”声明了接下来的函数或变量是以C的语法格式定义的，以C的方式编译和链接。  
因为C语言没有重载函数，C++中同一个函数参数不同，在编译器中记录的名字不尽相同，所以要通过extern“C”关键字告诉编译器这是一个C风格的函数/变量。  

•	模板函数和模板类的特例化  
以template<typename T>的格式，声明一个模板函数。  
模板特例化以template<>开头，类模板特例化则把里面原来T代表的类换成特例化的那个类名。也可以对类内部分的函数进行特例化，写法相同。  
 
•	C++的STL源码（这个系列也很重要，建议侯捷老师的STL源码剖析书籍与视频），其中包括内存池机制，各种容器的底层实现机制，算法的实现原理等）  
tobedone   

•	STL源码中的hashtable的实现  
哈希表在stl中底层是以数组和链表实现的，使用了开链法（链接法），在哈希冲突发生的时候，使用拉链法  
将该元素与会产生冲突的元素放在同一个链表里面，然后将这条链表放在数组中这个关键字对应的位置。  

•	STL中unordered_map和map的区别和应用场景  
map底层由红黑树实现，unordered_map底层由哈希表实现。大量无序的数据插入和查找的时候，哈希表占优势；而在操作有序的动态数据时，红黑树占优。  

•	STL中vector的实现  
vector底层数据结构是数组，内存是连续的。push_back的时候如果容量已满，就申请一块2倍大小的内存，将原来的内容拷贝过去，旧的内存释放。  

•	STL容器的几种迭代器以及对应的容器（输入迭代器，输出迭代器，前向迭代器，双向迭代器，随机访问迭代器）  
顺序容器：vector，deque是随机访问迭代器；list是双向迭代器；    
容器适配器：stack，queue，priority_queue没有迭代器；  
关联容器：set，map，multiset，multimap是双向迭代器；  
前向迭代器：unordered_set，unordered_map，unordered_multiset，unordered_multimap。  

•	STL中的traits技法（泛型编程获取未知类型时使用）tobedone  
type_traits，iterator_traits，char traits，allocator_traits，pointer_traits，array_traits  

•	vector使用的注意点及其原因，频繁对vector调用push_back()对性能的影响和原因。  
Vector本质上是数组，所以在除了尾部添加元素，都会有比较大的开销（全体元素后移）。  
调用push_back()会使容器频繁改变他的容量，而vector改变容量的时候需要在内存开辟一片原来大小2倍的空间，  
再将原来内存中的内容移动过去，会有很大的开销，可以考虑用reserve()为容器预留部分空间，减少重新扩容次数，提高效率。  

•	C++中的重载和重写的区别  
重载函数是全局通用的，只要是在一个命名空间里面的函数都可以进行重载。  
而重写是针对于虚函数的，子类可以重写父类定义的虚函数或者纯虚函数实现不同类的不同功能。  

•	C++内存管理，内存池技术（热门问题），与csapp中几种内存分配方式对比学习加深理解  
这里只简述G2.9的std::alloc：  
在不同大小容器请求内存的时候，需要调用malloc按照容器内存块的大小来申请内存。  
分配器维护一个freelist[16]长度为16的数组，里面每个位置都保存一条freelist链表。  
链表位置 = 请求内存大小BLOCK / 8 – 1；（因为数组从0开始，所以-1）。  
在申请内存时候，需要申请一块大内存，大小为BLOCK * 20 * 2（乘以2是因为多申请一倍作为余量，以供下次使用，减少malloc次数）+Roundup（申请总量>>4）。
内存池由startfree和endfree两个指针维护。  
注意几个变量：①申请总量，计算追加量的时候有用；②内存池大小，申请内存时候如果内存池余量重置，就不用调用malloc，直接切分内存池为20块。  
源代码：可见小区块以8K为基准，最大为128K，分为16条链表。  
数据结构：可见free_list是一个长度为16的数组，index函数求出处理申请的链表号。  
每次多申请出来的内存池由两个指针维护，并记录累积量用于求追加量。  
因为二级分配器（上面说的全是二级分配器）只处理128K以下块的内存分配，如果申请内存块>128K，就把请求交给第一级分配器。  
内存申请的三种情况：主要由chunk_alloc函数完成。  
①内存池够申请20块，划20块，降低水位。  
②内存池不够申请20块，那就能几块就划分几块，降低水位。  
③内存池不够一块，充值！先计算出需要申请的内存大小，通过malloc获取内存块。  
充值完成后，再次尝试满足内存申请。这样子就必定可以满足需求。  
 
•	介绍面向对象的三大特性，并且举例说明每一个  
继承，多态，封装。继承：父类子类。多态：虚函数。封装：类的接口。  

•	C++多态的实现  
基类内声明函数，前面加上virtual关键字，然后在派生类内override这个函数，就可以根据运行时传入参数的类型调用对应的函数。  

•	C++虚函数相关（虚函数表，虚函数指针），虚函数的实现原理（包括单一继承，多重继承等）（拓展问题：为什么基类指针指向派生类对象时可以调用派生类成员函数，基类的虚函数存放在内存的什么区，虚函数表指针vptr的初始化时间）  
只要在类内声明了虚函数，这个类的内存空间里面就有一个虚函数指针vptr，对应着一张虚函数表vtable，通过这个vptr可以在表里面找到需要调用的函数。  
单一继承的情况下，派生类的vptr只有一个。多重继承的情况下，根据基类有无虚函数，派生类的vptr会有多个，  
且派生类自定义新的虚函数将会在第一个类的虚函数表的后面进行扩充。  
Q1：Base \*p = new Derived(); 在调用p->fun()的时候，如果fun()是虚函数，  
这时候vptr其实已经指向虚函数表中派生类的部分，所以这里时间是调用p->vptr->fun()。  
Q2：因为派生类的虚函数指针都来自于基类，且都要操作这个指针，所以它理所应当在全局（静态）变量区。  
Q3：虚函数表是在编译期确定的，而vptr是在运行时（执行构造函数时）确定的，vptr会根据类决定vptr指向的位置。  

•	C++中类的数据成员和成员函数内存分布情况  
在类中添加一个数据成员，类的大小会改变，而添加一个非虚函数，类的大小不变，说明数据成员是放在栈区，而成员函数在代码区。  

•	this指针  
因为在类还没有构造成一个实例对象前，没有指针可以指向类自身，就好像你在房子里，但你却看不见房子全貌了。  
所以引出了this指针来操作类自身的非静态成员函数/变量。  

•	析构函数一般写成虚函数的原因  
因为派生类在进行析构的时候首先都是调用父类的析构函数，若父类的析构函数并非虚函数，内存就不能完全被释放，会产生内存泄露。  

•	构造函数、拷贝构造函数和赋值操作符的区别  
构造函数：对象不存在，没用别的对象初始化  
拷贝构造函数：对象不存在，用别的对象初始化  
赋值运算符：对象存在，用别的对象给它赋值  

•	构造函数声明为explicit  
explicit关键字修饰构造函数，可以防止不需要的隐式转换发生，且只对单个参数的构造函数有效。  

•	构造函数为什么一般不定义为虚函数  
构造函数就是用来构造一个新的对象的，虚函数是用来让已存在的对象增加新的特性的，若对象不存在，虚函数没有意义，不能本末倒置。  

•	构造函数的几种关键字(default delete 0)  
= default：将拷贝控制成员定义为=default显式要求编译器生成合成的版本  
= delete：将拷贝构造函数和拷贝赋值运算符定义删除的函数，阻止拷贝（析构函数不能是删除的函数 C++Primer P450）  
= 0：将虚函数定义为纯虚函数（纯虚函数无需定义，= 0只能出现在类内部虚函数的声明语句处；也可以为纯虚函数提供定义，不过函数体必须定义在类的外部）  

•	构造函数或者析构函数中调用虚函数会怎样  
结果不定，因为在构造/析构函数中，对象被当做基类的对象进行处理，这时候的调用的虚函数就是基类的版本，
如果基类版本的虚函数是个纯虚函数？那调用当然会出错。  

•	纯虚函数  
C++通过在virtual修饰的函数声明后加上=0将一个函数声明为纯虚函数。这样的函数高度抽象，由派生类去定义它的详情。  

•	静态类型和动态类型，静态绑定和动态绑定的介绍  
静态类型：指编译时已知的类型。  
动态类型：指运行时内存中对象的类型。  
静态绑定（statically bound）：又名前期绑定（eraly binding），绑定的是静态类型，所对应的函数或属性依赖于对象的静态类型，发生在编译期；  
动态绑定（dynamically bound）：又名后期绑定（late binding），绑定的是动态类型，所对应的函数或属性依赖于对象的动态类型，发生在运行期；  

•	引用是否能实现动态绑定，为什么引用可以实现  
可以，因为引用的类型在运行期是确定的，即对象的类型是确定的，传入一个确定类型的引用，编译器当然可以识别出来。  

•	深拷贝和浅拷贝的区别（举例说明深拷贝的安全性）  
深拷贝是将一个对象完全拷贝过来，创建一个副本，有单独的内存空间。
而浅拷贝只是给这个对象加上一个别名。在删除释放浅拷贝别名对象的时候，源对象也会被错误释放掉，而深拷贝不会存在这种情况。  

•	对象复用的了解，零拷贝的了解（暂略）tobedone  

•	介绍C++所有的构造函数  
默认构造函数：由编译器自动生成，可能存在错误。  
拷贝(赋值)构造函数：形参为const typename &，本质上是一次赋值操作。  
移动构造函数：形参为typename &&，使用了移动语义，是相较于拷贝构造函数来说更快更好的方法。  

•	什么情况下会调用拷贝构造函数（三种情况）  
假如有：class test，test A，test B。test fcn(test T)    
①A = B；/A(B)；创建新对象时。  
②fcn(B)；作为函数形参传入时。  
③A = fcn(B)；作为函数返回值时。  

•	结构体内存对齐方式和为什么要进行内存对齐？  
内存块的大小由结构体中最大的数据类型决定，数据按顺序放入内存块中，不够一块的也要填充上。  
内存对齐是为了加快内存寻址速度，CPU是按块访问内存的，若不对齐，在不同平台的CPU可能会有不同的反应，甚至罢工。  

•	内存泄露的定义，如何检测与避免？  
堆内存泄露：内存泄露指程序在申请内存后，无法释怀已申请的内存空间，导致内存最终用光，通常在new/delete或者malloc/free没有搭配使用的时候出现。  
系统资源泄露：socket，handle，interface等也可能发生内存泄露。  
可以通过在DEBUG模式下设置断点检查内存泄露的地方，最有效的避免方法就是改用只能指针。  

•	手写智能指针的实现（shared_ptr和weak_ptr实现的区别）  
重点在于删除器的设计，引用次数的计数逻辑。  
https://blog.csdn.net/chxw098/article/details/39496829  

•	智能指针的循环引用  
shared_ptr可能存在循环引用的情况，改用weak_ptr可破。  

•	遇到coredump要怎么调试tobedone  

•	内存检查工具的了解tobedone  

•	模板的用法与适用场景tobedone  

•	成员初始化列表的概念，为什么用成员初始化列表会快一些（性能优势）？  
在构造函数后面用冒号加上初始列表的方式可以达成直接初始化的构造函数，因为初始化相较于赋值/拷贝少了一步operator=，所以性能肯定是占优的。  

•	用过C++ 11吗，知道C++ 11哪些新特性？  
STL，lambda和bind，右值引用与移动语义，智能指针，类型推导，尾置返回类型，for（）循环遍历容器，
常量表达式constexpr，空指针nullptr，类里面的default，delete，override，final关键字。  

•	C++的调用惯例（简单一点C++函数调用的压栈过程）  
main函数执行到某个函数时，将它压入系统栈顶，该函数执行完毕后，根据它的返回地址，main函数进行跳转继续执行。（编译原理内容，涉及大量寄存器，暂略）  

•	C++的四种强制转换  
static_cast()：派生类到基类转换，基本类型之间转换使用。  
dynamic_cast()：基类要有虚函数，进行类型检测，基类到派生类，派生类之间转换。  
const_cast()：去const属性的时候使用。  
reinterpret_cast()：怎么转都行，但高危。  

•	C++中将临时变量作为返回值的时候的处理过程（栈上的内存分配、拷贝过程）    
临时变量从栈上被深拷贝到静态常量区，全局变量或者堆区中，然后在栈中销毁。  

•	C++的异常处理  
使用try……catch捕获异常和throw抛出异常。头文件exception有以下定义：  
而且每个异常都有有个what()方法放回const char\*描述异常信息。  

•	volatile关键字  
volatile有3个特性，1、易变性，在汇编层面反映出来，就是两条语句，下一条语句不会直接使用上一条语句对应的volatile变量的寄存器内容，而是重新从内存中读取。  
2、不可优化特性。volatile告诉编译器，不要对我这个变量进行各种激进的优化，甚至将变量直接消除，保证程序员写在代码中的指令，一定会被执行。  
3、顺序性，能够保证Volatile变量间的顺序性，编译器不会进行乱序优化。  

•	优化程序的几种方法（暂略）  

•	public，protected和private访问权限和继承  
public：类的函数√，类的对象√，派生类√，友元√，公有继承的函数属性不变。  
protected：类的函数√，类的对象×，派生类√，友元√，保护继承的函数，public和 protected变量都是protected属性。  
Private：类的函数√，类的对象×，派生类×，友元√，私有继承的函数都是private属性。  

•	decltype()和auto  
decltype()用于获取变量类型或者函数返回类型，auto都用于根据上下文推断对象的类型。  
auto会忽略顶层const，decltype()保留一切关键字。decltype(())两层括号时候永远得到一个引用类型。  

•	inline和宏定义的区别  
宏是简单的进行替换，预编译时展开。inline是函数，编译时展开，具有类型检测，语法判断等功能。  

•	C++和C的类型安全  
C++采用inline替换宏定义，dynamic_cast()进行下行转换，有了类型检查。  
且new不像malloc()那样返回void\*，编译器比C更加安全。  
