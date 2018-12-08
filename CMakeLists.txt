# 查询cmake的版本问题
cmake_minimum_required(VERSION 2.6)
project(zrpc)

# 外部依赖是openssl
find_library(LIB_OPENSSL NAMES crypto ssl)
