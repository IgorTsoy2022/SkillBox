#include <iostream>

template <typename T>
struct default_delete {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
public:
    unique_ptr(T* ptr)
        : ptr_(ptr)
    {}

    unique_ptr(T* ptr, Deleter del)
        : ptr_(ptr)
        , del_(del)
    {}

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& other)
        : ptr_(other.ptr_)
        , del_(std::move(other.del_))
    {
        other.ptr_ = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& other) {
        if (this != &other) {
            del_(ptr_);
            ptr_ = other.ptr_;
            del_ = std::move(other.del_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    T* get() const {
        return ptr_;
    }

    ~unique_ptr() {
        del_(ptr_);
    }
private:
    T* ptr_;
    [[no_unique_address]] Deleter del_;
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class ControlBlockBase {
public:
    explicit ControlBlockBase(T* ptr) noexcept
        : shared_count_(1)
        , weak_count_(1)
        , ptr_(ptr)
    {};

    void increment_shared() noexcept {
        ++shared_count_;
    }

    void increment_weak() noexcept {
        ++weak_count_;
    }

    void decrement_shared() noexcept {
        if (shared_count_ == 0) {
            throw std::logic_error("shared_count_ is already zero.");
        }
        --shared_count_;
    }

    void decrement_weak() noexcept {
        if (weak_count_ == 0) {
            throw std::logic_error("weak_count_ is already zero.");
        }
        --weak_count_;
    }

    void release_shared() noexcept {
        if (--shared_count_ < 1) {
            delete ptr_;
            if (--weak_count_ < 1) {
                delete this;
            }
        }
    }

    void release_weak() noexcept {
        if (--weak_count_ < 1) {
            delete this;
        }
    }

    size_t use_count() const noexcept {
        return shared_count_.load();
    }

    void increment_shared_lock() {
        // Сохраняем текущее значение shared_count
        size_t cur_value(shared_count.load());
        do {
            // Если счётчик сильных ссылок равен нулю (т.е. нет больше живых shared_ptr),
            // то новый shared_ptr создавать не из чего.
            if (!cur_value) {
                throw std::bad_weak_ptr();
            }
            // Пытаемся увеличить счётчик shared_count на единицу
            // Если в промежутке между сохранением shared_count в cur_value, shared_count изменился,
            // то операция compare_exchange_weak вернёт false, запишет новое значение shared_count в cur_value,
            // и цикл повторится
        } while (shared_count.compare_exchange_weak(cur_value, cur_value + 1));
    }

    virtual void distruct() = 0;

    virtual void ~ControlBlockBase() {
        release();
    }

private:
    std::atomic<size_t> shared_count_;
    std::atomic<size_t> weak_count_;
    T* ptr_;
};

template <typename T>
class PtrControlBlock : ControlBlockBase {
public:
    explicit PtrControlBlock(T* ptr)
        : ptr_(ptr)
    {}

    void distruct() override {
        delete ptr_;
    }

private:
    T* ptr_ = nullptr;
};

template <typename T>
class ObjControlBlock : ControlBlockBase {
public:
    template<typename ... Args>
    explicit ObjControlBlock(Args&& ...args)
        : obj_(args...)
    {}

    void distruct() override {}

private:
    T obj_{};
};


template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
public:
    shared_ptr() noexcept : ptr_(nullptr), counted(nullptr) {}

    explicit shared_ptr(T* p) {
        std::unique_ptr<T> holder(p);
        // new может кинуть исключение, и, если p не передать в unique_ptr,
        // память под p потеряется
        counted = new ControlBlock<T>(holder.get());
        ptr_ = holder.release();
    }

    ~shared_ptr() noexcept {
        release();
    }

    shared_ptr(const shared_ptr& other) noexcept : ptr(other.ptr), counted(other.counted) {
        increment_shared();
    }

    shared_ptr& operator=(const shared_ptr& other) noexcept {
        // Освобождаем владение предыдущим указателем
        release();

        // Выполняем присваивание
        ptr = other.ptr;
        counted = other.counted;

        // Устанавливаем владение новым указателем
        increment_shared();

        // Ура! Я не забыл вернуть *this!
        return *this;
    }

    T* get() const noexcept {
        return ptr;
    }

    size_t use_count() const noexcept {
        return counted != nullptr ? counted->use_count() : 0;
    }

private:
    void increment_shared() {
        if (counted) {
            counted->increment_shared();
        }
    }

    void release() {
        if (counted) {
            counted->release();
        }
    }

private:
    T* ptr_;
    ControlBlock<T>* counted;



    friend class weak_ptr<T>;
};

template <typename T, typename ... Args>
shared_ptr<T> make_shared(Args&& ... args) {
    shared_ptr<T> ptr = nullptr;
    auto object = new typename shared_ptr<T>::ObjControlBlock<T>(args ...);
    ptr.ptr_ = &object->obj_;
    ptr.cptr_ = object;
    return ptr;
}


template <typename T>
class weak_ptr {
    friend class shared_ptr<T>;
public:
    weak_ptr() noexcept : ptr(nullptr), counted(nullptr) {}

    weak_ptr(const weak_ptr& other) noexcept : ptr(other.ptr), counted(other.counted) {
        increment_weak();
    }

    weak_ptr(const shared_ptr<T>& p) noexcept : ptr(p.ptr), counted(p.counted) {
        increment_weak();
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept {
        release_weak();

        ptr = other.ptr;
        counted = other.counted;

        increment_weak();

        return *this;
    }

    weak_ptr& operator=(const shared_ptr<T>& p) {
        release_weak();

        ptr = p.ptr;
        counted = p.counted;

        increment_weak();

        return *this;
    }

    // Пытаемся сделать shared_ptr. Для этого вызывается конструктор shared_ptr(const weak_ptr &amp;);
    // В случае невозможности создать shared_ptr возвращается пустой объект
    shared_ptr<T> lock() noexcept {
        try {
            return shared_ptr<T>(*this);
        }
        catch (const std::bad_weak_ptr&) {
            return shared_ptr<T>();
        }
    }

    size_t use_count() const noexcept {
        return counted != nullptr ? counted->use_count() : 0;
    }

    const bool expired() const noexcept {
        return shared_count_.load() > 0;
    }

    const shared_ptr<T> lock() const noexcept {
        if (shared_count_.load() > 0) {
            return shared_ptr<T>();
        }
        return nullptr;
    }

private:
    void increment_weak() noexcept {
        if (counted) {
            counted->increment_weak();
        }
    }

    void release_weak() noexcept {
        if (counted) {
            counted->release_weak();
        }
    }

private:
    T* ptr;
    ControlBlock<T>* counted;
};

/*
template <typename T>
class shared_ptr {
public:
    shared_ptr(T* ptr)
        : ptr_(ptr)
        , counter_(new size_t(1))
    {};

    shared_ptr(const shared_ptr& other)
        : ptr_(other.ptr_)
        , counter_(other.counter_)
    {};

    shared_ptr(shared_ptr&& other)
        : ptr_(other.ptr_)
        , counter_(other.counter_) {
        other.ptr_ = nullptr;
        other.counter_ = nullptr;
    };

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    size_t use_count() const {
        return counter_;
    }

    ~shared_ptr() {
        if (*counter_ > 1) {
            --*counter_;
            return;
        }
        delete ptr_;
        delete counter_;
    }

private:
    T* ptr_ = nullptr;
    size_t* counter_ = 0;

    template <typename U>
    struct ControlBlock {
        size_t counter;
        U object;
    };

    template <typename U>
    class weak_ptr {};

    ControlBlock<T>* cptr_ = nullptr;

    template <typename U, typename... Args>
    friend shared_ptr<U> make_shared(Args&&... args);

    struct make_shared_t;

    template <typename... Args>
    shared_ptr(make_shared_t, ControlBlock<T>* storage_ptr)
        : cptr_(storage_ptr)
    {}
};
*/

/*
template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto ptr = new ControlBlock<T>(1, T(std::forward<Args>(args)...));
    return shared_ptr<T>(shared_ptr::make_shared_t(), ptr);
//    auto ptr = new T(std::forward<Args>(args)...);
//    return shared_ptr<T>(ptr);
}
*/

/*
template <typename T>
class weak_ptr {
public:
    weak_ptr(const shared_ptr<T>& ptr)
        : cptr_(ptr.cptr_)
    {};

    bool expired() const {
        return cptr_->counter > 0;
    }

    shared_ptr<T> lock() const {

    }
private:
    ControlBlock<T>* cptr_ = nullptr;
};
*/

struct S {
    int x = 0;
    std::string str;
    S(int x, const std::string& str)
        : x(x)
        , str(str)
    {};
};

int main() {

//    auto sp = make_shared<S>(5, "abc");

    return 0;
}