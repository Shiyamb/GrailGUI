#include <cstring>
#include <utility>

// Credit for this goes to Conor Taylor, they did the majority of the work. I'm
// just fitting it to the project. https://taylorconor.com/blog/enum-reflection/

// TODO: sort out everything in the namespaces. Some of these functions should
// be hidden in the anonymous namespace

namespace EnumCounter {

namespace {}  // anonymous namespace

/**
 * @brief Checks if an enum element is "valid" or not at compile time.
 *
 * This uses template meta-programming to iterate through an enum at compile
 * time and determine if each index corresponds to a valid enum value. By
 * counting one past the end, this can be used to count how many elements are
 * in the enum.
 *
 * @tparam E The enum type
 * @tparam V The enum Value
 * @return true
 * @return false
 */
template <typename E, E V>
constexpr bool is_valid() {
  // Name should contain the type signature of this function, which can be
  // inspected to determine the index of this element.
  auto name = __PRETTY_FUNCTION__;
  int i = strlen(name);

  // The type signature of this stored in name should contain the type and then
  // the value or a number. Examples of this (from GCC 10) are shown below:
  // "constexpr bool is_valid() [with E = Type; E V = Type::UINT16]" is valid
  // as UINT16 is an element of Type
  // "constexpr bool is_valid() [with E = Type; E V = 6]" is not valid

  // Walk back to the last space of the signature
  for (; i >= 0; --i) {
    if (name[i] == ' ') {
      break;
    }
  }

  // If the character after the space is a letter, then it likely is a class
  // name compliant with the C++ standard.
  char c = name[i + 1];
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/**
 * @brief The base case for count_valid.
 *
 * This is invoked when there is nothing left in the integer sequence used for
 * count_valid<E, B>
 *
 * @tparam E The enum type
 * @return constexpr int always 0
 */
template <typename E>
constexpr int count_valid() {
  return 0;
}

/**
 * @brief Returns the number of valid enum elements given an integer sequence
 *
 * This function takes an integer sequence of indices (therefore they must be
 * from (a >= 0) to (b <= a number less than the template recursion limit of
 * your compiler) and then checks a given enum at each index. The number
 * returned is the number of valid enum elements within that range.
 *
 * @tparam E The enum type
 * @tparam V The current enum value
 * @tparam P A sequence of the remaining indices to be checked
 * @return constexpr int The number of valid indices in the integer sequence
 */
template <typename E, E V, E... P>
constexpr int count_valid() {
  int current_valid = is_valid<E, V>();
  return count_valid<E, P...>() + (int)current_valid;
}

/**
 * @brief A wrapper for count_valid that uses an integer sequence
 *
 * This takes an integer sequence and uses it as the second template parameter
 * for count_valid
 *
 * @tparam E The enum type
 * @tparam I The remaining indices to be checked
 * @param unused The integer sequence containing the indices to be checked
 * @return constexpr int The number of valid indices in the integer sequence
 */
template <typename E, int... I>
constexpr int internal_element_count(std::integer_sequence<int, I...> unused) {
  return count_valid<E, (E)I...>();
}

/**
 * @brief A convenience wrapper for counting the number of valid enum elements
 *
 * This only checks the first 100 elements or so, although this can be invoked
 * with a different number by manually calling internal_element_count.
 *
 * @tparam E The enum type
 */
template <typename E>
struct element_count {
  static const int val =
      internal_element_count<E>(std::make_integer_sequence<int, 100>());
};

/**
 * @brief Counts the number of valid elements in an enum class
 *
 * This takes advantage of the continuity of enum classes to count all valid
 * elements in the enum class. It uses compile-time conditional evaluation to
 * determine if the next index points to a valid enum element, recurs if it is,
 * and then returns the number of valid elements at the end.
 *
 * This should not be called directly, but instead called through
 * enum_class_size()
 *
 * @tparam E The enum type
 * @tparam V The enum value currently being checked
 * @return constexpr size_t The number of valid elements in the enum class
 */
template <typename E, E V>
constexpr size_t enum_class_size_wrapper() {
  if constexpr (is_valid<E, V>()) {
    return enum_class_size_wrapper<E, (E)((int)V + 1)>();
  } else {
    return (int)V;
  }
}

// Enum classses are defined strictly on [0, enum_size], so it should be
// possible to do a version without having to invoke integer sequences

/**
 * @brief Counts the number of valid elements in an enum class
 *
 * This takes advantage of the continuity of enum classes to count all valid
 * elements in the enum class.
 *
 * @tparam E The enum type
 * @return constexpr size_t The number of valid elements in the enum class
 */
template <typename E>
constexpr size_t enum_class_size() {
  return enum_class_size_wrapper<E, (E)0>();
}

}  // namespace EnumCounter