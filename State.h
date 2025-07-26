#ifndef STATE_H
#define STATE_H

#include <vector>
#include <map>


/* 20.07.2025    8romeo8@list.ru       (+79780592624)
   ����� ��� �������� ��������� ������ � ��������� ����� �����������.
   ������������� ���� ����� ���������� ������ �� Canvas.
   �������� ���������:

   DefaultState - ��������� ����� �������� � �������� dxf �����.
   PointSelectionState - ��������� ������ ��������� ����� ������ ���������.
   PathSelectionState - ��������� ������ ���������� �������� ������.
   EquidistantSelectionState - ��������� ������ ����������� �������� ����������.
   PathGenerationState - ��������� ������������ �������� ���������� ��������.
   ReadyToStartState - ���������� ������������, ��������� ��� �� ��������.
   ZeroCalibrationState - ��������� ������ ������� �����.
   RunningWithCoolantState - ���������� ��������� � ���������� ���.
   DryRunningState - ���������� ��������� ��� ������� ����������.
   PausedState - ��������� ��������� ���������� ���������.
   EndProgramState - ��������� ���������.
   RewindState - �������� � �������� �����������.
   RapidMoveState - ���������������� ��� ��������� ������.

   ��� ������������

   State machineState;
	if (machineState.setState(PointSelectionState)) {
			// ������� �������
	} else {
			// ������������ �������
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

	 // ����� ����� ��� ������� ���������
	static const std::map<Status, uint16_t> stateButtonMasks;

	TRect buttonRect; // ������ ������� ������
	bool isHovered;   // ���� ��������� ����

    void initializeTransitions() {
        // ������������� ���������� ��������� ����� �����������
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

	~State() {  }  // �� �������� ������� ������

	void onPaint(TCanvas *Canvas, const TRect& drawArea);
	Status getState() const ;
	bool setState(Status newState);
	bool isValidTransition(Status newState) const;
	void isMouseDown(int x, int y);
	void isMouseMove(int x, int y);

	// ����� ��� ��������� ������ (������ ��� = ���� ������)
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

        // ����� ��� ��������� ����� ������ �� ���������
    uint16_t getButtonMask(Status state) const {
        return stateButtonMasks.at(state);
    }
};

#endif
