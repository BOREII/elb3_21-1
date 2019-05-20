#include <string>
#include <stdexcept>
#include <string>
#include <utility>
#include <functional>
#include <algorithm>
#include <optional>

#include "computations.hpp"

bool IsInteger(double value) {
	return std::floor(value) == std::ceil(value);
}

Calculator::Calculator()
    : state_(CalculatorState::REAL_TIME)
    , button_p_ (ButtonState::NOT_PRESSED)
    , button_f_ (ButtonState::NOT_PRESSED)
    , button_vp_(ButtonState::NOT_PRESSED)
    , registers_(8u)
    , number_   ("") {
        RAM = NULL;
        iterator = 0;
}

std::optional<double> Calculator::ExtractRegister(size_t register_num) {
	if (register_num < 1 || register_num > 8) {
		return std::nullopt;
	}
	double value = registers_[register_num - 1];
	registers_[register_num - 1] = 0;
	return value;
}

bool Calculator::SetRegister(size_t register_num, double value) {
	if (register_num < 1 || register_num > 8) {
		return false;
	}
	registers_[register_num - 1] = value;
	return true;
}

void Calculator::ClearNumber() {
	number_.clear();
	number_.str("");
}

double Calculator::ConstructNumber() const {
	if (const std::string number = number_.str(); number.empty()) {
		return 0;
	} else {
		return std::stod(number);
	}
}

double Calculator::ExtractNumber() {
	std::string number;
	number_ >> number;
	ClearNumber();
	return number.empty() ? 0 : std::stod(number);
}

bool Calculator::NumberIsInteger() const {
	const double number = ConstructNumber();
	return IsInteger(number);
}

bool Calculator::SwitchOnButton(ButtonState& button_state) noexcept {
	if (button_state == ButtonState::PRESSED) {
		return false;
	}
	button_state = ButtonState::PRESSED;
	return true;
}

bool Calculator::SwitchOffButton(ButtonState& button_state) noexcept {
	if (button_state == ButtonState::NOT_PRESSED) {
		return false;
	}
	button_state = ButtonState::NOT_PRESSED;
	return true;
}

const std::variant<double, std::string>
    Calculator::PerformUnaryComputation(std::function<double(double)>&& unary_function)
{
	try {
		const double result = unary_function(*ExtractRegister(1));
		SetRegister(1, result);
		ClearNumber();
		number_ << result;
		return result;
	} catch (std::exception& e) {
		return e.what();
	}
}

const std::variant<double, std::string>
    Calculator::PerformBinaryComputation(std::function<double(double, double)>&& binary_function)
{
	try {
		const double result = binary_function(*ExtractRegister(1), *ExtractRegister(2));
		SetRegister(1, result);
		number_ << result;
		return result;
	} catch (std::exception& e) {
		return e.what();
	}
}

Calculator::TextCtrlModifier Calculator::DigitButtonPressed(size_t digit) {
    if(state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            SwitchOffButton(button_p_);
            ClearNumber();
            return SetRegister(digit, ExtractNumber())
            ? TextCtrlModifier(TextCtrlNullaryOperation::CLEAR)
            : TextCtrlModifier(std::pair(TextCtrlUnaryOperation::ASSIGN, "Register does not exist"));
        } else if(button_f_ == ButtonState::PRESSED) {
            SwitchOffButton(button_f_);
            if (auto value = ExtractRegister(digit); value) {
                SetRegister(1, *value);
                return TextCtrlNullaryOperation::DO_NOTHING;
            }
            return std::pair(TextCtrlUnaryOperation::ASSIGN, "Register does not exist");
        } else {
            number_ << digit;
            return std::pair(TextCtrlUnaryOperation::APPEND, digit);
        }
    }
    else{
        
    }
}


Calculator::TextCtrlModifier Calculator::AddOrSinButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		ClearNumber();
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return std::sin(value); }));
		} else {
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::plus<>()));
		}
	}
    else{
        RAM[iterator] = Button::ADD;
    }
}

Calculator::TextCtrlModifier Calculator::SubtractOrCosButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		ClearNumber();
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return std::cos(value); }));
		} else {
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::minus<>()));
		}
	}
    else{
        RAM[iterator] = Button::SUBSTRACT;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::MultiplyOrPiButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			ClearNumber();
			number_ << PI;
			return std::pair(TextCtrlUnaryOperation::ASSIGN, PI);
		} else {
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::multiplies<>()));
		}
	}
    else{
        RAM[iterator] = Button::MULTIPLY;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::DivideOrExpButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		ClearNumber();
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return std::exp(value); }));
		} else {
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::divides<>()));
		}
	}
    else{
        RAM[iterator] = Button::DIVIDE;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::InsertButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return TextCtrlNullaryOperation::DO_NOTHING;
		} else {
			if (button_vp_ == ButtonState::PRESSED) {
				SwitchOffButton(button_vp_);
				return std::pair(TextCtrlUnaryOperation::ASSIGN,
					PerformUnaryComputation([&](double value) { return std::pow(*ExtractRegister(1), value); }));
			}
			SetRegister(1, ExtractNumber());
			return TextCtrlNullaryOperation::CLEAR;
		}
	}
    else{
        RAM[iterator] = Button::INSERT;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::SwapOrLogButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return std::log(value); }));
		} else {
			std::swap(registers_[0], registers_[1]);
			return TextCtrlNullaryOperation::DO_NOTHING;
		}
	}
    else{
        RAM[iterator] = Button::SWAP;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::PointOrInverseOrScrollLeftButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {

		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);

			double value = registers_.front();
			registers_.pop_front();
			registers_.push_back(value);

			return TextCtrlNullaryOperation::DO_NOTHING;
		} else if(button_f_ == ButtonState::PRESSED) {
			SwitchOffButton(button_f_);
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return 1 / value; }));
		} else {
			if (NumberIsInteger()) {
				number_ << '.';
				return std::pair(TextCtrlUnaryOperation::APPEND, '.');
			} else {
				ClearNumber();
				return std::pair(TextCtrlUnaryOperation::ASSIGN, "Number is already float");
			}
		}
	}
    else{
        RAM[iterator] = Button::POINT;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::NegativeOrSquareOrScrollRightButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);

			double value = registers_.back();
			registers_.pop_back();
			registers_.push_front(value);

			return TextCtrlNullaryOperation::DO_NOTHING;
		} else if(button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_f_);
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return value * value; }));
		} else {
			// Если стоит минус, то просто выбрасываем его
			// Иначе достаем число из потока, дописываем минус и вставляем обратно
			if (number_.peek() == '-') {
				number_.ignore();
			} else {
				number_ << -ExtractNumber();
			}
			return std::pair(TextCtrlUnaryOperation::ASSIGN, ConstructNumber());
		}
	}
    else{
        RAM[iterator] = Button::NEGATIVE;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::ScientificNotationOrSqrtButtonPressed() {
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_f_ == ButtonState::PRESSED) {
			SwitchOffButton(button_f_);
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (double value) { return std::sqrt(value); }));
		} else {
			SwitchOnButton(button_vp_);
			SetRegister(1, ExtractNumber());
			return TextCtrlNullaryOperation::CLEAR;
		}
	}
    else{
        RAM[iterator] = Button::SCIENTIFIC_NOTATION;
        iterator++;
    }
}

Calculator::TextCtrlModifier Calculator::FButtonPressed() {
	if(state_ == )
        if (button_f_ == ButtonState::NOT_PRESSED) {
            SwitchOnButton(button_f_);
        } else {
            SwitchOffButton(button_f_);
        }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::PButtonPressed() {
	if (button_p_ == ButtonState::NOT_PRESSED) {
		SwitchOnButton(button_p_);
	} else {
		SwitchOffButton(button_p_);
	}
	return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::CxButtonPressed() {
	SwitchOffButton(button_p_);
	SwitchOffButton(button_f_);
	SwitchOffButton(button_vp_);
	ClearNumber();
	for (auto& number : registers_) {
		number = 0;
	}
	return TextCtrlNullaryOperation::CLEAR;
}

Calculator::TextCtrlModifier Calculator::PowerOrNOPButtonPresssed(){
    if(state_ == CalculatorState:REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED) {
            
        }
        else{
            //bnary operartion x in power y
        }
    }
}

Calculator::TextCtrlModifier Calculator::BPOrEqualButtonPresssed(){
    if(state_ == CalculatorState:REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED) {
            
        }
        else{
            
        }
    }
}

Calculator::TextCtrlModifier Calculator:: PPOrLessButtonPressed(){
    if(state_ == CalculatorState:REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED) {
            
        }
        else{
            
        }
    }
}

Calculator::TextCtrlModifier Calculator::ShgOrRpButtonPressed() {
    if (state_ === state_ == CalculatorState::REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED){
            state_  = CalculatorState::PROGRAMMING;
            RAM = calloc(60, sizeof(int));
        }
        if(state_ == CalculatorState::PROGRAMMING){
            iterator ++;
        }
    }
    return TextCtrlNullaryOperation::CLEAR;
}

Calculator::TextCtrlModifier Calculator::ShgOrRpButtonPressed(){
    if(state_ == CalculatorState:REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED) {
            
        }
        else{
            
        }
    }
    
}

Calculator::TextCtrlModifier Calculator::VoOrMoreButtonPressed(){
    if(state_ == CalculatorState:REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED) {
            
        }
        else{
            
        }
    }
    
}

Calculator::TextCtrlModifier Calculator::SpOrNotEqualButtonPressed(){
    if(state_ == CalculatorState:REAL_TIME)
    {
        if (button_p_ == ButtonState::PRESSED) {
            
        }
        else{
            
        }
    }
    
}
