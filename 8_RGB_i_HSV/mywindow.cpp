// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"

// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"

// Definicja konstruktora, wywolujemy najpierw
// konstruktor klasy nadrzednej, nastepnie tworzymy
// obiekt klasy Ui_MyWindow reprezentujacy GUI
MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    // Wywolujemy funkcje tworzaca elementy GUI
    // Jej definicja znajduje sie w pliku "ui_mywindow.h"
    ui->setupUi(this);

    // Pobieramy wymiary i wspolrzedne lewego gornego naroznika ramki
    // i ustawiamy wartosci odpowiednich pol
    // Uwaga: ramke "rysujFrame" wykorzystujemy tylko do
    // wygodnego ustaiwenia tych wymiarow. Rysunek bedziemy wyswietlac
    // bezposrednio w glownym oknie aplikacji.
    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();

    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).
    img = new QImage(szer,wys,QImage::Format_RGB32);

}

// Definicja destruktora
MyWindow::~MyWindow()
{
    delete ui;
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku "Wyjscie" (exitButton)
// Uwaga: polaczenie tej funkcji z sygnalem "clicked"
// emitowanym przez przycisk jest realizowane
// za pomoca funkcji QMetaObject::connectSlotsByName(MyWindow)
// znajdujacej sie w automatycznie generowanym pliku "ui_mywindow.h"
// Nie musimy wiec sami wywolywac funkcji "connect"
void MyWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaznik do obiektu reprezentujacego aplikacje
    // quit() to funkcja (slot) powodujaca zakonczenie aplikacji z kodem 0 (brak bledu)
    qApp->quit();
}


// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*)
{
    // Obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);

    // Rysuje obrazek "img" w punkcie (poczX,poczY)
    // (tu bedzie lewy gorny naroznik)
    p.drawImage(poczX,poczY,*img);
}




//funkcja powoduje zamalowanie piksela danym kolorem.

void MyWindow::rysuj_piksel(int x, int y){
    if(x<szer && y < wys && x >= 0 && y >= 0){
        unsigned char *ptr;
        ptr = img->bits();
        ptr[szer*4*y + 4*x] = color.blue();
        ptr[szer*4*y + 4*x + 1] = color.green();
        ptr[szer*4*y + 4*x + 2] = color.red();
    }
}



void MyWindow::rysuj_rgb(char typ, int wartosc){

    switch(typ){
        case 'r':
            color.setRed(wartosc);
            for(int i = 0; i<szer; i++){
                for(int j = 0; j<wys; j++){
                    color.setGreen(i/2);
                    color.setBlue(j/2);
                    rysuj_piksel(i,j);
                }
            }
            break;

        case 'b':
            color.setBlue(wartosc);
            for(int i = 0; i<szer; i++){
                for(int j = 0; j<wys; j++){
                    color.setRed(i/2);
                    color.setGreen(j/2);
                    rysuj_piksel(i,j);
                }
            }
            break;

        case 'g':
            color.setGreen(wartosc);
            for(int i = 0; i<szer; i++){
                for(int j = 0; j<wys; j++){
                    color.setRed(i/2);
                    color.setBlue(j/2);
                    rysuj_piksel(i,j);
                }
            }
            break;

        default: break;
    }

    update();
}

void MyWindow::rysuj_hsv(char typ, int wartosc){

    double r,g,b;

    double h,s,v;

    double c,x,m;

    switch(typ){
        case 'h':
            h = (double)wartosc;
            for(int i = 0; i<szer; i++){
                for(int j = 0; j<wys; j++){
                    s = (double)j/512;
                    v = (double)i/512;
                    c = v * s;
                    double hp = (double)h/60.0;
                    x = c * (1 - abs(fmod(hp,2) - 1));

                         if(hp >= 0 && hp <= 1) {r=c; g=x; b=0;}
                    else if(hp >  1 && hp <= 2) {r=x; g=c; b=0;}
                    else if(hp >  2 && hp <= 3) {r=0; g=c; b=x;}
                    else if(hp >  3 && hp <= 4) {r=0; g=x; b=c;}
                    else if(hp >  4 && hp <= 5) {r=x; g=0; b=c;}
                    else if(hp >  5 && hp <= 6) {r=c; g=0; b=x;}
                    else {r=0; g=0; b=0;}
                    m = v - c;

                    color.setRed((r+m)*255);
                    color.setGreen((g+m)*255);
                    color.setBlue((b+m)*255);

                    rysuj_piksel(i,wys-j-1);
                }
            }
            break;

          case 's':

        s = (double)wartosc/512;
        for(int i = 0; i<szer; i++){
            for(int j = 0; j<wys; j++){
                h = (double)(i*360)/512;
                v = (double)j/512;
                c = v * s;
                double hp = (double)h/60.0;
                x = c * (1 - abs(fmod(hp,2) - 1));

                     if(hp >= 0 && hp <= 1) {r=c; g=x; b=0;}
                else if(hp >  1 && hp <= 2) {r=x; g=c; b=0;}
                else if(hp >  2 && hp <= 3) {r=0; g=c; b=x;}
                else if(hp >  3 && hp <= 4) {r=0; g=x; b=c;}
                else if(hp >  4 && hp <= 5) {r=x; g=0; b=c;}
                else if(hp >  5 && hp <= 6) {r=c; g=0; b=x;}
                else {r=0; g=0; b=0;}
                m = v - c;

                color.setRed((r+m)*255);
                color.setGreen((g+m)*255);
                color.setBlue((b+m)*255);

                rysuj_piksel(i,wys-j-1);
            }
        }
        break;

        case 'v':
            v = (double)wartosc/512;
            for(int i = 0; i<szer; i++){
                for(int j = 0; j<wys; j++){
                    s = (double)j/512;
                    h = (double)(i*360)/512;
                    c = v * s;
                    double hp = (double)h/60.0;
                    x = c * (1 - abs(fmod(hp,2) - 1));

                         if(hp >= 0 && hp <= 1) {r=c; g=x; b=0;}
                    else if(hp >  1 && hp <= 2) {r=x; g=c; b=0;}
                    else if(hp >  2 && hp <= 3) {r=0; g=c; b=x;}
                    else if(hp >  3 && hp <= 4) {r=0; g=x; b=c;}
                    else if(hp >  4 && hp <= 5) {r=x; g=0; b=c;}
                    else if(hp >  5 && hp <= 6) {r=c; g=0; b=x;}
                    else {r=0; g=0; b=0;}
                    m = v - c;

                    color.setRed((r+m)*255);
                    color.setGreen((g+m)*255);
                    color.setBlue((b+m)*255);

                    rysuj_piksel(i,wys-j-1);
                }
            }
            break;

        default: break;
    }

    update();
}



void MyWindow::on_redSlider_valueChanged(int value)
{
    ui->greenSlider->setSliderPosition(0);
    ui->blueSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_rgb('r', value);
}

void MyWindow::on_redSlider_sliderPressed()
{
    ui->greenSlider->setSliderPosition(0);
    ui->blueSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_rgb('r', ui->redSlider->value());
}

void MyWindow::on_greenSlider_valueChanged(int value)
{
    ui->blueSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_rgb('g', value);
}

void MyWindow::on_greenSlider_sliderPressed()
{
    ui->blueSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_rgb('g', ui->greenSlider->value());
}

void MyWindow::on_blueSlider_valueChanged(int value)
{
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_rgb('b', value);
}

void MyWindow::on_blueSlider_sliderPressed()
{
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_rgb('b', ui->blueSlider->value());
}

void MyWindow::on_hueSlider_valueChanged(int value)
{
    ui->blueSlider->setSliderPosition(0);
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_hsv('h', value);
}

void MyWindow::on_hueSlider_sliderPressed()
{
    ui->blueSlider->setSliderPosition(0);
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_hsv('h', ui->hueSlider->value());
}

void MyWindow::on_saturationSlider_valueChanged(int value)
{
    ui->blueSlider->setSliderPosition(0);
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_hsv('s', value);
}

void MyWindow::on_saturationSlider_sliderPressed()
{
    ui->blueSlider->setSliderPosition(0);
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->valueSlider->setSliderPosition(0);

    rysuj_hsv('s', ui->saturationSlider->value());
}

void MyWindow::on_valueSlider_valueChanged(int value)
{
    ui->blueSlider->setSliderPosition(0);
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);

    rysuj_hsv('v', value);
}

void MyWindow::on_valueSlider_sliderPressed()
{
    ui->blueSlider->setSliderPosition(0);
    ui->greenSlider->setSliderPosition(0);
    ui->redSlider->setSliderPosition(0);
    ui->hueSlider->setSliderPosition(0);
    ui->saturationSlider->setSliderPosition(0);

    rysuj_hsv('v', ui->valueSlider->value());
}
