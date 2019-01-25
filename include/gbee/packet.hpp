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

#include <array>
#include <cstddef>
#include <cstring>
#include <gbee/helpers.hpp>
#include <type_traits>

namespace gbee {

namespace details::packet {

template<auto id, typename Field>
struct lookup_field_base : public std::enable_if<Field::id == id, Field>
{};

template<auto id, typename... Fields>
struct lookup_field : public lookup_field_base<id, Fields>...
{};

template<bool found, auto id, std::size_t current_value, typename...>
struct offset_helper;

template<auto id, std::size_t current_value, typename Field, typename... Fields>
struct offset_helper<true, id, current_value, Field, Fields...>
  : public offset_helper<true, id, current_value, Fields...>
{};

template<auto id, std::size_t current_value, typename Field, typename... Fields>
struct offset_helper<false, id, current_value, Field, Fields...>
  : public offset_helper<Field::id == id,
                         id,
                         Field::id == id ? current_value : current_value + Field::size,
                         Fields...>
{};

template<bool found, auto id, std::size_t current_value>
struct offset_helper<found, id, current_value>
{
   static_assert(found);
   static constexpr std::size_t value{current_value};
};

template<auto id, typename... Fields>
struct offset : public offset_helper<false, id, 0, Fields...>
{};

} // namespace details::packet

template<auto initial_id, typename T>
struct Field
{
   using value_type = T;
   static constexpr auto id{initial_id};
   static constexpr std::size_t size{sizeof(value_type)};
};

template<typename... Fields>
class Packet
{
   static_assert(are_values_unique<Fields::id...>);

 private:
   template<auto id>
   struct lookup_field : public details::packet::lookup_field<id, Fields...>::type
   {};

   template<auto id>
   struct offset : public details::packet::offset<id, Fields...>
   {};

   template<auto id>
   using field_valie_type = typename lookup_field<id>::value_type;

 public:
   using id_type = std::decay_t<decltype((Fields::id, ...))>;

   static constexpr std::size_t size{(lookup_field<Fields::id>::size + ...)};

   template<auto id, std::size_t buffer_size>
   static void
   inject(std::array<std::uint8_t, buffer_size>& buffer,
          const field_valie_type<id>& value,
          std::size_t base_offset = 0)
   {
      static_assert(buffer_size >= size);
      const size_t field_offset{base_offset + offset<id>::value};
      std::memcpy(buffer.data() + field_offset, &value, lookup_field<id>::size);
   }

   template<auto id>
   static void
   extract(field_valie_type<id>& value, std::size_t base_offset = 0)
   {
      std::memcpy(&value, (void*) (0 + base_offset + offset<id>::value), lookup_field<id>::size);
   }
};

} // namespace gbee
