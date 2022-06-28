
#include <boost/predef.h> // Tools to identify the OS.

// We need this to enable cancelling of I/O operations on
// Windows XP, Windows Server 2003 and earlier.
// Refer to "http://www.boost.org/doc/libs/1_58_0/
// doc/html/boost_asio/reference/basic_stream_socket/
// cancel/overload1.html" for details.
#ifdef BOOST_OS_WINDOWS
#define _WIN32_WINNT 0x0501

#if _WIN32_WINNT <= 0x0502 // Windows Server 2003 or earlier.
#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO  
#endif
#endif

#include <boost/asio.hpp>

#include <thread>
#include <mutex>
#include <memory>
#include <iostream>

using namespace boost;

namespace http_errors {
    enum http_error_codes
    {
        invalid_response = 1
    };

    class http_errors_category
        : public boost::system::error_category
    {
    public:
        const char* name() const BOOST_SYSTEM_NOEXCEPT
        {
            return "http_errors";
        }

        std::string message(int e) const {
            switch (e) {
            case invalid_response:
                return "Server response cannot be parsed.";
                break;
            default:
                return "Unknown error.";
                break;
            }
        }
    };

    const boost::system::error_category&
        get_http_errors_category()
    {
        static http_errors_category cat;
        return cat;
    }

    boost::system::error_code
        make_error_code(http_error_codes e)
    {
        return boost::system::error_code(
            static_cast<int>(e), get_http_errors_category());
    }
} // namespace http_errors

