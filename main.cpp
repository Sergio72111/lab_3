#include <iostream>
#include <memory>
#include <stdexcept>

template<typename T>
class DynamicArray {
private:
    std::unique_ptr<T[]> data;
    size_t capacity;
    size_t size;

    void resize(size_t new_capacity) {
        std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = std::move(data[i]);
        }
        data = std::move(new_data);
        capacity = new_capacity;
    }

public:
    DynamicArray() : capacity(2), size(0) {
        data = std::make_unique<T[]>(capacity);
    }

    void push_back(T value) {
        if (size == capacity) {
            resize(static_cast<size_t>(capacity * 1.5));
        }
        data[size++] = std::move(value);
    }

    void insert(size_t index, T value) {
        if (index > size) throw std::out_of_range("Index out of range");
        if (size == capacity) {
            resize(static_cast<size_t>(capacity * 1.5));
        }
        for (size_t i = size; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = std::move(value);
        ++size;
    }

    void erase(size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --size;
    }

    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    size_t get_size() const { return size; }
};
template<typename T>
class Node {
public:
    T value;
    Node* next;

    Node(T val) : value(val), next(nullptr) {}
};

template<typename T>
class SinglyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr) {}

    void push_back(T value) {
        Node<T>* new_node = new Node<T>(value);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void insert(size_t index, T value) {
        if (index == 0) {
            Node<T>* new_node = new Node<T>(value);
            new_node->next = head;
            head = new_node;
            if (!tail) tail = new_node;
            return;
        }

        Node<T>* current = head;
        for (size_t i = 0; i < index - 1 && current != nullptr; ++i) {
            current = current->next;
        }
        
        if (!current) throw std::out_of_range("Index out of range");

        Node<T>* new_node = new Node<T>(value);
        new_node->next = current->next;
        current->next = new_node;

        if (!new_node->next) tail = new_node; // Update tail if needed
    }

    void erase(size_t index) {
        if (index == 0 && head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node<T>* current = head;
        for (size_t i = 0; i < index - 1 && current != nullptr; ++i) {
            current = current->next;
        }

        if (!current || !current->next) throw std::out_of_range("Index out of range");

        Node<T>* temp = current->next;
        current->next = temp->next;

        if (!current->next) tail = current; // Update tail if needed
        delete temp;
    }

    class Iterator {
      private:
         Node<T>* current;

      public:
         Iterator(Node<T>* node): current(node){}

         T& operator*(){ return current->value; }

         Iterator& operator++() { 
             current=current->next; 
             return *this; 
         }

         bool operator!=(const Iterator& other){ 
             return current!=other.current; 
         }
   };

   Iterator begin() { return Iterator(head); }
   
   Iterator end() { return Iterator(nullptr); }
};
template<typename T>
class DoublyNode {
public:
    T value;
    DoublyNode* next;
    DoublyNode* prev;

    DoublyNode(T val): value(val), next(nullptr), prev(nullptr){}
};

template<typename T>
class DoublyLinkedList {
private:
    DoublyNode<T>* head;
    DoublyNode<T>* tail;

public:
    DoublyLinkedList(): head(nullptr), tail(nullptr){}

    void push_back(T value){
       DoublyNode<T>* new_node=new DoublyNode<T>(value);
       if(!head){
           head=tail=new_node;
           return;
       }
       tail->next=new_node;
       new_node->prev=tail;
       tail=new_node;
   }

   void insert(size_t index, T value){
       // Аналогично реализации для однонаправленного списка
       // Добавьте код для вставки элемента по индексу
   }

   void erase(size_t index){
       // Аналогично реализации для однонаправленного списка
       // Добавьте код для удаления элемента по индексу
   }

   class Iterator{
      private:
         DoublyNode<T>* current;

      public:
         Iterator(DoublyNode<T>* node):current(node){}

         T& operator*(){return current->value;}

         Iterator& operator++(){
             current=current->next; 
             return *this; 
         }

         bool operator!=(const Iterator& other){ 
             return current!=other.current; 
         }
   };

   Iterator begin() { return Iterator(head); }
   
   Iterator end() { return Iterator(nullptr); }
};
int main() {
    
   // Тестирование последовательного контейнера
   DynamicArray<int> arr;

   for (int i = 0; i < 10; ++i){
       arr.push_back(i);
   }

   // Вывод содержимого контейнера
   for (size_t i=0;i<arr.get_size();++i){
       std::cout << arr[i] << " ";
   }
   
   std::cout << "\nРазмер: " << arr.get_size() << "\n";

   // Удаление третьего, пятого и седьмого элементов
   arr.erase(2); // Удаление третьего элемента
   arr.erase(3); // Удаление пятого элемента (после удаления третьего)
   arr.erase(4); // Удаление седьмого элемента (после двух удалений)

   // Вывод содержимого после удаления
   for (size_t i=0;i<arr.get_size();++i){
       std::cout << arr[i] << " ";
   }
   
   std::cout << "\n";

   // Добавление элемента 10 в начало контейнера
   arr.insert(0, 10);

   // Вывод содержимого после вставки в начало
   for (size_t i=0;i<arr.get_size();++i){
       std::cout << arr[i] << " ";
   }
   
   std::cout << "\n";

   // Добавление элемента 20 в середину контейнера
   arr.insert(4, 20);

   // Вывод содержимого после вставки в середину
   for (size_t i=0;i<arr.get_size();++i){
       std::cout << arr[i] << " ";
   }
   
   std::cout << "\n";

   // Добавление элемента 30 в конец контейнера
   arr.push_back(30);

   // Вывод содержимого после вставки в конец
   for (size_t i=0;i<arr.get_size();++i){
       std::cout << arr[i] << " ";
   }
   
   std::cout << "\n";

}
