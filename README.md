## 正确性测试

该测试实现在hnsw.cpp。使用如下命令运行正确性测试：

```
#编译链接正确性测试文件
make all
#运行正确性测试
make grade
#清理编译结果
make clean
```

实验数据（召回率、单次查询时延）将呈现在命令行界面（terminal）。




## 参数M的影响

在parameter.hpp文件中修改M，M_max的值，再使用如下命令运行正确性测试：

```
#编译链接正确性测试文件
make all
#运行正确性测试
make grade
#清理编译结果
make clean
```

实验数据（召回率、单次查询时延）将呈现在命令行界面（terminal）。



### 性能测试

该测试实现在test.cpp。使用如下命令运行性能测试：

```
#编译链接性能测试文件
make all
#运行性能测试
make test
#清理编译结果
make clean
```

为了编译运行test.cpp，在Makefile文件中的FLAGS里新增"-pthread"。

实验数据（单次插入时延，串行查询与并行查询的召回率、单次查询时延）将呈现在命令行界面（terminal）。