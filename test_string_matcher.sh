# Copyright (C) 2015 Jesse Busman
# You may not distribute this program or parts of this
# program in any way, shape or form without written
# permission from Jesse Busman (born 17 march 1996).
# This program is distributed in the hope that it will
# be useful, but WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. This message may not be changed
# or removed.

rm test_string_matcher.exe
rm _gen_tag_name_matcher.cpp
php string_matcher_generator.php tag_name_matcher.str > _gen_tag_name_matcher.cpp
g++ const_str.cpp test_string_matcher.cpp -std=c++11 -o test_string_matcher.exe
./test_string_matcher.exe
