#pragma once

#include <wx/wx.h>
#include <wx/txtstrm.h>

#include <deque>
#include <array>
#include <sstream>
#include <variant>
#include <optional>

enum class Button {
	ZERO               ,
	ONE                ,
	TWO                ,
	THREE              ,
	FOUR               ,
	FIVE               ,
	SIX                ,
	SEVEN              ,
	EIGHT              ,
	NINE               ,
	ADD                ,
	SIN                ,
	SUBTRACT           ,
	COS                ,
	MULTIPLY           ,
	PI                 ,
	DIVIDE             ,
	EXP                ,
	INSERT             ,
	SWAP               ,
	LOG                ,
	POINT              ,
	INVERSE            ,
	SCROLL_LEFT        ,
	NEGATIVE           ,
	SQUARE             ,
	SCROLL_RIGHT       ,
	SCIENTIFIC_NOTATION,
	SQRT               ,
	F                  ,
	SHG                ,
	SHG2               ,
	VO                 ,
	SP                 ,
	P                  ,
	POWER              ,
	NOP                ,
	BP                 ,
	PP                 ,
	CX
};

enum class ButtonState {
	NOT_PRESSED,
	PRESSED
};

enum class CalculatorState {
	REAL_TIME,
	PROGRAMMING
};

enum class TextCtrlNullaryOperation {
	CLEAR,
	DO_NOTHING
};

enum class TextCtrlUnaryOperation {
	APPEND,
	ASSIGN
};

bool IsInteger(double value);

class Calculator {
public:
    Calculator();

    using TextCtrlModifier =
    	std::variant<TextCtrlNullaryOperation,
                     std::pair<TextCtrlUnaryOperation, std::variant<double, std::string>>
                     >;

    // Кнопки с цифрами
    TextCtrlModifier DigitButtonPressed(size_t digit);

    // Кнопки посередине
    TextCtrlModifier AddOrSinButtonPressed();
    TextCtrlModifier SubtractOrCosButtonPressed();
	TextCtrlModifier MultiplyOrPiButtonPressed();
	TextCtrlModifier DivideOrExpButtonPressed();
	TextCtrlModifier InsertButtonPressed();
	TextCtrlModifier SwapOrLogButtonPressed();

	// Кнопки внизу
	TextCtrlModifier PointOrInverseOrScrollLeftButtonPressed();
	TextCtrlModifier NegativeOrSquareOrScrollRightButtonPressed();
	TextCtrlModifier ScientificNotationOrSqrtButtonPressed();
	TextCtrlModifier FButtonPressed();

	// Кнопки вверху
	TextCtrlModifier PButtonPressed();
	TextCtrlModifier CxButtonPressed();
    TextCtrlModifier PowerOrNOPButtonPresssed();
    TextCtrlModifier BPOrEqualButtonPresssed();
    TextCtrlModifier PPOrLessButtonPressed();
    TextCtrlModifier ShgOrRrButtonPressed();
    TextCtrlModifier ShgOrRpButtonPressed();
    TextCtrlModifier VoOrMoreButtonPressed();
    TextCtrlModifier SpOrNotEqualButtonPressed();
    
private:
	CalculatorState    state_    ; // Состояние калькулятора
    ButtonState        button_p_ ; // Состояние кнопки P
	ButtonState        button_f_ ; // Состояние кнопки F
	ButtonState        button_vp_; // Состояние кнопки VP
    std::deque<double> registers_;
    std::stringstream  number_   ;
    int *              RAM       ;
    int                iterator  ;

	const double PI = std::acos(-1);

	// Если номер регистр корректен, возвращает значение в регистре register_num (число от 1 до 8) и обнуляет его
	// Если номер регистра не корректен возвращает std::nullopt
	std::optional<double> ExtractRegister(size_t register_num);
	// Присвает регистру с номером register_num значение value и возвращает true, если номер регистра корректен
	bool SetRegister(size_t register_num, double value);

	// Обнуляет поток
	void ClearNumber();
	// Возвращает число из потока
	double ConstructNumber() const;
	// Извлекает число из потока, обнуляя поток
	double ExtractNumber();
	// Возвращает true, если в потоке целое число
	bool NumberIsInteger() const;

	// Включает кнопку и возвращает true, если она включена успешно (не была включена до этого)
	bool SwitchOnButton (ButtonState& button_state) noexcept;
	// Выключает кнопку и возвращает true, если выключена корректно (не была выключена до этого)
	bool SwitchOffButton (ButtonState& button_state) noexcept;

	// Возвращает значение unary_operation от первого регистра
	const std::variant<double, std::string> PerformUnaryComputation(std::function<double(double)>&& unary_function);
	// Возвращает значение binary_operation от первого и второго регистров
	const std::variant<double, std::string> PerformBinaryComputation(std::function<double(double, double)>&& binary_function);
};
