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

    layers[0].picture = new QImage(":/zima");
    layers[1].picture = new QImage(":/ksiazki");
    layers[2].picture = new QImage(":/leosia");
    layers[3].picture = new QImage(":/spiderman");
    layers[4].picture = new QImage(":/spongebob");
    layers[5].picture = new QImage(":/toystory");

    for(int i = 0; i<6; i++){
        layers[i].alpha = 100;
        layers[i].type = 0;
    }

    *img = QImage(":/black");
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


void MyWindow::odswiez_warstwy(){

    for(int i = 0; i<6; i++){
        if (layers[i].active) blend(*img, *layers[i].picture, layers[i].alpha, layers[i].type);
    }
    update();

}

void MyWindow::blend(QImage tlo, QImage pierwszy_plan, int alpha, int mode){

    double a = (double)alpha/100.0;
    QColor temp;
    uchar Fr,Fg,Fb;
    uchar Ar, Ag, Ab, Br, Bg, Bb;

    for(int i = 0; i<szer; i++){
        for(int j = 0; j<wys; j++){


            Ar = pierwszy_plan.pixelColor(i,j).red();
            Ag = pierwszy_plan.pixelColor(i,j).green();
            Ab = pierwszy_plan.pixelColor(i,j).blue();

            Br = tlo.pixelColor(i,j).red();
            Bg = tlo.pixelColor(i,j).green();
            Bb = tlo.pixelColor(i,j).blue();

            switch(mode){

                case 0:
                    Fr = Ar;
                    Fg = Ag;
                    Fb = Ab;
                    break;

                case 1:
                    Fr = (Ar > Br) ? Ar : Br;
                    Fg = (Ag > Bg) ? Ag : Bg;
                    Fb = (Ab > Bb) ? Ab : Bb;
                    break;

                case 2:
                    Fr = (Ar < Br) ? Ar : Br;
                    Fg = (Ag < Bg) ? Ag : Bg;
                    Fb = (Ab < Bb) ? Ab : Bb;
                    break;

                case 3:
                    Fr = ((Br == 0) ? Br : qMax(0, (255 - ((255 - Ar ) << 8 ) / Br)));
                    Fg = ((Bg == 0) ? Bg : qMax(0, (255 - ((255 - Ag ) << 8 ) / Bg)));
                    Fb = ((Bb == 0) ? Bb : qMax(0, (255 - ((255 - Ab ) << 8 ) / Bb)));
                    break;

                case 4:
                    Fr = (Ar + Br) / 2;
                    Fg = (Ag + Bg) / 2;
                    Fb = (Ab + Bb) / 2;
                    break;

                case 5:
                    Fr = qMin(255, (Ar + Br));
                    Fg = qMin(255, (Ag + Bg));
                    Fb = qMin(255, (Ab + Bb));
                    break;

                default: break;
            }
            temp.setRed((double)a * Fr + (double)(1.0-a) * Br);
            temp.setGreen((double)a * Fg + (double)(1.0-a) * Bg);
            temp.setBlue((double)a * Fb + (double)(1.0-a) * Bb);

            img->setPixelColor(i,j, temp);
        }
    }

}


void MyWindow::on_ResetButton_clicked()
{
    ui->layerRadio_1->setChecked(true);
    ui->alphaSlider->setValue(100);
    ui->layerCheckBox_1->setChecked(false);
    ui->layerCheckBox_2->setChecked(false);
    ui->layerCheckBox_3->setChecked(false);
    ui->layerCheckBox_4->setChecked(false);
    ui->layerCheckBox_5->setChecked(false);
    ui->layerCheckBox_6->setChecked(false);

    for(int i = 0; i<6; i++){
        layers[i].alpha=100;
        layers[i].type=0;
        layers[i].active=false;
    }

    *img = QImage(":/black");

}


void MyWindow::on_layerRadio_1_clicked()
{
    ktora_wybrana = 0;
    ui->alphaModeBox->setCurrentIndex(layers[0].type);
    ui->alphaSlider->setValue(layers[0].alpha);
}


void MyWindow::on_layerRadio_2_clicked()
{
    ktora_wybrana = 1;
    ui->alphaModeBox->setCurrentIndex(layers[1].type);
    ui->alphaSlider->setValue(layers[1].alpha);
}


void MyWindow::on_layerRadio_3_clicked()
{
    ktora_wybrana = 2;
    ui->alphaModeBox->setCurrentIndex(layers[2].type);
    ui->alphaSlider->setValue(layers[2].alpha);

}


void MyWindow::on_layerRadio_4_clicked()
{
    ktora_wybrana = 3;
    ui->alphaModeBox->setCurrentIndex(layers[3].type);
    ui->alphaSlider->setValue(layers[3].alpha);
}


void MyWindow::on_layerRadio_5_clicked()
{
    ktora_wybrana = 4;
    ui->alphaModeBox->setCurrentIndex(layers[4].type);
    ui->alphaSlider->setValue(layers[4].alpha);
}


void MyWindow::on_layerRadio_6_clicked()
{
    ktora_wybrana = 5;
    ui->alphaModeBox->setCurrentIndex(layers[5].type);
    ui->alphaSlider->setValue(layers[5].alpha);
}


void MyWindow::on_layerCheckBox_1_stateChanged(int arg1)
{
    if(arg1) {
        ui->layerRadio_1->setChecked(true);
        on_layerRadio_1_clicked();
    }
    layers[0].active = arg1;
    odswiez_warstwy();
}


void MyWindow::on_layerCheckBox_2_stateChanged(int arg1)
{
    if(arg1) {
        ui->layerRadio_2->setChecked(true);
        on_layerRadio_2_clicked();
    }
    layers[1].active = arg1;
    odswiez_warstwy();
}


void MyWindow::on_layerCheckBox_3_stateChanged(int arg1)
{
    if(arg1) {
        ui->layerRadio_3->setChecked(true);
        on_layerRadio_3_clicked();
    }
    layers[2].active = arg1;
    odswiez_warstwy();
}


void MyWindow::on_layerCheckBox_4_stateChanged(int arg1)
{
    if(arg1) {
        ui->layerRadio_4->setChecked(true);
        on_layerRadio_4_clicked();
    }
    layers[3].active = arg1;
    odswiez_warstwy();
}


void MyWindow::on_layerCheckBox_5_stateChanged(int arg1)
{
    if(arg1) {
        ui->layerRadio_5->setChecked(true);
        on_layerRadio_5_clicked();
    }
    layers[4].active = arg1;
    odswiez_warstwy();
}


void MyWindow::on_layerCheckBox_6_stateChanged(int arg1)
{
    if(arg1) {
        ui->layerRadio_6->setChecked(true);
        on_layerRadio_6_clicked();
    }
    layers[5].active = arg1;
    odswiez_warstwy();
}


void MyWindow::on_alphaModeBox_currentIndexChanged(int index)
{
    layers[ktora_wybrana].type = index;
    odswiez_warstwy();
}


void MyWindow::on_alphaSlider_valueChanged(int value)
{
    layers[ktora_wybrana].alpha = value;
    odswiez_warstwy();
}

