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
	STEP               ,
	REAL_TIME_MODE     ,
	STEP2              ,
	PROGRAMMING_MODE   ,
	SP                 ,
	VO                 ,
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

class Calculator {
public:
	Calculator();

	using TextCtrlModifier =
		std::variant<TextCtrlNullaryOperation,
	                 std::pair<TextCtrlUnaryOperation, std::variant<long double, std::string>>
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
	TextCtrlModifier PowerButtonPressed();
    TextCtrlModifier BPOrEqualButtonPresssed();
    TextCtrlModifier PPOrLessButtonPressed();
    TextCtrlModifier ShgOrRpButtonPressed();
    TextCtrlModifier ShgOrPpButtonPressed();
    TextCtrlModifier VoOrMoreButtonPressed();
    TextCtrlModifier SpOrNotEqualButtonPressed();
    void print();
private:
	struct RAM {
        RAM()
        : buffer()
        , i(0) {
        }
		std::array<Button, 60u>  buffer;
        int i;
	};

	CalculatorState         state_              ; // Состояние калькулятора
	ButtonState             button_p_           ; // Состояние кнопки P
	ButtonState             button_f_           ; // Состояние кнопки F
	ButtonState             button_vp_          ; // Состояние кнопки VP
    ButtonState             button_sp_          ;
	std::deque<long double> registers_          ;
	std::stringstream       number_             ;
	std::optional<RAM>      ram_                ;
	bool                    computation_happened;

	constexpr static const long double& PI = M_PI;

	// Если номер регистр корректен, возвращает значение в регистре register_num (число от 1 до 8) и обнуляет его
	// Если номер регистра не корректен возвращает std::nullopt
	std::optional<long double> ExtractRegister(size_t register_num);
	// Присвает регистру с номером register_num значение value и возвращает true, если номер регистра корректен
	bool SetRegister(size_t register_num, const long double& value);

	// Обнуляет поток
	void ClearNumber();
	// Извлекает число из потока, обнуляя поток
	long double ExtractNumber();
	// Возвращает true, если в потоке целое число
	bool NumberIsInteger() const;
	// Проверяет, пусто ли текущее число
	bool NumberIsEmpty() const;

	// Включает кнопку и возвращает true, если она включена успешно (не была включена до этого)
	bool SwitchOnButton (ButtonState& button_state) noexcept;
	// Выключает кнопку и возвращает true, если выключена корректно (не была выключена до этого)
	bool SwitchOffButton (ButtonState& button_state) noexcept;
	// Переводит калькулятор в состояние calculator_state,
	// возвращает true, если калькулятор был в противоположном состоянии
	bool SetState(CalculatorState calculator_state) noexcept;

	// Возвращает значение unary_operation от первого регистра
	const std::variant<long double, std::string> PerformUnaryComputation(std::function<long double(long double)> unary_function);
	// Возвращает значение binary_operation от первого и второго регистров
	const std::variant<long double, std::string> PerformBinaryComputation(std::function<long double(long double, long double)> binary_function);
	// Проверяет корректность числа и возвращает true, если оно корректно
	bool IsCorrect(const long double& value) const;
    //compares numbers in registers 1 and 2, and if true, moves iterartor
    void MoveToIf(std::function<bool(long double, long double)> binary_function);
    //executes one operation depending on it's ID
    void SwitchCase();
    //executes whole programm
    void ExecuteProgram();
    // Записать нажатие кнопки в массив кнопок
    void LogOperation(Button button);
    
};
