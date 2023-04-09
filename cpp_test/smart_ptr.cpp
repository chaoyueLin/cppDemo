#include <utility>
class shape
{
};


template <typename T>
class smart_ptr
{
private:
    T *ptr_;

public:
    smart_ptr(T *ptr = nullptr) : ptr_(ptr){

                                  };

    // smart_ptr(const smart_ptr& other) = delete;
    // smart_ptr& operator=(const smart_ptr& other) = delete;

    smart_ptr(smart_ptr &&other)
    {
        ptr_ = other.release();
    }
    smart_ptr &operator=(smart_ptr other)
    {
        other.swap(*this);
        return *this;
    }
    ~smart_ptr()
    {
        delete ptr_;
    };

    T *get() const
    {
        return ptr_;
    }

    T &operator*() const
    {
        return *ptr_;
    }
    T *operator->() const
    {
        return ptr_;
    }
    operator bool() const
    {
        return ptr_;
    }

    T *release()
    {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }
    void swap(smart_ptr &other)
    {
        using std::swap;
        swap(ptr_, other.ptr_);
    }
};

int main()
{

    smart_ptr<shape> ptr1{new shape()};
    // smart_ptr<Shape> ptr2{ptr1};             // 编译出错
    smart_ptr<shape> ptr3;
    // ptr3 = ptr1;                             // 编译出错
    ptr3 = std::move(ptr1);                 // OK，可以
    smart_ptr<shape> ptr4{std::move(ptr3)}; // OK，可以
};