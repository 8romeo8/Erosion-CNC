#include "DrawOrderer.h"
#include <cmath>   // ��� fabs
#include <vector>  // ��� std::vector

DrawOrderer::DrawOrderer(const std::vector<DxfPos>& source)
    : m_source(source) {}

std::vector<DrawOrderer::OrderStruct> DrawOrderer::createOrderedPath() {
	std::vector<OrderStruct> ordered;
    if (m_source.empty()) return ordered;

    // ������ ��� ������������ �������������� ���������
    std::vector<bool> used(m_source.size(), false);

    // ��� 1: ��������� ������ �������
	ordered.push_back(convertToOrderStruct(m_source[0]));
    used[0] = true;
	double currentX = m_source[0].x2;
	double currentY = m_source[0].y2;

    // ��� 2: ���� ����������� ��������
    bool found;
    do {
        found = false;
        for (size_t i = 0; i < m_source.size(); ++i) {
			if (used[i]) continue;
            // ��������� ���������� ����� � ��������
			if (isEqual(m_source[i].x1, currentX) &&
				isEqual(m_source[i].y1, currentY) ||
				isEqual(m_source[i].x2, currentX) &&
				isEqual(m_source[i].y2, currentY))
			{

				ordered.push_back(convertToOrderStruct(m_source[i]));
                used[i] = true;
                currentX = m_source[i].x2;
                currentY = m_source[i].y2;
                found = true;
                break;
            }
        }
    } while (found);

    // ��� 3: ��������� ���������� ��������
    for (size_t i = 0; i < m_source.size(); ++i) {
        if (!used[i]) {
            ordered.push_back(convertToOrderStruct(m_source[i]));
        }
    }

    return ordered;
}

bool DrawOrderer::isEqual(double a, double b) const {
    return std::fabs(a - b) < 1e-9;  // ��������� � ��������
}

DrawOrderer::OrderStruct DrawOrderer::convertToOrderStruct(const DxfPos& seg) const {
    return {
        seg.x1, seg.y1,  // ��������� �����
        seg.x2, seg.y2,  // �������� �����
        seg.i, seg.j,    // ����� ����
        seg.lineOrCircle // ��� ��������
    };
}
