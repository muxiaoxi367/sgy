

#include <ripple/basics/Slice.h>
#include <ripple/beast/unit_test.h>
#include <array>
#include <cstdint>

namespace ripple {
namespace test {

struct Slice_test : beast::unit_test::suite
{
    void run() override
    {
        std::uint8_t const data[] =
        {
            0xa8, 0xa1, 0x38, 0x45, 0x23, 0xec, 0xe4, 0x23,
            0x71, 0x6d, 0x2a, 0x18, 0xb4, 0x70, 0xcb, 0xf5,
            0xac, 0x2d, 0x89, 0x4d, 0x19, 0x9c, 0xf0, 0x2c,
            0x15, 0xd1, 0xf9, 0x9b, 0x66, 0xd2, 0x30, 0xd3
        };

        {
            testcase ("Equality & Inequality");

            Slice const s0{};

            BEAST_EXPECT (s0.size() == 0);
            BEAST_EXPECT (s0.data() == nullptr);
            BEAST_EXPECT (s0 == s0);

            for (std::size_t i = 0; i != sizeof(data); ++i)
            {
                Slice const s1 { data, i };

                BEAST_EXPECT (s1.size() == i);
                BEAST_EXPECT (s1.data() != nullptr);

                if (i == 0)
                    BEAST_EXPECT (s1 == s0);
                else
                    BEAST_EXPECT (s1 != s0);

                for (std::size_t j = 0; j != sizeof(data); ++j)
                {
                    Slice const s2 { data, j };

                    if (i == j)
                        BEAST_EXPECT (s1 == s2);
                    else
                        BEAST_EXPECT (s1 != s2);
                }
            }

            std::array<std::uint8_t, sizeof(data)> a;
            std::array<std::uint8_t, sizeof(data)> b;

            for (std::size_t i = 0; i != sizeof(data); ++i)
                a[i] = b[i] = data[i];

            BEAST_EXPECT (makeSlice(a) == makeSlice(b));
            b[7]++;
            BEAST_EXPECT (makeSlice(a) != makeSlice(b));
            a[7]++;
            BEAST_EXPECT (makeSlice(a) == makeSlice(b));
        }

        {
            testcase ("Indexing");

            Slice const s  { data, sizeof(data) };

            for (std::size_t i = 0; i != sizeof(data); ++i)
                BEAST_EXPECT (s[i] == data[i]);
        }

        {
            testcase ("Advancing");

            for (std::size_t i = 0; i < sizeof(data); ++i)
            {
                for (std::size_t j = 0; i + j < sizeof(data); ++j)
                {
                    Slice s (data + i, sizeof(data) - i);
                    s += j;

                    BEAST_EXPECT (s.data() == data + i + j);
                    BEAST_EXPECT (s.size() == sizeof(data) - i - j);
                }
            }
        }
    }
};

BEAST_DEFINE_TESTSUITE(Slice, ripple_basics, ripple);

}  
}  
























