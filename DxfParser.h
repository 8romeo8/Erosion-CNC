#ifndef DXF_PARSER_H
#define DXF_PARSER_H

/*  18.07.2025г      8romeo8@list.ru       (+79780592624)

   Класс служит для перебора файла dxf и заполенния масива структур для дальнейшей обработки

   ВАЖНО :
   Класс не парсит Сплайны и Полилинии.

   Задача сделать выборку из файла линий ,дуг, окружностей.
   Подготовил небольшую заготовку для полилиний.

   Алгоритм
   Принимаем ссылку на обьект масива типа DxfPos.
   Считываем файл Dxf и заполняем эту структуру

   В заполнении небольшая корекция , так как в дальнейшем будет формироваться Gcode то
   дуги и окружности будут иметь данные относитльного смещения I и J.
   Такой же формат формируется для GCode что немного не стандартно при парсинге DXF
*/

#include <vector>
#include <string>
#include <fstream>

// Структура для Points
struct DxfPos {
    double x1, y1;  // Начальная точка
    double x2, y2;  // Конечная точка
    double i, j;    // Центр дуги (относительно начальной точки)
    char lineOrCircle; // '1'-линия, '2'-дуга, '3'-окружность
};

class DxfParser {
public:
	void parse(const std::string& filePath, std::vector<DxfPos>* ToolDraw);

private:
	void trim(std::string& str);
	void handleSection(std::ifstream& file, bool& inEntities, bool& inBlock);
	void handleLine(std::ifstream& file, std::vector<DxfPos>* ToolDraw);
	void handleArc(std::ifstream& file, std::vector<DxfPos>* ToolDraw);
	void handleCircle(std::ifstream& file, std::vector<DxfPos>* ToolDraw);
	void handleLwPolyline(std::ifstream& file, std::vector<DxfPos>* ToolDraw);
	double safeStod(const std::string& str);
};

#endif // DXF_PARSER_H
