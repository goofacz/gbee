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

#include <cstdint>
#include <gbee/gbee.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <type_traits>

enum class Foo
{
   A,
   B,
   C,
   D,
   E
};

using namespace gbee;

using FooPacket = Packet<Field<Foo::A, std::uint16_t>,
                         Field<Foo::B, std::uint32_t>,
                         Field<Foo::C, std::uint8_t>,
                         Field<Foo::D, std::uint64_t>,
                         Field<Foo::E, std::uint8_t>>;

TEST(Field, validate)
{
   using Bar = Field<Foo::B, uint16_t>;
   EXPECT_EQ(Bar::id, Foo::B);
   EXPECT_EQ(Bar::size, sizeof(uint16_t));
   EXPECT_TRUE((std::is_same_v<Bar::value_type, uint16_t>) );
}

TEST(Field, inject)
{
   std::array<std::uint8_t, 16> buffer{{0}};
   static_assert(buffer.size() >= FooPacket::size);

   const std::uint16_t a_value{0x1111};
   FooPacket::inject<Foo::A>(buffer.data(), buffer.size(), a_value);
   EXPECT_THAT(buffer, ::testing::ElementsAre(0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint32_t b_value{0x22222222};
   FooPacket::inject<Foo::B>(buffer.data(), buffer.size(), b_value);
   EXPECT_THAT(buffer, ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00,
                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint8_t c_value{0x33};
   FooPacket::inject<Foo::C>(buffer.data(), buffer.size(), c_value);
   EXPECT_THAT(buffer, ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x00, 0x00,
                                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint64_t d_value{0x4444444444444444};
   FooPacket::inject<Foo::D>(buffer.data(), buffer.size(), d_value);
   EXPECT_THAT(buffer, ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44,
                                              0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00));

   const std::uint8_t e_value{0x55};
   FooPacket::inject<Foo::E>(buffer.data(), buffer.size(), e_value);
   EXPECT_THAT(buffer, ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44,
                                              0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x55));
}

TEST(Field, extract)
{
   const std::array<uint8_t, 16> buffer{{0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44, 0x44,
                                         0x44, 0x44, 0x44, 0x44, 0x44, 0x55}};
   static_assert(buffer.size() >= FooPacket::size);

   std::uint16_t a_value{0};
   FooPacket::extract<Foo::A>(buffer.data(), buffer.size(), a_value);
   EXPECT_EQ(a_value, 0x1111);

   std::uint32_t b_value{0};
   FooPacket::extract<Foo::B>(buffer.data(), buffer.size(), b_value);
   EXPECT_EQ(b_value, 0x22222222);

   std::uint8_t c_value{0};
   FooPacket::extract<Foo::C>(buffer.data(), buffer.size(), c_value);
   EXPECT_EQ(c_value, 0x33);

   std::uint64_t d_value{0};
   FooPacket::extract<Foo::D>(buffer.data(), buffer.size(), d_value);
   EXPECT_EQ(d_value, 0x4444444444444444);

   std::uint8_t e_value{0};
   FooPacket::extract<Foo::E>(buffer.data(), buffer.size(), e_value);
   EXPECT_EQ(e_value, 0x55);
}
