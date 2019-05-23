#include <string>
#include <utility>
#include <functional>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "computations.hpp"

#define lf(...) do{ fprintf(stderr, "%s\n", __func__); fflush(stdout);} while(0)

Calculator::Calculator()
    : state_(CalculatorState::REAL_TIME)
    , button_p_ (ButtonState::NOT_PRESSED)
    , button_f_ (ButtonState::NOT_PRESSED)
    , button_vp_(ButtonState::NOT_PRESSED)
    , button_sp_(ButtonState::NOT_PRESSED)
    , registers_(8u)
    , number_   ("")
    , computation_happened(false)
    , ram_(std::nullopt) {
}

std::optional<long double> Calculator::ExtractRegister(size_t register_num) {
	if (register_num < 1 || register_num > 8) {
		return std::nullopt;
	}
	long double value = registers_[register_num - 1];
	registers_[register_num - 1] = 0;
	return value;
}

bool Calculator::SetRegister(size_t register_num, const long double& value) {
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

long double Calculator::ExtractNumber() {
	std::string number;
	number_ >> number;
	ClearNumber();
	return number.empty() ? 0 : std::stold(number);
}

bool Calculator::NumberIsEmpty() const {
	return number_.str().empty();
}

bool Calculator::NumberIsInteger() const {
	if (NumberIsEmpty()) {
		return false;
	}

	const std::string str_number = number_.str();
	long double number = std::stold(str_number);
	return std::floor(number) == std::ceil(number);
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

bool Calculator::SetState(CalculatorState calculator_state) noexcept {
	if (state_ == calculator_state) {
		return false;
	}
    state_ = calculator_state;
    if (calculator_state == CalculatorState::PROGRAMMING) {
        ram_ = RAM();
    }
	return true;
}

bool Calculator::IsCorrect(const long double& value) const {
	return isfinite(value) && !isnan(value);
}

const std::variant<long double, std::string>
    Calculator::PerformUnaryComputation(std::function<long double(long double)> unary_function)
{
	computation_happened = true;
	const auto value = unary_function(*ExtractRegister(1));

	if (IsCorrect(value)) {
		SetRegister(1, value);
		ClearNumber();
		number_ << value;
		return value;
	}
	return "Incorrect computation";
}

const std::variant<long double, std::string>
    Calculator::PerformBinaryComputation(std::function<long double(long double, long double)> binary_function)
{
	computation_happened = true;
	const auto value = binary_function(*ExtractRegister(1), *ExtractRegister(2));

	if (IsCorrect(value)) {
		SetRegister(1, value);
		ClearNumber();
		number_ << value;
		return value;
	}
	return "Incorrect computation";
}

void Calculator::MoveToIf(std::function<bool(long double, long double)> binary_function){
    int address =
    10 * static_cast<int>(ram_->buffer[ram_->i+1])
       + static_cast<int>(ram_->buffer[ram_->i+2]);
    
    bool result;
    //if (binary_function != std::equal_to<>()) {
        result = binary_function(*ExtractRegister(1), 0);
    //}
    
    if (result){
        ram_->i = address;
    } else {
        ram_->i += 2;
    }
}

void Calculator::LogOperation(Button button) {
    ram_->buffer[ram_->i] = button;
    ++ram_->i;
}

//.............................................................................................
//..............................................................................................

Calculator::TextCtrlModifier Calculator::DigitButtonPressed(size_t digit) {
    lf();
    if (state_ == CalculatorState::REAL_TIME){
        if (button_p_ == ButtonState::PRESSED) {
            SwitchOffButton(button_p_);
            computation_happened = false;
            return SetRegister(digit, ExtractNumber())
            ? TextCtrlModifier(TextCtrlNullaryOperation::CLEAR)
            : TextCtrlModifier(std::pair(TextCtrlUnaryOperation::ASSIGN, "Register does not exist"));
        } else if (button_f_ == ButtonState::PRESSED) {
            SwitchOffButton(button_f_);
            if (auto value = ExtractRegister(digit); value) {
                SetRegister(1, *value);
                SetRegister(digit, *value);
                return std::pair(TextCtrlUnaryOperation::ASSIGN, *value);
            }
            return std::pair(TextCtrlUnaryOperation::ASSIGN, "Register does not exist");
        } else {
            bool number_is_entered_by_user = !NumberIsEmpty() && !computation_happened;
            computation_happened = false;
            
            number_ << digit;
            return number_is_entered_by_user
            ? std::pair(TextCtrlUnaryOperation::APPEND, digit)
            : std::pair(TextCtrlUnaryOperation::ASSIGN, digit);
        }
    } else {
        LogOperation(static_cast<Button>(digit));
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}


Calculator::TextCtrlModifier Calculator::AddOrSinButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		ClearNumber();
		computation_happened = false;
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return std::sin(value); }));
		} else {
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::plus<>()));
		}
	} else {
        LogOperation(Button::ADD);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::SubtractOrCosButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		ClearNumber();
		computation_happened = false;
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return std::cos(value); }));
		} else {
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::minus<>()));
		}
	} else {
        LogOperation(Button::SUBTRACT);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::MultiplyOrPiButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			ClearNumber();
			number_ << PI;
			computation_happened = true;
			return std::pair(TextCtrlUnaryOperation::ASSIGN, PI);
		} else {
			ClearNumber();
			computation_happened = false;
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::multiplies<>()));
		}
	} else {
        LogOperation(Button::MULTIPLY);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::DivideOrExpButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		ClearNumber();
		computation_happened = false;
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return std::exp(value); }));
		} else {
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation(std::divides<>()));
		}
	} else {
        LogOperation(Button::DIVIDE);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::InsertButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			return TextCtrlNullaryOperation::DO_NOTHING;
		} else {
			computation_happened = false;
			if (button_vp_ == ButtonState::PRESSED) {
				SwitchOffButton(button_vp_);
				return std::pair(TextCtrlUnaryOperation::ASSIGN,
					PerformUnaryComputation([&](const auto& value) {
						return value * std::pow(10, ExtractNumber());
					}));
			}
            SetRegister(2, *ExtractRegister(1));
			SetRegister(1, ExtractNumber());
			return TextCtrlNullaryOperation::CLEAR;
		}
	} else {
        LogOperation(Button::INSERT);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::SwapOrLogButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);
			computation_happened = false;
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return std::log(value); }));
		} else {
			std::swap(registers_[0], registers_[1]);
			return TextCtrlNullaryOperation::DO_NOTHING;
		}
	} else {
        LogOperation(Button::SWAP);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::PointOrInverseOrScrollLeftButtonPressed() {
    lf();
	if(state_ == CalculatorState::REAL_TIME) {

		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);

			const auto& value = registers_.front();
			registers_.pop_front();
			registers_.push_back(value);

			return TextCtrlNullaryOperation::DO_NOTHING;
		} else if(button_f_ == ButtonState::PRESSED) {
			SwitchOffButton(button_f_);
			computation_happened = false;
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return 1 / value; }));
		} else {
			computation_happened = false;
			if (NumberIsInteger()) {
				number_ << ".";
				return std::pair(TextCtrlUnaryOperation::APPEND, ".");
			} else {
				bool number_is_empty = NumberIsEmpty();
				ClearNumber();
				return number_is_empty
					? std::pair(TextCtrlUnaryOperation::ASSIGN, "Enter number first")
					: std::pair(TextCtrlUnaryOperation::ASSIGN, "Number is already float");
			}
		}
	} else {
        LogOperation(Button::POINT);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::NegativeOrSquareOrScrollRightButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);

			const auto& value = registers_.back();
			registers_.pop_back();
			registers_.push_front(value);

			return TextCtrlNullaryOperation::DO_NOTHING;
		} else if(button_f_ == ButtonState::PRESSED) {
			SwitchOffButton(button_f_);
			computation_happened = false;
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return value * value; }));
		} else {
			// Достаем число из потока, дописываем минус и вставляем обратно
			const auto cur_number = ExtractNumber();
			number_ << -cur_number;
			return std::pair(TextCtrlUnaryOperation::ASSIGN, -cur_number);
		}
	} else {
        LogOperation(Button::NEGATIVE);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::ScientificNotationOrSqrtButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_f_ == ButtonState::PRESSED) {
			SwitchOffButton(button_f_);
			computation_happened = false;
			ClearNumber();
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformUnaryComputation([] (const auto& value) { return std::sqrt(value); }));
		} else if (button_vp_ == ButtonState::PRESSED) {
			SwitchOffButton(button_vp_);
            return TextCtrlNullaryOperation::DO_NOTHING;
		} else {
			SwitchOnButton(button_vp_);
			computation_happened = false;
			SetRegister(1, ExtractNumber());
			return TextCtrlNullaryOperation::CLEAR;
		}
	} else {
        LogOperation(Button::SCIENTIFIC_NOTATION);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

//..............................UPPER BUTTONS............................................

Calculator::TextCtrlModifier Calculator::FButtonPressed() {
    lf();
    if (state_ == CalculatorState::REAL_TIME){
        if (button_f_ == ButtonState::NOT_PRESSED) {
            SwitchOnButton(button_f_);
        } else {
            SwitchOffButton(button_f_);
        }
        return TextCtrlNullaryOperation::DO_NOTHING;
    } else {
        LogOperation(Button::F);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::PButtonPressed() {
    lf();
    if (state_ == CalculatorState::REAL_TIME){
        if (button_p_ == ButtonState::NOT_PRESSED) {
            SwitchOnButton(button_p_);
        } else {
            SwitchOffButton(button_p_);
        }
    } else {
        LogOperation(Button::P);
    }
	return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::CxButtonPressed() {
    lf();
    if (button_p_ == ButtonState::NOT_PRESSED) {
        SwitchOffButton(button_p_);
        SwitchOffButton(button_f_);
        SwitchOffButton(button_vp_);
        computation_happened = false;
        ClearNumber();
        for (auto& number : registers_) {
            number = 0;
        }
        return TextCtrlNullaryOperation::CLEAR;
    } else {
        LogOperation(Button::CX);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::PowerButtonPressed() {
    lf();
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
            SwitchOffButton(button_p_);
            return TextCtrlNullaryOperation::DO_NOTHING;
		} else {
			ClearNumber();
			computation_happened = false;
			return std::pair(TextCtrlUnaryOperation::ASSIGN,
				PerformBinaryComputation( [&] (const auto& lhs, const auto& rhs) {
					return std::pow(lhs, rhs);
			}));
		}
	} else {
        LogOperation(Button::POWER);
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::BPOrEqualButtonPresssed(){
    lf();
    if (state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::equal_to<>());
            SwitchOffButton(button_p_);
        } else {
            int address =
            10 * static_cast<int>(ram_->buffer[ram_->i + 1])
               + static_cast<int>(ram_->buffer[ram_->i + 2]);
            ram_->i = address;
        }
    } else {
        LogOperation(Button::BP);
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator:: PPOrLessButtonPressed(){
    lf();
    if (state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::less<>());
            SwitchOffButton(button_p_);
        } else {
            ram_->i = 0;
        }
    } else {
        LogOperation(Button::PP);
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::ShgOrRpButtonPressed() {
    lf();
    if (button_p_ == ButtonState::PRESSED){
        SetState(CalculatorState::PROGRAMMING);
        SwitchOffButton(button_p_);
    } else {
        print();
        return std::pair(TextCtrlUnaryOperation::ASSIGN, ExtractNumber());
    }
    return TextCtrlNullaryOperation::CLEAR;
}

Calculator::TextCtrlModifier Calculator::ShgOrPpButtonPressed(){
    lf();
    if(state_ == CalculatorState::PROGRAMMING){
        std::cerr << "set to real time\n";
        SetState(CalculatorState::REAL_TIME);
    } else {
        SwitchCase();
        ++ram_->i;
    }
    return TextCtrlNullaryOperation::CLEAR;
}

Calculator::TextCtrlModifier Calculator::VoOrMoreButtonPressed(){
    lf();
    if (state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::greater<>());
            SwitchOffButton(button_p_);
        } else {
            ram_->i = 0;
        }
    } else {
        LogOperation(Button::VO);
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::SpOrNotEqualButtonPressed() {
    lf();
    if (state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::not_equal_to<>());
            SwitchOffButton(button_p_);
        } else {
            if (button_sp_ == ButtonState::NOT_PRESSED){
                button_sp_ = ButtonState::PRESSED;
                ExecuteProgram();
            } else {
                ram_ = std::nullopt;
                button_sp_ = ButtonState::NOT_PRESSED;
            }
        }
    } else {
        LogOperation(Button::SP);
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

void Calculator::ExecuteProgram(){
    lf();
    for( ; button_sp_ == ButtonState::PRESSED; ++ram_->i){
        SwitchCase();
    }
}

void Calculator::SwitchCase(){
    switch (ram_->buffer[ram_->i]) {
        case Button::ZERO:
            DigitButtonPressed(0u);
            break;
        case Button::ONE:
            DigitButtonPressed(1u);
            break;
        case Button::TWO:
            DigitButtonPressed(2u);
            break;
        case Button::THREE:
            DigitButtonPressed(3u);
            break;
        case Button::FOUR:
            DigitButtonPressed(4u);
            break;
        case Button::FIVE:
            DigitButtonPressed(5u);
            break;
        case Button::SIX:
            DigitButtonPressed(6u);
            break;
        case Button::SEVEN:
            DigitButtonPressed(7u);
            break;
        case Button::EIGHT:
            DigitButtonPressed(8u);
            break;
        case Button::NINE:
            DigitButtonPressed(9u);
            break;
        case Button::ADD:
            AddOrSinButtonPressed();
            break;
        case Button::SUBTRACT:
            SubtractOrCosButtonPressed();
            break;
        case Button::MULTIPLY:
            MultiplyOrPiButtonPressed();
            break;
        case Button::DIVIDE:
            DivideOrExpButtonPressed();
            break;
        case Button::INSERT:
            InsertButtonPressed();
            break;
        case Button::SWAP:
            SwapOrLogButtonPressed();
            break;
        case Button::POINT:
            PointOrInverseOrScrollLeftButtonPressed();
            break;
        case Button::NEGATIVE:
            NegativeOrSquareOrScrollRightButtonPressed();
            break;
        case Button::SCIENTIFIC_NOTATION:
            ScientificNotationOrSqrtButtonPressed();
            break;
        case Button::F:
            FButtonPressed();
            break;
        case Button::P:
            PButtonPressed();
            break;
        case Button::CX:
            CxButtonPressed();
            break;
        case Button::POWER:
            PowerButtonPressed();
            break;
        case Button::BP:
            BPOrEqualButtonPresssed();
            break;
        case Button::PP:
            PPOrLessButtonPressed();
            break;
        case Button::SP:
            SpOrNotEqualButtonPressed();
            break;
        case Button::VO:
            VoOrMoreButtonPressed();
            break;
        default:
            break;
    }
}

void Calculator::print(){
    for (int i(0); i < registers_.size(); i++) {
        std::cout << registers_[i];
        if (i != registers_.size() - 1) std::cout << ",";
    }
    std::cout << "\n";
}
