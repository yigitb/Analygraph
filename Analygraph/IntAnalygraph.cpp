#include <memory>

#include "IntAnalygraph.h"

IntAnalygraph::IntAnalygraph(unsigned w, unsigned h) {
	img_ = std::make_unique<Image>(w, h, 24);
}

IntAnalygraph::~IntAnalygraph() {
	img_.reset();
}

int IntAnalygraph::Compute(const Image* l, const Image* r, IAnalygraph::Mode mode) {
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

	for (int y = 0; y < static_cast<int>(l->h()); y++) {
		const uint8_t* pix_l = l->GetScanLine(y);
		const uint8_t* pix_r = r->GetScanLine(y);
		uint8_t* pix_o = img_->GetScanLine(y);
		for (unsigned x = 0; x < l->w(); x++) {
			pix_o[0] = (m_l[0] * pix_l[0] + m_l[1] * pix_l[1] + m_l[2] * pix_l[2] 
					  + m_r[0] * pix_r[0] + m_r[1] * pix_r[1] + m_r[2] * pix_r[2]) >> 10;
			pix_o[1] = (m_l[3] * pix_l[0] + m_l[4] * pix_l[1] + m_l[5] * pix_l[2]
					  + m_r[3] * pix_r[0] + m_r[4] * pix_r[1] + m_r[5] * pix_r[2]) >> 10;
			pix_o[2] = (m_l[6] * pix_l[0] + m_l[7] * pix_l[1] + m_l[8] * pix_l[2]
					  + m_r[6] * pix_r[0] + m_r[7] * pix_r[1] + m_r[8] * pix_r[2]) >> 10;

			pix_l += 3;
			pix_r += 3;
			pix_o += 3;
		}
	}

	return 0;
}

std::unique_ptr<Image> IntAnalygraph::GetImage() {
	return std::move(img_);
}