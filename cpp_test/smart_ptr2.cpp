
#include <utility> // std::swap
class shape
{
};
class shared_count
{
public:
    shared_count() noexcept
        : count_(1) {}
    void add_count() noexcept
    {
        ++count_;
    }
    long reduce_count() noexcept
    {
        return --count_;
    }
    long get_count() const noexcept
    {
        return count_;
    }

private:
    long count_;
};

template <typename T>
class smart_ptr
{
public:

    explicit smart_ptr(T *ptr = nullptr)
        : ptr_(ptr)
    {
        if (ptr)
        {
            shared_count_ =
                new shared_count();
        }
    }
    ~smart_ptr()
    {
        if (ptr_ &&
            !shared_count_
                 ->reduce_count())
        {
            delete ptr_;
            delete shared_count_;
        }
    }

    smart_ptr(const smart_ptr &other)
    {
        ptr_ = other.ptr_;
        if (ptr_)
        {
            other.shared_count_
                ->add_count();
            shared_count_ =
                other.shared_count_;
        }
    }

    smart_ptr &
    operator=(smart_ptr rhs) noexcept
    {
        rhs.swap(*this);
        return *this;
    }

    T *get() const noexcept
    {
        return ptr_;
    }
    long use_count() const noexcept
    {
        if (ptr_)
        {
            return shared_count_
                ->get_count();
        }
        else
        {
            return 0;
        }
    }
    void swap(smart_ptr &rhs) noexcept
    {
        using std::swap;
        swap(ptr_, rhs.ptr_);
        swap(shared_count_,
             rhs.shared_count_);
    }

    T &operator*() const noexcept
    {
        return *ptr_;
    }
    T *operator->() const noexcept
    {
        return ptr_;
    }
    operator bool() const noexcept
    {
        return ptr_;
    }

private:
    T *ptr_;
    shared_count *shared_count_;
};

int main()
{

    smart_ptr<shape> ptr1{new shape()};
    smart_ptr<shape> ptr2{ptr1};
    smart_ptr<shape> ptr3;
    ptr3 = ptr1;
    ptr3 = std::move(ptr1);                 // OK，可以
    smart_ptr<shape> ptr4{std::move(ptr3)}; // OK，可以
};