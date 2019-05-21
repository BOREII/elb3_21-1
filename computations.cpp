#include <string>
#include <stdexcept>
#include <string>
#include <utility>
#include <functional>
#include <algorithm>
#include <optional>
#include <cmath>

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
	return number.empty() ? 0 : std::stod(number);
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
    Calculator::PerformUnaryComputation(std::function<long double(long double)>&& unary_function)
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
    Calculator::PerformBinaryComputation(std::function<long double(long double, long double)>&& binary_function)
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

void Calculator::MoveToIf(std::function<bool(long double, long double)>&& binary_function){
    int address = 10 * static_cast<int>(*(ram_->current_operation+1)) +
    static_cast<int>(*(ram_->current_operation+2));
    
    bool compare = binary_function(*ExtractRegister(1), *ExtractRegister(2));
    
    if (compare)        ram_->current_operation = std::begin(ram_->buffer) + address;
    else                ram_->current_operation += 2;
}

//.......................................................................................

Calculator::TextCtrlModifier Calculator::DigitButtonPressed(size_t digit) {
    lf();
    if(state_ == CalculatorState::REAL_TIME){
        if (button_p_ == ButtonState::PRESSED) {
            SwitchOffButton(button_p_);
            computation_happened = false;
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
            bool number_is_entered_by_user = !NumberIsEmpty() && !computation_happened;
            computation_happened = false;
            
            number_ << digit;
            return number_is_entered_by_user
            ? std::pair(TextCtrlUnaryOperation::APPEND, digit)
            : std::pair(TextCtrlUnaryOperation::ASSIGN, digit);
        }
    }
    else{
        *ram_->current_operation = static_cast<Button>(digit);
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}


Calculator::TextCtrlModifier Calculator::AddOrSinButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
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
	}
    else{
        *ram_->current_operation = Button::ADD;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::SubtractOrCosButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
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
	}
    else{
        *ram_->current_operation = Button::SUBTRACT;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::MultiplyOrPiButtonPressed() {
    lf();
	if(state_ == CalculatorState::REAL_TIME) {
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
	}
    else{
        std::cout << "multiply\n";
        *ram_->current_operation = Button::MULTIPLY;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::DivideOrExpButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
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
	}
    else{
        *ram_->current_operation = Button::DIVIDE;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::InsertButtonPressed() {
    lf();
	if(state_ == CalculatorState::REAL_TIME) {
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
			SetRegister(1, ExtractNumber());
			return TextCtrlNullaryOperation::CLEAR;
		}
	}
    else{
        *ram_->current_operation = Button::INSERT;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::SwapOrLogButtonPressed() {
    lf();
	if(state_ == CalculatorState::REAL_TIME) {
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
	}
    else{
        *ram_->current_operation = Button::SWAP;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::PointOrInverseOrScrollLeftButtonPressed() {
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
	}
    else{
        *ram_->current_operation = Button::POINT;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::NegativeOrSquareOrScrollRightButtonPressed() {
	if(state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			SwitchOffButton(button_p_);

			const auto& value = registers_.back();
			registers_.pop_back();
			registers_.push_front(value);

			return TextCtrlNullaryOperation::DO_NOTHING;
		} else if(button_p_ == ButtonState::PRESSED) {
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
	}
    else{
        *ram_->current_operation = Button::NEGATIVE;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::ScientificNotationOrSqrtButtonPressed() {
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
	}
    else{
        *ram_->current_operation = Button::SCIENTIFIC_NOTATION;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

//..............................UPPER BUTTONS............................................

Calculator::TextCtrlModifier Calculator::FButtonPressed() {
    if(state_ == CalculatorState::REAL_TIME){
        if (button_f_ == ButtonState::NOT_PRESSED) {
            SwitchOnButton(button_f_);
        } else {
            SwitchOffButton(button_f_);
        }
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
    else{
        *ram_->current_operation = Button::F;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::PButtonPressed() {
    if(state_ == CalculatorState::REAL_TIME){
        if (button_p_ == ButtonState::NOT_PRESSED) {
            SwitchOnButton(button_p_);
        } else {
            SwitchOffButton(button_p_);
        }
    }
    else{
        *ram_->current_operation = Button::P;
        ++ram_->current_operation;
    }
	return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::CxButtonPressed() {
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
    }
    else{
        *ram_->current_operation = Button::CX;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::PowerButtonPressed() {
	if (state_ == CalculatorState::REAL_TIME) {
		if (button_p_ == ButtonState::PRESSED) {
			//not clear what to do
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
	}
    else{
        *ram_->current_operation = Button::POWER;
        ++ram_->current_operation;
        return TextCtrlNullaryOperation::DO_NOTHING;
    }
}

Calculator::TextCtrlModifier Calculator::BPOrEqualButtonPresssed(){
    if(state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::equal_to<>());
            SwitchOffButton(button_p_);
        } else {
            int address = 10 * static_cast<int>(*(ram_->current_operation+1)) +
            static_cast<int>(*(ram_->current_operation+2));
            ram_->current_operation = std::begin(ram_->buffer) + address;
        }
    } else {
        *ram_->current_operation = Button::BP;
        ++ram_->current_operation;
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator:: PPOrLessButtonPressed(){
    if(state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::less<>());
            SwitchOffButton(button_p_);
        }
        else {
            ram_->current_operation = std::begin(ram_->buffer);
        }
    }
    else{
        *ram_->current_operation = Button::PP;
        ++ram_->current_operation;
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::ShgOrRpButtonPressed() {
    if (button_p_ == ButtonState::PRESSED){
        SetState(CalculatorState::PROGRAMMING);
        SwitchOffButton(button_p_);
    }
    else{
        SwitchCase();
        ++ram_->current_operation;
    }
    return TextCtrlNullaryOperation::CLEAR;
}

Calculator::TextCtrlModifier Calculator::ShgOrPpButtonPressed(){
    if(state_ == CalculatorState::PROGRAMMING){
        SetState(CalculatorState::REAL_TIME);
        std::cout << "set to real time\n";
    }
    if(state_ == CalculatorState::REAL_TIME){
        SwitchCase();
        ++ram_->current_operation;
    }
    return TextCtrlNullaryOperation::CLEAR;
}

Calculator::TextCtrlModifier Calculator::VoOrMoreButtonPressed(){
    if(state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::greater_equal<>());
            SwitchOffButton(button_p_);
        } else {
            //?????
            ram_->current_operation = std::begin(ram_->buffer);
        }
    } else {
        *ram_->current_operation = Button::VO;
        ++ram_->current_operation;
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

Calculator::TextCtrlModifier Calculator::SpOrNotEqualButtonPressed(){
    if(state_ == CalculatorState::REAL_TIME) {
        if (button_p_ == ButtonState::PRESSED) {
            MoveToIf(std::not_equal_to<>());
            SwitchOffButton(button_p_);
        } else{
            if(button_sp_ == ButtonState::NOT_PRESSED){
                button_sp_ = ButtonState::PRESSED;
                ExecuteProgram();
            } else {
                //ram_ = std::nullopt;
                button_sp_ = ButtonState::NOT_PRESSED;
            }
        }
    } else {
        *ram_->current_operation = Button::SP;
        ++ram_->current_operation;
    }
    return TextCtrlNullaryOperation::DO_NOTHING;
}

void Calculator::ExecuteProgram(){
    std::cout<< "here\n";
    for(int i = 0; i < 5; i++){
        std::cout << static_cast<int>(ram_->buffer[i]) << "\n";
    }
    for( ; button_sp_ == ButtonState::PRESSED; ++ram_->current_operation){
        SwitchCase();
    }
}

void Calculator::SwitchCase(){
    switch (*ram_->current_operation)
    {
        case Button::ZERO:
            DigitButtonPressed(0);
            break;
            
        case Button::ONE:
            DigitButtonPressed(1);
            break;
            
        case Button::TWO:
            DigitButtonPressed(2);
            break;
            
        case Button::THREE:
            DigitButtonPressed(3);
            break;
            
        case Button::FOUR:
            DigitButtonPressed(4);
            break;
            
        case Button::FIVE:
            DigitButtonPressed(5);
            break;
            
        case Button::SIX:
            DigitButtonPressed(6);
            break;
            
        case Button::SEVEN:
            DigitButtonPressed(7);
            break;
            
        case Button::EIGHT:
            DigitButtonPressed(8);
            break;
            
        case Button::NINE:
            DigitButtonPressed(9);
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

