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

enum class Foo1
{
   A,
   B
};

enum class Foo2
{
   C,
   D,
   E
};

enum class Foo3
{
   F
};

using namespace gbee;

using FooGroup1 = Group<Field<Foo1::A, std::uint16_t>, Field<Foo1::B, std::uint32_t>>;

using FooGroup2 =
  Group<Field<Foo2::C, std::uint8_t>, Field<Foo2::D, std::uint64_t>, Field<Foo2::E, std::uint8_t>>;

using FooGroup3 = Group<Field<Foo3::F, std::uint32_t>>;

using FooFrame = Frame<FooGroup1, FooGroup2, FooGroup3>;

TEST(Frame, has_valid_buffer_size)
{
   using Sample1 = Frame<FooGroup1>;
   std::array<std::uint8_t, 6> sample1_buffer{{0}};
   Sample1 sample1_frame{sample1_buffer};
   EXPECT_TRUE(sample1_frame.has_valid_buffer_size());

   using Sample2 = Frame<FooGroup1, FooGroup3>;
   std::array<std::uint8_t, 10> sample2_buffer{{0}};
   Sample2 sample2_frame{sample2_buffer};
   EXPECT_TRUE(sample2_frame.has_valid_buffer_size());

   std::array<std::uint8_t, 20> foo_buffer{{0}};
   FooFrame foo_frame{foo_buffer};
   EXPECT_TRUE(foo_frame.has_valid_buffer_size());
}

TEST(Frame, inject)
{
   std::array<std::uint8_t, 20> buffer{{0}};

   FooFrame frame{buffer};
   static_assert(buffer.size() == FooFrame::size);

   const std::uint16_t a_value{0x1111};
   frame.inject<Foo1::A>(a_value);
   EXPECT_THAT(buffer,
               ::testing::ElementsAre(0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint32_t b_value{0x22222222};
   frame.inject<Foo1::B>(b_value);
   EXPECT_THAT(buffer,
               ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint8_t c_value{0x33};
   frame.inject<Foo2::C>(c_value);
   EXPECT_THAT(buffer,
               ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint64_t d_value{0x4444444444444444};
   frame.inject<Foo2::D>(d_value);
   EXPECT_THAT(buffer,
               ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44, 0x44,
                                      0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00));

   const std::uint8_t e_value{0x55};
   frame.inject<Foo2::E>(e_value);
   EXPECT_THAT(buffer,
               ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44, 0x44,
                                      0x44, 0x44, 0x44, 0x44, 0x44, 0x55, 0x00, 0x00, 0x00, 0x00));

   const std::uint32_t f_value{0x66666666};
   frame.inject<Foo3::F>(f_value);
   EXPECT_THAT(buffer,
               ::testing::ElementsAre(0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44, 0x44,
                                      0x44, 0x44, 0x44, 0x44, 0x44, 0x55, 0x66, 0x66, 0x66, 0x66));
}

TEST(Field, extract)
{
   std::array<uint8_t, 20> buffer{{0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x33, 0x44, 0x44, 0x44,
                                   0x44, 0x44, 0x44, 0x44, 0x44, 0x55, 0x66, 0x66, 0x66, 0x66}};

   FooFrame frame{buffer};
   static_assert(buffer.size() == FooFrame::size);

   std::uint16_t a_value{0};
   frame.extract<Foo1::A>(a_value);
   EXPECT_EQ(a_value, 0x1111);

   std::uint32_t b_value{0};
   frame.extract<Foo1::B>(b_value);
   EXPECT_EQ(b_value, 0x22222222);

   std::uint8_t c_value{0};
   frame.extract<Foo2::C>(c_value);
   EXPECT_EQ(c_value, 0x33);

   std::uint64_t d_value{0};
   frame.extract<Foo2::D>(d_value);
   EXPECT_EQ(d_value, 0x4444444444444444);

   std::uint8_t e_value{0};
   frame.extract<Foo2::E>(e_value);
   EXPECT_EQ(e_value, 0x55);
}
