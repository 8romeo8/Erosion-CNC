#ifndef DRAW_ORDERER_H
#define DRAW_ORDERER_H
/* 18.07.2025�      8romeo8@list.ru       (+79780592624)
	����� ������� ����������� ���������� ������������ �������.

	�����, ������� ����� ������������� ��������
	�� `vector<DxfPos>` � `vector<DrawStruct>` �� �������� ���������� ������.

	������ ���� ����� �� ���������� �� ��������� �����
	�� ��������� ����� ����� ������ ������ ������� ������ ������� ������.
    �� ���� ���������� ���������� ������ ������.
*/
#include "DxfParser.h"  // ��� ������� � DxfPos
#include <vector>

class DrawOrderer {
public:
	// ��������� ��� �������� ������������� ���������
	struct OrderStruct {
        double x1, y1;
        double x2, y2;
        double i, j;
        int Type;
    };

    // �����������, ����������� �������� ������
    DrawOrderer(const std::vector<DxfPos>& source);

    // �������� ����� ��������������
	std::vector<OrderStruct> createOrderedPath();

private:
    const std::vector<DxfPos>& m_source;  // �������� ������

    // ��������������� �������
    bool isEqual(double a, double b) const;
    OrderStruct convertToOrderStruct(const DxfPos& seg) const;
};

#endif // DRAW_ORDERER_H
