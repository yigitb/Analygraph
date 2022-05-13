// ZetaQuiz2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <chrono>

#include "Image.h"
#include "NaiveAnalygraph.h"
#include "IntAnalygraph.h"
#include "MPAnalygraph.h"
#include "SimdAnalygraph.h"
#include "MPSimdAnalygraph.h"

#define LOOPS 20

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char* message) {
	printf("\n*** ");
	if (fif != FIF_UNKNOWN) {
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}

std::unique_ptr<Image> timed_compute(IAnalygraph &f, const Image* l, const Image* r, IAnalygraph::Mode mode) {
	printf("Running %s algorithm\n", f.GetName());
	auto start = std::chrono::steady_clock::now();
	int ret = 0;
	for (int i = 0; i < LOOPS; i++)
		ret |= f.Compute(l, r, mode);
	auto end = std::chrono::steady_clock::now();

	if (ret)
		printf("something went wrong, output/timing is probably wrong\n");
	std::chrono::duration<double> time = end - start;
	printf("operation took %f seconds\n", time.count() / LOOPS);
	return f.GetImage();
}

int main(int argc, char* argv[]) {
	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	printf(FreeImage_GetVersion());
	printf("\n");
	printf(FreeImage_GetCopyrightMessage());
	printf("\n");

	if (argc != 3) {
		printf("Usage : %s <left image> <right image>\n", argv[0]);
		return 0;
	}

	Image left(argv[1]);
	Image right(argv[2]);

	if (left.type() != right.type() || left.h() != right.h() || left.w() != right.w() || left.bpp() != right.bpp()) {
		printf("image parameters don't match\n");
		exit(1);
	}

	if (left.type() != FIT_BITMAP || left.bpp() != 24) {
		printf("sorry, I only support 24b bitmaps for now\n");
		exit(1);
	}

	NaiveAnalygraph naive(left.w(), left.h());

	auto naiveImg = timed_compute(naive, &left, &right, IAnalygraph::AN_COLOR);
	if (naiveImg->Save("naive_output.png"))
		printf("couldn't save naive_output.png\n");

	IntAnalygraph intAn(left.w(), left.h());

	auto intImg = timed_compute(intAn, &left, &right, IAnalygraph::AN_COLOR);
	if (intImg->Save("int_output.png"))
		printf("couldn't save int_output.png\n");

	MPAnalygraph mpAn(left.w(), left.h());

	auto mpImg = timed_compute(mpAn, &left, &right, IAnalygraph::AN_COLOR);
	if (mpImg->Save("mp_output.png"))
		printf("couldn't save mp_output.png\n");

	SimdAnalygraph simdAn(left.w(), left.h());

	auto simdImg = timed_compute(simdAn, &left, &right, IAnalygraph::AN_COLOR);
	if (simdImg->Save("simd_output.png"))
		printf("couldn't save simd_output.png\n");

	MPSimdAnalygraph mpSimdAn(left.w(), left.h());

	auto mpSimdImg = timed_compute(mpSimdAn, &left, &right, IAnalygraph::AN_COLOR);
	if (mpSimdImg->Save("mpsimd_output.png"))
		printf("couldn't save mpsimd_output.png\n");

	return 0;
}