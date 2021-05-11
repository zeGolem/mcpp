#include <exception>

namespace mcpp
{
namespace utils
{
struct exception : public std::exception {
	const char *m_what;
	exception(const char *what) : m_what(what) {}
	const char *what() const throw() { return m_what; }
};
} // namespace utils
} // namespace mcpp
