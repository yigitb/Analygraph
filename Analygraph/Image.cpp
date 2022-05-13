#include <cstdio>
#include <stdexcept>

#include "Image.h"

Image::~Image() {
	if (bitmap) {
		FreeImage_Unload(bitmap);
		bitmap = nullptr;
	}
	type_ = FIT_UNKNOWN;
	w_ = 0;
	h_ = 0;
	bpp_ = 0;
}

Image::Image(const Image& other) {
	if (other.bitmap) {
		bitmap = FreeImage_Clone(other.bitmap);
		if (!bitmap)
			throw std::bad_alloc();
		type_ = other.type_;
		w_ = other.w_;
		h_ = other.h_;
		bpp_ = other.bpp_;
	}
	else {
		bitmap = nullptr;
		type_ = FIT_UNKNOWN;
		w_ = 0;
		h_ = 0;
		bpp_ = 0;
	}
}

Image::Image(Image&& other) noexcept {
	bitmap = other.bitmap;
	type_ = other.type_;
	w_ = other.w_;
	h_ = other.h_;
	bpp_ = other.bpp_;
	other.bitmap = nullptr;
	other.type_ = FIT_UNKNOWN;
	other.w_ = 0;
	other.h_ = 0;
	other.bpp_ = 0;
}

Image& Image::operator=(const Image& other) {
	if (other.bitmap) {
		bitmap = FreeImage_Clone(other.bitmap);
		if (!bitmap)
			throw std::bad_alloc();
		type_ = other.type_;
		w_ = other.w_;
		h_ = other.h_;
		bpp_ = other.bpp_;
	}
	else {
		bitmap = nullptr;
		type_ = FIT_UNKNOWN;
		w_ = 0;
		h_ = 0;
		bpp_ = 0;
	}

	return *this;
}

Image& Image::operator=(Image&& other) noexcept {
	bitmap = other.bitmap;
	type_ = other.type_;
	w_ = other.w_;
	h_ = other.h_;
	bpp_ = other.bpp_;
	other.bitmap = nullptr;
	other.type_ = FIT_UNKNOWN;
	other.w_ = 0;
	other.h_ = 0;

	return *this;
}

Image::Image(const char* path) {
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path, 0);
	bitmap = FreeImage_Load(fif, path, 0);

	if (!bitmap) {
		printf("couldn't load input file: %s\n", path);
		throw std::invalid_argument("could not open file");
	}

	type_ = FreeImage_GetImageType(bitmap);
	h_ = FreeImage_GetHeight(bitmap);
	w_ = FreeImage_GetWidth(bitmap);
	bpp_ = FreeImage_GetBPP(bitmap);

	printf("loaded image: type: %u, h: %u, w: %u, bpp: %u\n", type_, h_, w_, bpp_);
}

Image::Image(unsigned w, unsigned h, unsigned bpp) {
	bitmap = FreeImage_Allocate(w, h, bpp);
	if (!bitmap) {
		printf("couldn't allocate output\n");
		throw std::bad_alloc();
	}

	type_ = FIT_BITMAP;
	w_ = w;
	h_ = h;
	bpp_ = bpp;
}

std::unique_ptr<Image> Image::ConvertToBpp(unsigned bpp) const {
	if (bpp == bpp_)
		return std::make_unique<Image>(*this);

	auto ret = std::make_unique<Image>();
	if (bpp == 32) {
		ret->bitmap = FreeImage_ConvertTo32Bits(bitmap);
	}
	else if (bpp == 24) {
		ret->bitmap = FreeImage_ConvertTo24Bits(bitmap);
	}
	else {
		printf("unsupported bpp: %u\n", bpp);
		return nullptr;
	}

	ret->bpp_ = bpp;
	ret->w_ = w_;
	ret->h_ = h_;
	ret->type_ = type_;

	return ret;
};

uint8_t* Image::GetScanLine(unsigned y) {
	return FreeImage_GetScanLine(bitmap, y);
}

const uint8_t* Image::GetScanLine(unsigned y) const {
	return FreeImage_GetScanLine(bitmap, y);
}

int Image::Save(const char* path) const {
	if (FreeImage_Save(FIF_PNG, bitmap, path, 0))
		return 0;
	return -1;
}