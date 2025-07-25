	#include "State.h"
    #include "GraphicsBuilder.h"


void State::onPaint(TCanvas* Canvas, const TRect& drawArea) {
    //������������ ������ ������ ����� �����
	if(currentState == DefaultState) {
        int canvasWidth = drawArea.Width();
        int canvasHeight = drawArea.Height();

        // ������ ��������� ������
        int buttonWidth = 200;
        int buttonHeight = 50;
        int leftMargin = 50;
        int bottomMargin = 60;

        // ��������� ������� ������
        buttonRect = TRect(
            leftMargin,
            canvasHeight - bottomMargin - buttonHeight,
            leftMargin + buttonWidth,
            canvasHeight - bottomMargin
        );

        // ������������ ������������� ������ � ������ ���������
        Canvas->Brush->Color = isHovered ? clBtnHighlight : clBtnFace;
        Canvas->Pen->Color = clBlack;
        Canvas->Rectangle(buttonRect);

        // ����������� �����
        Canvas->Font->Name = "Tahoma";
        Canvas->Font->Size = 10;
        Canvas->Font->Style = TFontStyles() << fsBold;
        Canvas->Font->Color = clBlack;

        // ��������� ������� ������
        int textWidth = Canvas->TextWidth("������� ����� �����");
        int textHeight = Canvas->TextHeight("������� ����� �����");
        int textX = buttonRect.Left + (buttonWidth - textWidth) / 2;
        int textY = buttonRect.Top + (buttonHeight - textHeight) / 2;

        Canvas->TextOut(textX, textY, "������� ����� �����");
	}else
	//����� ������ ������
	if(currentState == PointSelectionState)
	{
      int canvasWidth = drawArea.Width();
        int canvasHeight = drawArea.Height();

        // ������ ��������� ������
        int buttonWidth = 200;
        int buttonHeight = 50;
        int leftMargin = 50;
        int bottomMargin = 60;

        // ��������� ������� ������
        buttonRect = TRect(
            leftMargin,
            canvasHeight - bottomMargin - buttonHeight,
            leftMargin + buttonWidth,
            canvasHeight - bottomMargin
        );

        // ������������ ������������� ������ � ������ ���������
        Canvas->Brush->Color = clBtnHighlight;
		Canvas->Pen->Color = clRed;
        Canvas->Rectangle(buttonRect);

        // ����������� �����
        Canvas->Font->Name = "Tahoma";
		Canvas->Font->Size = 10;
        Canvas->Font->Style = TFontStyles() << fsBold;
		Canvas->Font->Color = clRed;

		// ��������� ������� ������
		String str = "������� ����� �����";
		int textWidth = Canvas->TextWidth(str);
		int textHeight = Canvas->TextHeight(str);
        int textX = buttonRect.Left + (buttonWidth - textWidth) / 2;
        int textY = buttonRect.Top + (buttonHeight - textHeight) / 2;

		Canvas->TextOut(textX, textY, str);

    }
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

void State::isMouseDown(int x, int y) {
    if(currentState == DefaultState && buttonRect.Contains(TPoint(x, y))) {
        // ��������� ������� �� ������
		currentState = PointSelectionState;
		isHovered = false;
    }
}

void State::isMouseMove(int x, int y) {
	if(currentState == DefaultState) {
        // ���������, ��������� �� ������ ��� �������
        bool nowHovered = buttonRect.Contains(TPoint(x, y));

        // ���� ��������� ���������� - ����� ������������
        if(nowHovered != isHovered) {
			isHovered = nowHovered;
        }
	}else
	//��������� ������ ������ - ������������ ��� �����
	if(currentState == PointSelectionState)
	{

    }


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



