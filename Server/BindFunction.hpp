#ifndef RHIO_BINDFUNCTION_HPP
#define RHIO_BINDFUNCTION_HPP

namespace RhIO {

/**
 * Custom placeholder for bind with
 * compile time argument number parameter
 */
template <int>
struct placeholder_custom {};

}

namespace std {

/**
 * Register RhIO::placeholder_custom is valid placeholder
 * inside std namespace
 */
template <int N>
struct is_placeholder<RhIO::placeholder_custom<N>> : integral_constant<int, N+1> {};

/**
 * Overloading standart to_string
 * with dummy string conversion
 */
inline string to_string(const string& str)
{
    return str;
}

}

namespace RhIO {

/**
 * Generic conbersion from string to typed value
 * (only for typical type)
 */
template<typename T>
struct FromStringError : std::false_type {};
template <typename T>
struct FromString {
    inline static T convert(const std::string& str) {
        static_assert(FromStringError<T>::value, 
            "RhIO type used is unsupported by automatic function binding");
        (void) str;
        return T();
    }
    inline static std::string type() {
        return "ERROR";
    }
};
template <>
struct FromString<bool> {
    inline static bool convert(const std::string& str) {
        if (str == "true" || str == "1") {
            return true;
        } else if (str == "false" || str == "0") {
            return false;
        } else {
            return false;
        }
    }
    inline static std::string type() {
        return "bool";
    }
};
template <>
struct FromString<int> {
    inline static int convert(const std::string& str) {
        return stoi(str);
    }
    inline static std::string type() {
        return "int";
    }
};
template <>
struct FromString<long> {
    inline static long convert(const std::string& str) {
        return stol(str);
    }
    inline static std::string type() {
        return "long";
    }
};
template <>
struct FromString<float> {
    inline static float convert(const std::string& str) {
        return stof(str);
    }
    inline static std::string type() {
        return "float";
    }
};
template <>
struct FromString<double> {
    inline static double convert(const std::string& str) {
        return stod(str);
    }
    inline static std::string type() {
        return "double";
    }
};
template <>
struct FromString<std::string> {
    inline static std::string convert(const std::string& str) {
        return str;
    }
    inline static std::string type() {
        return "string";
    }
};

/**
 * Compile time sequence of integer
 */
template <int...>
struct int_sequence {};
template <int N, int... IntSeq>
struct make_int_sequence : make_int_sequence<N-1, N-1, IntSeq...> {};
template <int... IntSeq>
struct make_int_sequence<0, IntSeq...> : int_sequence<IntSeq...> {};

/**
 * Custom binding for std::function
 */
template <typename Ret, typename Arg, typename ... Args, int ... IntSeq>
std::function<Ret(Args...)> custom_bind_aux(std::function<Ret(Arg, Args...)> func, Arg arg, int_sequence<IntSeq...>)
{
    return std::bind(func, arg, placeholder_custom<IntSeq>()...);
}
template <typename Ret, typename Arg, typename ... Args>
std::function<Ret(Args...)> custom_bind(std::function<Ret(Arg, Args...)> func, Arg arg)
{
    return custom_bind_aux(func, arg, make_int_sequence<sizeof...(Args)>());
}

/**
 * Custom binding for this to member method
 */
template <typename T, typename Ret, typename ... Args, int ... IntSeq>
std::function<Ret(Args...)> custom_bind_member_aux(Ret (T::*func)(Args...), T* self, int_sequence<IntSeq...>)
{
    return std::bind(func, self, placeholder_custom<IntSeq>()...);
}
template <typename T, typename Ret, typename ... Args>
std::function<Ret(Args...)> custom_bind_member(Ret (T::*func)(Args...), T* self)
{
    return custom_bind_member_aux(func, self, make_int_sequence<sizeof...(Args)>());
}

/**
 * Bind given string parameters to function
 */
template <int N, typename Ret> 
std::function<Ret(void)> params_bind(
    std::function<Ret(void)> func, 
    const std::vector<std::string>& params)
{
    (void)params;
    return func;
}
template <int N, typename Ret, typename Arg, typename ... Args>
std::function<Ret(void)> params_bind(
    std::function<Ret(Arg, Args...)> func, 
    const std::vector<std::string>& params)
{
    if (params.size() <= N) {
        throw std::runtime_error("RhIO bind error: not enough parameters");
    }
    Arg val = FromString<Arg>::convert(params.at(N));
    return params_bind<N+1, Ret, Args...>(custom_bind(func, val), params);
}
template <int N, typename Ret, typename Arg>
std::function<Ret(void)> params_bind(
    std::function<Ret(Arg)> func, 
    const std::vector<std::string>& params)
{
    if (params.size() != N-1) {
        throw std::runtime_error("RhIO bind error: size mismatch");
    }
    Arg val = FromString<Arg>::convert(params[N]);
    return custom_bind(func, val);
}

/**
 *
 */
template <typename T, typename Ret, typename ... Args>
void Bind::bindFunc(
    const std::string& name, 
    const std::string& comment, 
    Ret (T::*func)(Args...), 
    T& self,
    const std::vector<std::string>& defaultArgs)
{
    //Check that if default arguments value are given, 
    //all arguments are listed
    if (
        defaultArgs.size() > 0 && 
        defaultArgs.size() != sizeof...(Args)
    ) {
        throw std::logic_error(
            "RhIO default parameters given with invalid size");
    }
    node().newCommand(name, comment, 
    [func, &self, &name](const std::vector<std::string>& params) -> std::string {
        auto tmpFunc = custom_bind_member(func, &self);
        auto tmpFunc2 = params_bind<0, Ret, Args...>(tmpFunc, params);
        return std::to_string(tmpFunc2());
    });
}

}
    
#endif

