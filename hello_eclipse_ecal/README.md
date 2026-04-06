# Hello Eclipse eCAL - 示例应用

使用 Eclipse eCAL 中间件的 Bazel 示例应用，包含 Publisher 和 Subscriber 两个程序。

## 概述

本示例演示如何在 Bazel 项目中使用 Eclipse eCAL 进行进程间通信：
- **publisher**: 发布字符串消息
- **subscriber**: 接收并显示消息

## 目录结构

```
hello_eclipse_ecal/
├── MODULE.bazel          # Bazel 模块定义
├── BUILD.bazel           # 构建规则
├── README.md             # 本文档
├── publisher.cc          # Publisher 源码
├── subscriber.cc         # Subscriber 源码
└── ecal.yaml             # eCAL 配置文件（可选）
```

## 前置条件

1. Bazel 9.0+
2. Eclipse eCAL 库（见 `../thirdparty/eclipse-ecal-vendor`）
3. GCC/G++ 或 Clang

## 构建

### 1. 构建 Publisher

```bash
cd eros/playground/hello_eclipse_ecal

# 构建 Release 版本
bazel build //:publisher --config=linux_arm64

# 构建 Debug 版本
bazel build //:publisher --config=debug
```

### 2. 构建 Subscriber

```bash
# 构建 Release 版本
bazel build //:subscriber --config=linux_arm64
```

### 构建输出

```
bazel-bin/
├── publisher     # Publisher 可执行文件
└── subscriber    # Subscriber 可执行文件
```

## 运行

### 方法 1: 设置 LD_LIBRARY_PATH

```bash
# 设置 eCAL 库路径
export LD_LIBRARY_PATH=/home/user/programs/eros-ws/eros/thirdparty/eclipse-ecal-vendor/bazel-bin/copy_ecal_core/ecal_core/lib:$LD_LIBRARY_PATH

# 运行 Publisher（在一个终端）
./bazel-bin/publisher

# 运行 Subscriber（在另一个终端）
./bazel-bin/subscriber
```

### 方法 2: 使用包装脚本

创建运行脚本 `run_example.sh`：

```bash
#!/bin/bash
ECAL_LIB_PATH="/home/user/programs/eros-ws/eros/thirdparty/eclipse-ecal-vendor/bazel-bin/copy_ecal_core/ecal_core/lib"
export LD_LIBRARY_PATH=$ECAL_LIB_PATH:$LD_LIBRARY_PATH

echo "Starting Publisher..."
./bazel-bin/publisher &
PUBLISHER_PID=$!

sleep 2

echo "Starting Subscriber..."
./bazel-bin/subscriber &
SUBSCRIBER_PID=$!

# 运行 10 秒后停止
sleep 10

kill $PUBLISHER_PID $SUBSCRIBER_PID 2>/dev/null
echo "Example completed."
```

## 预期输出

### Publisher 输出

```
[eCAL][Config] Specified yaml configuration path not valid:"ecal.yaml". Using default configuration.
Publisher started. Press Enter to stop...
Sent: Hello eCAL! Counter: 0
Sent: Hello eCAL! Counter: 1
Sent: Hello eCAL! Counter: 2
Sent: Hello eCAL! Counter: 3
...
```

### Subscriber 输出

```
[eCAL][Config] Specified yaml configuration path not valid:"ecal.yaml". Using default configuration.
Subscriber started. Press Enter to stop...
Received: Hello eCAL! Counter: 0
Received: Hello eCAL! Counter: 1
Received: Hello eCAL! Counter: 2
...
```

## 代码说明

### Publisher (publisher.cc)

```cpp
#include <ecal/ecal.h>
#include <ecal/msg/string/publisher.h>

int main() {
  // 初始化 eCAL
  eCAL::Initialize("eCAL Publisher");
  
  // 创建字符串 Publisher，发布到 "hello_world_topic"
  eCAL::string::CPublisher publisher("hello_world_topic");
  
  // 循环发布消息
  while (eCAL::Ok()) {
    publisher.Send("Hello eCAL!");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  
  // 清理
  eCAL::Finalize();
}
```

### Subscriber (subscriber.cc)

```cpp
#include <ecal/ecal.h>
#include <ecal/msg/string/subscriber.h>

// 回调函数
void receive_callback(const char* topic_name, const std::string& message) {
  std::cout << "Received: " << message << std::endl;
}

int main() {
  // 初始化 eCAL
  eCAL::Initialize("eCAL Subscriber");
  
  // 创建字符串 Subscriber，订阅 "hello_world_topic"
  eCAL::string::CSubscriber subscriber("hello_world_topic");
  
  // 注册回调
  subscriber.AddReceiveCallback(std::bind(receive_callback, _1, _2));
  
  // 等待消息
  while (eCAL::Ok()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  
  // 清理
  eCAL::Finalize();
}
```

## MODULE.bazel 配置

```python
module(
    name = "hello_eclipse_ecal",
    version = "1.0.0",
    compatibility_level = 1,
)

# 依赖 eCAL vendor
bazel_dep(name = "eclipse_ecal_vendor")
local_path_override(
    module_name = "eclipse_ecal_vendor",
    path = "../../thirdparty/eclipse-ecal-vendor",
)

# 依赖 EROS Forge
bazel_dep(name = "eros_forge")
local_path_override(
    module_name = "eros_forge",
    path = "../../forge",
)
```

## BUILD.bazel 配置

```python
load("@rules_cc//cc:defs.bzl", "cc_binary")

# Publisher
cc_binary(
    name = "publisher",
    srcs = ["publisher.cc"],
    deps = [
        "@eclipse_ecal_vendor//:ecal",  # eCAL 库
    ],
)

# Subscriber
cc_binary(
    name = "subscriber",
    srcs = ["subscriber.cc"],
    deps = [
        "@eclipse_ecal_vendor//:ecal",  # eCAL 库
    ],
)
```

## 跨平台编译

```bash
# ARM64 原生编译
bazel build //:publisher --config=linux_arm64

# x86_64 原生编译
bazel build //:publisher --config=linux_x86_64

# 从 x86_64 交叉编译到 ARM64
bazel build //:publisher --config=linux_x86_64_cross_arm64

# ARM64 交叉编译（使用自定义 glibc）
bazel build //:publisher --config=linux_arm64_cross_arm64
```

## 故障排查

### 问题：找不到 libecal_core.so

**错误信息**：
```
error while loading shared libraries: libecal_core.so.0: cannot open shared object file
```

**解决方法**：
```bash
export LD_LIBRARY_PATH=/path/to/eclipse-ecal-vendor/bazel-bin/ecal_core/lib:$LD_LIBRARY_PATH
```

### 问题：eCAL 初始化失败

**可能原因**：
- 多个实例使用相同的名称
- 网络配置问题

**解决方法**：
- 确保每个进程有唯一的名称：`eCAL::Initialize("Unique Name")`
- 检查 eCAL 配置文件

### 问题：Subscriber 收不到消息

**可能原因**：
- Publisher 和 Subscriber 没有在同一网络
- 主题名称不匹配

**解决方法**：
- 确认主题名称完全一致：`"hello_world_topic"`
- 检查防火墙设置

## 扩展示例

### 使用 Protobuf 消息

eCAL 支持多种消息格式，包括 Google Protobuf：

```cpp
#include <ecal/msg/protobuf/publisher.h>
#include "my_message.pb.h"

eCAL::protobuf::CPublisher<MyMessage> publisher("my_topic");
MyMessage msg;
msg.set_id(123);
msg.set_data("hello");
publisher.Send(msg);
```

### 使用服务/客户端模式

```cpp
// Server
eCAL::CServiceServer server("my_service");
server.AddMethodHandler("add", add_callback);

// Client
eCAL::CServiceClient client("my_service");
client.CallMethod("add", request, response);
```

## 参考资源

- [eCAL 官方文档](https://ecal.io)
- [eCAL C++ API 文档](https://eclipse-ecal.github.io/ecal/)
- [eCAL 示例代码](https://github.com/eclipse-ecal/ecal/tree/master/samples)
- [EROS Forge 文档](../../forge/documents/bazel9-cmake-forge-best-practices.md)

## 许可证

本示例使用 Apache License 2.0（与 eCAL 一致）。
