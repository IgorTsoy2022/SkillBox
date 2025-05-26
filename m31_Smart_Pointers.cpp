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

namespace ptr {
    class ControlBlockBase {
    public:
        ControlBlockBase() {}

        ControlBlockBase(const size_t _shared_count, const size_t _weak_count = 0)
            : shared_count(_shared_count)
            , weak_count(_weak_count)
        {
        }

        const size_t use_count() const noexcept {
            return shared_count;
        }

        virtual void distruct() {}

        virtual ~ControlBlockBase() {
            std::cout << "~ControlBlockBase\n";
        }

        size_t shared_count = 0;
        size_t weak_count = 0;
    };

    template <typename T>
    class PtrControlBlock : public ControlBlockBase {
    public:
        explicit PtrControlBlock(const size_t shared_count,
            const size_t weak_count, T* ptr)
            : ControlBlockBase(shared_count, weak_count)
            , ptr_(ptr)
        {
        }

        void distruct() override {
            delete ptr_;
            ptr_ = nullptr;
        }

        ~PtrControlBlock() {
            std::cout << "~PtrControlBlock\n";
            if (ptr_ != nullptr) {
                delete ptr_;
            }
        }
    private:
        T* ptr_ = nullptr;
    };

    template <typename T>
    class ObjControlBlock : public ControlBlockBase {
    public:
        template<typename ... Args>
        explicit ObjControlBlock(Args&& ...args)
            : ControlBlockBase(1)
            , obj_(args...)
        {
        }

        T& get_object() noexcept {
            return obj_;
        }

        void distruct() override {}

        ~ObjControlBlock() {
            std::cout << "~ObjControlBlock\n";
        }
    private:
        T obj_{};
    };
}

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
//    friend class weak_ptr<T>;
public:
    shared_ptr() {}

    template <typename... Args>
    shared_ptr(Args&&... args)
        : ptr_(new T(std::forward<Args>(args)...))
    {
        cptr_ = new ptr::PtrControlBlock<T>(1, 0, ptr_);
    }

    shared_ptr(T* ptr)
        : ptr_(ptr)
    {
        cptr_ = new ptr::PtrControlBlock<T>(1, 0, ptr_);
    }

    shared_ptr(shared_ptr& other) noexcept
        : ptr_(other.ptr_)
        , cptr_(other.cptr_)
    {
        increment_shared();
    }

    shared_ptr(weak_ptr<T>& ptr) noexcept
    {
        if (ptr.use_count() > 0) {
            ptr_ = ptr.get();
            cptr_ = ptr.get_cptr();
            increment_shared();
        }
    }

    shared_ptr& operator=(const shared_ptr& other) noexcept {

        decrement_shared();

        ptr_ = other.ptr_;
        cptr_ = other.cptr_;

        increment_shared();

        return *this;
    }

    shared_ptr& operator=(weak_ptr<T>& ptr) noexcept {

        decrement_shared();

    	if (ptr.use_count() > 0) {
            ptr_ = ptr.get();
            cptr_ = ptr.get_cptr();
            increment_shared();
        }
        else {
            ptr_ = nullptr;
            cptr_ = nullptr;
        }

        return *this;
    }

    void set_ptr(T* ptr) {
        ptr_ = ptr;
    }

    void set_cptr(ptr::ControlBlockBase* cptr) {
        cptr_ = cptr;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    ptr::ControlBlockBase* get_cptr() const noexcept {
        return cptr_;
    }

    const size_t use_count() const noexcept {
        return cptr_ != nullptr ? cptr_->shared_count : 0;
    }

    void reset() {
        decrement_shared();
        ptr_ = nullptr;
        cptr_ = nullptr;
    }

    ~shared_ptr() noexcept {
        std::cout << "~shared_ptr\n";
        decrement_shared();
    }
private:
    T* ptr_ = nullptr;
    ptr::ControlBlockBase* cptr_ = nullptr;

    void increment_shared() {
        if (cptr_ == nullptr) {
            return;
        }
        ++cptr_->shared_count;
    }

    void decrement_shared() {
        if (cptr_ == nullptr) {
            return;
        }

        --cptr_->shared_count;

        if (cptr_->shared_count < 1) {
            if (cptr_->weak_count < 1) {
                delete cptr_;
                cptr_ = nullptr;
            }
            else {
                cptr_->distruct();
                ptr_ = nullptr;
            }
        }
    }
};

template <typename T, typename ... Args>
shared_ptr<T> make_shared(Args&& ... args) {
	shared_ptr<T> ptr;
    auto cptr = new ptr::ObjControlBlock<T>{ T(std::forward<Args>(args)...) };
    ptr.set_ptr(&cptr->get_object());
    ptr.set_cptr(cptr);
    return shared_ptr<T>(ptr);
}

template <typename T>
class weak_ptr {
//    friend class shared_ptr<T>;
public:
    weak_ptr() {}

    template <typename... Args>
    weak_ptr(Args&&... args)
    {
        cptr_ = new ptr::PtrControlBlock<T>(0, 1, ptr_);
    }

    weak_ptr(T* ptr) noexcept
        : ptr_(ptr)
    {
        cptr_ = new ptr::PtrControlBlock<T>(0, 1, ptr_);
    }

    weak_ptr(weak_ptr& other) noexcept
        : ptr_(other.ptr_)
        , cptr_(other.cptr_)
    {
        if (other.cptr_ == nullptr) {
            cptr_ = new ptr::PtrControlBlock<T>(0, 1, ptr_);
            other.cptr_ = cptr_;
        }
        increment_weak();
    }

    weak_ptr(shared_ptr<T>& ptr) noexcept
        : ptr_(ptr.get())
        , cptr_(ptr.get_cptr())
    {
        increment_weak();
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept {

        decrement_weak();

        ptr_ = other.ptr_;
        cptr_ = other.cptr_;

        increment_weak();

        return *this;
    }

    weak_ptr& operator=(const shared_ptr<T>& ptr) {

        decrement_weak();

        ptr_ = ptr.get();
        cptr_ = ptr.get_cptr();

        increment_weak();

        return *this;
    }

    T* get() const noexcept {
        return ptr_;
    }

    ptr::ControlBlockBase* get_cptr() const noexcept {
        return cptr_;
    }

    size_t use_weak_count() const noexcept {
        return cptr_ != nullptr ? cptr_->weak_count : 0;
    }

    size_t use_count() const noexcept {
        return cptr_ != nullptr ? cptr_->shared_count : 0;
    }

    const bool expired() const noexcept {
        return cptr_ != nullptr ? cptr_->shared_count > 0 : false;
    }

    shared_ptr<T> lock() noexcept {
        if (cptr_ != nullptr) {
            if (cptr_->shared_count > 0) {
                return shared_ptr<T>(*this);
            }
        }
        return shared_ptr<T>();
    }

    void reset() {
        decrement_weak();
        ptr_ = nullptr;
        cptr_ = nullptr;
    }

    ~weak_ptr() noexcept {
        std::cout << "~weak_ptr\n";
        decrement_weak();
    }
private:
    T* ptr_ = nullptr;
    ptr::ControlBlockBase* cptr_ = nullptr;

    void increment_weak() {
        if (cptr_ == nullptr) {
            return;
        }
        ++cptr_->weak_count;
    }

    void decrement_weak() {
        if (cptr_ == nullptr) {
            return;
        }

        --cptr_->weak_count;

        if (cptr_->weak_count < 1) {
            if (cptr_->shared_count < 1) {
                delete cptr_;
                cptr_ = nullptr;
            }
        }
        else {
            if (cptr_->shared_count < 1) {
                ptr_ = nullptr;
            }
        }
    }

};

struct S {
    int x = 0;
    std::string str;
    S(int x, const std::string& str)
        : x(x)
        , str(str)
    {};
};

int main() {
    auto p1 = shared_ptr<int>(5);
    int x = 6;
    auto p2 = shared_ptr<int>(x);
    auto p3 = shared_ptr<int>(new int(7));
    auto p4 = make_shared<int>(8);
    auto p5 = p1;

    std::cout << *p1 << " count=" << p1.use_count() << std::endl;
    std::cout << *p2 << " count=" << p2.use_count() << std::endl;
    std::cout << *p3 << " count=" << p3.use_count() << std::endl;
    std::cout << *p4 << " count=" << p4.use_count() << std::endl;
    std::cout << *p5 << " count=" << p5.use_count() << std::endl;

    p5 = p4;

    std::cout << *p1 << " count=" << p1.use_count() << std::endl;
    std::cout << *p2 << " count=" << p2.use_count() << std::endl;
    std::cout << *p3 << " count=" << p3.use_count() << std::endl;
    std::cout << *p4 << " count=" << p4.use_count() << std::endl;
    std::cout << *p5 << " count=" << p5.use_count() << std::endl;

    auto ps = shared_ptr<S>(6, "abc");
    std::cout << ps->x << " " << ps->str << std::endl;

    auto w1 = weak_ptr<int>(5);
    auto w2 = weak_ptr<int>(x);
    auto w3 = weak_ptr<int>(new int(7));
    auto w4 = w1;

    auto w5 = weak_ptr<int>(p1);
    auto w6 = weak_ptr<int>(w5);
    weak_ptr<int> w7 = p2;
    auto w8 = w5;

    std::cout << " count=" << w1.use_count() << " weak count=" << w1.use_weak_count() << std::endl;
    std::cout << " count=" << w2.use_count() << " weak count=" << w2.use_weak_count() << std::endl;
    std::cout << " count=" << w3.use_count() << " weak count=" << w3.use_weak_count() << std::endl;
    std::cout << " count=" << w4.use_count() << " weak count=" << w4.use_weak_count() << std::endl;
    std::cout << " count=" << w5.use_count() << " weak count=" << w5.use_weak_count() << std::endl;
    std::cout << " count=" << w6.use_count() << " weak count=" << w6.use_weak_count() << std::endl;
    std::cout << " count=" << w7.use_count() << " weak count=" << w7.use_weak_count() << std::endl;
    std::cout << " count=" << w8.use_count() << " weak count=" << w8.use_weak_count() << std::endl;

    auto pw = w5.lock();
    std::cout << " count=" << w5.use_count() << " weak count=" << w5.use_weak_count() << std::endl;

    p1.reset();
    p2.reset();
    p3.reset();
    p4.reset();

    std::cout << " count=" << w1.use_count() << " weak count=" << w1.use_weak_count() << std::endl;
    std::cout << " count=" << w2.use_count() << " weak count=" << w2.use_weak_count() << std::endl;
    std::cout << " count=" << w3.use_count() << " weak count=" << w3.use_weak_count() << std::endl;
    std::cout << " count=" << w4.use_count() << " weak count=" << w4.use_weak_count() << std::endl;

   
    return 0;
}