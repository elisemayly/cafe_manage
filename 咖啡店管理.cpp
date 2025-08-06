#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_DESC_LENGTH 100
#define MAX_INGREDIENTS 10
#define MAX_ITEMS_IN_ORDER 10
#define CONSOLE_WIDTH 80 // 假设控制台宽度为 80 个字符

// 商品信息结构体
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    char description[MAX_DESC_LENGTH];
    char category[MAX_NAME_LENGTH];
    int stock;
    char ingredients[MAX_INGREDIENTS][MAX_NAME_LENGTH]; // 配料
    int calories; // 热量
    int sold_quantity; // 已售数量
    float total_sales; // 总销售额
    int discount; // 折扣
} Product;

// 订单中的商品项结构体
typedef struct {
    int product_id;
    int quantity;
} OrderItem;

// 订单信息结构体
typedef struct {
    int order_id;
    char customer_info[MAX_NAME_LENGTH];
    time_t order_time;
    int status; // 0: 未完成, 1: 已完成, 2: 已取消
    OrderItem items[MAX_ITEMS_IN_ORDER];
    int num_items;
    int customer_level; // 顾客星级
} Order;

// 商品链表节点
typedef struct ProductNode {
    Product product;
    struct ProductNode *next;
} ProductNode;

// 订单链表节点
typedef struct OrderNode {
    Order order;
    struct OrderNode *next;
} OrderNode;

// 用户信息结构体
typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int level; // 用户级别，例如管理员、普通用户等
} User;

// 全局变量：商品链表头节点、订单链表头节点和用户列表
ProductNode *productList = NULL;
OrderNode *orderList = NULL;
User userList[] = {{"admin", "admin123", 1}, {"user", "user123", 2}};
int numUsers = sizeof(userList) / sizeof(userList[0]);
int next_order_id = 1;

// 函数声明
void displayMenu();
void displayProduct(Product product);
void displayOrder(Order order);
void addProduct();
void deleteProduct();
void updateProduct();
Product* findProduct(int id);
void listAllProducts();
void createOrder();
void cancelOrder();
void completeOrder();
void listAllOrders();
void saveOrdersToFile();
void loadOrdersFromFile();
void printCentered(const char *text);
void salesStatistics();
void saveSalesToFile();
int authenticateUser(const char *username, const char *password);

int main() {
    loadOrdersFromFile(); // 从文件加载订单信息

    int choice;
    do {
	    printf("\n-- 登录 --\n");
	    char username[MAX_NAME_LENGTH];
	    char password[MAX_NAME_LENGTH];
	    printf("用户名: ");
	    scanf("%s", username);
	    printf("密码: ");
	    scanf("%s", password);
	    int userLevel = authenticateUser(username, password);
	    if (userLevel > 0) {
	        printf("登录成功！\n");
	        do {
	            displayMenu(userLevel); // 根据用户级别显示菜单
	            printf("请输入您的选择: ");
	            scanf("%d", &choice);
	            switch (choice) {
	                case 1:
	                    if (userLevel == 1) {
	                        addProduct(); // 只有管理员可以添加商品
	                    } else {
	                        printf("无权限执行此操作！\n");
	                    }
	                    break;
	                case 2:
	                    if (userLevel == 1) {
	                        deleteProduct(); // 只有管理员可以删除商品
	                    } else {
	                        printf("无权限执行此操作！\n");
	                    }
	                    break;
	                case 3:
	                    if (userLevel == 1) {
	                        updateProduct(); // 只有管理员可以更新商品
	                    } else {
	                        printf("无权限执行此操作！\n");
	                    }
	                    break;
	                case 4:
	                    listAllProducts();
	                    break;
	                case 5:
	                    createOrder();
	                    break;
	                case 6:
	                    cancelOrder();
	                    break;
	                case 7:
	                    completeOrder();
	                    break;
	                case 8:
	                    listAllOrders();
	                    break;
	                case 9:
	                    salesStatistics();
	                    break;
	                case 10:
	                    saveOrdersToFile();
	                    saveSalesToFile();
	                    printf("退出...\n");
	                    break;
	                default:
	                    printf("无效选择！请输入 1 到 10 之间的数字。\n");
	            }
	        } while (choice != 10);
	    } else {
	        printf("用户名或密码错误，请重新登录。\n");
	    }
	} while (1);

    return 0;
}

void printCentered(const char *text) {
    int len = strlen(text);
    int spaces = (CONSOLE_WIDTH - len) / 2;
    for (int i = 0; i < spaces; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// 显示菜单选项
void displayMenu(int userLevel) {
    printf("\n-- 主菜单 --\n");
    printf("1. 添加商品\n");
    printf("2. 删除商品\n");
    printf("3. 更新商品信息\n");
    printf("4. 查看所有商品\n");
    printf("5. 创建订单\n");
    printf("6. 取消订单\n");
    printf("7. 完成订单\n");
    printf("8. 查看所有订单\n");
    printf("9. 财务统计\n");
    printf("10. 退出\n");
    if (userLevel == 1) {
        printf("注意: 以上操作仅限管理员。\n");
    }
}


// 显示单个商品信息
void displayProduct(Product product) {
    printf("================================\n");
    printf("商品ID: %d\n", product.id);
    printf("名称: %s\n", product.name);
    printf("价格: %.2f\n", product.price);
    printf("描述: %s\n", product.description);
    printf("分类: %s\n", product.category);
    printf("库存: %d\n", product.stock);
    printf("配料: ");
    for (int i = 0; i < MAX_INGREDIENTS && product.ingredients[i][0] != '\0'; i++) {
        printf("%s ", product.ingredients[i]);
    }
    printf("\n");
    printf("热量: %d\n", product.calories);
    printf("================================\n");
}

// 添加商品信息
void addProduct() {
    Product product;
    printf("请输入商品ID: ");
    scanf("%d", &product.id);
    printf("请输入商品名称: ");
    scanf("%s", product.name);
    printf("请输入商品价格: ");
    scanf("%f", &product.price);
    printf("请输入商品描述: ");
    scanf("%s", product.description);
    printf("请输入商品分类: ");
    scanf("%s", product.category);
    printf("请输入商品库存: ");
    scanf("%d", &product.stock);
    printf("请输入商品热量: ");
    scanf("%d", &product.calories);

    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        printf("请输入配料 (输入'0'结束): ");
        scanf("%s", product.ingredients[i]);
        if (strcmp(product.ingredients[i], "0") == 0) {
            product.ingredients[i][0] = '\0'; // 结束配料输入
            break;
        }
    }

    ProductNode *newNode = (ProductNode*)malloc(sizeof(ProductNode));
    newNode->product = product;
    newNode->next = productList;
    productList = newNode;

    printf("商品添加成功！\n");
}

// 删除商品信息
void deleteProduct() {
    int id;
    printf("请输入要删除的商品ID: ");
    scanf("%d", &id);

    ProductNode *prev = NULL;
    ProductNode *current = productList;
    while (current != NULL && current->product.id != id) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("未找到商品！\n");
        return;
    }
    if (prev == NULL) {
        productList = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    printf("商品ID %d 删除成功！\n", id);
}

// 修改商品信息
void updateProduct() {
    int id;
    printf("请输入要修改的商品ID: ");
    scanf("%d", &id);

    Product *product = findProduct(id);
    if (product == NULL) {
        printf("未找到商品！\n");
        return;
    }

    int choice;
    do {
        printf("\n-- 修改商品信息 --\n");
        printf("1. 修改名称\n");
        printf("2. 修改价格\n");
        printf("3. 修改描述\n");
        printf("4. 修改分类\n");
        printf("5. 修改库存\n");
        printf("6. 修改热量\n");
        printf("7. 修改配料\n");
        printf("8. 完成修改\n");
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入新名称: ");
                scanf("%s", product->name);
                break;
            case 2:
                printf("请输入新价格: ");
                scanf("%f", &product->price);
                break;
            case 3:
                printf("请输入新描述: ");
                scanf("%s", product->description);
                break;
            case 4:
                printf("请输入新分类: ");
                scanf("%s", product->category);
                break;
            case 5:
                printf("请输入新库存: ");
                scanf("%d", &product->stock);
                break;
            case 6:
                printf("请输入新热量: ");
                scanf("%d", &product->calories);
                break;
            case 7:
                for (int i = 0; i < MAX_INGREDIENTS; i++) {
                    printf("请输入新配料 (输入'0'结束): ");
                    scanf("%s", product->ingredients[i]);
                    if (strcmp(product->ingredients[i], "0") == 0) {
                        product->ingredients[i][0] = '\0'; // 结束配料输入
                        break;
                    }
                }
                break;
            case 8:
                printf("修改完成。\n");
                break;
            default:
                printf("无效选择，请输入 1 到 8 之间的数字。\n");
        }
    } while (choice != 8);

    printf("商品更新成功！\n");
}

// 查找商品信息
Product* findProduct(int id) {
    ProductNode *current = productList;
    while (current != NULL && current->product.id != id) {
        current = current->next;
    }
    if (current == NULL) {
        return NULL;
    }
    return &(current->product);
}

// 列出所有商品信息
void listAllProducts() {
    ProductNode *current = productList;
    printf("\n-- 所有商品信息 --\n");
    while (current != NULL) {
        displayProduct(current->product);
        current = current->next;
    }
}

// 创建订单
void createOrder() {
    Order order;
    order.order_id = next_order_id++;
    printf("请输入顾客信息: ");
    scanf("%s", order.customer_info);
    order.order_time = time(NULL);
    order.status = 0; // 0: 未完成
    order.num_items = 0;

    int product_id, quantity;
    while (1) {
        printf("请输入商品ID (输入-1结束): ");
        scanf("%d", &product_id);
        if (product_id == -1) {
            break;
        }
        Product *product = findProduct(product_id);
        if (product == NULL) {
            printf("商品ID %d 不存在，请重新输入。\n", product_id);
            continue;
        }
        printf("请输入数量: ");
        scanf("%d", &quantity);
        order.items[order.num_items].product_id = product_id;
        order.items[order.num_items].quantity = quantity;
        order.num_items++;
    }

    OrderNode *newNode = (OrderNode*)malloc(sizeof(OrderNode));
    newNode->order = order;
    newNode->next = orderList;
    orderList = newNode;

    printf("订单创建成功！订单ID: %d\n", order.order_id);
}

// 取消订单
void cancelOrder() {
    int order_id;
    printf("请输入要取消的订单ID: ");
    scanf("%d", &order_id);

    OrderNode *current = orderList;
    while (current != NULL && current->order.order_id != order_id) {
        current = current->next;
    }
    if (current == NULL) {
        printf("未找到订单！\n");
        return;
    }
    if (current->order.status == 2) {
        printf("订单已取消！\n");
        return;
    }

    current->order.status = 2; // 2: 已取消
    printf("订单ID %d 取消成功！\n", order_id);
}

// 完成订单
void completeOrder() {
    int order_id;
    printf("请输入要完成的订单ID: ");
    scanf("%d", &order_id);

    OrderNode *current = orderList;
    while (current != NULL && current->order.order_id != order_id) {
        current = current->next;
    }
    if (current == NULL) {
        printf("未找到订单！\n");
        return;
    }
    if (current->order.status == 2) {
        printf("无法完成已取消的订单！\n");
        return;
    }

    current->order.status = 1; // 1: 已完成
    printf("订单ID %d 完成成功！\n", order_id);
}

// 显示单个订单信息
void displayOrder(Order order) {
    printf("订单ID: %d\n", order.order_id);
    printf("顾客信息: %s\n", order.customer_info);
    printf("订单时间: %s", ctime(&order.order_time));
    printf("订单状态: %s\n", order.status == 0 ? "未完成" : (order.status == 1 ? "已完成" : "已取消"));
    printf("商品项:\n");
    for (int i = 0; i < order.num_items; i++) {
        printf("  商品ID: %d, 数量: %d\n", order.items[i].product_id, order.items[i].quantity);
    }
    printf("--------------------------------\n");
}

// 列出所有订单信息
void listAllOrders() {
    OrderNode *current = orderList;
    printf("\n-- 所有订单信息 --\n");
    while (current != NULL) {
        displayOrder(current->order);
        current = current->next;
    }
}

// 保存订单信息到文件
void saveOrdersToFile() {
    FILE *file = fopen("orders.txt", "w");
    if (file == NULL) {
        printf("无法打开文件！\n");
        return;
    }
    OrderNode *current = orderList;
    while (current != NULL) {
        Order order = current->order;
        fprintf(file, "%d %s %ld %d %d\n", order.order_id, order.customer_info, order.order_time, order.status, order.num_items);
        for (int i = 0; i < order.num_items; i++) {
            fprintf(file, "%d %d\n", order.items[i].product_id, order.items[i].quantity);
        }
        current = current->next;
    }
    fclose(file);
    printf("订单信息已保存到文件。\n");
}

// 从文件加载订单信息
void loadOrdersFromFile() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        return; // 文件不存在则跳过
    }
    while (!feof(file)) {
        Order order;
        fscanf(file, "%d %s %ld %d %d", &order.order_id, order.customer_info, &order.order_time, &order.status, &order.num_items);
        for (int i = 0; i < order.num_items; i++) {
            fscanf(file, "%d %d", &order.items[i].product_id, &order.items[i].quantity);
        }
        OrderNode *newNode = (OrderNode*)malloc(sizeof(OrderNode));
        newNode->order = order;
        newNode->next = orderList;
        orderList = newNode;
        if (order.order_id >= next_order_id) {
            next_order_id = order.order_id + 1;
        }
    }
    fclose(file);
}
// 销售统计
void salesStatistics() {
    float total_sales = 0;
    printf("\n-- 销售统计 --\n");
    OrderNode *current = orderList;
    while (current != NULL) {
        Order order = current->order;
        for (int i = 0; i < order.num_items; i++) {
            int product_id = order.items[i].product_id;
            int quantity = order.items[i].quantity;
            Product *product = findProduct(product_id);
            if (product != NULL) {
                product->sold_quantity += quantity;
                product->total_sales += quantity * product->price;
                total_sales += quantity * product->price;
            }
        }
        current = current->next;
    }
    printf("总销售额: %.2f\n", total_sales);
    printf("销售详情已更新。\n");
}

// 保存销售统计信息到文件
void saveSalesToFile() {
    FILE *file = fopen("sales.txt", "w");
    if (file == NULL) {
        printf("无法打开文件！\n");
        return;
    }
    ProductNode *current = productList;
    while (current != NULL) {
        Product product = current->product;
        fprintf(file, "%d %d %.2f %.2f\n", product.id, product.sold_quantity, product.price, product.total_sales);
        current = current->next;
    }
    fclose(file);
    printf("销售统计信息已保存到文件。\n");
}
// 用户认证函数
int authenticateUser(const char *username, const char *password) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(username, userList[i].username) == 0 && strcmp(password, userList[i].password) == 0) {
            return 1; // 认证成功
        }
    }
    return 0; // 认证失败
}
