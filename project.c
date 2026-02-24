#include <stdio.h>
#include <string.h>
#include <stdlib.h> // 内存管理
#define CHAR_MAXSIZE 20
/* 设立结构体管理相关信息 */
typedef struct Account
{
    char id[CHAR_MAXSIZE];
    char password[CHAR_MAXSIZE];
    char name[CHAR_MAXSIZE];
    double balance;
    struct Account *next; // 下一个数据的位置地址
}Account;

/* 函数声明 */
void show_1stMenu(void);
void show_2ndMenu(void);
void afterLogin(void);
Account *addNode(char *id, char *password, char *name,double balance);
Account *accountExist(char *id);
int passwordConfirm(char password[]);
void registerAccount(void);
int loginAccount(void);
void showInformation(void);
void deposit(void);
void withdraw(void);
void transfer(void);
void changePWD(void);
void deregister(void);
void mainLoop(void);
void saveData(void);
void loadData(void);
void freeNode(void);

// 头节点创立
Account *head = NULL;
// 目前登录的人的节点
Account *curUser = NULL;

/* 新建一级菜单 */
void show_1stMenu(void){
    printf("--------1st Menu--------\n");
    printf("------1. Register-------\n");
    printf("--------2. Login--------\n");
    printf("--------0. Exit---------\n");
}
void show_2ndMenu(void){
    printf("--------2nd Menu--------\n"); 
    printf("-----1. Information-----\n"); //展示信息 showInformation
    printf("-------2. Deposit-------\n"); //存钱
    printf("------3. Withdraw-------\n"); //取钱
    printf("------4. Transfer-------\n"); //本地银行转账
    printf("------5. PwdChange------\n"); //修改密码
    printf("-----6. Deregister------\n"); //注销账号
    printf("------0. Exit SYS-------\n"); //退出系统
}

/* 新建二级菜单 */
void afterLogin(void){
    while(curUser != NULL){
        show_2ndMenu();
        int choice;
        printf("Please select your choice: "); scanf("%d", &choice);
        switch (choice){
            case 1:
                showInformation();//展示信息函数
                break;
            case 2:
                deposit(); // 存钱
                break;
            case 3:
                withdraw(); // 取钱
                break;
            case 4:
                transfer(); // 本地银行转账
                break;
            case 5:
                changePWD(); // 修改密码
                break;
            case 6:
                deregister(); // 注销账号
                break;
            case 0:
                return;
            }
    }
}

/*创建一个新的节点进行数据的添加*/
Account* addNode(char *id, char *password, char *name,double balance){
    Account *newNode = (Account*)malloc(sizeof(Account));

    strcpy(newNode->id, id);
    strcpy(newNode->password, password);
    strcpy(newNode->name, name);
    newNode->balance = balance;

    // 采用头插法进行设置
    newNode->next = head;
    head = newNode;

    return newNode;
}

/* 检查账号是否已经注册过 */
Account* accountExist(char *id){
    Account *p = head;
    while (p != NULL){
        if (strcmp(p->id, id) == 0) return p; // 代表账号已经注册过
        p = p->next;
    }
    return NULL; // 代表账号未注册
}

int passwordConfirm(char password[]){
    char tempPassword[CHAR_MAXSIZE];
    printf("Please confirm your password: "); scanf("%19s", tempPassword);
    if (strcmp(password, tempPassword) == 0) return 1; // 代表密码正确
    return 0; // 代表密码错误
}

void registerAccount(void){ // 注册的函数实现
    printf("Please enter the id you want to set: ");
    char id[CHAR_MAXSIZE], name[CHAR_MAXSIZE], password[CHAR_MAXSIZE]; scanf("%19s", id);
    if (accountExist(id) != NULL){
        printf("The account you set is existing");
        return;
        system("pause");
    }

    // 账号没有注册条件
    printf("Please enter your name: "); scanf("%19s", name);
    printf("Please set your password: ");
    while(scanf("%19s", password) == 1){
        if (passwordConfirm(password) == 1) break;
        else{
            printf("Password not match, try again: ");
        }
    }
    // 链表节点添加数据
    Account* node = addNode(id, password, name, 100.00); // 注册赠送100块钱
    printf("For new account, you can get %.2f", node->balance);
    system("pause");
}

//登录账号
int loginAccount(void){
    char id[CHAR_MAXSIZE], password[CHAR_MAXSIZE];
    printf("Please enter your account: "); scanf("%19s", id);
    Account *targetAccount = accountExist(id);
    if(targetAccount == NULL){
        printf("The account doesn't exist");
        system("pause");
        return 0;
    }
    // 确认是否存在账号才输入密码
    printf("Please enter your password: "); scanf("%19s", password);
    if(strcmp(targetAccount->password, password) == 0){
        curUser = targetAccount; // 登录之后，一定确定是当前登录人，才可以进入管理页面
        return 1;
    }
    else{
        printf("The password entered is wrong");
        system("pause");
        return 0;
    }
}

// 展示信息的函数
void showInformation(void){
    printf("----Self information----\n");
    printf("id: %s\n", curUser->id);
    printf("name: %s\n", curUser->name);
    printf("balance: %.2f\n", curUser->balance);
}

// 存钱函数
void deposit(void){
    double moneyDeposit;
    printf("Please place the money you input: "); scanf("%lf", &moneyDeposit);
    if (moneyDeposit > 0){
        curUser->balance += moneyDeposit;
        printf("Successfully deposit %.2f, Now balance: %.2f", moneyDeposit, curUser->balance);
    }
    else{printf("You haven't place the money you input");}
}

// 提取存款
void withdraw(void){
    double moneyWithdraw;
    printf("Please withdraw the money you want: "); scanf("%lf", &moneyWithdraw);
    if(moneyWithdraw <= 0){
        printf("Unvalid withdraw amout");
        return;
    }
    if(moneyWithdraw > curUser->balance){
        printf("balance is not enough");
        return;
    }
    else{
        curUser->balance -= moneyWithdraw;
        printf("Successfully withdraw: %.2f, Now balcance: %.2f", moneyWithdraw, curUser->balance);
    }
}

void transfer(void){
    char idTransfer[CHAR_MAXSIZE];
    double moneyTransfer;
    printf("Enter the id you want to transfer: "); scanf("%19s", idTransfer);
    Account *targetAccout = accountExist(idTransfer);
    if(targetAccout == NULL){
        printf("The account you wanna tranfer doesn't exist");
        return;
    }
    if(targetAccout == curUser){
        printf("Cannot transfer to yourself!\n");
        return;
    }
    printf("Please enter amout you want to transfer: ");
    scanf("%lf", &moneyTransfer);
    if(moneyTransfer <= 0){
        printf("Unvalid transfer money");
        return;
    }
    if(moneyTransfer > curUser->balance){
        printf("Your balance is not enough");
        return;
    }
    else{
        targetAccout->balance += moneyTransfer;
        curUser->balance -= moneyTransfer;
        printf("Successfully tranfer: %.2f, Now balance: %.2f", moneyTransfer, curUser->balance);
    }
}

// 修改密码
void changePWD(void){
    char oldPassword[CHAR_MAXSIZE], newPassword[CHAR_MAXSIZE];
    char confirmedPassword[CHAR_MAXSIZE];
    printf("Please enter your old password: ");
    while(scanf("%19s", oldPassword) == 1){
        if(strcmp(oldPassword, curUser->password) == 0)
            break;
        else{
            printf("Please enter again: ");
        }
    }

    printf("Please enter your new password: ");
    while(scanf("%19s", newPassword) == 1){
        if(passwordConfirm(newPassword))
            break;
        else{
            printf("Password not match, please try again: ");
        }
    }
    strcpy(curUser->password, newPassword);
    printf("Successfully change your password");
}

// 注销账号
void deregister(void){
    char confirm;
    printf("Warning: The current action cannot be reversed.");
    printf("Are you sure want to delete your account? (y/n): ");
    getchar();
    scanf("%c", &confirm);
    if (confirm != 'y' && confirm != 'Y'){
        printf("Operation cancelled.\n");
        return; // 退出当前栈函数
    }

    //删除头节点
    if (curUser == head){
        head = head->next;
        free(curUser);
    }
    else{
        // 要考虑尾部节点的问题
        Account *p = head; // 记录当前节点的位置
        while(p->next != NULL && p->next != curUser){
            p = p->next;
        }
        if(p->next == curUser){
            Account *tempNode = curUser->next;
            p->next = tempNode;
            free(curUser);
        }
    }
    curUser =NULL;
    printf("Successfully deregister your accout. System will logout");
    return;
}

void mainLoop(void){
    int choice;
    while (1){
        show_1stMenu();
        printf("Welcome to ATM, Please select your work: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                registerAccount();
                break;
            case 2:
                // 登录系统实现
                if(loginAccount() == 1) afterLogin();
                // 退出之后当前用户要设为没有
                curUser = NULL;
                break;
            case 0:
                // 退出系统实现
                system("cls");
                printf("Welcome use ATM again\n");
                return;
                break;
        }
    }
}

void saveData(void){
    FILE *fp = fopen("ATM_data.txt", "w");
    if(fp == NULL){
        printf("ERROR: The data doesn't exist");
        return;
    }
    Account *p = head; // 状态节点表示
    while(p != NULL){
        fprintf(fp, "%s %s %s %.2f\n", 
        p->id, p->password, p->name, p->balance);
        p = p->next;
    }
    fclose(fp);
    printf("Data have been saved successfully\n");
}

void loadData(void){
    FILE *fp = fopen("ATM_data.txt", "r");
    if(fp == NULL){
        return;
    }
    char id[CHAR_MAXSIZE], password[CHAR_MAXSIZE], name[CHAR_MAXSIZE];
    double balance;

    while(fscanf(fp, "%s %s %s %lf", id, password, name, &balance) != EOF){
        addNode(id, password, name, balance);
    }

    fclose(fp);
    printf("Data have been load successfully\n");
}

void freeNode(void){
    Account *p = head; // 状态节点
    if (p == NULL){
        return;
    }
    while(p != NULL){
        Account *temp = p;
        p = p->next;
        free(temp);
    }
    head = NULL;
}

int main(void){
    // 加载数据
    loadData();
    // 加载菜单
    mainLoop();
    // 保存数据
    saveData();
    // 清楚开辟内存区的所有缓存
    freeNode();
}
