//
// Created by xomag on 11.08.2020.
//

#ifndef OTUSHOMEWORKS_LIST_H
#define OTUSHOMEWORKS_LIST_H
#include "pch.h"

template <typename T>
class Iterator : public virtual std::iterator<std::forward_iterator_tag, T> {
public:
    Iterator(T* p) : iter(p){}

    Iterator(const Iterator<T> & it) : iter(it.iter){}

    bool operator!=(const Iterator & it){
        return iter != it.iter;
    }

    bool operator==(const Iterator & it){
        return iter == it.iter;
    }

    auto& operator* (){
        return (*iter).data;
    }

    typename Iterator::pointer operator-> (){
        return iter;
    }

    Iterator& operator++(){
        iter = iter->next.iter;
        return *this;
    }

    Iterator& operator++(int){
        iter = iter->next.iter;
        return *this;
    }

    Iterator advance(size_t size){
        Iterator advanced = iter;
        while (size){
            --size;
            ++advanced;
        }
        return advanced;
    }

    typename Iterator::pointer get(){
        return iter;
    }


private:
    T* iter;
};

template <typename T, typename Alloc = std::allocator<T>, typename RefT = T&, typename ConstRefT = const T&>
struct List {
private:
    struct Node;

public:
    using value_type = T;
    using Ref_Type = RefT;
    using iterator = Iterator<Node>;

    List() {}

    template <typename Iterator>
    List(Iterator first, Iterator last){
        size_t i = 0;
        for (auto it = first; it != last; it++)
            i++;
        iterator cur = alloc_node.allocate(i);
        for (auto it = first; it != last; it++){
            alloc_node.construct(cur.get(), *it);
            cur->next = node_;
            node_ = cur;
            cur++;
            size_++;
        }
    }

    iterator begin(){
        return node_;
    }

    iterator begin() const {
        return node_;
    }

    iterator end(){
        return node_.advance(size_);
    }

    iterator end() const {
        return node_.advance(size_);
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    template <typename ... Args>
    void emplace_front(Args && ... value){
        iterator tmp = alloc_node.allocate(1);
        alloc_node.construct(tmp.get(), std::forward<Args>(value)...);
        tmp->next = node_;
        node_ = tmp;
        size_++;
    }

    void pop_front(){
        iterator tmp = node_->next;
        alloc_node.destroy(node_.get());
        node_ = tmp;
        size_--;
    }

    ~List () {
        while (node_->next != nullptr){
            pop_front();
        }
        alloc_node.deallocate(node_.get(), 1);
    }

private:
    struct Node{
        iterator next = nullptr;
        value_type data;

        template<typename... Args>
        explicit Node (Args&&... val): data (std::forward<Args>(val)...), next(nullptr) {
        }
    };
    iterator node_ = nullptr;
    size_t size_ = 0;
    typename Alloc::template rebind<Node>::other alloc_node{};
};

template <typename Iterator>
List(Iterator first, Iterator last) -> List<typename std::iterator_traits<Iterator>::value_type>;


#endif //OTUSHOMEWORKS_LIST_H
