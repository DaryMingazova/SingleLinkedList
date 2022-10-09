#pragma once
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>


template <typename Type>
class SingleLinkedList {
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
 
    Node head_;
	Node *tail_;
    size_t size_ = 0;
public:
    // Шаблон класса «Базовый Итератор»
    // определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
	private:
        // Разрешаем SingleLinkedList обращаться к приватной области
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) : node_(node) {}
        Node* node_ = nullptr;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
 
		BasicIterator() = default;
 
		// Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }
 
		// Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;
 
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {return node_ == rhs.node_;}
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {return node_ != rhs.node_;}
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {return node_ == rhs.node_;}
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {return node_ != rhs.node_;}
 
        BasicIterator& operator++() noexcept { 
            if(node_ != nullptr) {
                node_ = node_->next_node;
            }
            return *this; 
        }
        BasicIterator operator++(int) noexcept { 
            auto old_value(*this); // Сохраняем прежнее значение объекта для последующего возврата
            ++(*this); // используем логику префиксной формы инкремента
            return old_value;
        }
 
        [[nodiscard]] reference operator*() const noexcept { return node_->value;}
        [[nodiscard]] pointer operator->() const noexcept { return &node_->value;}
 
    };
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
 
    SingleLinkedList(std::initializer_list<Type> values) {
        auto it = values.end();
		it--;
		while(it != values.begin()) {
			PushFront(*it);
			--it;
		}
		PushFront(*it);
    }
 
    SingleLinkedList(const SingleLinkedList& other) {
        // Реализуйте конструктор самостоятельно
		assert(size_ == 0 && head_.next_node == nullptr);
		
        SingleLinkedList temp_list;
 
        for(auto it_begin = other.begin(); it_begin != other.end(); it_begin++) {
            temp_list.push_back(*it_begin);
        }
        this->swap(temp_list);
    }
 
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(begin() != rhs.begin()) {
           auto lst(rhs);
        swap(lst);
        }
 
        return *this;
    }
 
    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->head_.next_node, other.head_.next_node);
		std::swap(this->size_, other.size_);
    }
 
    // Константные версии begin/end для обхода списка без возможности модификации его элементов
    [[nodiscard]] ConstIterator begin() const noexcept { return ConstIterator{head_.next_node};}
    [[nodiscard]] ConstIterator end() const noexcept { return ConstIterator{nullptr};}
    // Методы для удобного получения константных итераторов у неконстантного контейнера
    [[nodiscard]] ConstIterator cbegin() const noexcept { return ConstIterator{head_.next_node};}
    [[nodiscard]] ConstIterator cend() const noexcept { return ConstIterator{nullptr};}
 
    [[nodiscard]] Iterator begin() noexcept {
        // Благодаря дружбе SingleLinkedList имеет доступ к приватному конструктору своего итератора
        return Iterator{head_.next_node};
    }
      // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
         
        return Iterator(const_cast<Node*>(&head_));
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept { return ConstIterator(const_cast<Node*>(&head_)); }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept { return ConstIterator(const_cast<Node*>(&head_));  }

    /*Вставляет элемент value после элемента, на который указывает pos.Возвращает итератор на вставленный элемент. Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии*/
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
         pos.node_ ->next_node=new Node(value,pos.node_->next_node);
         ++size_;
         return Iterator(pos.node_->next_node);
     }
         
        
    void PopFront() noexcept {
        if (head_.next_node != nullptr) {
        Node* ptr = head_.next_node->next_node;
        head_.next_node->next_node=nullptr;
        delete head_.next_node;
        head_.next_node= ptr;
    	}
    }

    /*Удаляет элемент, следующий за pos. Возвращает итератор на элемент, следующий /за удалённым*/
    Iterator EraseAfter(ConstIterator pos) noexcept {
    	Node* temp=pos.node_->next_node;
    	pos.node_->next_node=pos.node_->next_node->next_node;
    	delete temp;
    	--size_;
    	return Iterator(pos.node_->next_node);
   
   }
   
    [[nodiscard]] Iterator end() noexcept {
        // Благодаря дружбе SingleLinkedList имеет доступ к приватному конструктору своего итератора
        return Iterator{nullptr};
    }
 
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
		if(size_ == 1) {
			tail_ = head_.next_node;
		}
    }
 
	Type get_tail_value() {
		return tail_->value;
	}
 
	void push_back(const Type& value) {
		Node* p = new Node(value, nullptr);
		if (IsEmpty()) {
			head_.next_node = p;
			tail_ = p;
            ++size_;
			return;
		}
    	tail_->next_node = p;
    	tail_ = p;
        ++size_;
	}
 
    // Очищает список за время O(N)
    void Clear() noexcept {
        while(head_.next_node != nullptr) {
            Node* next = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next;
        }
        //delete head_.next_node;
		head_.next_node = nullptr;
		tail_ = nullptr;
        size_ = 0;
    }
    
	SingleLinkedList() = default;
 
    ~SingleLinkedList() {
        Clear();
    }
 
    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
 
    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return size_ == 0;
    }
 
 
};
 
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}
 
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
 
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
 
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
 
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return !std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}
 
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}
 
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
} 
 
 
