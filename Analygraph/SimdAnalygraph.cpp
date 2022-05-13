#include <memory>

#include "SimdAnalygraph.h"

#include <immintrin.h>

SimdAnalygraph::SimdAnalygraph(unsigned w, unsigned h) {
	img_ = std::make_unique<Image>(w, h, 24);
}

SimdAnalygraph::~SimdAnalygraph() {
	img_.reset();
}

int SimdAnalygraph::Compute(const Image* l, const Image* r, IAnalygraph::Mode mode) {
	if (mode < 0 || mode > sizeof(left_shorts) / sizeof(left_shorts[0])) {
		printf("unsupported mode: %u\n", mode);
		return -1;
	}

	if (!img_->h()) {
		printf("get me an image first\n");
		return -1;
	}

	const unsigned short* m_l = left_shorts[mode];
	const unsigned short* m_r = right_shorts[mode];

	__m128i m_bl = _mm_setr_epi32(m_l[0], m_l[1], m_l[2], 0);
	__m128i m_br = _mm_setr_epi32(m_r[0], m_r[1], m_r[2], 0);
	__m128i m_gl = _mm_setr_epi32(m_l[3], m_l[4], m_l[5], 0);
	__m128i m_gr = _mm_setr_epi32(m_r[3], m_r[4], m_r[5], 0);
	__m128i m_rl = _mm_setr_epi32(m_l[6], m_l[7], m_l[8], 0);
	__m128i m_rr = _mm_setr_epi32(m_r[6], m_r[7], m_r[8], 0);

	for (int y = 0; y < l->h(); y++) {
		const uint8_t* pix_l = l->GetScanLine(y);
		const uint8_t* pix_r = r->GetScanLine(y);
		uint8_t* pix_o = img_->GetScanLine(y);
		for (int x = 0; x < static_cast<int>(l->w()); x++) {
			__m128i xmm_l = _mm_loadu_si128((const __m128i*)pix_l);
			xmm_l = _mm_unpacklo_epi8(xmm_l, _mm_setzero_si128());
			xmm_l = _mm_unpacklo_epi16(xmm_l, _mm_setzero_si128());

			__m128i xmm_r = _mm_loadu_si128((const __m128i*)pix_r);
			xmm_r = _mm_unpacklo_epi8(xmm_r, _mm_setzero_si128());
			xmm_r = _mm_unpacklo_epi16(xmm_r, _mm_setzero_si128());

			__m128i x1 = _mm_mullo_epi32(m_bl, xmm_l);
			__m128i x2 = _mm_mullo_epi32(m_br, xmm_r);
			x1 = _mm_add_epi32(x1, x2);
			x1 = _mm_hadd_epi32(x1, x1);
			x1 = _mm_hadd_epi32(x1, x1);
			pix_o[0] = _mm_cvtsi128_si32(x1) >> 10;

			x1 = _mm_mullo_epi32(m_gl, xmm_l);
			x2 = _mm_mullo_epi32(m_gr, xmm_r);
			x1 = _mm_add_epi32(x1, x2);
			x1 = _mm_hadd_epi32(x1, x1);
			x1 = _mm_hadd_epi32(x1, x1);
			pix_o[1] = _mm_cvtsi128_si32(x1) >> 10;

			x1 = _mm_mullo_epi32(m_rl, xmm_l);
			x2 = _mm_mullo_epi32(m_rr, xmm_r);
			x1 = _mm_add_epi32(x1, x2);
			x1 = _mm_hadd_epi32(x1, x1);
			x1 = _mm_hadd_epi32(x1, x1);
			pix_o[2] = _mm_cvtsi128_si32(x1) >> 10;

			pix_l += 3;
			pix_r += 3;
			pix_o += 3;
		}
	}

	return 0;
}

std::unique_ptr<Image> SimdAnalygraph::GetImage() {
	return std::move(img_);
}