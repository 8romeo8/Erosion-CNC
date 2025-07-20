#ifndef GRAPHICSBUILDER_H
#define GRAPHICSBUILDER_H

/* 18.07.2025г      8romeo8@list.ru       (+79780592624)
    Класс подготовит графику к отображению а так же обработает работу с мышью
*/

#include <vector>
#include <Vcl.Graphics.hpp>
#include "DrawOrderer.h"
#include <set>
#include "State.h"   //Управление состояниями

// Предварительное объявление класса DrawOrderer, чтобы избежать циклических зависимостей
class DrawOrderer;

// Структуры для элементов отрисовки и настроек
struct DrawToolLine {
    double x1, y1, x2, y2, i, j;
    int Type; // 1 = линия, 2 = дуга, 3 = окружность
};

struct DrawSettings {
    double xOffset = 0.0;    // Смещение по X (пиксели)
    double yOffset = 0.0;    // Смещение по Y (пиксели)
    double scale = 1.0;      // Масштаб (пикселей на единицу)
    TColor lineColor = clBlack;
    TColor arcColor = clBlue;
    TColor circleColor = clRed;
	TColor backgroundColor = clWhite;
	TRect drawArea; //Размеры отображаемой области
};

class GraphicsBuilder {
public:
    GraphicsBuilder(const std::vector<DrawOrderer::OrderStruct>& source);
    State machineState; //Класс сотояний станка
    // Установка новых настроек
    void setSettings(const DrawSettings& settings);
    DrawSettings getSettings() const;
    void updateTransformedData();
    void paint(TCanvas* canvas, const TRect& drawArea);
    void mouseDown(int x, int y);
    void mouseMove(int x, int y);
    void mouseUp();
    void mouseWheel(int delta, int x, int y);
	void fitToView(const TRect& drawArea);
    //методы для работы с выделением
    void selectElement(int index);
    void deselectElement(int index);
	void clearSelection();
    const std::set<int>& getSelectedIndices() const;

    // Поиск элементов под курсором
    void findElementsAt(int screenX, int screenY, double tolerance = 5.0);

    // Проверка попадания точки на конкретный элемент
	bool isPointOnElement(int index, int screenX, int screenY, double tolerance = 5.0) const;

private:
    const std::vector<DrawOrderer::OrderStruct>& m_source;
    std::vector<DrawToolLine> m_transformed;
    DrawSettings m_settings;
    bool m_dragging = false;
    int m_lastX = 0, m_lastY = 0;

    void updateBounds(const DrawOrderer::OrderStruct& elem,
                     double& minX, double& maxX,
                     double& minY, double& maxY);
    DrawToolLine transformElement(const DrawOrderer::OrderStruct& elem, int canvasHeight) const;
    double worldToScreenX(double worldX) const;
	double worldToScreenY(double worldY, int canvasHeight) const;
    double worldToScale(double worldX) const;
	void drawArc(TCanvas* canvas, DrawToolLine& arc) const;
	void drawCircle(TCanvas* canvas, DrawToolLine& circle) const;
    std::set<int> m_selectedIndices; // Индексы выделенных элементов

    // Преобразование экранных координат в мировые
    double screenToWorldX(int screenX) const;
	double screenToWorldY(int screenY) const;
};

#endif // GRAPHICSBUILDER_H
