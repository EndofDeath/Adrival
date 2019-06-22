#ifndef ADRIVAL_NOCOPYABLE_H
#define ADRIVAL_NOCOPYABLE_H

namespace adrival {

class nocopyable
{
public:
	nocopyable() = default;
	nocopyable(const nocopyable&) = delete;
	nocopyable& operator=(const nocopyable&) = delete;
};

}
#endif // !ADRIVAL_NOCOPYABLE_H