#ifndef WEBSERV_SFINAE_HPP_
#define WEBSERV_SFINAE_HPP_

template<class T, T v>
struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant type;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T> struct is_int_template           : public false_type {};
template <> struct is_int_template<int>                     : public true_type {};


template <class T> struct remove_const { typedef T type; };
template <class T> struct remove_const<const T> { typedef T type; };

template <class T> struct is_integral : public is_int_template<typename remove_const<T>::type> {};


template<bool, class Type = void>
struct enable_if {};

template <class Type>
struct enable_if<true, Type>
{
  typedef Type type;
};

template <class T>
class DetectInt {
 private:
  typedef char yes[1];
  typedef yes no[2];
  template<typename C> static yes& test (typename C::value_type* = 0) {};
  template<typename> static no& test(...) {};
 public:
  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

#endif
