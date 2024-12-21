#include <app/series.hpp>

#include <cstdint>

template <> const std::string Series<uint8_t>::dtype_name = "U8";

template <> const std::string Series<uint16_t>::dtype_name = "U16";

template <> const std::string Series<uint32_t>::dtype_name = "U32";

template <> const std::string Series<uint64_t>::dtype_name = "U64";

template <> const std::string Series<int8_t>::dtype_name = "I8";

template <> const std::string Series<int16_t>::dtype_name = "I16";

template <> const std::string Series<int32_t>::dtype_name = "I32";

template <> const std::string Series<int64_t>::dtype_name = "I64";

template <> const std::string Series<float>::dtype_name = "F32";

template <> const std::string Series<double>::dtype_name = "F64";

template <> const std::string Series<std::string>::dtype_name = "Str";
