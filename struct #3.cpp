#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Реализация с использованием вектора
vector<int> replaceWithNextLarger(const vector<int>& A) {
    int N = A.size();
    vector<int> result(N, 0); // Инициализируем результат нулями
    vector<int> nextLarger(N, -1); // Массив для хранения индексов ближайших больших элементов

    // Находим ближайший больший элемент для каждого элемента
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (A[j] > A[i]) {
                nextLarger[i] = j; // Запоминаем индекс ближайшего большего элемента
                break; // Переходим к следующему элементу, так как нашли ближайший
            }
        }
    }

    // Заменяем элементы в результирующем массиве
    for (int i = 0; i < N; ++i) {
        if (nextLarger[i] != -1) { // Если есть ближайший больший элемент
            result[i] = A[nextLarger[i]];
        }
    }

    return result;
}

// Реализация с использованием связанного списка
struct Node {
    int data;
    Node* next;
};

// Функция для создания нового узла
Node* newNode(int data) {
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

// Функция для вставки нового узла в начало списка
Node* insertAtBeginning(Node* head, int data) {
    Node* newNode1 = newNode(data);
    newNode1->next = head;
    return newNode1;
}

// Функция для печати списка
void printList(Node* head) {
    while (head != nullptr) {
        std::cout << head->data << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

// Функция для замены элементов на ближайший больший элемент
Node* replaceWithNextGreater(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head; // Пустой список или список с одним элементом
    }

    Node* current = head;
    Node* prev = nullptr;
    Node* nextGreater = nullptr;

    while (current != nullptr) {
        // Ищем следующий больший элемент
        nextGreater = current->next;
        while (nextGreater != nullptr && nextGreater->data <= current->data) {
            nextGreater = nextGreater->next;
        }

        // Заменяем значение
        if (nextGreater != nullptr) {
            current->data = nextGreater->data;
        }
        else {
            current->data = 0;
        }

        // Переход к следующему узлу
        prev = current;
        current = current->next;
    }

    return head;
}

// Реализация с использованием массива
void replaceWithNextGreater(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int nextGreater = -1; // Инициализируем значение nextGreater как -1

        // Ищем следующий больший элемент
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[i]) {
                nextGreater = arr[j];
                break; // Прерываем цикл, как только нашли nextGreater
            }
        }

        // Заменяем элемент на nextGreater или на 0, если его нет
        if (nextGreater != -1) {
            arr[i] = nextGreater;
        }
        else {
            arr[i] = 0;
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    // Исходные данные
    vector<int> A = { 1, 3, 2, 5, 3, 4 };
    int arr[] = { 1, 3, 2, 5, 3, 4 };
    int n = sizeof(arr) / sizeof(arr[0]);

    // Время выполнения для вектора
    auto start_vector = chrono::high_resolution_clock::now();
    vector<int> result_vector = replaceWithNextLarger(A);
    auto end_vector = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_vector = end_vector - start_vector;

    // Время выполнения для связанного списка
    Node* head = newNode(4);
    head = insertAtBeginning(head, 3);
    head = insertAtBeginning(head, 5);
    head = insertAtBeginning(head, 2);
    head = insertAtBeginning(head, 3);
    head = insertAtBeginning(head, 1);

    auto start_list = chrono::high_resolution_clock::now();
    head = replaceWithNextGreater(head);
    auto end_list = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_list = end_list - start_list;

    // Время выполнения для массива
    auto start_array = chrono::high_resolution_clock::now();
    replaceWithNextGreater(arr, n);
    auto end_array = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_array = end_array - start_array;

    // Вывод результатов
    cout << "Результат для вектора: ";
    for (int i : result_vector) {
        cout << i << " ";
    }
    cout << endl;

    cout << "Результат для связанного списка: ";
    printList(head);

    cout << "Результат для массива: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "Время выполнения для вектора: " << duration_vector.count() << " секунд" << endl;
    cout << "Время выполнения для связанного списка: " << duration_list.count() << " секунд" << endl;
    cout << "Время выполнения для массива: " << duration_array.count() << " секунд" << endl;

    return 0;
}