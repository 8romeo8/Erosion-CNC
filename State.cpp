    #include "State.h"

    // �������������� ��������� � ����������� �� ��������� ������
	void State::onPaint(TCanvas Canvas) {
        // ���������� ��������� � ����������� �� currentState
    }

    // ���������� ������� ������
	Status State::getState() const {
        return currentState;
    }

    // ��������� ������ ��������� � ��������� ���������� ��������
	bool State::setState(Status newState) {
        if (isValidTransition(newState)) {
            currentState = newState;
            return true;
        }
        return false;
    }

    // �������� ����������� �������� � ��������� ���������
	bool State::isValidTransition(Status newState) const {
        auto it = validTransitions.find(currentState);
        if (it != validTransitions.end()) {
            const std::vector<Status>& allowedStates = it->second;
            return std::find(allowedStates.begin(), allowedStates.end(), newState) != allowedStates.end();
        }
        return false;
    }

	// ��������� ������� ���� (����� �������� ������� ����� �����������)
	void State::isMouseDown(int x, int y) {
        // ���������� ��������� ����� � ����������� �� currentState
	}

    //��������� ����������� ����
	void State::isMouseMove(int x, int y)
	{

	}

const std::map<Status, uint16_t> State::stateButtonMasks = {
    {DefaultState,
        State::BTN_LOAD_FILE},

    {PointSelectionState,
        State::BTN_LOAD_FILE},

	{PathSelectionState,
        State::BTN_ZERO | State::BTN_START | State::BTN_ON_OFF_SOJ},

	{EquidistantSelectionState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

	{PathGenerationState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

	{ReadyToStartState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

	{ZeroCalibrationState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

	{RunningWithCoolantState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

   {DryRunningState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

   {PausedState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

   {EndProgramState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

   {RewindState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ},

   {RapidMoveState,
		State::BTN_STOP | State::BTN_ON_OFF_SOJ}
};