    #include "frame.hpp"
#include "computations.hpp"

#include <iomanip>
#include <numeric>

bool MainApp::OnInit() {
    auto frame = new MainFrame("Calculator", wxSize(300, 400));
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
    , panel (new wxPanel(this))
    , text_ctrl(new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(20, 10), wxSize(200, 50))) {

    panel    ->SetBackgroundColour({135, 206, 235});
    text_ctrl->SetBackgroundColour({255, 255, 255});

    //........................................NUMBER_BUTTONS........................................

	auto buttonZero  = new wxButton(panel, MakeId(Button::ZERO) , "0",  wxPoint(20 , 270),  BUTTON_SIZE);
	auto buttonOne   = new wxButton(panel, MakeId(Button::ONE)  , "1",  wxPoint(20 , 230),  BUTTON_SIZE);
	auto buttonTwo   = new wxButton(panel, MakeId(Button::TWO)  , "2",  wxPoint(70 , 230),  BUTTON_SIZE);
	auto buttonThree = new wxButton(panel, MakeId(Button::THREE), "3",  wxPoint(120, 230),  BUTTON_SIZE);
	auto buttonFour  = new wxButton(panel, MakeId(Button::FOUR) , "4",  wxPoint(20 , 190),  BUTTON_SIZE);
	auto buttonFive  = new wxButton(panel, MakeId(Button::FIVE) , "5",  wxPoint(70 , 190),  BUTTON_SIZE);
	auto buttonSix   = new wxButton(panel, MakeId(Button::SIX)  , "6",  wxPoint(120, 190),  BUTTON_SIZE);
	auto buttonSeven = new wxButton(panel, MakeId(Button::SEVEN), "7",  wxPoint(20 , 150),  BUTTON_SIZE);
	auto buttonEight = new wxButton(panel, MakeId(Button::EIGHT), "8",  wxPoint(70 , 150),  BUTTON_SIZE);
	auto buttonNine  = new wxButton(panel, MakeId(Button::NINE) , "9",  wxPoint(120, 150),  BUTTON_SIZE);

	buttonZero ->Bind(wxEVT_BUTTON, &MainFrame::OnZero , this);
	buttonOne  ->Bind(wxEVT_BUTTON, &MainFrame::OnOne  , this);
	buttonTwo  ->Bind(wxEVT_BUTTON, &MainFrame::OnTwo  , this);
	buttonThree->Bind(wxEVT_BUTTON, &MainFrame::OnThree, this);
	buttonFour ->Bind(wxEVT_BUTTON, &MainFrame::OnFour , this);
	buttonFive ->Bind(wxEVT_BUTTON, &MainFrame::OnFive , this);
	buttonSix  ->Bind(wxEVT_BUTTON, &MainFrame::OnSix  , this);
	buttonSeven->Bind(wxEVT_BUTTON, &MainFrame::OnSeven, this);
	buttonEight->Bind(wxEVT_BUTTON, &MainFrame::OnEight, this);
	buttonNine ->Bind(wxEVT_BUTTON, &MainFrame::OnNine , this);

	//.....................................MIDDLE_BUTTONS........................................

	auto text_sin = new wxStaticText(panel, MakeId(Button::SIN), "sin"   , wxPoint(230, 180));
	auto text_cos = new wxStaticText(panel, MakeId(Button::COS), "cos"   , wxPoint(230, 220));
	auto text_pi  = new wxStaticText(panel, MakeId(Button::PI) , "pi"    , wxPoint(180, 180));
	auto text_exp = new wxStaticText(panel, MakeId(Button::EXP), "exp(x)", wxPoint(170, 220));
	auto text_log = new wxStaticText(panel, MakeId(Button::LOG), "ln"    , wxPoint(180, 140));

	auto button_add_or_sin      = new wxButton(panel, MakeId(Button::ADD)     , "+"  , wxPoint(220, 190), BUTTON_SIZE);
	auto button_subtract_or_cos = new wxButton(panel, MakeId(Button::SUBTRACT), "-"  , wxPoint(220, 230), BUTTON_SIZE);
	auto button_multiply_or_pi  = new wxButton(panel, MakeId(Button::MULTIPLY), "х"  , wxPoint(170, 190), BUTTON_SIZE);
	auto button_divide_or_exp   = new wxButton(panel, MakeId(Button::DIVIDE)  , "/"  , wxPoint(170, 230), BUTTON_SIZE);
	auto button_insert          = new wxButton(panel, MakeId(Button::INSERT)  , "↑"  , wxPoint(220, 150), BUTTON_SIZE);
	auto button_swap_or_log     = new wxButton(panel, MakeId(Button::SWAP)    , "swp", wxPoint(170, 150), BUTTON_SIZE);

	button_add_or_sin     ->Bind(wxEVT_BUTTON, &MainFrame::OnAddOrSin     , this);
	button_subtract_or_cos->Bind(wxEVT_BUTTON, &MainFrame::OnSubtractOrCos, this);
	button_multiply_or_pi ->Bind(wxEVT_BUTTON, &MainFrame::OnMultiplyOrPi , this);
	button_divide_or_exp  ->Bind(wxEVT_BUTTON, &MainFrame::OnDivideOrExp  , this);
	button_insert         ->Bind(wxEVT_BUTTON, &MainFrame::OnInsert       , this);
	button_swap_or_log    ->Bind(wxEVT_BUTTON, &MainFrame::OnSwapOrLog    , this);

	//......................................LOWER_BUTTONS..............................................

	auto text_inverse             = new wxStaticText(panel, MakeId(Button::INVERSE)            , "1/x", wxPoint(80 , 260));
	auto text_square              = new wxStaticText(panel, MakeId(Button::SQUARE)             , "x^2", wxPoint(130, 260));
	auto text_scientific_notation = new wxStaticText(panel, MakeId(Button::SCIENTIFIC_NOTATION), "√x" , wxPoint(180, 260));
	auto text_scroll_left         = new wxStaticText(panel, MakeId(Button::SCROLL_LEFT)        , "⟲", wxPoint(80 , 300));
    auto text_scroll_right        = new wxStaticText(panel, MakeId(Button::SCROLL_RIGHT)       , "↻", wxPoint(130, 300));

	auto button_point_or_inverse_or_scroll_left    = new wxButton(panel, MakeId(Button::POINT)   , "."  , wxPoint(70 , 270), BUTTON_SIZE);
	auto button_negative_or_square_or_scroll_right = new wxButton(panel, MakeId(Button::NEGATIVE), "/-/", wxPoint(120, 270), BUTTON_SIZE);
	auto button_scientific_notation_or_sqrt        = new wxButton(panel, MakeId(Button::SQRT)    , "ВП" , wxPoint(170, 270), BUTTON_SIZE);
	auto button_f                                  = new wxButton(panel, MakeId(Button::F)       , "F"  , wxPoint(220, 270), BUTTON_SIZE);

	button_point_or_inverse_or_scroll_left   ->Bind(wxEVT_BUTTON, &MainFrame::OnPointOrInverseOrScrollLeft   , this);
	button_negative_or_square_or_scroll_right->Bind(wxEVT_BUTTON, &MainFrame::OnNegativeOrSquareOrScrollRight, this);
	button_scientific_notation_or_sqrt       ->Bind(wxEVT_BUTTON, &MainFrame::OnScientificNotationOrSqrt     , this);
	button_f                                 ->Bind(wxEVT_BUTTON, &MainFrame::OnF                            , this);

	//........................................UPPER_BUTTONS............................................

	auto text_shg   = new wxStaticText(panel, MakeId(Button::STEP) , "рр" , wxPoint(80 , 60 ));
	auto text_shg2  = new wxStaticText(panel, MakeId(Button::STEP2), "рп" , wxPoint(130, 60 ));
	auto text_vo    = new wxStaticText(panel, MakeId(Button::VO)   , "x≥0", wxPoint(180, 60 ));
	auto text_sp    = new wxStaticText(panel, MakeId(Button::SP)   , "x≠0", wxPoint(230, 60 ));
	auto text_nop   = new wxStaticText(panel, MakeId(Button::NOP)  , "НОП", wxPoint(120, 100));
	auto text_bp    = new wxStaticText(panel, MakeId(Button::BP)   , "x=0", wxPoint(180, 100));
	auto text_pp    = new wxStaticText(panel, MakeId(Button::PP)   , "pp" , wxPoint(230, 100));

	auto button_p     = new wxButton(panel, MakeId(Button::P)    , "P"  , wxPoint(70 , 110), BUTTON_SIZE);
	auto button_cx    = new wxButton(panel, MakeId(Button::CX)   , "Cx" , wxPoint(20 , 110), BUTTON_SIZE);
	auto button_power = new wxButton(panel, MakeId(Button::POWER), "x^y", wxPoint(120, 110), BUTTON_SIZE);
	auto button_step  = new wxButton(panel, MakeId(Button::STEP) , "ШГ" , wxPoint(70 , 70 ), BUTTON_SIZE);
	auto button_step2 = new wxButton(panel, MakeId(Button::STEP2), "ШГ" , wxPoint(120, 70 ), BUTTON_SIZE);
	auto button_vo    = new wxButton(panel, MakeId(Button::VO)   , "В/О", wxPoint(170, 70 ), BUTTON_SIZE);
	auto button_sp    = new wxButton(panel, MakeId(Button::SP)   , "С/П", wxPoint(220, 70 ), BUTTON_SIZE);
	auto button_bp    = new wxButton(panel, MakeId(Button::BP)   , "БП" , wxPoint(170, 110), BUTTON_SIZE);
	auto button_pp    = new wxButton(panel, MakeId(Button::PP)   , "ПП" , wxPoint(220, 110), BUTTON_SIZE);

	button_p    ->Bind(wxEVT_BUTTON, &MainFrame::OnP    , this);
	button_cx   ->Bind(wxEVT_BUTTON, &MainFrame::OnCx   , this);
	button_power->Bind(wxEVT_BUTTON, &MainFrame::OnPower, this);
	button_step ->Bind(wxEVT_BUTTON, &MainFrame::OnShg  , this);
	button_step2->Bind(wxEVT_BUTTON, &MainFrame::OnShg2 , this);
	button_vo   ->Bind(wxEVT_BUTTON, &MainFrame::OnVo   , this);
	button_sp   ->Bind(wxEVT_BUTTON, &MainFrame::OnSp   , this);
	button_bp   ->Bind(wxEVT_BUTTON, &MainFrame::OnBp   , this);
	button_pp   ->Bind(wxEVT_BUTTON, &MainFrame::OnPp   , this);

	Centre();
}

void MainFrame::UpdateTextCtrl(Calculator::TextCtrlModifier item) {
	if (std::holds_alternative<TextCtrlNullaryOperation>(item)) {
		if (std::get<TextCtrlNullaryOperation>(item) == TextCtrlNullaryOperation::CLEAR) {
			text_ctrl->Clear();
		}
		return;
	}

	auto& [operation, text] =
		std::get<std::pair<TextCtrlUnaryOperation, std::variant<long double, std::string>>>(item);

	if (operation == TextCtrlUnaryOperation::ASSIGN) {
		text_ctrl->Clear();
	}

	if (std::holds_alternative<std::string>(text)) {
		*text_ctrl << std::get<std::string>(text);
	} else {
		std::stringstream ss;
		ss << std::defaultfloat << std::setprecision(20) <<  std::get<long double>(text);
		*text_ctrl << ss.str();
	}
}


//.........................................Функционал кнопок.................................................
//___________________________________________Цифры__________________________________________________


void MainFrame::OnZero(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(0));
}

void MainFrame::OnOne(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(1));
}

void MainFrame::OnTwo(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(2));
}

void MainFrame::OnThree(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(3));
}

void MainFrame::OnFour(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(4));
}

void MainFrame::OnFive(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(5));
}

void MainFrame::OnSix(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(6));
}

void MainFrame::OnSeven(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(7));
}

void MainFrame::OnEight(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(8));
}

void MainFrame::OnNine(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DigitButtonPressed(9));
}

//__________________________________________Кнопки посередине_____________________________________________

void MainFrame::OnAddOrSin(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.AddOrSinButtonPressed());
}

void MainFrame::OnSubtractOrCos(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.SubtractOrCosButtonPressed());
}

void MainFrame::OnMultiplyOrPi(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.MultiplyOrPiButtonPressed());
}

void MainFrame::OnDivideOrExp(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.DivideOrExpButtonPressed());
}

void MainFrame::OnInsert(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.InsertButtonPressed());
}

void MainFrame::OnSwapOrLog(wxCommandEvent& event){
    UpdateTextCtrl(calculator.SwapOrLogButtonPressed());
}

//__________________________________________Нижние кнопки_____________________________________________


void MainFrame::OnPointOrInverseOrScrollLeft(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.PointOrInverseOrScrollLeftButtonPressed());
}

void MainFrame::OnNegativeOrSquareOrScrollRight(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.NegativeOrSquareOrScrollRightButtonPressed());
}

void MainFrame::OnScientificNotationOrSqrt (wxCommandEvent& event) {
	UpdateTextCtrl(calculator.ScientificNotationOrSqrtButtonPressed());
}

void MainFrame::OnF(wxCommandEvent& event) {
	UpdateTextCtrl(calculator.FButtonPressed());
}


//.........................................UPPER.......................................................
//это раздел функций, которые в основном используются в режиме программируемого калькулятора

void MainFrame::OnP(wxCommandEvent& event){
	UpdateTextCtrl(calculator.PButtonPressed());
}

void MainFrame::OnCx(wxCommandEvent& event){
	UpdateTextCtrl(calculator.CxButtonPressed());
}

void MainFrame::OnPower(wxCommandEvent& event){
    UpdateTextCtrl(calculator.PowerButtonPressed());
}

void MainFrame::OnShg(wxCommandEvent& event){
    UpdateTextCtrl(calculator.ShgOrPpButtonPressed());
}

void MainFrame::OnShg2(wxCommandEvent& event){
    UpdateTextCtrl(calculator.ShgOrRpButtonPressed());
}

void MainFrame::OnVo(wxCommandEvent& event){
    UpdateTextCtrl(calculator.VoOrMoreButtonPressed());
}

void MainFrame::OnSp(wxCommandEvent& event){
    UpdateTextCtrl(calculator.SpOrNotEqualButtonPressed());
}


void MainFrame::OnBp(wxCommandEvent& event){
    UpdateTextCtrl(calculator.BPOrEqualButtonPresssed());
}

void MainFrame::OnPp(wxCommandEvent& event){
    UpdateTextCtrl(calculator.PPOrLessButtonPressed());
}

//..........................................................................................................

IMPLEMENT_APP(MainApp)
