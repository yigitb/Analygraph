#pragma once
#include <memory>

#include "Image.h"

class IAnalygraph {
public:
	enum Mode {
		AN_TRUE,
		AN_GRAY,
		AN_COLOR,
		AN_HALF_COLOR,
		AN_3D_OPT,
	};

	IAnalygraph() = default;
	virtual ~IAnalygraph() {};
	virtual int Compute(const Image* l, const Image *r, Mode mode) = 0;
	virtual std::unique_ptr<Image> GetImage() = 0;

	virtual const char* GetName() const = 0;

	IAnalygraph(const IAnalygraph&) = delete;
	IAnalygraph(IAnalygraph&&) = delete;

	IAnalygraph& operator=(const IAnalygraph&) = delete;
	IAnalygraph& operator=(IAnalygraph&&) = delete;
};