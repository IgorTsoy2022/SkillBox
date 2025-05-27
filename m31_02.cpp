#include <iostream>

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
//            std::cout << "~ControlBlockBase\n";
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
//            std::cout << "~PtrControlBlock\n";
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
//            std::cout << "~ObjControlBlock\n";
        }
    private:
        T obj_{};
    };
} // namespace ptr

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
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

    shared_ptr(shared_ptr* other) noexcept
    {
        if (other != nullptr) {
            ptr_ = other->ptr_;
            cptr_ = other->cptr_;
            increment_shared();
        }
    }

    shared_ptr(weak_ptr<T>& ptr) noexcept
    {
        if (ptr.use_count() > 0) {
            ptr_ = ptr.get();
            cptr_ = ptr.get_cptr();
            increment_shared();
        }
    }

    shared_ptr(weak_ptr<T>* ptr) noexcept
    {
        if (ptr->use_count() > 0) {
            ptr_ = ptr->get();
            cptr_ = ptr->get_cptr();
            increment_shared();
        }
    }

    shared_ptr& operator=(shared_ptr& other) noexcept {

        decrement_shared();

        ptr_ = other.ptr_;
        cptr_ = other.cptr_;

        increment_shared();

        return *this;
    }

    shared_ptr* operator=(shared_ptr* other) noexcept {

        decrement_shared();

        if (other != nullptr) {
            ptr_ = other->ptr_;
            cptr_ = other->cptr_;

            increment_shared();
        }
        else {
            ptr_ = nullptr;
            cptr_ = nullptr;
        }

        return this;
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

    bool operator==(const shared_ptr& other) const {
        return (ptr_ == other.ptr_) && (cptr_ == other.cptr_);
    }

    bool operator==(const shared_ptr* other) const {
        if (other == nullptr) {
            return (ptr_ == nullptr) && (cptr_ == nullptr);
        }
        return (ptr_ == other->ptr_) && (cptr_ == other->cptr_);
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
//        std::cout << "~shared_ptr\n";
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
            }
            ptr_ = nullptr;
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

    weak_ptr(weak_ptr* other) noexcept
    {
        if (other != nullptr) {
            ptr_ = other->ptr_;
            cptr_ = other->cptr_;
            increment_weak();
        }
    }

    weak_ptr(shared_ptr<T>& ptr) noexcept
        : ptr_(ptr.get())
        , cptr_(ptr.get_cptr())
    {
        increment_weak();
    }

    weak_ptr(shared_ptr<T>* ptr) noexcept
    {
        if (ptr != nullptr) {
            ptr_ = ptr->ptr_;
            cptr_ = ptr->cptr_;
            increment_weak();
        }
    }

    weak_ptr& operator=(weak_ptr& other) noexcept {

        decrement_weak();

        ptr_ = other.ptr_;
        cptr_ = other.cptr_;

        increment_weak();

        return *this;
    }

    weak_ptr* operator=(weak_ptr* other) noexcept {

        decrement_weak();

        if (other != nullptr) {
            ptr_ = other->ptr_;
            cptr_ = other->cptr_;

            increment_weak();
        }
        else {
            ptr_ = nullptr;
            cptr_ = nullptr;
        }

        return this;
    }

    weak_ptr& operator=(shared_ptr<T>& ptr) {

        decrement_weak();

        ptr_ = ptr.get();
        cptr_ = ptr.get_cptr();

        increment_weak();

        return *this;
    }

    bool operator==(const weak_ptr& other) const {
        return (ptr_ == other.ptr_) && (cptr_ == other.cptr_);
    }

    bool operator==(const weak_ptr* other) const {
        if (other == nullptr) {
            return (ptr_ == nullptr) && (cptr_ == nullptr);
        }
        return (ptr_ == other->ptr_) && (cptr_ == other->cptr_);
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
//        std::cout << "~weak_ptr\n";
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
                ptr_ = nullptr;
            }
        }
        else {
            if (cptr_->shared_count < 1) {
                ptr_ = nullptr;
            }
        }
    }

};

class Toy {
public:
    Toy() {}

    Toy(const std::string& name)
        : name_(name)
    {}

    const std::string_view name() const {
        return name_;
    }

    ~Toy() {
        std::cout << "The toy \"" << name_ << "\" has been destroyed." << std::endl;
    }
private:
    std::string name_ = "unknown";
};

using shared_ptr_toy = shared_ptr<Toy>;

int main() {
    {
        shared_ptr_toy toy_01 = make_shared<Toy>("ball");
        shared_ptr_toy toy_02(toy_01);
        shared_ptr_toy toy_03("duck");
        std::cout << "=================================================" << std::endl;
        std::cout << toy_01->name() << " links:" << toy_01.use_count() << "  "
                  << toy_02->name() << " links:" << toy_02.use_count() << "  "
                  << toy_03->name() << " links:" << toy_03.use_count() << std::endl;
        std::cout << "=================================================" << std::endl;
        toy_02 = toy_03;
        std::cout << toy_01->name() << " links:" << toy_01.use_count() << "  "
                  << toy_02->name() << " links:" << toy_02.use_count() << "  "
                  << toy_03->name() << " links:" << toy_03.use_count() << std::endl;
        std::cout << "=================================================" << std::endl;
        toy_01.reset();
        std::cout << "toy_01->name()" << " links:" << toy_01.use_count() << "  "
                  << toy_02->name() << " links:" << toy_02.use_count() << "  "
                  << toy_03->name() << " links:" << toy_03.use_count() << std::endl;
        std::cout << "=================================================" << std::endl;
    }

    auto ball1 = shared_ptr_toy("ball1");
    auto ball2 = ball1;
    std::cout << "ball1=" << ball1->name() << " shared_count=" << ball1.use_count() << std::endl;
    std::cout << "ball2=" << ball2->name() << " shared_count=" << ball2.use_count() << std::endl;

    std::cout << "ball2=nullptr:\n";
    ball2 = nullptr;
    std::cout << std::boolalpha << "(ball2==nullptr) = " << (ball2 == nullptr) << "\n";
    std::cout << "ball1=" << ball1->name() << " shared_count=" << ball1.use_count() << std::endl;

    std::shared_ptr<Toy> p = std::make_shared<Toy>("ball");
    std::cout << "ball=" << p->name() << " shared_count=" << p.use_count() << std::endl;
    p.reset();
    std::cout << "ball=" << "p->name()" << " shared_count=" << p.use_count() << std::endl;

    return 0;
}