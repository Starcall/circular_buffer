//
// Created by Trubeckoj Bogdan on 23.06.2018.
//

#ifndef CIRCULAR_BUFFER_CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_CIRCULAR_BUFFER_H

#include <cstdio>
#include <new>
#include <cstddef>
#include <algorithm>
#include <cstring>


template <typename T>
class circular_buffer {
    T* buffer;
    size_t buffer_cap;
    size_t buffer_size;
    size_t head;
    size_t tail;

    size_t next(size_t x) {
        x++;
        if (x == buffer_cap)
            x = 0;
        return x;
    }

    size_t prev(size_t x) {
        if (x == 0)
            x = buffer_cap;
        x--;
        return x;
    }
     void ensure_capacity(size_t x) {
        if (x < buffer_cap)
            return;
         circular_buffer buf = circular_buffer(!buffer_cap ? 2 : buffer_cap * 2);
         for (size_t i = 0; i < buffer_size; i++) {
             buf.push_back((*this)[i]);
         }
         swap(*this, buf);
     };
public:
    explicit circular_buffer(size_t n = 0) {
        buffer = (T *) (malloc(sizeof(T) * n));
        buffer_cap = n;
        head = tail = buffer_size = 0;
    };
    circular_buffer(circular_buffer<T> const& rhs) : circular_buffer(rhs.buffer_cap) {
        for (auto it : rhs) {
            push_back(it);
        }
    };
    ~circular_buffer() {
        for (size_t i = head; i != tail; i = next(i)) {
            (buffer + i)->~T();
        }
        free(buffer);
        buffer = nullptr;
    };

    template<typename U>
    class Iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = U;
        using pointer = U*;
        using reference = U&;
        using iterator_category = std::random_access_iterator_tag;

        friend class circular_buffer;

        template<typename V>
        Iterator(const Iterator<V> &other, typename std::enable_if<std::is_same<U, const V>::value>::type * = nullptr) :
                my_pointer(other.my_pointer), start(other.start), index(other.index), capacity(other.capacity) {}

        Iterator<U>& operator++() {
            *this += 1;
            return *this;
        }


        const Iterator<U> operator++(int) {
            Iterator tmp(my_pointer, index, start, capacity);
            *this += 1;
            return tmp;
        }

        Iterator<U>& operator--() {
            *this -= 1;
            return *this;
        }

        const Iterator<U> operator--(int) {
            Iterator tmp(my_pointer, index, start, capacity);
            *this -= 1;
            return tmp;
        }

        Iterator<U> operator+(ptrdiff_t k) {
            Iterator tmp(my_pointer, index, start, capacity);
            return tmp += k;
        }

        Iterator<U> operator-(ptrdiff_t k) {
            Iterator tmp(my_pointer, index, start, capacity);
            return tmp -= k;
        }

        Iterator<U>& operator+=(ptrdiff_t k) {
            if (k < 0) return *this -= -k;
            index += k;
            my_pointer += k;
            if (index >= capacity) {
                index -= capacity;
                my_pointer -= capacity;
            }
            return *this;
        }

        Iterator<U>& operator-=(ptrdiff_t k) {
            if (k < 0) return *this += -k;
            if (index < static_cast<size_t>(k)) {
                index += capacity;
                my_pointer += capacity;
            }
            index -= k;
            my_pointer -= k;
            return *this;
        }

        U& operator*() const {
            return *my_pointer;
        }

        U* operator->() const {
            return my_pointer;
        }

        friend bool operator==(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            return lhs.my_pointer == rhs.my_pointer;
        }

        friend bool operator!=(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            return lhs.my_pointer != rhs.my_pointer;
        }

        friend ptrdiff_t operator-(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            if ((lhs.index < lhs.start) == (rhs.index < rhs.start)) {
                return lhs.index - rhs.index;
            }
            if (lhs.index < rhs.index) {
                return lhs.index - rhs.index + lhs.capacity;
            }
            return lhs.index - rhs.index - lhs.capacity;
        }

        friend bool operator<(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            if ((lhs.index >= lhs.start) != (rhs.index >= rhs.start)) {
                return lhs.index < rhs.index;
            }
            return lhs.index > rhs.index;
        }

        friend bool operator>=(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            return !(lhs < rhs);
        }

        friend bool operator>(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(Iterator<U> const& lhs, Iterator<U> const& rhs) {
            return !(lhs > rhs);
        }


    private:
        Iterator(U *ptr, size_t ind, size_t start, size_t cap) : my_pointer(ptr), index(ind), start(start), capacity(cap) {}
        U* my_pointer;
        size_t index, start, capacity;
    };

    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(buffer + head, head, head, buffer_cap);
    }

    const_iterator begin() const {
        return const_iterator(buffer + head, head, head, buffer_cap);
    }

    iterator end() {
        return iterator(buffer + tail, tail, head, buffer_cap);
    }

    const_iterator end() const {
        return const_iterator(buffer + tail, tail, head, buffer_cap);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }


    circular_buffer<T>& operator = (circular_buffer<T> rhs) {
        swap((*this), rhs);
        return (*this);
    };

    T& operator[](size_t i) noexcept {
        if (head + i < buffer_cap) {
            return buffer[head + i];
        } else {
            return buffer[head + i - buffer_cap];
        }
    };
    T const& operator[](size_t i) const noexcept {
        if (head + i < buffer_cap) {
            return buffer[head + i];
        } else {
            return buffer[head + i - buffer_cap];
        }
    };

    T const& front() const noexcept {
        return (*this)[0];
    }

    T& front() noexcept {
        return (*this)[0];
    }
    T const &back() const noexcept {
        return (*this)[buffer_size - 1];
    }

    T& back() noexcept {
        return (*this)[buffer_size - 1];
    }

    bool empty() const noexcept {
        return buffer_size == 0;
    }
    void clear() noexcept {
        for (size_t i = head; i != tail; i = next(i)) {
            (buffer + i)->~T();
        }
        free(buffer);
        buffer = nullptr;
        buffer_size = buffer_cap = head = tail = 0;
    };
    size_t size() const noexcept {
        return buffer_size;
    }

    friend void swap (circular_buffer<T>& lhs, circular_buffer<T>& rhs) noexcept {
        std::swap(lhs.buffer_size, rhs.buffer_size);
        std::swap(lhs.buffer_cap, rhs.buffer_cap);
        std::swap(lhs.buffer, rhs.buffer);
        std::swap(lhs.tail, rhs.tail);
        std::swap(lhs.head, rhs.head);
    };

    void push_back(T const& value) {
        ensure_capacity(buffer_size + 1);
        new (buffer + tail) T(value);
        tail = next(tail);
        buffer_size++;
    };
    void push_front(T const& value) {
        ensure_capacity(buffer_size + 1);
       // buffer[prev(head)] = value;
        new (buffer + prev(head)) T(value);
        head = prev(head);
        buffer_size++;
    };
    void pop_back() noexcept {
        tail = prev(tail);
        buffer_size--;
        (buffer + tail)->~T();
    };
    void pop_front() noexcept {
        (buffer + head)->~T();
        buffer_size--;
        head = next(head);
    };

    iterator insert(const_iterator ci_pos, T const& value) {
        ensure_capacity(buffer_size + 1);
        size_t pos = static_cast<size_t>(ci_pos - begin());

        if (buffer_size == 0) {
            (*this).push_back(value);

        } else {

            if (pos >= buffer_size - pos) {
                push_back(back());
                for (size_t i = buffer_size - 1; i > pos; --i) {
                    (*this)[i] = (*this)[prev(i)];
                }
            } else {
                push_front(front());
                for (size_t i = 0; i < pos; ++i) {
                    (*this)[i] = (*this)[next(i)];
                }
            }
            (*this)[pos] = value;
            pos += head;
            if (pos >= buffer_cap)
                pos -= buffer_cap;
        }
        return iterator(buffer + pos, pos, head, buffer_cap);
    };
    iterator erase(const_iterator ci_pos) {
        size_t pos = static_cast<size_t> (ci_pos - begin());
        if (pos >= buffer_size - pos) {
            if (buffer_size) {
                for (size_t i = pos; i < buffer_size - 1; ++i) {
                    (*this)[i] = (*this)[next(i)];
                }
            }
            pop_back();
        } else {
            for (size_t i = pos; i > 0; --i) {
                (*this)[i] = (*this)[prev(i)];
            }
            pop_front();
        }
        pos += head;
        if (pos >= buffer_cap)
            pos -= buffer_cap;

        return iterator(buffer + pos, pos, head, buffer_cap);
    };
};


#endif //CIRCULAR_BUFFER_CIRCULAR_BUFFER_H
