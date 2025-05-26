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

class ControlBlockBase {
public:
    ControlBlockBase() {}

    ControlBlockBase(const size_t _shared_count, const size_t _weak_count = 0)
        : shared_count(_shared_count)
        , weak_count(_weak_count)
    {}

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
    explicit PtrControlBlock(const size_t shared_count, const size_t weak_count, T* ptr)
        : ControlBlockBase(shared_count, weak_count)
        , ptr_(ptr)
    {}

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
    {}

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

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
public:
    shared_ptr() {}

    template <typename... Args>
    explicit shared_ptr(Args&&... args)
        : ptr_(new T(std::forward<Args>(args)...))
    {
        cptr_ = new PtrControlBlock<T>(1, 0, ptr_);
    }

    explicit shared_ptr(T* ptr)
        : ptr_(ptr)
    {
        cptr_ = new PtrControlBlock<T>(1, 0, ptr_);
    }

    explicit shared_ptr(shared_ptr& other) noexcept
        : ptr_(other.ptr_)
        , cptr_(other.cptr_)
    {
        ++cptr_->shared_count;
    }

    explicit shared_ptr(weak_ptr<T>& ptr) noexcept
    {
        if (ptr.use_count() > 0) {
            ptr_ = ptr.get();
            cptr_ = ptr.get_cptr();
            ++cptr_->shared_count;
        }
    }

    shared_ptr& operator=(shared_ptr& other) noexcept {

        --cptr_->shared_count;
        release_shared();

        ptr_ = other.ptr_;
        cptr_ = other.cptr_;

        ++cptr_->shared_count;

        return *this;
    }

    shared_ptr& operator=(weak_ptr<T>& ptr) {

        --cptr_->shared_count;
    	release_shared();

    	if (ptr.use_count() > 0) {
            ptr_ = ptr.get();
            cptr_ = ptr.get_cptr();
            ++cptr_->shared_count;
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

    void set_cptr(ControlBlockBase* cptr) {
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

    ControlBlockBase* get_cptr() const noexcept {
        return cptr_;
    }

    const size_t use_count() const noexcept {
        return cptr_->shared_count;
    }

   void release_shared() {
        if (cptr_->shared_count < 1) {
            if (cptr_->weak_count < 1) {
                delete cptr_;
            }
            else {
                cptr_->distruct();
            }
        }
    }

    ~shared_ptr() noexcept {
        std::cout << "~shared_ptr\n";
        --cptr_->shared_count;
        release_shared();
    }
private:
    T* ptr_ = nullptr;
    ControlBlockBase* cptr_ = nullptr;

//    friend class weak_ptr<T>;
};

template <typename T, typename ... Args>
shared_ptr<T> make_shared(Args&& ... args) {
	shared_ptr<T> ptr;
    auto cptr = new ObjControlBlock<T>(T(std::forward<Args>(args)...));
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
    explicit weak_ptr(Args&&... args)
    {
        cptr_ = new PtrControlBlock<T>(0, 1, ptr_);
    }

    weak_ptr(T* ptr) noexcept
        : ptr_(ptr)
    {
        cptr_ = new PtrControlBlock<T>(0, 1, ptr_);
    }

    weak_ptr(weak_ptr& other) noexcept
        : ptr_(other.ptr_)
        , cptr_(other.cptr_)
    {
        if (other.cptr_ == nullptr) {
            cptr_ = new PtrControlBlock<T>(0, 1, ptr_);
            other.cptr_ = cptr_;
        }
        ++cptr_->weak_count;
    }

    weak_ptr(shared_ptr<T>& ptr) noexcept
        : ptr_(ptr.get())
        , cptr_(ptr.get_cptr())
    {
        ++cptr_->weak_count;
    }

    weak_ptr& operator=(weak_ptr& other) noexcept {

        release_weak();

        ptr_ = other.ptr_;
        cptr_ = other.cptr_;

        ++cptr_->weak_count;

        return *this;
    }

    weak_ptr& operator=(shared_ptr<T>& ptr) {

        release_weak();

        ptr_ = ptr.get();
        cptr_ = ptr.get_cptr();

        ++cptr_->weak_count;

        return *this;
    }

    T* get() const noexcept {
        return ptr_;
    }

    ControlBlockBase* get_cptr() const noexcept {
        return cptr_;
    }

    size_t use_weak_count() const noexcept {
        return cptr_->weak_count;
    }

    size_t use_count() const noexcept {
        return cptr_->shared_count;
    }

    const bool expired() const noexcept {
        return cptr_->shared_count > 0;
    }

    shared_ptr<T> lock() noexcept {
        if (cptr_ != nullptr) {
            if (cptr_->shared_count > 0) {
                return shared_ptr<T>(*this);
            }
        }
        return shared_ptr<T>();
    }

    void release_weak() {
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

    ~weak_ptr() noexcept {
        std::cout << "~weak_ptr\n";
        release_weak();
    }
private:
    T* ptr_ = nullptr;
    ControlBlockBase* cptr_ = nullptr;
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
    auto p2 = shared_ptr<int>(p1);
    auto p3 = shared_ptr<int>(new int(8));
    auto p4 = make_shared<int>(9);
    p4 = p1;
//    auto p5 = p1;
    
    std::cout << *p1 << std::endl;
    std::cout << *p2 << std::endl;
    std::cout << *p3 << std::endl;
    std::cout << *p4 << std::endl;

    std::cout << "p1 count=" << p1.use_count() << std::endl;
    std::cout << "p2 count=" << p2.use_count() << std::endl;
    std::cout << "p3 count=" << p3.use_count() << std::endl;
    std::cout << "p4 count=" << p4.use_count() << std::endl;

    auto ps = shared_ptr<S>(6, "abc");
    std::cout << ps->x << " " << ps->str << std::endl;


    auto xs = make_shared<int>(10);
    std::cout << *xs << std::endl;
    std::cout << "xs count=" << xs.use_count() << std::endl;

    auto w1 = weak_ptr<int>(0);
    std::cout << w1.use_count() << "\n";
//    auto pw1 = w1.lock();

    auto w2 = weak_ptr<int>(p1);
    std::cout << "w2 count=" << w2.use_count() << "\n"; 
    auto pw2 = w2.lock();
    std::cout << "w2 count=" << w2.use_count() << "\n"; 
    auto w3 = weak_ptr<int>(w2);
    auto w4 = weak_ptr<int>(90);
    
    auto wps = weak_ptr<S>(9, "www");
    auto pwps = w2.lock();
    std::cout << "pwps count=" << pwps.use_count() << "\n"; 
    
    p1.release_shared();
    p2.release_shared();
    p4.release_shared();
    pw2.release_shared();
    pwps.release_shared();
    std::cout << "pwps count=" << p1.use_count() << "\n";
    
    return 0;
}