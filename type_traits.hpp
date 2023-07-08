#include<cstddef>

template<class T> struct type_is{using type = T;};

template<bool, class T> struct do_if : type_is<T> {};
template<class T> struct do_if<false, T> {};

template<bool, class T, class U> struct choose : type_is<T> {};
template<class T, class U> struct choose<false, T, U> : type_is<U> {};

template<typename...> using voidify = void;





template <typename T> constexpr inline bool is_bool = false;
template<> constexpr inline bool is_bool<bool> = true;

template<class T, T v> struct integral_constant {
 static constexpr T value = v;
};

struct true_type{
 static constexpr bool value = true;
};

struct false_type{
 static constexpr bool value = false;
};

template<class T, class U> constexpr inline bool are_same = false;
template<class T> constexpr inline bool are_same<T, T> = true;

template<class T, typename... Us> constexpr inline bool is_one_of = (are_same<T, Us> || ...);





template <typename T> constexpr inline bool is_const = false;
template <typename T> constexpr inline bool is_const<const T> = true;

template <typename T> constexpr inline bool is_volatile = false;
template <typename T> constexpr inline bool is_volatile<volatile T> = true;

template <typename T> constexpr inline bool is_const_volatile = false;
template <typename T> constexpr inline bool is_const_volatile<const volatile T> = true;

template <typename T> constexpr inline bool is_lref = false;
template <typename T> constexpr inline bool is_lref<T&> = true;

template <typename T> constexpr inline bool is_rref = false;
template <typename T> constexpr inline bool is_rref<T&&> = true;

template <typename T> constexpr inline bool is_any_ref = is_lref<T> || is_rref<T>;

template<class T> struct add_const : type_is<const T> {};
template<class T> struct add_const<const T> : type_is<const T> {};

template<class T> struct remove_const : type_is<T> {};
template<class T> struct remove_const<const T> : type_is<T> {};

template<class T> struct add_volatile : type_is<volatile T> {};
template<class T> struct add_volatile<volatile T> : type_is<T> {};

template<class T> struct remove_volatile : type_is<T> {};
template<class T> struct remove_volatile<volatile T> : type_is<T> {};

template<class T> struct add_cv : type_is<add_const<add_volatile<T>>> {};
template<class T> struct remove_cv : type_is<remove_volatile<remove_const<T>>> {};

template<class T> struct remove_refs : type_is<T> {};
template<class T> struct remove_refs<T&> : type_is<T> {};
template<class T> struct remove_refs<T&&> : type_is<T> {};

template<class T, class = void> struct add_lref : type_is<T> {};
template<class T> struct add_lref<T, voidify<T&>> : type_is<T&> {};

template<class T, class = void> struct add_rref : type_is<T> {};
template<class T> struct add_rref<T, voidify<T&&>> : type_is<T&&> {};

template<class T> struct remove_cvref : type_is<remove_cv<remove_refs<T>>> {};





template <typename T> constexpr inline bool is_signed = is_one_of<remove_cvref<T>, char, int, short, long long>;
template <typename T> constexpr inline bool is_unsigned = is_one_of<remove_cvref<T>, unsigned char, unsigned short, unsigned int, unsigned long long>;
template <typename T> constexpr inline bool is_integral = is_signed<T> || is_unsigned<T> || is_bool<T>;
template <typename T> constexpr inline bool is_floating_point = is_one_of<remove_cvref<T>, float, double, long double>;
template <typename T> constexpr inline bool is_arithmetic = is_integral<T> || is_floating_point<T>;

template <typename T> constexpr inline bool is_void = are_same<remove_cv<T>::type, void>;
template <typename T> constexpr inline bool is_nullptr_t = are_same<remove_cv<T>::type, std::nullptr_t>;
template <typename T> constexpr inline bool is_pointer = false;
template <typename T> constexpr inline bool is_pointer<T*> = true;
template <typename T> constexpr inline bool is_pointer<const T*> = true;
template <typename T> constexpr inline bool is_pointer<volatile T*> = true;
template <typename T> constexpr inline bool is_pointer<const volatile T*> = true;

template<class T> struct add_pointer : type_is<T*> {};
template<class T> struct remove_pointer : type_is<T> {};
template<class T> struct remove_pointer<T*> : type_is<T> {};

namespace {
 template <typename T> constexpr inline bool is_memberptr_h = false;
 template<class T, class U> constexpr inline bool is_memberptr_h<T U::*> = true;
}
template <typename T> constexpr inline bool is_memberptr = is_memberptr_h<remove_refs<T>::type>;
template <typename T> constexpr inline bool is_fundamental = is_arithmetic<T> || is_void<T> || is_nullptr_t<T>;
template <typename T> constexpr inline bool is_compound = !is_fundamental<T>;

template <typename T> constexpr inline bool is_unbounded_array = false;
template <typename T> constexpr inline bool is_unbounded_array<T[]> = true;
template<class T, size_t N>
constexpr inline bool is_bounded_array = false;
template<class T, size_t N>
constexpr inline bool is_bounded_array<T[N], N> = true;


template <typename T> constexpr inline bool is_array = false;
template <typename T> constexpr inline bool is_array<T[]> = true;
template<class T, size_t N>
constexpr inline bool is_array<T[N]> = true;





template<class T> constexpr inline size_t rank = 0;

template<class T> constexpr inline size_t rank<T[]> = 1 + rank<T>;

template<class T, size_t N> constexpr size_t rank<T[N]> = 1 + rank<T>;

template<class T> struct remove_extent : type_is<T> {};
template<class T> struct remove_extent<T[]> : type_is<T> {};
template<class T, size_t N> struct remove_extent<T[N]> : type_is<T> {};

template<class T> struct remove_all_extents : type_is<T> {} ;
template<class T> struct remove_all_extents<T[]> : type_is<typename remove_all_extents<T>::type> {};
template<class T, size_t N> struct remove_all_extents<T[N]> : type_is<typename remove_all_extents<T>::type> {};


namespace {
 template<class T> using copy_assignment_t = decltype(declval<T&>() = declval<T const&>());
 template<class T> using move_assignment_t = decltype(declval<T&>() = declval<T&&>());
}

template<class T, class = void> constexpr inline bool is_copy_assignable = false;
template<class T> constexpr inline bool is_copy_assignable<T, voidify<copy_assignment_t<T>>> = are_same<T&, copy_assignment_t<T>>;

template<class T, class = void> constexpr inline bool is_move_assignable = false;
template<class T> constexpr inline bool is_move_assignable<T, voidify<move_assignment_t<T>>> = are_same<T&, move_assignment_t<T>>;

template<class T, class = void> constexpr inline bool is_default_constructible = false;
template<class T> constexpr inline bool is_default_constructible<T, voidify<decltype(T())>> = true;

template<class T, class = void> constexpr inline bool is_copy_constructible = false;
template<class T> constexpr inline bool is_copy_constructible<T, voidify<decltype(T(declval<const T&>()))>> = true;


template<class T, class = void> constexpr inline bool is_move_constructible = false;
template<class T> constexpr inline bool is_move_constructible<T, voidify<decltype(T(declval<T&&>()))>> = true;

namespace {
 template<class T, class U = typename remove_all_extents<T>::type> using has_destructor = decltype(declval<U>().~U());
}
template<class T, class = void> constexpr inline bool is_destructible = false;
template<class T> constexpr inline bool is_destructible<T, voidify<has_destructor<T>>> = true;




template <typename T> constexpr inline bool is_function = !is_const<const T> && !is_any_ref<T>;




namespace {
    template <class T, class = void>
    struct is_UDT_h : false_type { };
    template <class T>
    struct is_UDT_h<T, voidify<char T::*>> : true_type { };
}
template <typename T> constexpr inline bool is_UDT = is_UDT_h<T>::value;

template <typename T> constexpr inline bool is_member_variable_ptr = is_memberptr<T> && !is_function<T>;
template <typename T> constexpr inline bool is_member_function_ptr = is_memberptr<T> && is_function<T>;

template<class T, class = void> constexpr inline bool is_enum = false;
template<class T> constexpr inline bool is_enum<T, voidify<decltype(+T{})>> = true;

template <typename T> constexpr inline bool is_scalar = is_arithmetic<T> || is_enum<T> || is_pointer<T> || is_memberptr<T> || is_nullptr_t<T>;




namespace{
    template <class B>
    true_type test_pre_ptr_convertible(const volatile B*);
    template <class>
    false_type test_pre_ptr_convertible(const volatile void*);

    template <class, class>
    auto test_pre_is_base_of(...) -> true_type;
    template <class B, class D>
    auto test_pre_is_base_of(int) ->
        decltype(test_pre_ptr_convertible<B>(static_cast<D*>(nullptr)));
}

template<class Base, class Derived>
constexpr inline bool is_base_of = is_UDT<Base> && is_UDT<Derived> && decltype(test_pre_is_base_of<Base, Derived>(0))::value;


template<class From, class To>
struct is_convertible_h
{
private:
  static void pre_test(To);
  template<class F>
  static auto test(int) -> decltype(pre_test(declval<F>()), true_type{});
  template<typename>
  static auto test(...) -> false_type;
public:
  static constexpr bool value = decltype(test<From>(0))::value;
  constexpr operator bool() { return value; }
};

template<class From, class To> constexpr inline bool is_convertible = is_convertible_h<From, To>::value;


