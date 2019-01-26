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
#include <gbee/helpers.hpp>
#include <type_traits>

namespace gbee {

namespace details::frame {
template<typename Id, typename Packet>
struct lookup_packet_base
  : public std::enable_if<std::is_same_v<Id, typename Packet::id_type>, Packet>
{};

template<typename Id, typename... Packets>
struct lookup_packet : public lookup_packet_base<Id, Packets>...
{};

template<bool found, typename Id, std::size_t current_value, typename...>
struct offset_helper;

template<typename Id, std::size_t current_value, typename Packet, typename... Packets>
struct offset_helper<true, Id, current_value, Packet, Packets...>
  : public offset_helper<true, Id, current_value, Packets...>
{};

template<typename Id, std::size_t current_value, typename Packet, typename... Packets>
struct offset_helper<false, Id, current_value, Packet, Packets...>
  : public offset_helper<
      std::is_same_v<typename Packet::id_type, Id>,
      Id,
      std::is_same_v<typename Packet::id_type, Id> ? current_value : current_value + Packet::size,
      Packets...>
{};

template<bool found, typename Id, std::size_t current_value>
struct offset_helper<found, Id, current_value>
{
   static_assert(found);
   static constexpr std::size_t value{current_value};
};

template<typename Id, typename... Packets>
struct offset : public offset_helper<false, Id, 0, Packets...>
{};

} // namespace details::frame

template<typename... Packets>
class Frame
{
   static_assert(are_types_unique<typename Packets::id_type...>);

 private:
   template<typename Id>
   struct lookup_packet : public details::frame::lookup_packet<Id, Packets...>::type
   {};

   template<typename Id>
   struct offset : public details::frame::offset<Id, Packets...>
   {};

 public:
   static constexpr std::size_t size{(lookup_packet<typename Packets::id_type>::size + ...)};

   template<std::size_t array_size>
   explicit Frame(std::array<std::uint8_t, array_size>& initial_buffer)
     : buffer{initial_buffer.data()}, buffer_size{array_size}
   {
      // TODO Throw exception if array_size < size?
   }

   bool
   has_valid_buffer_size() const
   {
      return buffer_size <= size;
   }

   template<auto id, typename T>
   void
   inject(const T& value)
   {
      using packet_type = typename details::frame::lookup_packet<decltype(id), Packets...>::type;
      constexpr std::size_t base_offset{offset<decltype(id)>::value};
      packet_type::template inject<id>(buffer, buffer_size, value, base_offset);
   }

   template<auto id, typename T>
   void
   extract(T& value) const
   {
      using packet_type = typename details::frame::lookup_packet<decltype(id), Packets...>::type;
      constexpr std::size_t base_offset{offset<decltype(id)>::value};
      packet_type::template extract<id>(buffer, buffer_size, value, base_offset);
   }

   std::uint8_t* buffer;
   const std::size_t buffer_size;
};

} // namespace gbee
