### 项目 README

#### 项目名称
TBS_CPP

#### 项目简介
TBS_CPP 是一个 C++ 项目，旨在提供一系列基础工具和库，以简化开发过程并提高代码质量。该项目包括多个模块，每个模块都封装了特定的功能。

#### 目录结构
```
TBS_CPP/
├── include/
│   └── base/
│       ├── base.hpp
│       ├── defs.h
│       ├── guard.hpp
│       ├── wrapper.hpp
│       └── result.hpp
├── src/
│   └── (源代码文件)
├── test/
│   └── (测试文件)
├── CMakeLists.txt
└── README.md
```


#### 主要模块
- **base.hpp**: 头文件，包含其他基础模块的头文件。
- **defs.h**: 定义常量、宏等基本定义。
- **guard.hpp**: 提供资源管理的 RAII 模式。
- **wrapper.hpp**: 封装常用功能，如智能指针、函数包装等。
- **result.hpp**: 处理错误和结果的类。

#### 编译与运行
1. 确保已安装 CMake 和支持 C++17 的编译器。
2. 打开终端，进入项目根目录。
3. 运行以下命令进行编译：
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行可执行文件：
   ```sh
   ./your_executable
   ```


#### 测试
1. 在 `build` 目录下，运行以下命令进行测试：
   ```sh
   ctest
   ```


#### 贡献指南
欢迎贡献代码！请遵循以下步骤：
1. Fork 本仓库。
2. 创建一个新的分支。
3. 提交您的更改。
4. 推送更改到您的仓库。
5. 提交 Pull Request。

#### 许可证
本项目采用 MIT 许可证，详情参见 [LICENSE](LICENSE) 文件。
