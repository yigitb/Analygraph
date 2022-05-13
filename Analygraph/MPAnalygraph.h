#pragma once
#include "IAnalygraph.h"
class MPAnalygraph :
	public IAnalygraph
{
public:
	MPAnalygraph(unsigned w, unsigned h);
	virtual ~MPAnalygraph();

	virtual int Compute(const Image* l, const Image* r, Mode mode);
	virtual std::unique_ptr<Image> GetImage();

	virtual const char* GetName() const {
		return "MP";
	}

private:
	// Assume BGR format
	static constexpr const unsigned short left_shorts[][9] = {
		{0, 0, 0, 0, 0, 0, 117, 601, 306},
		{0, 0, 0, 0, 0, 0, 117, 601, 306},
		{0, 0, 0, 0, 0, 0, 0, 0, 1024},
		{0, 0, 0, 0, 0, 0, 117, 601, 306},
		{0, 0, 0, 0, 0, 0, 307, 716, 0},
	};

	static constexpr const unsigned short right_shorts[][9] = {
		{117, 601, 306, 0, 0, 0, 0, 0, 0},
		{117, 601, 306, 117, 601, 306, 0, 0, 0},
		{1024, 0, 0, 0, 1024, 0, 0, 0, 0},
		{1024, 0, 0, 0, 1024, 0, 0, 0, 0},
		{1024, 0, 0, 0, 1024, 0, 0, 0, 0},
	};

	std::unique_ptr<Image> img_;
};

