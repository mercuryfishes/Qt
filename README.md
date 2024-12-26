# *Qt翻金币项目改进*

## 一、代码优化

在 Qt 中，可以通过以下方式封装函数，使其能够在多个类中使用：

### **1. 全局函数（普通函数）**

如果函数不需要依赖特定类的成员变量或状态，可以定义为一个普通函数。

实现步骤

- 在一个公共的 `.h` 文件中声明函数：
![image](https://github.com/user-attachments/assets/f91a7f96-d283-4513-b6c7-15dbb48810d8)

- 在对应的 `.cpp` 文件中实现:
![image](https://github.com/user-attachments/assets/2baabbfa-e81c-4857-921e-a738f0ea856b)

- 在需要使用的地方包含该 `.h` 文件:
![image](https://github.com/user-attachments/assets/7e7a4e8a-77ce-4619-aa3a-82d37a1b4b2c)

### 2. **静态函数**

如果函数逻辑与某个类相关但不依赖于具体的实例，可以定义为静态函数。静态函数属于**类**，但不依赖于**类的实例**。

实现步骤

- 在类的声明中添加 `static` 修饰符:
![image](https://github.com/user-attachments/assets/e17a2eed-dbee-43bf-baae-cc129371eb72)

- 在类的实现中定义该函数。
![image](https://github.com/user-attachments/assets/49487e8c-3106-4a31-9cb8-c833135789ae)

- 使用时需要带上类名
![image](https://github.com/user-attachments/assets/c4135f0c-49fc-42b5-adbf-4f4b98ce3ba5)

### 3. **继承基类**

如果函数逻辑与某些类相关且需要依赖于类的成员变量，可以将函数封装在一个基类中，通过继承的方式让多个类共享该函数:

![image](https://github.com/user-attachments/assets/70df3db8-38d8-4d6a-bcf1-f3945093c14b)


### 4.小技巧

可以建立一个工具类，封装可能重复使用的函数

