#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_DESC_LENGTH 100
#define MAX_INGREDIENTS 10
#define MAX_ITEMS_IN_ORDER 10
#define CONSOLE_WIDTH 80 // �������̨���Ϊ 80 ���ַ�

// ��Ʒ��Ϣ�ṹ��
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    char description[MAX_DESC_LENGTH];
    char category[MAX_NAME_LENGTH];
    int stock;
    char ingredients[MAX_INGREDIENTS][MAX_NAME_LENGTH]; // ����
    int calories; // ����
    int sold_quantity; // ��������
    float total_sales; // �����۶�
    int discount; // �ۿ�
} Product;

// �����е���Ʒ��ṹ��
typedef struct {
    int product_id;
    int quantity;
} OrderItem;

// ������Ϣ�ṹ��
typedef struct {
    int order_id;
    char customer_info[MAX_NAME_LENGTH];
    time_t order_time;
    int status; // 0: δ���, 1: �����, 2: ��ȡ��
    OrderItem items[MAX_ITEMS_IN_ORDER];
    int num_items;
    int customer_level; // �˿��Ǽ�
} Order;

// ��Ʒ����ڵ�
typedef struct ProductNode {
    Product product;
    struct ProductNode *next;
} ProductNode;

// ��������ڵ�
typedef struct OrderNode {
    Order order;
    struct OrderNode *next;
} OrderNode;

// �û���Ϣ�ṹ��
typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int level; // �û������������Ա����ͨ�û���
} User;

// ȫ�ֱ�������Ʒ����ͷ�ڵ㡢��������ͷ�ڵ���û��б�
ProductNode *productList = NULL;
OrderNode *orderList = NULL;
User userList[] = {{"admin", "admin123", 1}, {"user", "user123", 2}};
int numUsers = sizeof(userList) / sizeof(userList[0]);
int next_order_id = 1;

// ��������
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
    loadOrdersFromFile(); // ���ļ����ض�����Ϣ

    int choice;
    do {
	    printf("\n-- ��¼ --\n");
	    char username[MAX_NAME_LENGTH];
	    char password[MAX_NAME_LENGTH];
	    printf("�û���: ");
	    scanf("%s", username);
	    printf("����: ");
	    scanf("%s", password);
	    int userLevel = authenticateUser(username, password);
	    if (userLevel > 0) {
	        printf("��¼�ɹ���\n");
	        do {
	            displayMenu(userLevel); // �����û�������ʾ�˵�
	            printf("����������ѡ��: ");
	            scanf("%d", &choice);
	            switch (choice) {
	                case 1:
	                    if (userLevel == 1) {
	                        addProduct(); // ֻ�й���Ա���������Ʒ
	                    } else {
	                        printf("��Ȩ��ִ�д˲�����\n");
	                    }
	                    break;
	                case 2:
	                    if (userLevel == 1) {
	                        deleteProduct(); // ֻ�й���Ա����ɾ����Ʒ
	                    } else {
	                        printf("��Ȩ��ִ�д˲�����\n");
	                    }
	                    break;
	                case 3:
	                    if (userLevel == 1) {
	                        updateProduct(); // ֻ�й���Ա���Ը�����Ʒ
	                    } else {
	                        printf("��Ȩ��ִ�д˲�����\n");
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
	                    printf("�˳�...\n");
	                    break;
	                default:
	                    printf("��Чѡ�������� 1 �� 10 ֮������֡�\n");
	            }
	        } while (choice != 10);
	    } else {
	        printf("�û�����������������µ�¼��\n");
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

// ��ʾ�˵�ѡ��
void displayMenu(int userLevel) {
    printf("\n-- ���˵� --\n");
    printf("1. �����Ʒ\n");
    printf("2. ɾ����Ʒ\n");
    printf("3. ������Ʒ��Ϣ\n");
    printf("4. �鿴������Ʒ\n");
    printf("5. ��������\n");
    printf("6. ȡ������\n");
    printf("7. ��ɶ���\n");
    printf("8. �鿴���ж���\n");
    printf("9. ����ͳ��\n");
    printf("10. �˳�\n");
    if (userLevel == 1) {
        printf("ע��: ���ϲ������޹���Ա��\n");
    }
}


// ��ʾ������Ʒ��Ϣ
void displayProduct(Product product) {
    printf("================================\n");
    printf("��ƷID: %d\n", product.id);
    printf("����: %s\n", product.name);
    printf("�۸�: %.2f\n", product.price);
    printf("����: %s\n", product.description);
    printf("����: %s\n", product.category);
    printf("���: %d\n", product.stock);
    printf("����: ");
    for (int i = 0; i < MAX_INGREDIENTS && product.ingredients[i][0] != '\0'; i++) {
        printf("%s ", product.ingredients[i]);
    }
    printf("\n");
    printf("����: %d\n", product.calories);
    printf("================================\n");
}

// �����Ʒ��Ϣ
void addProduct() {
    Product product;
    printf("��������ƷID: ");
    scanf("%d", &product.id);
    printf("��������Ʒ����: ");
    scanf("%s", product.name);
    printf("��������Ʒ�۸�: ");
    scanf("%f", &product.price);
    printf("��������Ʒ����: ");
    scanf("%s", product.description);
    printf("��������Ʒ����: ");
    scanf("%s", product.category);
    printf("��������Ʒ���: ");
    scanf("%d", &product.stock);
    printf("��������Ʒ����: ");
    scanf("%d", &product.calories);

    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        printf("���������� (����'0'����): ");
        scanf("%s", product.ingredients[i]);
        if (strcmp(product.ingredients[i], "0") == 0) {
            product.ingredients[i][0] = '\0'; // ������������
            break;
        }
    }

    ProductNode *newNode = (ProductNode*)malloc(sizeof(ProductNode));
    newNode->product = product;
    newNode->next = productList;
    productList = newNode;

    printf("��Ʒ��ӳɹ���\n");
}

// ɾ����Ʒ��Ϣ
void deleteProduct() {
    int id;
    printf("������Ҫɾ������ƷID: ");
    scanf("%d", &id);

    ProductNode *prev = NULL;
    ProductNode *current = productList;
    while (current != NULL && current->product.id != id) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("δ�ҵ���Ʒ��\n");
        return;
    }
    if (prev == NULL) {
        productList = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    printf("��ƷID %d ɾ���ɹ���\n", id);
}

// �޸���Ʒ��Ϣ
void updateProduct() {
    int id;
    printf("������Ҫ�޸ĵ���ƷID: ");
    scanf("%d", &id);

    Product *product = findProduct(id);
    if (product == NULL) {
        printf("δ�ҵ���Ʒ��\n");
        return;
    }

    int choice;
    do {
        printf("\n-- �޸���Ʒ��Ϣ --\n");
        printf("1. �޸�����\n");
        printf("2. �޸ļ۸�\n");
        printf("3. �޸�����\n");
        printf("4. �޸ķ���\n");
        printf("5. �޸Ŀ��\n");
        printf("6. �޸�����\n");
        printf("7. �޸�����\n");
        printf("8. ����޸�\n");
        printf("����������ѡ��: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("������������: ");
                scanf("%s", product->name);
                break;
            case 2:
                printf("�������¼۸�: ");
                scanf("%f", &product->price);
                break;
            case 3:
                printf("������������: ");
                scanf("%s", product->description);
                break;
            case 4:
                printf("�������·���: ");
                scanf("%s", product->category);
                break;
            case 5:
                printf("�������¿��: ");
                scanf("%d", &product->stock);
                break;
            case 6:
                printf("������������: ");
                scanf("%d", &product->calories);
                break;
            case 7:
                for (int i = 0; i < MAX_INGREDIENTS; i++) {
                    printf("������������ (����'0'����): ");
                    scanf("%s", product->ingredients[i]);
                    if (strcmp(product->ingredients[i], "0") == 0) {
                        product->ingredients[i][0] = '\0'; // ������������
                        break;
                    }
                }
                break;
            case 8:
                printf("�޸���ɡ�\n");
                break;
            default:
                printf("��Чѡ�������� 1 �� 8 ֮������֡�\n");
        }
    } while (choice != 8);

    printf("��Ʒ���³ɹ���\n");
}

// ������Ʒ��Ϣ
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

// �г�������Ʒ��Ϣ
void listAllProducts() {
    ProductNode *current = productList;
    printf("\n-- ������Ʒ��Ϣ --\n");
    while (current != NULL) {
        displayProduct(current->product);
        current = current->next;
    }
}

// ��������
void createOrder() {
    Order order;
    order.order_id = next_order_id++;
    printf("������˿���Ϣ: ");
    scanf("%s", order.customer_info);
    order.order_time = time(NULL);
    order.status = 0; // 0: δ���
    order.num_items = 0;

    int product_id, quantity;
    while (1) {
        printf("��������ƷID (����-1����): ");
        scanf("%d", &product_id);
        if (product_id == -1) {
            break;
        }
        Product *product = findProduct(product_id);
        if (product == NULL) {
            printf("��ƷID %d �����ڣ����������롣\n", product_id);
            continue;
        }
        printf("����������: ");
        scanf("%d", &quantity);
        order.items[order.num_items].product_id = product_id;
        order.items[order.num_items].quantity = quantity;
        order.num_items++;
    }

    OrderNode *newNode = (OrderNode*)malloc(sizeof(OrderNode));
    newNode->order = order;
    newNode->next = orderList;
    orderList = newNode;

    printf("���������ɹ�������ID: %d\n", order.order_id);
}

// ȡ������
void cancelOrder() {
    int order_id;
    printf("������Ҫȡ���Ķ���ID: ");
    scanf("%d", &order_id);

    OrderNode *current = orderList;
    while (current != NULL && current->order.order_id != order_id) {
        current = current->next;
    }
    if (current == NULL) {
        printf("δ�ҵ�������\n");
        return;
    }
    if (current->order.status == 2) {
        printf("������ȡ����\n");
        return;
    }

    current->order.status = 2; // 2: ��ȡ��
    printf("����ID %d ȡ���ɹ���\n", order_id);
}

// ��ɶ���
void completeOrder() {
    int order_id;
    printf("������Ҫ��ɵĶ���ID: ");
    scanf("%d", &order_id);

    OrderNode *current = orderList;
    while (current != NULL && current->order.order_id != order_id) {
        current = current->next;
    }
    if (current == NULL) {
        printf("δ�ҵ�������\n");
        return;
    }
    if (current->order.status == 2) {
        printf("�޷������ȡ���Ķ�����\n");
        return;
    }

    current->order.status = 1; // 1: �����
    printf("����ID %d ��ɳɹ���\n", order_id);
}

// ��ʾ����������Ϣ
void displayOrder(Order order) {
    printf("����ID: %d\n", order.order_id);
    printf("�˿���Ϣ: %s\n", order.customer_info);
    printf("����ʱ��: %s", ctime(&order.order_time));
    printf("����״̬: %s\n", order.status == 0 ? "δ���" : (order.status == 1 ? "�����" : "��ȡ��"));
    printf("��Ʒ��:\n");
    for (int i = 0; i < order.num_items; i++) {
        printf("  ��ƷID: %d, ����: %d\n", order.items[i].product_id, order.items[i].quantity);
    }
    printf("--------------------------------\n");
}

// �г����ж�����Ϣ
void listAllOrders() {
    OrderNode *current = orderList;
    printf("\n-- ���ж�����Ϣ --\n");
    while (current != NULL) {
        displayOrder(current->order);
        current = current->next;
    }
}

// ���涩����Ϣ���ļ�
void saveOrdersToFile() {
    FILE *file = fopen("orders.txt", "w");
    if (file == NULL) {
        printf("�޷����ļ���\n");
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
    printf("������Ϣ�ѱ��浽�ļ���\n");
}

// ���ļ����ض�����Ϣ
void loadOrdersFromFile() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        return; // �ļ�������������
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
// ����ͳ��
void salesStatistics() {
    float total_sales = 0;
    printf("\n-- ����ͳ�� --\n");
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
    printf("�����۶�: %.2f\n", total_sales);
    printf("���������Ѹ��¡�\n");
}

// ��������ͳ����Ϣ���ļ�
void saveSalesToFile() {
    FILE *file = fopen("sales.txt", "w");
    if (file == NULL) {
        printf("�޷����ļ���\n");
        return;
    }
    ProductNode *current = productList;
    while (current != NULL) {
        Product product = current->product;
        fprintf(file, "%d %d %.2f %.2f\n", product.id, product.sold_quantity, product.price, product.total_sales);
        current = current->next;
    }
    fclose(file);
    printf("����ͳ����Ϣ�ѱ��浽�ļ���\n");
}
// �û���֤����
int authenticateUser(const char *username, const char *password) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(username, userList[i].username) == 0 && strcmp(password, userList[i].password) == 0) {
            return 1; // ��֤�ɹ�
        }
    }
    return 0; // ��֤ʧ��
}
