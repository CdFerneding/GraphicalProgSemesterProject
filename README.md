# PROG20002 Graphics Programming

## Evaluation

This is a **PASS**. The features and the rendering looks ok, however I have a couple of comments (see below)


## Segmentation fault on output

There is a segmentation fault on output. Most likely, a OpenGL function has been called after `glfwTerminate()` in your `IndexBuffer` destructor (at that point there is no valid OpenGL context). This would typically lower your grade during the exam.

``` txt
(gdb) run
Starting program: /home/rafael/src/prog2002/repos/group_16/Debug/bin/assignment 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
4.3.0 NVIDIA 525.147.05

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff6cbcaa0 in ?? ()
(gdb) backtrace
#0  0x00007ffff6cbcaa0 in ?? ()
#1  0x0000555555589669 in IndexBuffer::~IndexBuffer (this=0x555555a3cec0, __in_chrg=<optimized out>)
    at /home/rafael/src/prog2002/repos/group_16/framework/Rendering/IndexBuffer.cpp:12
#2  0x0000555555588ffe in __gnu_cxx::new_allocator<IndexBuffer>::destroy<IndexBuffer> (this=0x555555a3cec0, 
    __p=0x555555a3cec0) at /usr/include/c++/11/ext/new_allocator.h:168
#3  0x0000555555588ef1 in std::allocator_traits<std::allocator<IndexBuffer> >::destroy<IndexBuffer> (__a=..., 
    __p=0x555555a3cec0) at /usr/include/c++/11/bits/alloc_traits.h:535
#4  0x00005555555888b5 in std::_Sp_counted_ptr_inplace<IndexBuffer, std::allocator<IndexBuffer>, (__gnu_cxx::_Lock_policy)2>::_M_dispose (this=0x555555a3ceb0) at /usr/include/c++/11/bits/shared_ptr_base.h:528
#5  0x00005555555835e7 in std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release (this=0x555555a3ceb0)
    at /usr/include/c++/11/bits/shared_ptr_base.h:168
#6  0x0000555555581219 in std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count (this=0x7fffffffd748, 
    __in_chrg=<optimized out>) at /usr/include/c++/11/bits/shared_ptr_base.h:705
#7  0x000055555557ea16 in std::__shared_ptr<IndexBuffer, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr (this=0x7fffffffd740, 
    __in_chrg=<optimized out>) at /usr/include/c++/11/bits/shared_ptr_base.h:1154
#8  0x000055555557ea36 in std::shared_ptr<IndexBuffer>::~shared_ptr (this=0x7fffffffd740, __in_chrg=<optimized out>)
    at /usr/include/c++/11/bits/shared_ptr.h:122
#9  0x000055555557d44d in AssignementApplication::Run (this=0x7fffffffdb40)
    at /home/rafael/src/prog2002/repos/group_16/assignment/src/assignment.cpp:567
#10 0x000055555555c085 in main (argc=1, argv=0x7fffffffde08)
    at /home/rafael/src/prog2002/repos/group_16/assignment/src/main.cpp:10
```
## Memory leaks

You are using the `new` operator without issuing corresponding `delete`:

``` txt
File: src/assignment.cpp
 149  24     std::vector<float> newSelectionSquare = {
 156  12     return newSelectionSquare;
 342  24     auto* shaderGrid = new Shader(VS_Grid, FS_Grid);
 345  26     auto* shaderSquare = new Shader(VS_Square, FS_Square);
 348  24     auto* shaderUnit = new Shader(VS_Unit, FS_Unit);
```

This will lead to memory leaks and it is somehting that in the exam would typically lower your grade. Use of smart pointers can be an alternative

## Other minor issues

You should avoid the use of relative includes:

``` c++
// rendering framework
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
```
Have a look at the small fix available in this branch and correct (if there are more).
