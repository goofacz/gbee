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
#include <gtest/gtest.h>
#include <type_traits>

enum class Foo
{
   A = 4,
   B,
   C
};

using namespace gbee;

TEST(Field, validate)
{
   using Bar = Field<Foo::B, uint16_t>;
   EXPECT_EQ(Bar::id, Foo::B);
   EXPECT_EQ(Bar::size, sizeof(uint16_t));
   EXPECT_TRUE((std::is_same_v<Bar::value_type, uint16_t>) );
}
