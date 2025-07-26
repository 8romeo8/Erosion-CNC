#ifndef STATE_H
#define STATE_H

#include <vector>
#include <map>


/* 20.07.2025    8romeo8@list.ru       (+79780592624)
   Класс для хранения состояния машины и переходов между состояниями.
   Дополнительно этот класс отображает кнопки на Canvas.
   Описание состояний:

   DefaultState - Состояние после загрузки и парсинга dxf файла.
   PointSelectionState - Состояние выбора первичной точки старта программы.
   PathSelectionState - Состояние выбора траектории движения машины.
   EquidistantSelectionState - Состояние выбора направления смещения траектории.
   PathGenerationState - Состояние формирования итоговой траектории движения.
   ReadyToStartState - Траектория сформирована, программа ещё не запущена.
   ZeroCalibrationState - Процедура поиска нулевой точки.
   RunningWithCoolantState - Выполнение программы с включенной СОЖ.
   DryRunningState - Выполнение программы без системы охлаждения.
   PausedState - Временная остановка выполнения программы.
   EndProgramState - Программа завершена.
   RewindState - Движение в обратном направлении.
   RapidMoveState - Позиционирование над выбранной точкой.

   Как использовать

   State machineState;
	if (machineState.setState(PointSelectionState)) {
			// Переход успешен
	} else {
			// Недопустимый переход
	}
*/

enum Status {
    DefaultState,
    PointSelectionState,
    PathSelectionState,
    EquidistantSelectionState,
    PathGenerationState,
    ReadyToStartState,
    ZeroCalibrationState,
    RunningWithCoolantState,
    DryRunningState,
    PausedState,
    EndProgramState,
    RewindState,
    RapidMoveState
};

struct DrawSettings;

class State {
private:
    Status currentState;
	std::map<Status, std::vector<Status>> validTransitions;

	DrawSettings *m_settings;

	 // Карта масок для каждого состояния
	static const std::map<Status, uint16_t> stateButtonMasks;

	TRect buttonRect; // Храним область кнопки
	bool isHovered;   // Флаг наведения мыши

    void initializeTransitions() {
        // Инициализация допустимых переходов между состояниями
        validTransitions[DefaultState] = {PointSelectionState};
		validTransitions[PointSelectionState] = {PointSelectionState,PathSelectionState, DefaultState};
		validTransitions[PathSelectionState] = {PointSelectionState,EquidistantSelectionState, PointSelectionState};
		validTransitions[EquidistantSelectionState] = {PointSelectionState,PathGenerationState, PathSelectionState};
		validTransitions[PathGenerationState] = {PointSelectionState,ReadyToStartState, EquidistantSelectionState};
		validTransitions[ReadyToStartState] = {PointSelectionState,ZeroCalibrationState, RapidMoveState, PathGenerationState};
		validTransitions[ZeroCalibrationState] = {PointSelectionState,ReadyToStartState, RunningWithCoolantState, DryRunningState};
        validTransitions[RunningWithCoolantState] = {PausedState, EndProgramState, DryRunningState};
        validTransitions[DryRunningState] = {PausedState, EndProgramState, RunningWithCoolantState};
		validTransitions[PausedState] = {RunningWithCoolantState, DryRunningState, ReadyToStartState, EndProgramState};
		validTransitions[EndProgramState] = {PointSelectionState,ReadyToStartState, DefaultState};
        validTransitions[RewindState] = {PausedState, RunningWithCoolantState, DryRunningState};
        validTransitions[RapidMoveState] = {ReadyToStartState, ZeroCalibrationState};
	}

public:

	State::State(DrawSettings *set)
		: currentState(DefaultState),
        isHovered(false),
      buttonRect(0, 0, 0, 0)
	{
		initializeTransitions();
        m_settings = set;
	}

	~State() {  }  // Не забываем очищать память

	void onPaint(TCanvas *Canvas, const TRect& drawArea);
	Status getState() const ;
	bool setState(Status newState);
	bool isValidTransition(Status newState) const;
	void isMouseDown(int x, int y);
	void isMouseMove(int x, int y);

	// Флаги для состояния кнопок (каждый бит = одна кнопка)
	enum ButtonFlags {
		BTN_LOAD_FILE     = 0b0000000000001,  // 1 << 0
		BTN_ZERO          = 0b0000000000010,  // 1 << 1
		BTN_GO_KADR       = 0b0000000000100,  // 1 << 2
		BTN_SDVIG         = 0b0000000001000,  // 1 << 3
		BTN_STOP_ANGLE    = 0b0000000010000,  // 1 << 4
		BTN_BACKWARD      = 0b0000000100000,  // 1 << 5
		BTN_FORWARD       = 0b0000001000000,  // 1 << 6
		BTN_ON_OFF_SOJ    = 0b0000010000000,  // 1 << 7
		BTN_ON_OFF_GENERIC= 0b0000100000000,  // 1 << 8
		BTN_BARABAN_ON_OFF= 0b0001000000000,  // 1 << 9
		BTN_STOP          = 0b0010000000000,  // 1 << 10
		BTN_START         = 0b0100000000000,  // 1 << 11
	};

        // Метод для получения маски кнопок по состоянию
    uint16_t getButtonMask(Status state) const {
        return stateButtonMasks.at(state);
    }
};

#endif
