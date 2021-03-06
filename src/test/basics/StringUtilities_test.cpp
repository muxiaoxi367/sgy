

#include <ripple/basics/Slice.h>
#include <ripple/basics/StringUtilities.h>
#include <ripple/basics/ToString.h>
#include <ripple/beast/unit_test.h>

namespace ripple {

class StringUtilities_test : public beast::unit_test::suite
{
public:
    void testUnHexSuccess (std::string const& strIn, std::string const& strExpected)
    {
        auto rv = strUnHex (strIn);
        BEAST_EXPECT(rv.second);
        BEAST_EXPECT(makeSlice(rv.first) == makeSlice(strExpected));
    }

    void testUnHexFailure (std::string const& strIn)
    {
        auto rv = strUnHex (strIn);
        BEAST_EXPECT(! rv.second);
        BEAST_EXPECT(rv.first.empty());
    }

    void testUnHex ()
    {
        testcase ("strUnHex");

        testUnHexSuccess ("526970706c6544", "RippleD");
        testUnHexSuccess ("A", "\n");
        testUnHexSuccess ("0A", "\n");
        testUnHexSuccess ("D0A", "\r\n");
        testUnHexSuccess ("0D0A", "\r\n");
        testUnHexSuccess ("200D0A", " \r\n");
        testUnHexSuccess ("282A2B2C2D2E2F29", "(*+,-./)");

        testUnHexFailure ("123X");
        testUnHexFailure ("V");
        testUnHexFailure ("XRP");
    }

    void testParseUrl ()
    {
        testcase ("parseUrl");

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain.empty());
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path.empty());
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme:///"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain.empty());
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "lower://domain"));
            BEAST_EXPECT(pUrl.scheme == "lower");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path.empty());
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "UPPER://domain:234/"));
            BEAST_EXPECT(pUrl.scheme == "upper");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(*pUrl.port == 234);
            BEAST_EXPECT(pUrl.path == "/");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "Mixed://domain/path"));
            BEAST_EXPECT(pUrl.scheme == "mixed");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/path");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://[::1]:123/path"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "::1");
            BEAST_EXPECT(*pUrl.port == 123);
            BEAST_EXPECT(pUrl.path == "/path");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://user:pass@domain:123/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username == "user");
            BEAST_EXPECT(pUrl.password == "pass");
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(*pUrl.port == 123);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://user@domain:123/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username == "user");
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(*pUrl.port == 123);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://:pass@domain:123/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password == "pass");
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(*pUrl.port == 123);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://domain:123/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(*pUrl.port == 123);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://user:pass@domain/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username == "user");
            BEAST_EXPECT(pUrl.password == "pass");
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://user@domain/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username == "user");
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://:pass@domain/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password == "pass");
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://domain/abc:321"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/abc:321");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme:///path/to/file"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain.empty());
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/path/to/file");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (
                  pUrl, "scheme://user:pass@domain/path/with/an@sign"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username == "user");
            BEAST_EXPECT(pUrl.password == "pass");
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/path/with/an@sign");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (
                  pUrl, "scheme://domain/path/with/an@sign"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "domain");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/path/with/an@sign");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "scheme://:999/"));
            BEAST_EXPECT(pUrl.scheme == "scheme");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == ":999");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(parseUrl (pUrl, "http://::1:1234/validators"));
            BEAST_EXPECT(pUrl.scheme == "http");
            BEAST_EXPECT(pUrl.username.empty());
            BEAST_EXPECT(pUrl.password.empty());
            BEAST_EXPECT(pUrl.domain == "::0.1.18.52");
            BEAST_EXPECT(! pUrl.port);
            BEAST_EXPECT(pUrl.path == "/validators");
        }

        {
            parsedURL pUrl;
            BEAST_EXPECT(! parseUrl (pUrl, ""));
            BEAST_EXPECT(! parseUrl (pUrl, "nonsense"));
            BEAST_EXPECT(! parseUrl (pUrl, "://"));
            BEAST_EXPECT(! parseUrl (pUrl, ":///"));
        }

        {
            std::string strUrl("s://" + std::string(8192, ':'));
            parsedURL pUrl;
            BEAST_EXPECT(! parseUrl (pUrl, strUrl));
        }
    }

    void testToString ()
    {
        testcase ("toString");
        auto result = to_string("hello");
        BEAST_EXPECT(result == "hello");
    }

    void run () override
    {
        testParseUrl ();
        testUnHex ();
        testToString ();
    }
};

BEAST_DEFINE_TESTSUITE(StringUtilities, ripple_basics, ripple);

} 
























