# 作业一
本作业要求实现一个c++的向量运算库，支持向量的算术运算。
所有函数声明写在`vector.h`中，所有函数定义写在`vector.cpp`中。
你修改vector.h,verctor.cpp，同时编写一个CMakelists.txt来完成作业。
同时，需要安装的库为gtest

```bash
sudo apt install gtest
```
在CMakeLists.txt中加入这段来调用，链接gtest的库
```
find_package(GTest REQUIRED)
...
target_link_libraries(main
        GTest::GTest
        GTest::Main
)
```

```cpp
using Vector = std::vector<double>;
```

所有函数都放在 algebra 命名空间下.
函数列表

- zeros
创建长度为 n 的全零向量
Vector zeros(size_t n);


- ones
创建长度为 n 的全一向量
Vector ones(size_t n);


- random
创建长度为 n 的随机向量，元素在 [min, max] 之间（用 <random> 库）
Vector random(size_t n, double min, double max);


- show
美观地输出向量，保留 3 位小数（用 <iomanip>）
void show(const Vector& v);


- sum
  - **向量加常数**
  ```
    Vector sum(const Vector& v, double c);
    ```
  - **向量加向量**
  ```
    Vector sum(const Vector& v1, const Vector& v2);
    ```
- multiply
  - **向量乘常数**
  ```
    Vector multiply(const Vector& v, double c);
    ```
  - **向量点乘**
  ```
    double multiply(const Vector& v1, const Vector& v2);
    ```
- norm
  - **计算向量的欧氏范数**
    ```
    double norm(const Vector& v);
     ```

- normalize
  - **返回单位向量**
    ```
    Vector normalize(const Vector& v);
     ```

- concatenate
  - **拼接两个向量**
    ```
    Vector concatenate(const Vector& v1, const Vector& v2);
     ```

注意事项
所有函数都要检查输入合法性（如长度不匹配、空向量等），不合法时抛出 std::logic_error。
空向量的 norm 定义为 0。
其他
所有实现写在 hw1.cpp，声明写在 hw1.h。
如需测试，可在 main.cpp 的 debug 区域编写测试代码
<p  align="center"> <b>GOOD LUCK</b> </p>
