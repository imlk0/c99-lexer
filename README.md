# lexical-analyzer

用C++实现的一个，基于C99标准的词法分析器

### feature

- [x] 列出识别出的记号
- [x] 使用色彩模式打印匹配后的源程序（需要支持控制字符的终端）
- [x] 输出统计信息
- [x] 错误报告、使用“恐慌模式”解决错误

### how to build

- 通过make编译
依赖环境：gcc、make
直接执行:
```
make
```
生成文件位置：`./build/linux/x86_64/release/lexical-analyzer`

- 通过xmake编译
依赖环境：gcc、xmake
执行：
```
xmake
```
生成文件位置：`./build/linux/x86_64/release/lexical-analyzer`

##### 测试样例

我们在`test`目录下准备了7个测试样例


### usage

```
Usage: lexical-analyzer [-l] [-c] [-c] [-h] [FILE]

-l, --lexemes   print all lexemes matched.
-c, --color     print input strings in color mode.
-s, --statistics        print statistics message.
-h, --help      print this help message.
FILE            source code file to analysis. stdin will be used to read if FILE was not specified.
```

##### example

从标准输入读取内容

```
./lexical-analyzer
```

从源文件`src.c`读取内容，检测是否存在错误

```
./lexical-analyzer src.c
```

从源文件`src.c`读取内容，依次列出所有记号

```
./lexical-analyzer -l src.c
```

从源文件`src.c`读取内容，输出统计信息

```
./lexical-analyzer -s src.c
```

从源文件`src.c`读取内容，以色彩模式打印匹配结果

```
./lexical-analyzer -c src.c
```
