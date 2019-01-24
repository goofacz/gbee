//
// Copyright (C) 2019 Tomasz Jankowski tomasz.jankowski.mail AT gmail.com>
//
// This is part of GBee library.
//
// GBee is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GBee is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#pragma once

#include <type_traits>

namespace gbee {

namespace details {

template<auto...>
struct are_values_unique;

template<>
struct are_values_unique<>
{
   static constexpr bool value{true};
};

template<auto element, auto... elements>
struct are_values_unique<element, elements...>
{
   static constexpr bool value{((element != elements) && ...) &&
                               are_values_unique<elements...>::value};
};

template<typename...>
struct are_types_unique;

template<>
struct are_types_unique<>
{
   static constexpr bool value{true};
};

template<typename T, typename... Ts>
struct are_types_unique<T, Ts...>
{
   static constexpr bool value{((!std::is_same_v<T, Ts>) &&...) && are_types_unique<Ts...>::value};
};

} // namespace details

template<auto... Ts>
inline constexpr bool are_values_unique{details::are_values_unique<Ts...>::value};

template<typename... Ts>
inline constexpr bool are_types_unique{details::are_types_unique<Ts...>::value};

} // namespace gbee
