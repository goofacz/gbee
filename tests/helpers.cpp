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

using namespace gbee;

TEST(Helpers, are_values_unique)
{
   EXPECT_TRUE((are_values_unique<1>) );
   EXPECT_TRUE((are_values_unique<1, 2>) );
   EXPECT_TRUE((are_values_unique<1, 2, 3, 4>) );

   EXPECT_FALSE((are_values_unique<1, 1>) );
   EXPECT_FALSE((are_values_unique<1, 2, 3, 2>) );
}

TEST(Helpers, are_types_unique)
{
   EXPECT_TRUE((are_types_unique<int>) );
   EXPECT_TRUE((are_types_unique<int, double>) );
   EXPECT_TRUE((are_types_unique<uint8_t, uint16_t, uint32_t, uint64_t>) );

   EXPECT_FALSE((are_types_unique<int, int>) );
   EXPECT_FALSE((are_types_unique<uint8_t, uint16_t, uint32_t, uint16_t>) );
}
