// https://godbolt.org/z/1r19TMqdf
#include <iostream>
#include <stdexcept>
#include <cstring>

template<typename T>
class LightVector {
public:
    LightVector()
        : capacity_(1), size_(0), data_(static_cast<T*>(std::malloc(sizeof(T) * capacity_))) {
        if (!data_) {
            throw std::bad_alloc();
        }
    }

    // Copy constructor
    LightVector(const LightVector& other)
        : capacity_(other.capacity_), size_(other.size_), data_(static_cast<T*>(std::malloc(sizeof(T) * capacity_))) {
        if (!data_) {
            throw std::bad_alloc();
        }
        std::memcpy(data_, other.data_, size_ * sizeof(T));
    }

    // Copy assignment operator
    LightVector& operator=(const LightVector& other) {
        if (this == &other) return *this; // Handle self-assignment

        // Free existing resources
        std::free(data_);

        // Allocate new resources and copy data
        capacity_ = other.capacity_;
        size_ = other.size_;
        data_ = static_cast<T*>(std::malloc(sizeof(T) * capacity_));
        if (!data_) {
            throw std::bad_alloc();
        }
        std::memcpy(data_, other.data_, size_ * sizeof(T));

        return *this;
    }

    ~LightVector() {
        std::free(data_);
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize_internal(capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        --size_;
    }

    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }

    std::size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    template<size_t N>
    void resize(T (&arr)[N]) {
        if (N > capacity_) {
            resize_internal(N);
        }
        std::memcpy(data_, arr, N * sizeof(T));
        size_ = N;
    }

    template<size_t N>
    LightVector& operator=(T (&arr)[N]) {
        resize(arr);
        return *this;
    }

private:
    void resize_internal(std::size_t new_capacity) {
        T* new_data = static_cast<T*>(std::realloc(data_, sizeof(T) * new_capacity));
        if (!new_data) {
            throw std::bad_alloc();
        }
        data_ = new_data;
        capacity_ = new_capacity;
    }

    std::size_t capacity_;
    std::size_t size_;
    T* data_;
};

// Specialization for char
template<>
class LightVector<char> {
public:
    LightVector()
        : capacity_(1), size_(0), data_(static_cast<char*>(std::malloc(sizeof(char) * capacity_))) {
        if (!data_) {
            throw std::bad_alloc();
        }
    }

    // Copy constructor
    LightVector(const LightVector& other)
        : capacity_(other.capacity_), size_(other.size_), data_(static_cast<char*>(std::malloc(sizeof(char) * capacity_))) {
        if (!data_) {
            throw std::bad_alloc();
        }
        std::memcpy(data_, other.data_, size_ * sizeof(char));
    }

    // Copy assignment operator
    LightVector& operator=(const LightVector& other) {
        if (this == &other) return *this; // Handle self-assignment

        // Free existing resources
        std::free(data_);

        // Allocate new resources and copy data
        capacity_ = other.capacity_;
        size_ = other.size_;
        data_ = static_cast<char*>(std::malloc(sizeof(char) * capacity_));
        if (!data_) {
            throw std::bad_alloc();
        }
        std::memcpy(data_, other.data_, size_ * sizeof(char));

        return *this;
    }

    ~LightVector() {
        std::free(data_);
    }

    void push_back(const char& value) {
        if (size_ >= capacity_) {
            resize_internal(capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        --size_;
    }

    char& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }

    const char& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return data_[index];
    }

    std::size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    template<size_t N>
    LightVector& operator=(const char (&arr)[N]) {
        resize(arr);
        return *this;
    }

    LightVector& operator=(const char* str) {
        std::size_t len = std::strlen(str);
        resize_internal(len + 1); // +1 for null terminator
        std::memcpy(data_, str, len);
        data_[len] = '\0'; // Ensure null-terminated
        size_ = len;
        return *this;
    }

    void print() const {
        std::cout << data_ << std::endl;
    }

private:
    void resize_internal(std::size_t new_capacity) {
        char* new_data = static_cast<char*>(std::realloc(data_, sizeof(char) * new_capacity));
        if (!new_data) {
            throw std::bad_alloc();
        }
        data_ = new_data;
        capacity_ = new_capacity;
    }

    std::size_t capacity_;
    std::size_t size_;
    char* data_;
};

/////////////////////////////////////////////////////

template<class T>
static LightVector<T> hammer_of_thor  ( LightVector<T> vec )
{
    LightVector<char> vec2;

    vec = "Hello, world!";
    vec.print(); // Outputs: Hello, world!

    vec2 = vec;
  
    return vec2 ;
}

int main() {
    LightVector<char> vec = hammer_of_thor( LightVector<char>{} ) ;
    vec.print(); // Outputs: Hello, world!


    return 0;
}
