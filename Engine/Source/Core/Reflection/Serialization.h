#pragma once
#include "Core/Config.h"
#include "Core/Object/IObject.h"


namespace Eggy
{
	template<typename T, typename U, typename... Rest>
	struct is_one_of : std::bool_constant<is_one_of<T, U>::value || is_one_of<T, Rest...>::value> {};
	
	template<typename T, typename U, typename... Rest>
	constexpr bool is_one_of_v = is_one_of<T, U, Rest...>::value;

	template<typename T, typename U>
	struct is_one_of<T, U> : std::is_same<T, U> {};


	template<typename Inst, template<typename ...> typename Tmpl>
	struct is_instantiation_of : std::false_type {};

	template<typename Inst, template<typename ...> typename Tmpl>
	constexpr bool is_instantiation_of_v = is_instantiation_of<Inst, Tmpl>::value;

	template<template<typename...> typename Tmpl, typename... Args>
	struct is_instantiation_of<Tmpl<Args...>, Tmpl> : std::true_type {};


	namespace Serialization
	{
		using std::enable_if_t;
		using std::decay_t;
		using std::is_array_v;
		using std::stringstream;

		// Primitive
		template<typename T>
		enable_if_t<
			is_one_of_v<
				decay_t<T>,
				int, long, long long, unsigned,
				unsigned long, unsigned long long,
				float, double, long double
			>,
			String
		> Serialize(const T& value)
		{
			return std::to_string(value);
		}

		// string, char
		template<typename T>
		enable_if_t <is_one_of_v <decay_t<T>, String, char>, String> Serialize(const T& object)
		{
			stringstream ss;
			ss << '"' << object << '"';
			return ss.str();
		}

		// char*
		static inline String Serialize(const char* s)
		{
			return Serialize(String(s));
		}

		// void, nullptr
		template<typename T>
		enable_if_t <is_one_of_v <decay_t<T>, void, nullptr_t>, String> Serialize(const T&)
		{
			return "";
		}

		// bool
		template<typename T>enable_if_t <is_one_of_v <decay_t<T>, bool>, String> Serialize(const T& value)
		{
			return value ? "true" : "false";
		}

		// vector, list, set...
		template<template<typename...> class Tmpl, typename... Args>
		std::enable_if_t<
			is_instantiation_of_v<Tmpl<Args...>, std::vector> ||
			is_instantiation_of_v<Tmpl<Args...>, std::list> ||
			is_instantiation_of_v<Tmpl<Args...>, std::set>, // ...
			String
		>
			Serialize(const Tmpl<Args...>& obj)
		{
			stringstream ss;
			ss << "[";
			for (auto itr = obj.begin(); itr != obj.end();)
			{
				ss << Serialize(*itr);
				if (++itr != obj.end())  ss << ",";
			}
			ss << "]";
			return ss.str();
		}

		// map
		template<template<typename...> class Tmpl, typename... Args>
		std::enable_if_t<
			is_instantiation_of_v<Tmpl<Args...>, std::map> ||
			is_instantiation_of_v<Tmpl<Args...>, std::unordered_map> ||
			is_instantiation_of_v<Tmpl<Args...>, std::multimap>, // ...
			String
		>
			Serialize(const Tmpl<Args...>& obj)
		{
			stringstream ss;
			ss << "{";
			for (auto itr = obj.begin(); itr != obj.end();)
			{
				ss << Serialize(itr->first);
				ss << ":";
				ss << Serialize(itr->second);
				if (++itr != obj.end())  ss << ",";
			}
			ss << "}";
			return ss.str();
		}

		// pair
		template<typename T, typename U>
		String Serialize(const std::pair<T, U>& obj)
		{
			stringstream ss;
			ss << "{" << Serialize(obj.first) << ":" << Serialize(obj.second) << "}";
			return ss.str();
		}
		// array
		template<typename T>
		std::enable_if_t<is_array_v<T>, String> Serialize(const T& arr)
		{
			stringstream ss;
			ss << "[";
			for (size_t i = 0; i < extent<T>::value; ++i)
			{
				ss << Serialize(arr[i]);
				if (i != extent<T>::value - 1)  ss << ",";
			}
			ss << "]";
			return ss.str();
		}

		// std::array
		template<typename T, size_t N>
		String Serialize(const std::array<T, N>& obj)
		{
			stringstream ss;
			ss << "[";
			for (auto itr = obj.begin(); itr != obj.end();)
			{
				ss << Serialize(*itr);
				if (++itr != obj.end())  ss << ",";
			}
			ss << "]";
			return ss.str();
		}

		// std::tuple
		template<size_t N, typename... Args>
		std::enable_if_t<N == sizeof...(Args) - 1, String> Serialize(const std::tuple<Args...>& obj)
		{
			stringstream ss;
			ss << Serialize(get<N>(obj)) << "]";
			return ss.str();
		}
		template<size_t N, typename... Args>
		std::enable_if_t<N != 0 && N != sizeof...(Args) - 1, String> Serialize(const std::tuple<Args...>& obj)
		{
			stringstream ss;
			ss << Serialize(get<N>(obj)) << "," << Serialize<N + 1, Args...>(obj);
			return ss.str();
		}

		template<size_t N, typename... Args>
		std::enable_if_t<N == 0, String> Serialize(const std::tuple<Args...>& obj)
		{
			stringstream ss;
			ss << "[" << Serialize(get<N>(obj)) << "," << Serialize<N + 1, Args...>(obj);
			return ss.str();
		}

		// char*
		static inline String Serialize(const IObject* object)
		{
			return object->Serialize();
		}

	}
}

