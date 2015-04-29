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
struct is_placeholder<RhIO::placeholder_custom<N>> 
    : integral_constant<int, N+1> {};

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
 * Generic conversion from string to typed value
 * (only for typical type) and print type textual
 * name
 */
//Used to fail static_assert
template<typename T>
struct FromStringError : std::false_type {};
//Default (not implemented) implemented case
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
//Type specialization
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
 * Bind one given argument at first place 
 * and set all remaining arguements as placeholder
 */
template <typename Ret, typename Arg, typename ... Args, int ... IntSeq>
std::function<Ret(Args...)> custom_bind_aux(
    std::function<Ret(Arg, Args...)> func, Arg arg, int_sequence<IntSeq...>)
{
    return std::bind(func, arg, placeholder_custom<IntSeq>()...);
}
template <typename Ret, typename Arg, typename ... Args>
std::function<Ret(Args...)> custom_bind(
    std::function<Ret(Arg, Args...)> func, Arg arg)
{
    return custom_bind_aux(func, arg, make_int_sequence<sizeof...(Args)>());
}

/**
 * Custom binding for this to member method
 * Bind given member method to its instance object
 * and set all remaining arguments to placeholders
 */
template <typename T, typename Ret, typename ... Args, int ... IntSeq>
std::function<Ret(Args...)> custom_bind_member_aux(
    Ret (T::*func)(Args...), T* self, int_sequence<IntSeq...>)
{
    return std::bind(func, self, placeholder_custom<IntSeq>()...);
}
template <typename T, typename Ret, typename ... Args>
std::function<Ret(Args...)> custom_bind_member(Ret (T::*func)(Args...), T* self)
{
    return custom_bind_member_aux(
        func, self, make_int_sequence<sizeof...(Args)>());
}

/**
 * Parse given string arguments and default values
 * and bind to given function
 */
template <int N, typename Ret> 
std::function<Ret(void)> params_bind(
    std::function<Ret(void)> func, 
    const std::vector<std::string>& params,
    const std::vector<std::string>& defaultArgs)
{
    (void)params;
    (void)defaultArgs;
    return func;
}
template <int N, typename Ret, typename Arg, typename ... Args>
std::function<Ret(void)> params_bind(
    std::function<Ret(Arg, Args...)> func, 
    const std::vector<std::string>& params,
    const std::vector<std::string>& defaultArgs)
{
    Arg val;
    if (N < params.size()) {
        val = FromString<Arg>::convert(params.at(N));
    } else if (N < defaultArgs.size() && defaultArgs.at(N) != "") {
        val = FromString<Arg>::convert(defaultArgs.at(N));
    } else {
        throw std::runtime_error(std::string(
            "RhIO bind error at argument ") + std::to_string(N+1));
    }

    return params_bind<N+1, Ret, Args...>(
        custom_bind(func, val), params, defaultArgs);
}

/**
 * Build and return given function type argument 
 * textual parameters usage using given default
 * arguments
 */
template <int N>
std::string bind_usage_aux(const std::vector<std::string>& defaultArgs)
{
    (void)defaultArgs;
    return std::string();
}
template <int N, typename Arg, typename ... Args>
std::string bind_usage_aux(const std::vector<std::string>& defaultArgs)
{
    std::string part = FromString<Arg>::type();
    if (
        N < defaultArgs.size() && 
        defaultArgs.at(N) != ""
    ) {
        //Print default value
        part += "|" + defaultArgs.at(N);
    } 
    return "<" + part + "> " + bind_usage_aux<N+1, Args...>(defaultArgs);
}
template <typename Ret, typename ... Args>
std::string bind_usage(const std::vector<std::string>& defaultArgs)
{
    //Print arguments and return type
    return bind_usage_aux<0, Args...>(defaultArgs) 
        + "--> <" + FromString<Ret>::type() + ">";
}

/**
 * Implement command method binding
 */
template <typename T, typename Ret, typename ... Args>
void Bind::bindFunc(
    const std::string& name, 
    const std::string& comment, 
    Ret (T::*func)(Args...), 
    T& self,
    const std::vector<std::string>& defaultArgs)
{
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Check that if default arguments value are given, 
    //all arguments are listed
    if (
        defaultArgs.size() > 0 && 
        defaultArgs.size() != sizeof...(Args)
    ) {
        throw std::logic_error(
            "RhIO default parameters given with invalid size");
    }
    //Create and implement the command
    node().newCommand(name, comment, 
    [func, &self, name, defaultArgs]
    (const std::vector<std::string>& params) -> std::string {
        //Bind member method with given class instance
        auto tmpFunc = custom_bind_member(func, &self);
        //Bind function parameters with parsed value on
        //command line
        try {
            auto tmpFunc2 = params_bind<0, Ret, Args...>(
                tmpFunc, params, defaultArgs);
            return std::to_string(tmpFunc2());
        } catch (const std::exception& e) {
            if (std::string(e.what()).find("RhIO bind error") != 
                std::string::npos
            ) {
                //Catch bind error and display usage
                return std::string(e.what()) + ".\nUSAGE: " 
                    + std::string(name) + " " 
                    + bind_usage<Ret, Args...>(defaultArgs);
            } else {
                //Catch user exception and forward
                return "User exception: " + std::string(e.what());
            }
        }

    });
}

}
    
#endif

