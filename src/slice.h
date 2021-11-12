#include <cstddef>

/// <summary>
/// The design of slice is owed to the "The C++ Programming language", Bjarne Stroustrup,
/// 4th edition, section 29.4.1, page 841.
/// </summary>
class slice {
private:
	/// <summary>
	/// First index
	/// </summary>
	int start;

	/// <summary>
	/// Number of indices included
	/// </summary>
	int length;

	/// <summary>
	/// Distance between the elements in the sequence
	/// </summary>
	int stride;
public:
	slice() :start{ -1 }, length{ -1 }, stride{ -1 }
	{}
	explicit slice(int s):start{s},length{-1},stride{1}
	{}
	slice(int s, int l, int n=1): start{s},length{l},stride{n}
	{}

	int operator()(int i) const
	{
		return (start + stride * i);
	}

	int getLength() const
	{
		return length;
	}
};