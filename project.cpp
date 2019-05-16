#include "project.hpp"
/*  обычный режим работы почти закончен, нужно написать программируемый для этого есть массив RAM
    но я не уверена как его  реализовывать
    нужно записывать команды, а потом их исполнять
 я вот думала мб сделать по дибильному как делала овсянникова: просто в в програмируемом режиме
 записывать в массив указатели на функции а потом запускать массив на исполнение
 */
Calculator::Calculator(){
    state = true;           //real time caculator, false - программируемый калькулятор
    p_button = false;       //weather P button was pressed or not
    f_button = false;       //needed for extracting numbers from registers and scrolling deque
    vp_button = false;      //needed for floating point arithmetic
    regs.resize(8, 0);
};

Calculator calc;

std::string DoubleToString(double n){
    std::ostringstream strs;
    strs << n;
    return strs.str();
};

bool MainApp::OnInit() {
    auto frame = new MainFrame("Calculator", wxSize(300, 400));
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
    , panel (new wxPanel(this))
    , textControl(new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(20, 10), wxSize(200, 50))) {

    panel->SetBackgroundColour(wxColour(135, 206, 235));
    textControl->SetBackgroundColour(wxColour(255, 255, 255));
    
    //........................................NUMBER_BUTTONS........................................

    auto buttonZero  = new wxButton(panel,  MakeId(Button::ZERO), "0",  wxPoint(20, 270),  buttonSize);
    auto buttonOne   = new wxButton(panel,   MakeId(Button::ONE), "1",  wxPoint(20, 230),  buttonSize);
    auto buttonTwo   = new wxButton(panel,   MakeId(Button::TWO), "2",  wxPoint(70, 230),  buttonSize);
    auto buttonThree = new wxButton(panel, MakeId(Button::THREE), "3", wxPoint(120, 230),  buttonSize);
    auto buttonFour  = new wxButton(panel,  MakeId(Button::FOUR), "4",  wxPoint(20, 190),  buttonSize);
    auto buttonFive  = new wxButton(panel,  MakeId(Button::FIVE), "5",  wxPoint(70, 190),  buttonSize);
    auto buttonSix   = new wxButton(panel,   MakeId(Button::SIX), "6", wxPoint(120, 190),  buttonSize);
    auto buttonSeven = new wxButton(panel, MakeId(Button::SEVEN), "7",  wxPoint(20, 150),  buttonSize);
    auto buttonEight = new wxButton(panel, MakeId(Button::EIGHT), "8",  wxPoint(70, 150),  buttonSize);
    auto buttonNine  = new wxButton(panel,  MakeId(Button::NINE), "9", wxPoint(120, 150),  buttonSize);

    buttonZero ->Bind(wxEVT_BUTTON,  &MainFrame::OnZero, this);
    buttonOne  ->Bind(wxEVT_BUTTON,   &MainFrame::OnOne, this);
    buttonTwo  ->Bind(wxEVT_BUTTON,   &MainFrame::OnTwo, this);
    buttonThree->Bind(wxEVT_BUTTON, &MainFrame::OnThree, this);
    buttonFour ->Bind(wxEVT_BUTTON,  &MainFrame::OnFour, this);
    buttonFive ->Bind(wxEVT_BUTTON,  &MainFrame::OnFive, this);
    buttonSix  ->Bind(wxEVT_BUTTON,   &MainFrame::OnSix, this);
    buttonSeven->Bind(wxEVT_BUTTON, &MainFrame::OnSeven, this);
    buttonEight->Bind(wxEVT_BUTTON, &MainFrame::OnEight, this);
    buttonNine ->Bind(wxEVT_BUTTON,  &MainFrame::OnNine, this);

    //.....................................LEFT_MIDDLE_BUTTONS........................................

    auto textAdd      = new wxStaticText(panel,      MakeId(Button::ADD),     "sin", wxPoint(230, 180));
    auto textSubtract = new wxStaticText(panel, MakeId(Button::SUBTRACT),     "cos", wxPoint(230, 220));
    auto textMultiply = new wxStaticText(panel, MakeId(Button::MULTIPLY),      "pi", wxPoint(180, 180));
    auto textDivide   = new wxStaticText(panel,   MakeId(Button::DIVIDE),  "exp(x)", wxPoint(170, 220));
    auto textUp       = new wxStaticText(panel,       MakeId(Button::UP), "exp(ix)", wxPoint(220, 140));
    auto textSwap     = new wxStaticText(panel,     MakeId(Button::SWAP),      "ln", wxPoint(180, 140));

    auto buttonAdd      = new wxButton(panel,      MakeId(Button::ADD),   "+", wxPoint(220, 190), buttonSize);
    auto buttonSubtract = new wxButton(panel, MakeId(Button::SUBTRACT),   "-", wxPoint(220, 230), buttonSize);
    auto buttonMultiply = new wxButton(panel, MakeId(Button::MULTIPLY),   "х", wxPoint(170, 190), buttonSize);
    auto buttonDivide   = new wxButton(panel,   MakeId(Button::DIVIDE),   "/", wxPoint(170, 230), buttonSize);
    auto buttonUp       = new wxButton(panel,       MakeId(Button::UP),   "↑", wxPoint(220, 150), buttonSize);
    auto buttonSwap     = new wxButton(panel,     MakeId(Button::SWAP), "swp", wxPoint(170, 150), buttonSize);

    buttonAdd     ->Bind(wxEVT_BUTTON,      &MainFrame::OnAdd, this);
    buttonSubtract->Bind(wxEVT_BUTTON, &MainFrame::OnSubtract, this);
    buttonMultiply->Bind(wxEVT_BUTTON, &MainFrame::OnMultiply, this);
    buttonDivide  ->Bind(wxEVT_BUTTON,   &MainFrame::OnDivide, this);
    buttonUp      ->Bind(wxEVT_BUTTON,       &MainFrame::OnUp, this);
    buttonSwap    ->Bind(wxEVT_BUTTON,     &MainFrame::OnSwap, this);

    //........................................UPPER_BUTTONS............................................

    auto textShg   = new wxStaticText(panel,   MakeId(Button::SHG),  "рр",   wxPoint(80, 60));
    auto textShg2  = new wxStaticText(panel,  MakeId(Button::SHG2),  "рп",  wxPoint(130, 60));
    auto textVo    = new wxStaticText(panel,    MakeId(Button::VO), "x≥0",  wxPoint(180, 60));
    auto textSp    = new wxStaticText(panel,    MakeId(Button::SP), "x≠0",  wxPoint(230, 60));
    auto textPower = new wxStaticText(panel, MakeId(Button::POWER), "НОП", wxPoint(120, 100));
    auto textBp    = new wxStaticText(panel,    MakeId(Button::BP), "x=0", wxPoint(180, 100));
    auto textPp    = new wxStaticText(panel,    MakeId(Button::PP),  "pp", wxPoint(230, 100));

    auto buttonShg   = new wxButton(panel,   MakeId(Button::SHG),  "ШГ",   wxPoint(70, 70), buttonSize);
    auto buttonShg2  = new wxButton(panel,  MakeId(Button::SHG2),  "ШГ",  wxPoint(120, 70), buttonSize);
    auto buttonVo    = new wxButton(panel,    MakeId(Button::VO), "В/О",  wxPoint(170, 70), buttonSize);
    auto buttonSp    = new wxButton(panel,    MakeId(Button::SP), "С/П",  wxPoint(220, 70), buttonSize);
    auto buttonP     = new wxButton(panel,     MakeId(Button::P),   "P",  wxPoint(70, 110), buttonSize);
    auto buttonPower = new wxButton(panel, MakeId(Button::POWER), "x^y", wxPoint(120, 110), buttonSize);
    auto buttonBp    = new wxButton(panel,    MakeId(Button::BP),  "БП", wxPoint(170, 110), buttonSize);
    auto buttonPp    = new wxButton(panel,    MakeId(Button::PP),  "ПП", wxPoint(220, 110), buttonSize);
    auto buttonCx    = new wxButton(panel,    MakeId(Button::CX),  "Cx",  wxPoint(20, 110), buttonSize);

    buttonShg  ->Bind(wxEVT_BUTTON,   &MainFrame::OnShg, this);
    buttonShg2 ->Bind(wxEVT_BUTTON,  &MainFrame::OnShg2, this);
    buttonVo   ->Bind(wxEVT_BUTTON,    &MainFrame::OnVo, this);
    buttonSp   ->Bind(wxEVT_BUTTON,    &MainFrame::OnSp, this);
    buttonP    ->Bind(wxEVT_BUTTON,     &MainFrame::OnP, this);
    buttonPower->Bind(wxEVT_BUTTON, &MainFrame::OnPower, this);
    buttonBp   ->Bind(wxEVT_BUTTON,    &MainFrame::OnBp, this);
    buttonPp   ->Bind(wxEVT_BUTTON,    &MainFrame::OnPp, this);
    buttonCx   ->Bind(wxEVT_BUTTON,    &MainFrame::OnCx, this);

    //......................................LOWER_BUTTONS..............................................

    auto textPoint    = new wxStaticText(panel,    MakeId(Button::POINT), "1/x",  wxPoint(80, 260));
    auto textNegative = new wxStaticText(panel, MakeId(Button::NEGATIVE), "x^2", wxPoint(130, 260));
    auto textVp       = new wxStaticText(panel,       MakeId(Button::VP),  "√x", wxPoint(180, 260));

    auto buttonPoint    = new wxButton(panel,    MakeId(Button::POINT),   ".",  wxPoint(70, 270), buttonSize);
    auto buttonNegative = new wxButton(panel, MakeId(Button::NEGATIVE), "/-/", wxPoint(120, 270), buttonSize);
    auto buttonVp       = new wxButton(panel,       MakeId(Button::VP),  "ВП", wxPoint(170, 270), buttonSize);
    auto buttonF        = new wxButton(panel,        MakeId(Button::F),   "F", wxPoint(220, 270), buttonSize);

    buttonPoint   ->Bind(wxEVT_BUTTON,    &MainFrame::OnPoint, this);
    buttonNegative->Bind(wxEVT_BUTTON, &MainFrame::OnNegative, this);
    buttonVp      ->Bind(wxEVT_BUTTON,       &MainFrame::OnVp, this);
    buttonF       ->Bind(wxEVT_BUTTON,        &MainFrame::OnF, this);

    Centre();
};

//...........................................ON_BUTTON.................................................
//___________________________________________NUMBERS__________________________________________________


//.............................!!!! The numbers are basicly complited !!!...............................
// they just may need a bit of refactoring))

void MainFrame::OnZero(wxCommandEvent& event){
    textControl->AppendText("0");
    calc.number +="0";
    
    calc.p_button = false;
    calc.f_button = false;
};

void MainFrame::OnOne(wxCommandEvent& event){
    if (calc.p_button)                                      //inserting number into register
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 1, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)                                 //extracting number from register
    {
        calc.regs[0] = calc.regs[1];                        //when we extract, we than push number to the first register
        calc.regs[1] = 0;                                   //I was filling all the gaps with zeros
        calc.f_button = false;                              //but I am not very good at working with deque
    }
    else                                                    //regular state of work
    {
        textControl->AppendText("1");
        calc.number +="1";
    }
};

void MainFrame::OnTwo(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 2, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[2];
        calc.regs[2] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("2");
        calc.number +="2";
    }
};

void MainFrame::OnThree(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 3, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[3];
        calc.regs[3] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("3");
        calc.number +="3";
    }
};

void MainFrame::OnFour(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 4, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[4];
        calc.regs[4] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("4");
        calc.number +="4";
    }
};

void MainFrame::OnFive(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 5, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[5];
        calc.regs[5] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("5");
        calc.number +="5";
    }
};

void MainFrame::OnSix(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 6, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[6];
        calc.regs[6] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("6");
        calc.number +="6";
    }
};

void MainFrame::OnSeven(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 7, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[7];
        calc.regs[7] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("7");
        calc.number +="7";
    }
};

void MainFrame::OnEight(wxCommandEvent& event){
    if (calc.p_button)
    {
        auto iter = calc.regs.cbegin();
        calc.regs.insert(iter + 8, stod(calc.number));
        
        textControl->Clear();
        calc.number.clear();
        calc.p_button = false;
    }
    else if (calc.f_button)
    {
        calc.regs[0] = calc.regs[8];
        calc.regs[8] = 0;
        calc.f_button = false;
    }
    else
    {
        textControl->AppendText("8");
        calc.number +="8";
    }
};

void MainFrame::OnNine(wxCommandEvent& event){
    textControl->AppendText("9");
    calc.number +="9";
};

//__________________________________________OPERATIONS_____________________________________________

void MainFrame::OnSwap(wxCommandEvent& event){
    textControl->Clear();
    if(calc.state)
    {
        double res = 0;
        
        if (calc.p_button)
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            res = log(a);
            calc.number = DoubleToString(res);
            textControl->AppendText(calc.number);
        }
        else
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double b = calc.regs.front();
            calc.regs.front();
            
            calc.regs.push_front(a);
            calc.regs.push_front(b);
        }
    }
};

void MainFrame::OnUp(wxCommandEvent& event){
    textControl->Clear();
    if(calc.state)
    {
        if (calc.p_button)
        {
            //e^(ix) ???
        }
        else
        {
            double number = stod(calc.number);
            
            if (calc.vp_button)
            {
                number *=  pow(10, calc.regs.front());
                calc.regs.pop_front();
                calc.vp_button = false;
            }
            
            calc.regs.push_front(number);
            calc.regs.resize(8, 0);
            calc.number.clear();
        }
    }
};

void MainFrame::OnMultiply(wxCommandEvent& event){
    textControl->Clear();
    if(calc.state)
    {
        double res = 0;
        
        if (calc.p_button)
        {
            res = M_PI;
        }
        else
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double b = calc.regs.front();
            calc.regs.front();
            res = a * b;
        }
        
        calc.number += DoubleToString(res);
        textControl->AppendText(calc.number);
    }
};

void MainFrame::OnAdd(wxCommandEvent& event){
    textControl->Clear();
    if(calc.state)
    {
        double res = 0;
        
        if (calc.p_button)
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            res = sin(a);
        }
        else
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double b = calc.regs.front();
            calc.regs.front();
            res = a + b;
        }
        
        calc.number = DoubleToString(res);
        textControl->AppendText(calc.number);
    }
};

void MainFrame::OnDivide(wxCommandEvent& event){
    textControl->Clear();
    if(calc.state)
    {
        double res = 0;
        
        if (calc.p_button)
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            res = exp(a);
        }
        else
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double b = calc.regs.front();
            calc.regs.front();
            res = b / a;
        }
        
        calc.number += DoubleToString(res);
        textControl->AppendText(calc.number);
    }
};

void MainFrame::OnSubtract(wxCommandEvent& event){
    textControl->Clear();
    if(calc.state)
    {
        double res = 0;
        
        if (calc.p_button)
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            res = cos(a);
        }
        else
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double b = calc.regs.front();
            calc.regs.front();
            res = b - a;
        }
        
        calc.number = DoubleToString(res);
        textControl->AppendText(calc.number);
    }
};

//.........................................UPPER.......................................................
//это раздел функций, которые в основном используются в режиме программируемого калькулятора
/*
 Программа всегда заканчивается командой «C/П».
 Подпрограммы записываются с адресов после команды «C/П». Подпрограмма начинается записью «ПП». Каждая подпрограмма заканчивается командой «В/О».
 Для запуска программы нужно перейти в рабочий режим: «Р»«РР».
 Затем поставить счетчик команд на 00: «В/О», занести данные в соответствующие регистры и запустить программу «С/П».
 Для выполнения по шагам, клавиша «ПП»
 */

void MainFrame::OnShg(wxCommandEvent& event){
    textControl->Clear();
};

//я не знаю для чего эти функции в обычном режиме, они кажется имеют смысл с нажатой кнопко Р

void MainFrame::OnShg2(wxCommandEvent& event){
    textControl->Clear();
};

void MainFrame::OnVo(wxCommandEvent& event){
    textControl->Clear();
};

void MainFrame::OnSp(wxCommandEvent& event){
    textControl->Clear();
};

void MainFrame::OnP(wxCommandEvent& event){
    calc.p_button = true;
    textControl->Clear();
};

void MainFrame::OnPower(wxCommandEvent& event){
    textControl->Clear();
};

void MainFrame::OnBp(wxCommandEvent& event){
    textControl->Clear();
};

void MainFrame::OnPp(wxCommandEvent& event){
    textControl->Clear();
};

void MainFrame::OnCx(wxCommandEvent& event){
    textControl->Clear();
    calc.regs.clear();
    calc.number.clear();
};
//..............................................LOWEER......................................................
void MainFrame::OnPoint(wxCommandEvent& event){
    if(calc.state)                                      //regular state of work
    {
        //this is wevry weird, P button is responsible for red inscriptions
        // and F button is responsible for blue inscriptions
        if (calc.f_button)
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double res = 1/a;
            calc.number = DoubleToString(res);
            textControl->AppendText(calc.number);
        }
        else if(calc.p_button)
        {
            //нужно прокрутить вdeque влево ( сори, немногь не хватило времени )
        }
        else
        {
            textControl->AppendText(".");
            calc.number +=".";
        }
    }
};

void MainFrame::OnNegative(wxCommandEvent& event){
    if(calc.state)
    {
        if (calc.f_button)
        {
            double a = calc.regs.front();
            calc.regs.pop_front();
            double res = a * a;
            calc.number = DoubleToString(res);
        }
        else if(calc.p_button)
        {
            //нужно прокрутить вdeque вправо
        }
        else
        {
            textControl->Clear();
            calc.number.insert(calc.number.begin(), '-');
        }
        textControl->AppendText(calc.number);
    }
};

void MainFrame::OnVp(wxCommandEvent& event){
    if (calc.f_button)
    {
        double a = calc.regs.front();
        calc.regs.pop_front();
        double res = sqrt(a);
        calc.number = DoubleToString(res);
        textControl->AppendText(calc.number);
    }
    else
    {
        calc.vp_button = true;
        double number = stod(calc.number);
        calc.regs.push_front(number);
        calc.number.clear();
        textControl->Clear();
        
    }
};

void MainFrame::OnF(wxCommandEvent& event){
    calc.f_button = true;
};

//..........................................................................................................

IMPLEMENT_APP(MainApp)
