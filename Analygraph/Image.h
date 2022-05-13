#pragma once

#include <cstdint>
#include <memory>

#include "FreeImage.h"

class Image
{
public:
	Image() : w_(0), h_(0), bpp_(0), type_(FIT_UNKNOWN), bitmap(nullptr) {};
	~Image();

	Image(const Image& other);
	Image(Image&& other) noexcept;

	Image& operator=(const Image& other);
	Image& operator=(Image&& other) noexcept;

	Image(const char* path);

	Image(unsigned w, unsigned h, unsigned bpp);

	const uint8_t* GetScanLine(unsigned y) const;

	uint8_t* GetScanLine(unsigned y);

	std::unique_ptr<Image> ConvertToBpp(unsigned bpp) const;

	int Save(const char *path) const;

	FREE_IMAGE_TYPE type() const {
		return type_;
	}

	unsigned w() const {
		return w_;
	}

	unsigned h() const {
		return h_;
	}

	unsigned bpp() const {
		return bpp_;
	}

private:
	FIBITMAP* bitmap;
	FREE_IMAGE_TYPE type_;
	unsigned w_;
	unsigned h_;
	unsigned bpp_;
};

