#include <memory>

#include "NaiveAnalygraph.h"

NaiveAnalygraph::NaiveAnalygraph(unsigned w, unsigned h) {
	img_ = std::make_unique<Image>(w, h, 24);
}

NaiveAnalygraph::~NaiveAnalygraph() {
	img_.reset();
}

int NaiveAnalygraph::Compute(const Image* l, const Image* r, IAnalygraph::Mode mode) {
	if (mode < 0 || mode > sizeof(left_floats) / sizeof(left_floats[0])) {
		printf("unsupported mode: %u\n", mode);
		return -1;
	}

	if (!img_->h()) {
		printf("get me an image first\n");
		return -1;
	}

	const float* m_l = left_floats[mode];
	const float* m_r = right_floats[mode];

	for (unsigned y = 0; y < l->h(); y++) {
		const uint8_t* pix_l = l->GetScanLine(y);
		const uint8_t* pix_r = r->GetScanLine(y);
		uint8_t* pix_o = img_->GetScanLine(y);
		for (unsigned x = 0; x < l->w(); x++) {
			pix_o[0] = static_cast<uint8_t>(m_l[0] * pix_l[0] + m_l[1] * pix_l[1] + m_l[2] * pix_l[2]);
			pix_o[1] = static_cast<uint8_t>(m_l[3] * pix_l[0] + m_l[4] * pix_l[1] + m_l[5] * pix_l[2]);
			pix_o[2] = static_cast<uint8_t>(m_l[6] * pix_l[0] + m_l[7] * pix_l[1] + m_l[8] * pix_l[2]);
			pix_o[0] += static_cast<uint8_t>(m_r[0] * pix_r[0] + m_r[1] * pix_r[1] + m_r[2] * pix_r[2]);
			pix_o[1] += static_cast<uint8_t>(m_r[3] * pix_r[0] + m_r[4] * pix_r[1] + m_r[5] * pix_r[2]);
			pix_o[2] += static_cast<uint8_t>(m_r[6] * pix_r[0] + m_r[7] * pix_r[1] + m_r[8] * pix_r[2]);

			pix_l += 3;
			pix_r += 3;
			pix_o += 3;
		}
	}

	return 0;
}

std::unique_ptr<Image> NaiveAnalygraph::GetImage() {
	return std::move(img_);
}