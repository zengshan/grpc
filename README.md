# grpc

# environment
# 提供ssl的环境
apt-get install libopenssl

# build

1、cd build
2、cmake ..
3、make && make install

# src

1、third_party用来存储第三方的库文件
2、源代码文件结构
    >> src
        > net用来存储网络库的基本封装
        > timer需要服务器提供可用的timer来做定时器和循环
        > utils用来提供通用的操作函数
        > py_interface用来在C++接口对外提供一个python库函数（网络层和定时器）

3、本文全部基于C++11，二进制压缩基于protobuf。无其他第三方库问题
