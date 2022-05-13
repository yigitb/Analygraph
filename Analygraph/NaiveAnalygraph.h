#pragma once

#include <memory>

#include "IAnalygraph.h"

class NaiveAnalygraph :
    public IAnalygraph
{
public:
	NaiveAnalygraph(unsigned w, unsigned h);
	virtual ~NaiveAnalygraph();

	virtual int Compute(const Image* l, const Image* r, Mode mode);
	virtual std::unique_ptr<Image> GetImage();

	virtual const char* GetName() const {
		return "Naive";
	}

private:
	// Assume BGR format
	static constexpr const float left_floats[][9] = {
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.114f, 0.587f, 0.299f},
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.114f, 0.587f, 0.299f},
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.114f, 0.587f, 0.299f},
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.7f, 0.0f},
	};

	static constexpr const float right_floats[][9] = {
		{0.114f, 0.587f, 0.299f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{0.114f, 0.587f, 0.299f, 0.114f, 0.587f, 0.299f, 0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	};

	std::unique_ptr<Image> img_;
};

