#include <iostream> // Подключаем библиотеку для ввода-вывода

template <typename T> // Шаблонный класс, который может работать с любым типом T
// DynamicArray: Реализует динамический массив с методами добавления (push_back), вставки (insert), удаления (erase) и получения размера (size). Метод resize используется для увеличения емкости массива при необходимости.
class DynamicArray {
private:
    T* data; // Указатель на массив элементов типа T
    size_t capacity; // Вместимость массива (максимальное количество элементов)
    size_t length; // Текущая длина массива (количество добавленных элементов)
    
    // Служит для увеличения размера массива, когда размер достигает capacity, он вызывается и увеличивает размер на 50%
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity]; // Создаем новый массив с новой вместимостью
        for (size_t i = 0; i < length; ++i) { // Копируем существующие элементы в новый массив
            new_data[i] = std::move(data[i]); // Перемещаем элементы из старого массива в новый
        }
        delete[] data; // Освобождаем память старого массива
        data = new_data; // Перенаправляем указатель на новый массив
        capacity = new_capacity; // Обновляем вместимость
    }

public:
    // Начальная вместимость равна 10. Это означает, что при создании контейнера выделяется память для хранения 10 элементов типа T
    DynamicArray(size_t initial_capacity = 10) 
        : capacity(initial_capacity), length(0), data(new T[initial_capacity]) {} // Инициализируем поля

    ~DynamicArray() { // Деструктор для освобождения памяти
        delete[] data; // Освобождаем память, выделенную под массив
    }

    DynamicArray(const DynamicArray& other) // Конструктор копирования
        : capacity(other.capacity), length(other.length), data(new T[other.capacity]) { // Копирует данные и выделяет память для нового массива
        for (size_t i = 0; i < length; ++i) { // Копируем элементы из другого массива
            data[i] = other.data[i]; 
        }
    }

    DynamicArray(DynamicArray&& other) noexcept // Конструктор перемещения, который принимает r-value ссылку на другой объект, noexcept - без исключений
        : capacity(other.capacity), length(other.length), data(other.data) { // Перемещаем ресурсы из другого объекта
        other.data = nullptr; // Обновляем ресурсы у перемещаемого объекта 
        other.length = 0; // Обнуляем длину перемещаемого объекта
        other.capacity = 0; // Обнуляем вместимость перемещаемого объекта
    }

    DynamicArray& operator=(const DynamicArray& other) { // Оператор присваивания копирования 
        if (this == &other) return *this; // Проверка для предотвращения ненужного выполнения кода (самоприсваивание)
        delete[] data; // Освобождаем память старого массива
        capacity = other.capacity; // Копируем вместимость
        length = other.length; // Копируем длину
        data = new T[capacity]; // Выделяем память для нового массива
        for (size_t i = 0; i < length; ++i) { // Копируем элементы из другого массива
            data[i] = other.data[i];
        }
        return *this; // Возвращаем текущий объект для цепочки присваиваний
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept { // Оператор присваивания перемещения 
        if (this == &other) return *this; // Проверка на самоприсваивание
        delete[] data; // Освобождаем память старого массива
        capacity = other.capacity; // Перемещаем вместимость из другого объекта
        length = other.length; // Перемещаем длину из другого объекта
        data = other.data; // Перемещаем указатель на массив из другого объекта
        other.data = nullptr; // Обнуляем указатель у перемещаемого объекта
        other.length = 0; // Обнуляем длину перемещаемого объекта
        other.capacity = 0; // Обнуляем вместимость перемещаемого объекта
        return *this; // Возвращаем текущий объект для цепочки присваиваний
    }

    void push_back(const T& value) { // Метод добавления элемента в конец массива
        if (length == capacity) { // Если массив заполнен, нужно увеличить его размер
            resize(capacity + capacity / 2); // Увеличиваем размер массива на 50%
        }
        data[length++] = value; // Добавляем элемент и увеличиваем длину на 1
    }

    void insert(size_t index, const T& value) { // Метод вставки элемента по индексу 
        if (index > length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс
        if (length == capacity) { // Если массив заполнен, нужно увеличить его размер 
            resize(capacity + capacity / 2);
        }
        
        for (size_t i = length; i > index; --i) { // Сдвигаем элементы вправо, чтобы освободить место для нового элемента 
            data[i] = std::move(data[i - 1]); 
        }
        
        data[index] = value; // Вставляем новый элемент по указанному индексу 
        ++length; // Увеличиваем длину на 1 
    }

    void erase(size_t index) { // Метод удаления элемента по индексу 
        if (index >= length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс
        
        for (size_t i = index; i < length - 1; ++i) { // Сдвигаем элементы влево, чтобы удалить элемент по указанному индексу 
            data[i] = std::move(data[i + 1]);
        }
        
        --length; // Уменьшаем длину на 1 
    }

    size_t size() const { return length; } // Метод получения текущего размера массива

    T& operator[](size_t index) { // Оператор доступа к элементам по индексу 
        if (index >= length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс 
        return data[index]; // Возвращаем элемент по указанному индексу 
    }

    void print() const { // Метод для вывода элементов массива на экран 
        for (size_t i = 0; i < length; ++i) { 
            std::cout << data[i] << (i < length - 1 ? ", " : ""); // Выводим элементы через запятую 
        }
        std::cout << std::endl; // Переход на новую строку после вывода всех элементов 
    }
};


template <typename T> // Шаблонный класс, который может работать с любым типом T
class DoublyNode { // Определение класса узла двусвязного списка
public:
    T data; // Данные, хранящиеся в узле
    DoublyNode* next; // Указатель на следующий узел
    DoublyNode* prev; // Указатель на предыдущий узел

    // Конструктор, инициализирующий данные и устанавливающий указатели на nullptr
    DoublyNode(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T> // Шаблонный класс для итератора двусвязного списка
class DoublyLinkedListIterator {
private:
    DoublyNode<T>* current; // Указатель на текущий узел, на который указывает итератор

public:
    // Конструктор итератора, инициализирующий текущий узел
    DoublyLinkedListIterator(DoublyNode<T>* node) : current(node) {}

    // Оператор разыменования для доступа к данным текущего узла
    T& operator*() {
        return current->data; // Возвращаем ссылку на данные текущего узла
    }

    // Префиксный инкремент: перемещает итератор к следующему узлу
    DoublyLinkedListIterator& operator++() {
        if (current) current = current->next; // Если текущий узел не nullptr, переходим к следующему узлу
        return *this; // Возвращаем текущий итератор для цепочки вызовов
    }

    // Префиксный декремент: перемещает итератор к предыдущему узлу
    DoublyLinkedListIterator& operator--() {
        if (current) current = current->prev; // Если текущий узел не nullptr, переходим к предыдущему узлу
        return *this; // Возвращаем текущий итератор для цепочки вызовов
    }

    // Оператор неравенства для сравнения двух итераторов
    bool operator!=(const DoublyLinkedListIterator& other) const {
        return current != other.current; // Возвращает true, если текущие узлы не равны (итераторы указывают на разные места)
    }
};

template <typename T> // Шаблонный класс, который может работать с любым типом T
class DoublyLinkedList { // Определение класса двусвязного списка
private:
    DoublyNode<T>* head; // Указатель на первый элемент списка (голова)
    DoublyNode<T>* tail; // Указатель на последний элемент списка (хвост)
    size_t length; // Размер списка (количество элементов)

public:
    // Конструктор, инициализирующий пустой список
    DoublyLinkedList() : head(nullptr), tail(nullptr), length(0) {}

    // Деструктор для освобождения памяти, выделенной под элементы списка
    ~DoublyLinkedList() {
        while (head) { // Пока голова не равна nullptr
            DoublyNode<T>* temp = head; // Сохраняем текущую голову
            head = head->next; // Перемещаем голову на следующий элемент
            delete temp; // Освобождаем память текущего элемента
        }
    }

    // Метод добавления элемента в конец списка
    void push_back(const T& value) { 
        DoublyNode<T>* newNode = new DoublyNode<T>(value); // Создаем новый узел с переданным значением
        if (!head) { // Если список пуст
            head = tail = newNode; // Устанавливаем новый узел как голову и хвост
        } else { 
            tail->next = newNode; // Устанавливаем указатель next у текущего хвоста на новый узел
            newNode->prev = tail;  // Устанавливаем указатель prev у нового узла на текущий хвост
            tail = newNode; // Обновляем хвост на новый узел
        } 
        ++length; // Увеличиваем размер списка на 1
    }

    // Метод добавления элемента в начало списка
    void push_front(const T& value) { 
        DoublyNode<T>* newNode = new DoublyNode<T>(value); // Создаем новый узел с переданным значением
        if (!head) { // Если список пуст
            head = tail = newNode; // Устанавливаем новый узел как голову и хвост
        } else { 
            newNode->next = head; // Устанавливаем указатель next у нового узла на текущую голову
            head->prev = newNode;  // Устанавливаем указатель prev у текущей головы на новый узел
            head = newNode; // Обновляем голову на новый узел
        } 
        ++length; // Увеличиваем размер списка на 1
    }

    // Метод вставки элемента по индексу
    void insert(size_t index, const T& value) { 
        if (index > length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс
        
        if (index == 0) { // Если индекс равен 0, добавляем элемент в начало
            push_front(value); 
            return; 
        } 
        
        if (index == length) { // Если индекс равен длине, добавляем элемент в конец
            push_back(value); 
            return; 
        } 

        DoublyNode<T>* newNode = new DoublyNode<T>(value); // Создаем новый узел с переданным значением 
        DoublyNode<T>* current = head; // Начинаем с головы

        for (size_t i = 0; i < index; ++i) { // Ищем узел по индексу 
            current = current->next; 
        } 

        newNode->next = current;      // Устанавливаем указатель next у нового узла на текущий узел 
        newNode->prev = current->prev; // Устанавливаем указатель prev у нового узла на предыдущий узел

        if (current->prev) // Если предыдущий узел существует, обновляем его указатель next 
            current->prev->next = newNode;

        current->prev = newNode; // Обновляем указатель prev у текущего узла

        if (index == 0)           // Если вставка происходит в начало, обновляем голову 
            head = newNode;

        ++length; // Увеличиваем размер списка на 1 
    }

    // Метод удаления элемента по индексу 
    void erase(size_t index) { 
        if (index >= length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс 
        
        DoublyNode<T>* current = head; // Начинаем с головы

        for (size_t i = 0; i < index; ++i) // Ищем узел по индексу 
            current = current->next;

        if (current->prev)             // Обновляем указатель next у предыдущего узла, если он существует 
            current->prev->next = current->next;

        if (current->next)             // Обновляем указатель prev у следующего узла, если он существует 
            current->next->prev = current->prev;

        if (current == head)           // Если удаляем голову, обновляем голову на следующий элемент 
            head = current->next;

        if (current == tail)           // Если удаляем хвост, обновляем хвост на предыдущий элемент 
            tail = current->prev;

        delete current;                // Освобождаем память удаляемого узла

        --length;                      // Уменьшаем размер списка на 1 
    }

    // Метод для вывода элементов списка на экран 
    void print() const { 
      DoublyNode<T>* current= head;  // Начинаем с головы 
      while(current != nullptr){      // Пока не достигли конца списка 
          std::cout << current -> data << " ";  // Выводим данные текущего узла 
          current=current -> next;   // Переходим к следующему узлу 
      } 
      std::cout << std::endl;       // Переход на новую строку после вывода всех элементов 
   } 

   size_t getSize() const { return length; }  // Метод получения текущего размера списка 

   DoublyLinkedListIterator<T> begin() { 
      return DoublyLinkedListIterator<T>(head); // Возвращаем итератор на голову списка 
   } 

   DoublyLinkedListIterator<T> end() { 
      return DoublyLinkedListIterator<T>(nullptr); // Возвращаем итератор на nullptr для обозначения конца списка 
   } 

};

template <typename T> // Шаблонный класс, который может работать с любым типом T
class Node { // Определение класса узла для однонаправленного связного списка
public:
    T data; // Данные, хранящиеся в узле
    Node* next; // Указатель на следующий узел

    // Конструктор, инициализирующий данные и устанавливающий указатель на nullptr
    Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T> 
// SinglyLinkedList: Реализует однонаправленный связный список с аналогичными методами. Каждый узел хранит данные и указатель на следующий узел.
class SinglyLinkedListIterator { // Определение класса итератора для однонаправленного связного списка
private:
    Node<T>* current; // Указатель на текущий узел

public:
    // Конструктор итератора, инициализирующий текущий узел
    SinglyLinkedListIterator(Node<T>* node) : current(node) {}

    // Оператор разыменования для доступа к данным текущего узла
    T& operator*() {
        return current->data; // Возвращаем ссылку на данные текущего узла
    }

    // Префиксный инкремент: перемещает итератор к следующему узлу
    SinglyLinkedListIterator& operator++() {
        if (current) current = current->next; // Если текущий узел не nullptr, переходим к следующему узлу
        return *this; // Возвращаем текущий итератор для цепочки вызовов
    }

    // Оператор неравенства для сравнения двух итераторов
    bool operator!=(const SinglyLinkedListIterator& other) const {
        return current != other.current; // Возвращает true, если текущие узлы не равны (итераторы указывают на разные места)
    }
};

template <typename T> // Шаблонный класс для однонаправленного связного списка
class SinglyLinkedList {
private:
    Node<T>* head; // Указатель на первый элемент списка (голова)
    size_t length; // Размер списка (количество элементов)

public:
    // Конструктор, инициализирующий пустой список
    SinglyLinkedList() : head(nullptr), length(0) {}

    // Деструктор для освобождения памяти, выделенной под элементы списка
    ~SinglyLinkedList() {
        while (head) { // Пока голова не равна nullptr
            Node<T>* temp = head; // Сохраняем текущую голову
            head = head->next; // Перемещаем голову на следующий элемент
            delete temp; // Освобождаем память текущего элемента
        }
    }

    // Метод добавления элемента в конец списка
    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value); // Создаем новый узел с переданным значением
        if (!head) { // Если список пуст
            head = newNode; // Устанавливаем новый узел как голову списка
        } else {
            Node<T>* temp = head; // Начинаем с головы списка
            while (temp->next) { // Проходим по списку до последнего узла
                temp = temp->next;
            }
            temp->next = newNode; // Устанавливаем указатель next у последнего узла на новый узел
        }
        ++length; // Увеличиваем размер списка на 1
    }

    // Метод вставки элемента по индексу
    void insert(size_t index, const T& value) {
        if (index > length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс
        
        Node<T>* newNode = new Node<T>(value); // Создаем новый узел с переданным значением
        
        if (index == 0) { // Если индекс равен 0, добавляем элемент в начало списка
            newNode->next = head; // Устанавливаем указатель next у нового узла на текущую голову
            head = newNode; // Обновляем голову на новый узел
        } else {
            Node<T>* temp = head; // Начинаем с головы
            
            for (size_t i = 0; i < index - 1; ++i) { // Ищем узел по индексу 
                temp = temp->next;
            }
            
            newNode->next = temp->next; // Устанавливаем указатель next у нового узла на следующий за текущим узлом 
            temp->next = newNode; // Устанавливаем указатель next у текущего узла на новый узел 
        }
        ++length; // Увеличиваем размер списка на 1
    }

    // Метод удаления элемента по индексу 
    void erase(size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range"); // Проверка на допустимый индекс 
        
        Node<T>* temp = head; // Начинаем с головы
        
        if (index == 0) { // Если удаляем голову списка 
            head = head->next; // Обновляем голову на следующий элемент 
            delete temp; // Освобождаем память удаляемого элемента 
        } else {
            for (size_t i = 0; i < index - 1; ++i) { // Ищем предыдущий элемент по индексу 
                temp = temp->next;
            }
            
            Node<T>* toDelete = temp->next; // Сохраняем указатель на удаляемый элемент 
            temp->next = toDelete->next;  // Обновляем указатель next у предыдущего элемента 
            delete toDelete;               // Освобождаем память удаляемого элемента 
        }
        
        --length;  // Уменьшаем размер списка на 1 
    }

    size_t size() const { return length; }  // Метод получения текущего размера списка 

    SinglyLinkedListIterator<T> begin() { 
      return SinglyLinkedListIterator<T>(head);  // Возвращаем итератор на голову списка 
   } 

   SinglyLinkedListIterator<T> end() { 
      return SinglyLinkedListIterator<T>(nullptr);  // Возвращаем итератор на nullptr для обозначения конца списка 
   } 

   void print() const { 
       Node<T>* temp = head;  // Начинаем с головы 
       while (temp) {          // Пока не достигли конца списка 
           std::cout << temp->data << (temp->next ? ", " : "");  // Выводим данные текущего узла 
           temp = temp->next;  // Переходим к следующему узлу 
       }
       std::cout << std::endl;  // Переход на новую строку после вывода всех элементов 
   }
};
int main() {
    DynamicArray<int> arr;

    
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i);
    }

    std:: cout << "Вывод ожидаемого контейнера: " << '\n';
    arr.print(); 

    // Вывод размера
    std::cout << "Size: " << arr.size() << std::endl; // Size: 10

    // Удаление элементов (удаляется на элемент меньше, т.к. нумеруются с нуля)
    arr.erase(2);  
    arr.erase(4);  
    arr.erase(5);  

    std::cout << "Вывод содержимого на экран " << '\n';
    arr.print();

    arr.insert(0, 10);
    std::cout << "Вывод содержимого с десяткой в начале " << '\n';
    arr.print();  

    arr.insert(4, 20);
    std::cout << "Вывод содержимого с 20 в середине " << '\n';
    arr.print();  

    arr.push_back(30);

    std::cout << "Вывод содержимого с 30 в конце " << '\n';
    arr.print();  
    
    SinglyLinkedList<int> list;

    // Добавление 10 элементов
    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
    }
    std:: cout << "Вывод однонаправленного списка: " << '\n';
    
    list.print();

    list.erase(2);
    list.erase(4);
    list.erase(5);
    
    list.print();

    list.insert(0, 10);
    list.print(); 

    list.insert(4, 20);
    list.print(); 

    list.push_back(30);
    list.print();
    
    DoublyLinkedList<int> doubleList;

    for (int i=0; i<10; ++i){
        doubleList.push_back(i);
    }

    std::cout << "Двунаправленный список:\n";
    doubleList.print();

    doubleList.erase(2); 
    doubleList.insert(4, 20);

    std::cout << "После изменений:\n";
    doubleList.print();

    SinglyLinkedList<int> sing_list_1;
    
    for (int i = 0; i < 10; i++)
    {
     sing_list_1.push_back(i);
    }

    std::cout << "Однонаправленный список:\n";

    for (auto it = sing_list_1.begin(); it != sing_list_1.end(); ++it) {
        std::cout << *it << " "; 
    }

    std::cout << std::endl;

    DoublyLinkedList<int> double_list_1;
    for (int i = 0; i < 10; ++i) {
        double_list_1.push_back(i);
    }

    std::cout << "Двунаправленный список:\n";
    
    for (auto it = double_list_1.begin(); it != double_list_1.end(); ++it) {
        std::cout << *it << " "; 
    }
    
    std::cout << std::endl;

    return 0;
}   