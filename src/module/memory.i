/* -----------------------------------------------------------------------------
 * memory.i
 *
 * std::shared_ptr and std::weak_ptr typemaps
 * ----------------------------------------------------------------------------- */

%{
#include <memory>
%}

namespace std {

template<class T>
class shared_ptr;

template<class T>
class weak_ptr;

template<class T>
class shared_ptr {
  public:
    shared_ptr();
    
    template<class Y>
    explicit shared_ptr( Y * p );
    
    template<class Y, class D>
    shared_ptr(Y * p, D d);
    
    template<class Y, class D, class A>
    shared_ptr( Y * p, D d, A a );
    
    // copy constructor
    shared_ptr( shared_ptr const & r );
    
    template<class Y>
    explicit shared_ptr(weak_ptr<Y> const & r);
    
    template<class Y>
    shared_ptr( shared_ptr<Y> const & r );
    
    template< class Y >
    shared_ptr( shared_ptr<Y> const & r, T * p );
    
    // assignment
    //shared_ptr & operator=( shared_ptr const & r );
    
    //template<class Y>
    //shared_ptr & operator=(shared_ptr<Y> const & r);
    
    // =========================
    
    void reset();
    
    template< class Y >
    void reset( Y* ptr );
    
    template< class Y, class Deleter >
    void reset( Y* ptr, Deleter d );
    
    template< class Y, class Deleter, class Alloc >
    void reset( Y* ptr, Deleter d, Alloc alloc );
    
    template<class Y> void reset( shared_ptr<Y> const & r, T * p );
    
    // Getters
    
    T& operator* () const;
    T* operator-> () const;
    T* get() const;
    
    //explicit operator bool() const;
    
    bool unique() const;
    long use_count() const;
    
    void swap(shared_ptr<T> & other);
    
  private:
    template<class Y> bool owner_before( shared_ptr<Y> const & rhs ) const;
    template<class Y> bool owner_before( weak_ptr<Y> const & rhs ) const;
};

template<class T>
class weak_ptr {
  public:
    weak_ptr();
    weak_ptr( weak_ptr const & r );
    //weak_ptr & operator=( weak_ptr const & r );
    
    template<class Y>
    weak_ptr( weak_ptr<Y> const & r );
    
    template<class Y>
    weak_ptr( shared_ptr<Y> const & r );
    
    //template<class Y>
    //weak_ptr & operator=(weak_ptr<Y> const & r);
    
    //template<class Y>
    //weak_ptr & operator=(shared_ptr<Y> const & r);
    
    // =========================
    
    shared_ptr<T> lock() const;
    long use_count() const;
    bool expired() const;
    void reset();
    void swap( weak_ptr& r );
    
    template<class Y> bool owner_before( weak_ptr<Y> const & rhs ) const;
    template<class Y> bool owner_before( shared_ptr<Y> const & rhs ) const;
    
  private:
    template<class Y> friend class weak_ptr;
    template<class Y> friend class shared_ptr;
};

template<class T>
class enable_shared_from_this {
  public:
    shared_ptr<T> shared_from_this();
    shared_ptr<T const> shared_from_this() const;
  protected:
    enable_shared_from_this() throw();
    enable_shared_from_this(const enable_shared_from_this&) throw();
    //enable_shared_from_this& operator=(const enable_shared_from_this&) throw();
    ~enable_shared_from_this() throw();
};

}