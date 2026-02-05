# 基于 C 语言链表实现的 ATM 管理系统
这是一个基于 C 语言开发的简单控制台 ATM（自动取款机）系统。该项目主要通过单向链表来动态管理用户信息，支持账户的增删改查以及数据的持久化存储。

## 📖 项目简介
该系统模拟了真实的银行账户操作流程，包含两级菜单系统。

* 一级菜单：负责账户的注册、登录与系统退出。

* 二级菜单：登录后进行存款、取款、转账、信息查询、修改密码及注销账户等操作。

系统会在启动时从 ATM_data.txt 加载数据，并在退出时将链表中的数据回写至文件，保证数据不丢失。

## 🏗️ 核心架构：链表实现 (Linked List Implementation)
本系统不使用固定大小的数组，而是完全依赖动态内存分配和单向链表来管理账户。这使得系统能够处理不定数量的用户，并在运行时灵活地申请和释放内存。

**1. 数据节点设计**

每个账户被定义为一个结构体节点，包含用户数据和指向下一个节点的指针 next。

```C
typedef struct Account
{
    char id[CHAR_MAXSIZE];       // 账号 ID
    char password[CHAR_MAXSIZE]; // 密码
    char name[CHAR_MAXSIZE];     // 用户名
    double balance;              // 余额
    struct Account *next;        // 指向下一个账户节点的指针
} Account;

// 全局头节点，初始化为空
Account *head = NULL;
```
**2. 节点的添加（头插法）**

在注册新账户时，系统使用malloc动态分配内存，并采用头插法 (Head Insertion) 将新节点加入链表。这意味着新注册的用户总是位于链表的第一个位置，这样做的时间复杂度为 O(1)。

```C
/* 创建并添加新节点 */
Account* addNode(char *id, char *password, char *name, double balance){
    // 1. 动态分配内存
    Account *newNode = (Account*)malloc(sizeof(Account));

    // 2. 填充数据
    strcpy(newNode->id, id);
    strcpy(newNode->password, password);
    strcpy(newNode->name, name);
    newNode->balance = balance;

    // 3. 调整指针（头插法核心逻辑）
    newNode->next = head; // 新节点指向原本的头节点
    head = newNode;       // 头指针更新为新节点

    return newNode;
}
```
**3. 节点的查找（遍历）**

在登录或转账时，需要验证账号是否存在。系统通过遍历链表来查找匹配的 ID。

```C
/* 遍历链表查找账号 */
Account* accountExist(char *id){
    Account *p = head;
    while (p != NULL){
        // 比较 ID 是否匹配
        if (strcmp(p->id, id) == 0) return p; // 找到并返回节点指针
        p = p->next; // 移动到下一个节点
    }
    return NULL; // 未找到
}
```
**4. 节点的删除（注销账户）**

注销功能涉及链表节点的删除操作。算法需要处理两种情况：

1. 删除的是头节点。

2. 删除的是中间或尾部节点（需要找到前驱节点）。

```C
void deregister(){
    // ... (省略确认交互代码)

    // 情况 1: 删除的是头节点
    if (curUser == head){
        head = head->next; // 头指针后移
        free(curUser);     // 释放内存
    }
    // 情况 2: 删除的是中间或尾部节点
    else{
        Account *p = head;
        // 遍历查找前驱节点 (p)
        while(p->next != NULL && p->next != curUser){
            p = p->next;
        }
        // 找到前驱节点后，进行断链操作
        if(p->next == curUser){
            Account *tempNode = curUser->next; // 暂存下一个节点地址
            p->next = tempNode;                // 前驱节点跳过当前节点
            free(curUser);                     // 释放当前节点内存
        }
    }
    curUser = NULL; // 置空当前用户指针，防止悬空指针
    // ...
}
```
**5. 内存释放（系统退出）**

程序结束前，必须遍历整个链表，逐个释放所有节点占用的堆内存，防止内存泄漏。

```C
void freeNode(){
    Account *p = head; 
    if (p == NULL){
        return;
    }
    while(p != NULL){
        Account *temp = p; // 记录当前节点
        p = p->next;       // 指针先移向下一个
        free(temp);        // 安全释放当前节点
    }
    head = NULL;
}
```
## 🛠️ 功能列表
**账户安全**
* 注册 (Register): 检查 ID 重复，两次密码确认，默认赠送 100 元体验金。

* 登录 (Login): 验证 ID 和密码，成功后进入二级菜单。

* 修改密码 (Change PWD): 验证旧密码后更新新密码。

* 注销 (Deregister): 永久删除账户及其数据。

**银行业务**
* 信息查询 (Information): 显示 ID、姓名及当前余额。

* 存款 (Deposit): 增加当前账户余额。

* 取款 (Withdraw): 校验余额充足后扣款。

* 转账 (Transfer): 查找目标账户节点，实现两个节点间的余额数值转移（原子性操作）。

**数据持久化**
* Load Data: 程序启动时读取 ATM_data.txt，重建链表。

* Save Data: 程序退出时遍历链表，将所有节点信息按格式写入文件。

## 🚀 编译与运行
**编译环境**
* 任何标准 GCC 编译器或 C/C++ IDE (Visual Studio, Dev-C++, CLion, VS Code)。

**编译命令**
```Bash
gcc main.c -o atm_system[自定义] //本项目位project.c
```
**运行**

* Windows:

    ```Bash
    ./atm_system.exe
    ```
* Linux/Mac:

    ```Bash
    ./atm_system
    ```
## ⚠️ 注意事项
* 数据文件: 首次运行时如果没有 ATM_data.txt 属于正常现象，保存数据后会自动创建。

* 输入限制: ID 和密码的最大长度定义为 CHAR_MAXSIZE (20字符)。

* 内存管理: 代码中包含完整的 malloc 与 free 配对逻辑，确保系统的健壮性。