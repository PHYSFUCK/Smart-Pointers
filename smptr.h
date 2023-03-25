#include<iostream>


using i64 = int64_t;
using u64 = uint64_t;

template <class T>

class UniquePtr {
    public:
    UniquePtr() : ptr(nullptr){}

    T& operator*() {
        return *ptr;

    }

    T* operator->() {
        return ptr;
    }

    ~UniquePtr() {delete ptr;}
    
    UniquePtr(T * value) {
        ptr = value;
    }
    
    UniquePtr& operator=(T * value){
        if (ptr == value){
            return *this;
        }
        T * tmp = value;
        std::swap(tmp, ptr);
        return *this;
    }
    
    UniquePtr(const UniquePtr&) = delete;
    
    UniquePtr& operator= (const UniquePtr&) = delete;
    
    UniquePtr(UniquePtr&& rhs) {
        ptr = nullptr;
        std::swap(ptr, rhs.ptr);
    }
    
    UniquePtr& operator=(UniquePtr&& rhs){
        if(this == &rhs){
            return *this;
        }
        UniquePtr tmp(std::move(rhs));
        std::swap(ptr, tmp.ptr);
        return *this;
    }
    
    
    private:
    T * ptr;
};

struct RefCounter {
    i64 strong_counter = 0;
    i64 weak_counter = 0;
};

template <class T>

class SharedPtr {
    public:
    SharedPtr(){
        ptr = nullptr;
        ref_count->strong_counter = 0;
        ref_count->weak_counter = 0;
        //weak_count = new i64(0);
    }

    ~SharedPtr() {
        //std::cout << "DT " << ptr << '\n';
        if (ref_count == nullptr){
            delete ptr;
            delete ref_count;
        } else {
            if (ref_count->strong_counter > 0){
                //std::cout << "Case 1" << '\n';
                --(ref_count->strong_counter);
            }
            if (ref_count ->strong_counter <= 0) {
                delete ptr;
                if (ref_count->weak_counter <= 0){
                //std::cout << "Case 2" << '\n';
                //std::cout << ptr << '\n';                 
                delete ref_count; 
                }
            }
        }
    }

    void Reset() {
        if (ref_count == nullptr){
            return;
        }
        //std::cout << "reset" << '\n';
        
        //std::cout << *ref_count << '\n';
        --(ref_count->strong_counter);
        //std::cout << 2 << '\n';
        if (ref_count->strong_counter <= 0) {
            T * new_ptr = nullptr;           
            //T * weak = nullptr;
            std::swap(ptr, new_ptr);
            delete new_ptr;

            if (ref_count->weak_counter <=0 ){
                RefCounter * count = nullptr;
                std::swap(ref_count, count);
                
                delete count;
            }
        }
    }

    SharedPtr(const SharedPtr& rhs) {
        ptr = rhs.ptr;
        ref_count = rhs.ref_count;
        //weak_count = rhs.weak_count;
        if (rhs.ptr != nullptr) {
            ++(ref_count->strong_counter);
        }
    }

    SharedPtr& operator=(const SharedPtr& rhs){
        //std::cout << "IN " << '\n';
        if (this == &rhs) {
            //std::cout << "IF" << '\n';
            return *this;
        }
        
        --(ref_count->strong_counter);
        if (ref_count->strong_counter <= 0) {
            //std::cout << "ptr " << ptr << '\n';
            //std::cout << "Going to del "<< ptr << '\n';
            delete ptr;

            //i64 * count = new i64(0);
            //std::swap(ref_count, count);
            if (ref_count->weak_counter <= 0 ){
                delete ref_count;
            }
            //std::cout << "After swap " << *ptr << '\n'; 
        }
        ptr = rhs.ptr;
        ref_count = rhs.ref_count;
        /*
        SharedPtr tmp(rhs);
        std::swap(tmp.ptr, ptr);
        std::swap(tmp.ref_count, ref_count);  //Добавление уже существующего объекта
        */
        //ptr = rhs.ptr;
        //ref_count = rhs.ref_count;

        if (rhs.ptr != nullptr) {
            ++(ref_count->strong_counter);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& rhs) {
        ptr = nullptr;
        ref_count = new RefCounter;
        //std :: cout << "ref count of null " << *ref_count << '\n';
        std::swap(ptr, rhs.ptr);
        std::swap(ref_count, rhs.ref_count);
        //std :: cout << "init " << rhs.ptr << '\n';
    }

    SharedPtr& operator=(SharedPtr&& rhs) {
        //std::cout << ptr << '\n';
        if (this == &rhs) {
            return *this;
        }
        //std::cout << *rhs.RefCount() << '\n';
        SharedPtr tmp(std::move(rhs));
        //std::cout << 1 << '\n'; 
        std::swap(tmp.ptr, ptr);
        //std::cout << 2 << '\n'; 
        std::swap(tmp.ref_count, ref_count);
        //std::cout << *ref_count << '\n';
        return *this;
    }

    SharedPtr(T * value) {
        ptr = value;
        ref_count = new RefCounter;
        ref_count->strong_counter = 1;
    }

    SharedPtr& operator=(T * value) {
        --(ref_count->strong_counter);
        if (ref_count->strong_counter <= 0) {
            //std::cout << "ptr " << ptr << '\n';
            //std::cout << "Going to del "<< ptr << '\n';
            delete ptr;

            //i64 * count = new i64(0);
            //std::swap(ref_count, count);
            if (ref_count->weak_counter <= 0 ){
                delete ref_count;
            }
        }
        SharedPtr tmp(value);
        std::swap(ptr, tmp.ptr);
        std::swap(ref_count, tmp.ref_count);
        return *this;

    }

    T& operator*() {
        return *ptr;

    }

    T* operator->() {
        return ptr;
    }

    RefCounter* RefCount() {
        return ref_count;
    }

    T* Get() {
        return ptr;
    }


    
    private:
    T * ptr;
    RefCounter * ref_count;
    

};

template <class T>

class WeakPtr {
    public:
    WeakPtr() {
        ptr = nullptr;
        ref_count->weak_counter = 1;
        
        
    }

    ~WeakPtr() {
        if (ref_count == nullptr){
        
        }
        else {if (ref_count->weak_counter > 0){
            --ref_count->weak_counter;
        }
        if (ref_count->weak_counter <= 0 ){
            if (ref_count -> strong_counter <=0) {
                delete ref_count;
            }
        }
        }
    
    }

    WeakPtr(const WeakPtr& rhs) {
        ptr = rhs.ptr;
        ref_count = rhs.ref_count;
        ++(ref_count->weak_counter);
    }

    WeakPtr& operator=(const WeakPtr& rhs){
        if (this == &rhs) {
            return *this;
        }
        --(ref_count->weak_counter);
        ptr = rhs.ptr;
        ref_count = rhs.ref_count;
        return *this;
    }

    WeakPtr(WeakPtr&& rhs){
        ptr = nullptr;
        ref_count = nullptr;
        std::swap(ptr, rhs.ptr);
        std::swap(ref_count, rhs.ref_count);
        
    }

    WeakPtr& operator=(WeakPtr&& rhs) {
        if (this == &rhs) {
            return *this;
        }
        WeakPtr tmp(std::move(rhs));
        std::swap(ptr, tmp.ptr);
        std::swap(ref_count, tmp.ref_count);
        return *this;

    }

    WeakPtr(SharedPtr<T> rhs){

        ptr = rhs.Get();
        ref_count = rhs.RefCount();
        ++(ref_count->weak_counter);
    }

    WeakPtr& operator=(SharedPtr<T> rhs) {
        --(ref_count->weak_counter);
        if (ref_count->strong_counter<=0){
            if(ref_count->weak_counter <= 0){
                delete ref_count;
            }
        }
        ptr = rhs.Get();
        ref_count = rhs.RefCount();
        return *this;
    }

    void Reset() {
        if (ref_count == nullptr) {
            return;
        }
        --(ref_count->weak_counter);
        if (ref_count->strong_counter <= 0 ){
            RefCounter * count = nullptr;
            std::swap(ref_count, count);
            delete count;
        }
    }

    bool Expired() {
        //std::cout << "IN expired " << ptr << '\n';
        if (ref_count->strong_counter == 0) {
            return true;
        }

        return false;
    }
    T* Lock() {
        return ptr;
    }

    i64 GetCount(){
        return ref_count->strong_counter;
    }

    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }
    private:
    T * ptr;
    RefCounter * ref_count;
};