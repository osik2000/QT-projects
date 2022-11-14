// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"

// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"

#include <QDebug>
#include <QColorDialog>

// Definicja konstruktora, wywolujemy najpierw
// konstruktor klasy nadrzednej, nastepnie tworzymy
// obiekt klasy Ui_MyWindow reprezentujacy GUI
MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow){
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
    ui->scalingAllSlider->setVisible(false);
    ui->rzutowanieXButton->setVisible(false);
    ui->translationXButton->setVisible(false);
    ui->translationYButton->setVisible(false);
    ui->translationZButton->setVisible(false);
    ui->rotationXButton->setVisible(false);
    ui->rotationYButton->setVisible(false);
    ui->rotationZButton->setVisible(false);
    ui->scalingAllButton->setVisible(false);
    ui->scalingXButton->setVisible(false);
    ui->scalingYButton->setVisible(false);
    ui->scalingZButton->setVisible(false);
    ui->shearingXButton->setVisible(false);
    ui->shearingYButton->setVisible(false);
    ui->shearingZButton->setVisible(false);
    transformation();
}

// Definicja destruktora
MyWindow::~MyWindow(){
    delete ui;
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku "Wyjscie" (exitButton)
// Uwaga: polaczenie tej funkcji z sygnalem "clicked"
// emitowanym przez przycisk jest realizowane
// za pomoca funkcji QMetaObject::connectSlotsByName(MyWindow)
// znajdujacej sie w automatycznie generowanym pliku "ui_mywindow.h"
// Nie musimy wiec sami wywolywac funkcji "connect"
void MyWindow::on_exitButton_clicked(){
    // qApp to globalny wskaznik do obiektu reprezentujacego aplikacje
    // quit() to funkcja (slot) powodujaca zakonczenie aplikacji z kodem 0 (brak bledu)
    qApp->quit();
}

// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*){
    // Obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);
    // Rysuje obrazek "img" w punkcie (poczX,poczY)
    // (tu bedzie lewy gorny naroznik)
    p.drawImage(poczX,poczY,*img);
}

QColor MyWindow::pobierz_kolor(QImage *image, int x, int y){
    QColor out;
    if(x<szer && y < wys && x >= 0 && y >= 0){
        unsigned char *ptr;
        ptr = image->bits();
        out.setBlue(ptr[szer*4*y + 4*x]);
        out.setGreen(ptr[szer*4*y + 4*x + 1]);
        out.setRed(ptr[szer*4*y + 4*x + 2]);
        return out;
    }
    else return QColor::fromRgb(1,1,1,255);
}

void MyWindow::rysuj_piksel(int x, int y, QColor color, QImage *image){
    if(x<szer && y < wys && x >= 0 && y >= 0){
        unsigned char *ptr;
        ptr = image->bits();
        ptr[szer*4*y + 4*x] = color.blue();
        ptr[szer*4*y + 4*x + 1] = color.green();
        ptr[szer*4*y + 4*x + 2] = color.red();
    }
}

void MyWindow::rysuj_wierzcholek(int x, int y, QColor color){
    if(x<szer && y < wys && x >= 0 && y >= 0 && gruboscpedzla == 1){
        unsigned char *ptr;
        ptr = img->bits();
        ptr[szer*4*y + 4*x] = color.blue();
        ptr[szer*4*y + 4*x + 1] = color.green();
        ptr[szer*4*y + 4*x + 2] = color.red();
    }
    else if (gruboscpedzla>1){
        unsigned char *ptr;
        ptr = img->bits();

        for (int iy = gruboscpedzla; iy>=0; iy--)
            for (int ix = gruboscpedzla; ix>=0; ix--){
                if(x+ix-(gruboscpedzla/2)<szer && y+iy-(gruboscpedzla/2) < wys && x+ix-(gruboscpedzla/2) > 0 && y+iy-(gruboscpedzla/2) > 0){
                    ptr[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2))] = color.blue();
                    ptr[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2)) + 1] = color.green();
                    ptr[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2)) + 2] = color.red();
                }
        }
    }
}

void MyWindow::rysuj_odcinek(double x0, double y0, double x1, double y1, QColor color){
    double x,y;
    double a,b;

    if (x1-x0 != 0) {
        a = (y1-y0)/(x1-x0);
        b = y0 - a*x0;
    }
    if(x1-x0==0){
        x = x1;

        if (y1<y0){
            int temp = y0;
            y0 = y1;
            y1 = temp;
        }
        for (int y=y0; y<=y1; y++){
            rysuj_piksel(x,y,color,img);
        }
    }
    else{
        if (a>-1 && a<1){
            if (x1<x0){
                int temp = x0;
                x0 = x1;
                x1 = temp;
            }
            for (int x=x0; x<=x1; x++){
                y = (int)round(a * x + b);
                rysuj_piksel(x,y,color,img);
            }
        }
        else {
            if (y1<y0){
                int temp = y0;
                y0 = y1;
                y1 = temp;
            }
            for (int y=y0; y<=y1; y++){
                x = (int)round((y - b) / a);
                rysuj_piksel(x,y,color,img);
            }
        }
    }
    // Odswiezamy komponent
    update();
}


void MyWindow::czysc(){
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();
    int i,j;
    for(i=0; i<wys; i++)
        for(j=0; j<szer; j++){
            ptr[szer*4*i + 4*j]=0; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
        }
}

void MyWindow::zeruj_macierze(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            matrix_translation[i][j]= 0;
            matrix_temp[i][j]= 0;
            if (i == j){
                matrix_result[i][j] = 1;
                matrix_scaling[i][j]= 1;
            }
            else{
                matrix_result[i][j] = 0;
                matrix_rotationX[i][j]= 0;
                matrix_rotationY[i][j]= 0;
                matrix_rotationZ[i][j]= 0;
                matrix_scaling[i][j]= 0;
            }
        }
    }
}

void MyWindow::pomnoz_macierz(QString typ){


    double wynik_temp;

//    qDebug() << "pomnoz_macierz(QString "<<typ<<");";

    if((typ == "rotationX") || (typ == "rotationY") || (typ == "rotationZ") || (typ == "scaling") || (typ == "translation") || (typ == "shearingX") || (typ == "shearingY") || (typ == "shearingZ")){

        double macierz[4][4];
        double temp[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                if (typ == "rotationX") macierz[i][j] = matrix_rotationX[i][j];
                if (typ == "rotationY") macierz[i][j] = matrix_rotationY[i][j];
                if (typ == "rotationZ") macierz[i][j] = matrix_rotationZ[i][j];
                if (typ == "shearingX") macierz[i][j] = matrix_shearingX[i][j];
                if (typ == "shearingY") macierz[i][j] = matrix_shearingY[i][j];
                if (typ == "shearingZ") macierz[i][j] = matrix_shearingZ[i][j];
                if (typ == "scaling") macierz[i][j] = matrix_scaling[i][j];
                if (typ == "translation") macierz[i][j] = matrix_translation[i][j];
            }
        }
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                wynik_temp = 0;
                for(int k=0; k<4; k++){
                    wynik_temp = wynik_temp + matrix_result[i][k] * macierz[k][j];
                }
                temp[i][j] = wynik_temp;
            }
        }
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                matrix_result[i][j] = temp[i][j];
            }
        }
    }
    else{
        double macierz[4];
        for(int i=0; i<4; i++){
            if (typ == "p1") macierz[i] = matrix_p1[i];
            if (typ == "p2") macierz[i] = matrix_p2[i];
            if (typ == "p3") macierz[i] = matrix_p3[i];
            if (typ == "p4") macierz[i] = matrix_p4[i];
            if (typ == "p5") macierz[i] = matrix_p5[i];
            if (typ == "p6") macierz[i] = matrix_p6[i];
            if (typ == "p7") macierz[i] = matrix_p7[i];
            if (typ == "p8") macierz[i] = matrix_p8[i];
        }

        for(int i=0; i<4; i++){
            wynik_temp = 0;
            for(int j=0; j<4; j++){
                    wynik_temp = wynik_temp + matrix_result[i][j] * macierz[j];
                if(i<3){
                    if (typ == "p1") matrix_p1_3D_result[i] = wynik_temp;
                    if (typ == "p2") matrix_p2_3D_result[i] = wynik_temp;
                    if (typ == "p3") matrix_p3_3D_result[i] = wynik_temp;
                    if (typ == "p4") matrix_p4_3D_result[i] = wynik_temp;
                    if (typ == "p5") matrix_p5_3D_result[i] = wynik_temp;
                    if (typ == "p6") matrix_p6_3D_result[i] = wynik_temp;
                    if (typ == "p7") matrix_p7_3D_result[i] = wynik_temp;
                    if (typ == "p8") matrix_p8_3D_result[i] = wynik_temp;
                }
                else if (macierz[i]!=1) qDebug() << "sprawdz!! macierz[" <<i<<"] = "<<macierz[i]; // sprawdzenie p1-8[4]
            }
        }
    }
}

void MyWindow::translation(){
    matrix_translation[0][0] = 1;
    matrix_translation[0][3] = parameter_translatedX;
    matrix_translation[1][1] = 1;
    matrix_translation[1][3] = parameter_translatedY;
    matrix_translation[2][2] = 1;
    matrix_translation[2][3] = parameter_translatedZ;
    matrix_translation[3][3] = 1;
}

void MyWindow::rotationX(){
    double alpha = (double)parameter_rotatedX * (double)M_PI / 180.0;
    matrix_rotationX[0][0] = 1;
    matrix_rotationX[1][1] = cos(alpha);
    matrix_rotationX[1][2] = -sin(alpha);
    matrix_rotationX[2][1] = sin(alpha);
    matrix_rotationX[2][2] = cos(alpha);
    matrix_rotationX[3][3] = 1;
}
void MyWindow::rotationY(){
    double alpha = (double)parameter_rotatedY * (double)M_PI / 180.0;
    matrix_rotationY[0][0] = cos(alpha);
    matrix_rotationY[0][2] = sin(alpha);
    matrix_rotationY[1][1] = 1;
    matrix_rotationY[2][0] = -sin(alpha);
    matrix_rotationY[2][2] = cos(alpha);
    matrix_rotationY[3][3] = 1;
}
void MyWindow::rotationZ(){
    double alpha = (double)parameter_rotatedZ * (double)M_PI / 180.0;
    matrix_rotationZ[0][0] = cos(alpha);
    matrix_rotationZ[0][1] = -sin(alpha);
    matrix_rotationZ[1][0] = sin(alpha);
    matrix_rotationZ[1][1] = cos(alpha);
    matrix_rotationZ[2][2] = 1;
    matrix_rotationZ[3][3] = 1;
}

void MyWindow::shearingX(){
    matrix_shearingX[0][0] = 1;
    matrix_shearingX[1][0] = parameter_shearingY;
    matrix_shearingX[1][1] = 1;
    matrix_shearingX[2][0] = parameter_shearingZ;
    matrix_shearingX[2][2] = 1;
    matrix_shearingX[3][3] = 1;
}
void MyWindow::shearingY(){
    matrix_shearingY[0][0] = 1;
    matrix_shearingY[0][1] = parameter_shearingX;
    matrix_shearingY[1][1] = 1;
    matrix_shearingY[2][1] = parameter_shearingZ;
    matrix_shearingY[2][2] = 1;
    matrix_shearingY[3][3] = 1;
}
void MyWindow::shearingZ(){
    matrix_shearingZ[0][0] = 1;
    matrix_shearingZ[0][2] = parameter_shearingX;
    matrix_shearingZ[1][1] = 1;
    matrix_shearingZ[1][2] = parameter_shearingY;
    matrix_shearingZ[2][2] = 1;
    matrix_shearingZ[3][3] = 1;
}

void MyWindow::scaling(){
    matrix_scaling[0][0] = parameter_scaledX;
    matrix_scaling[1][1] = parameter_scaledY;
    matrix_scaling[2][2] = parameter_scaledZ;
    matrix_scaling[3][3] = 1;
}

void MyWindow::rzutuj_punkty(){
    matrix_p1_2D_result[0] = (matrix_p1_3D_result[0] * parameter_view) / (matrix_p1_3D_result[2]+parameter_view) + 300;
    matrix_p1_2D_result[1] = (matrix_p1_3D_result[1] * parameter_view) / (matrix_p1_3D_result[2]+parameter_view) + 300;
    if ((matrix_p1_3D_result[2]+parameter_view) >= 0) matrix_p1_3D_result[2] = -parameter_view;
    matrix_p2_2D_result[0] = (matrix_p2_3D_result[0] * parameter_view) / (matrix_p2_3D_result[2]+parameter_view) + 300;
    matrix_p2_2D_result[1] = (matrix_p2_3D_result[1] * parameter_view) / (matrix_p2_3D_result[2]+parameter_view) + 300;
    if ((matrix_p2_3D_result[2]+parameter_view) >= 0) matrix_p2_3D_result[2] = -parameter_view;
    matrix_p3_2D_result[0] = (matrix_p3_3D_result[0] * parameter_view) / (matrix_p3_3D_result[2]+parameter_view) + 300;
    matrix_p3_2D_result[1] = (matrix_p3_3D_result[1] * parameter_view) / (matrix_p3_3D_result[2]+parameter_view) + 300;
    if ((matrix_p3_3D_result[2]+parameter_view) >= 0) matrix_p3_3D_result[2] = -parameter_view;
    matrix_p4_2D_result[0] = (matrix_p4_3D_result[0] * parameter_view) / (matrix_p4_3D_result[2]+parameter_view) + 300;
    matrix_p4_2D_result[1] = (matrix_p4_3D_result[1] * parameter_view) / (matrix_p4_3D_result[2]+parameter_view) + 300;
    if ((matrix_p4_3D_result[2]+parameter_view) >= 0) matrix_p4_3D_result[2] = -parameter_view;
    matrix_p5_2D_result[0] = (matrix_p5_3D_result[0] * parameter_view) / (matrix_p5_3D_result[2]+parameter_view) + 300;
    matrix_p5_2D_result[1] = (matrix_p5_3D_result[1] * parameter_view) / (matrix_p5_3D_result[2]+parameter_view) + 300;
    if ((matrix_p5_3D_result[2]+parameter_view) >= 0) matrix_p5_3D_result[2] = -parameter_view;
    matrix_p6_2D_result[0] = (matrix_p6_3D_result[0] * parameter_view) / (matrix_p6_3D_result[2]+parameter_view) + 300;
    matrix_p6_2D_result[1] = (matrix_p6_3D_result[1] * parameter_view) / (matrix_p6_3D_result[2]+parameter_view) + 300;
    if ((matrix_p6_3D_result[2]+parameter_view) >= 0) matrix_p6_3D_result[2] = -parameter_view;
    matrix_p7_2D_result[0] = (matrix_p7_3D_result[0] * parameter_view) / (matrix_p7_3D_result[2]+parameter_view) + 300;
    matrix_p7_2D_result[1] = (matrix_p7_3D_result[1] * parameter_view) / (matrix_p7_3D_result[2]+parameter_view) + 300;
    if ((matrix_p7_3D_result[2]+parameter_view) >= 0) matrix_p7_3D_result[2] = -parameter_view;
    matrix_p8_2D_result[0] = (matrix_p8_3D_result[0] * parameter_view) / (matrix_p8_3D_result[2]+parameter_view) + 300;
    matrix_p8_2D_result[1] = (matrix_p8_3D_result[1] * parameter_view) / (matrix_p8_3D_result[2]+parameter_view) + 300;
    if ((matrix_p8_3D_result[2]+parameter_view) >= 0) matrix_p8_3D_result[2] = -parameter_view;
}

void MyWindow::rysuj_krawedzie(){
//      dolne krawedzie
    rysuj_odcinek(matrix_p1_2D_result[0],matrix_p1_2D_result[1],matrix_p2_2D_result[0],matrix_p2_2D_result[1], colorA);
    rysuj_odcinek(matrix_p2_2D_result[0],matrix_p2_2D_result[1],matrix_p3_2D_result[0],matrix_p3_2D_result[1], colorA);
    rysuj_odcinek(matrix_p3_2D_result[0],matrix_p3_2D_result[1],matrix_p4_2D_result[0],matrix_p4_2D_result[1], colorA);
    rysuj_odcinek(matrix_p4_2D_result[0],matrix_p4_2D_result[1],matrix_p1_2D_result[0],matrix_p1_2D_result[1], colorA);
//      gorne krawedzie
    rysuj_odcinek(matrix_p5_2D_result[0],matrix_p5_2D_result[1],matrix_p6_2D_result[0],matrix_p6_2D_result[1], colorB);
    rysuj_odcinek(matrix_p6_2D_result[0],matrix_p6_2D_result[1],matrix_p7_2D_result[0],matrix_p7_2D_result[1], colorB);
    rysuj_odcinek(matrix_p7_2D_result[0],matrix_p7_2D_result[1],matrix_p8_2D_result[0],matrix_p8_2D_result[1], colorB);
    rysuj_odcinek(matrix_p8_2D_result[0],matrix_p8_2D_result[1],matrix_p5_2D_result[0],matrix_p5_2D_result[1], colorB);
//      boczne krawedzie
    rysuj_odcinek(matrix_p1_2D_result[0],matrix_p1_2D_result[1],matrix_p5_2D_result[0],matrix_p5_2D_result[1], colorC);
    rysuj_odcinek(matrix_p2_2D_result[0],matrix_p2_2D_result[1],matrix_p6_2D_result[0],matrix_p6_2D_result[1], colorC);
    rysuj_odcinek(matrix_p3_2D_result[0],matrix_p3_2D_result[1],matrix_p7_2D_result[0],matrix_p7_2D_result[1], colorC);
    rysuj_odcinek(matrix_p4_2D_result[0],matrix_p4_2D_result[1],matrix_p8_2D_result[0],matrix_p8_2D_result[1], colorC);
//      wierzcholki
    rysuj_wierzcholek(matrix_p1_2D_result[0], matrix_p1_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p2_2D_result[0], matrix_p2_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p3_2D_result[0], matrix_p3_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p4_2D_result[0], matrix_p4_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p5_2D_result[0], matrix_p5_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p6_2D_result[0], matrix_p6_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p7_2D_result[0], matrix_p7_2D_result[1], colorW);
    rysuj_wierzcholek(matrix_p8_2D_result[0], matrix_p8_2D_result[1], colorW);
}


void MyWindow::transformation(){

    czysc();

    scaling();
    translation();
    rotationX();
    rotationY();
    rotationZ();
    shearingX();
    shearingY();
    shearingZ();

    pomnoz_macierz("translation");
    pomnoz_macierz("rotationX");
    pomnoz_macierz("rotationY");
    pomnoz_macierz("rotationZ");
    pomnoz_macierz("shearingX");
    pomnoz_macierz("shearingY");
    pomnoz_macierz("shearingZ");
    pomnoz_macierz("scaling");

    pomnoz_macierz("p1");
    pomnoz_macierz("p2");
    pomnoz_macierz("p3");
    pomnoz_macierz("p4");
    pomnoz_macierz("p5");
    pomnoz_macierz("p6");
    pomnoz_macierz("p7");
    pomnoz_macierz("p8");

    rzutuj_punkty();
    rysuj_krawedzie();

    zeruj_macierze();
    update();
}


void MyWindow::on_resetButton_clicked(){

        czysc();
        zeruj_macierze();

        colorA = QColor::fromRgb(128,0,0,255);
        colorB = QColor::fromRgb(0,128,0,255);
        colorC = QColor::fromRgb(0,0,128,255);
        colorW = QColor::fromRgb(255,0,0,255);

        ui->gruboscSlider->setValue(2);
        ui->translationXSlider->setValue(0);
        ui->translationYSlider->setValue(0);
        ui->translationZSlider->setValue(0);
        ui->rotationXSlider->setValue(0);
        ui->rotationYSlider->setValue(0);
        ui->rotationZSlider->setValue(0);
        ui->shearingXSlider->setValue(0);
        ui->shearingYSlider->setValue(0);
        ui->shearingZSlider->setValue(0);
        ui->rzutowanieSlider->setValue(500);

        if(proporcje)
            ui->scalingAllSlider->setValue(100);
        else{
            ui->scalingXSlider->setValue(100);
            ui->scalingYSlider->setValue(100);
            ui->scalingZSlider->setValue(100);
        }

        transformation();

        update();

}

void MyWindow::on_translationXSlider_valueChanged(int value){
    if(value == 0)
        ui->translationXButton->setVisible(false);
    else
        ui->translationXButton->setVisible(true);
    parameter_translatedX = value;
    transformation();
}

void MyWindow::on_translationYSlider_valueChanged(int value){
    if(value == 0)
        ui->translationYButton->setVisible(false);
    else
        ui->translationYButton->setVisible(true);
    parameter_translatedY = value;
    transformation();
}

void MyWindow::on_translationZSlider_valueChanged(int value)
{
    if(value == 0)
        ui->translationZButton->setVisible(false);
    else
        ui->translationZButton->setVisible(true);
    parameter_translatedZ = value;
    transformation();
}


void MyWindow::on_scalingXSlider_valueChanged(int value){
    if(value == 100)
        ui->scalingXButton->setVisible(false);
    else
        ui->scalingXButton->setVisible(true);
    parameter_scaledX = round(double(value))/100;
    if(proporcje){
        ui->scalingYSlider->setValue(value);
        ui->scalingZSlider->setValue(value);
    }
    transformation();
}

void MyWindow::on_scalingYSlider_valueChanged(int value){
    if(value == 100)
        ui->scalingYButton->setVisible(false);
    else
        ui->scalingYButton->setVisible(true);
    parameter_scaledY = round(double(value))/100;
    if(proporcje){
        ui->scalingXSlider->setValue(value);
        ui->scalingZSlider->setValue(value);
    }
    transformation();
}

void MyWindow::on_scalingZSlider_valueChanged(int value)
{
    if(value == 100)
        ui->scalingZButton->setVisible(false);
    else
        ui->scalingZButton->setVisible(true);
    parameter_scaledZ = round(double(value))/100;
    if(proporcje){
        ui->scalingXSlider->setValue(value);
        ui->scalingYSlider->setValue(value);
    }
    transformation();
}

void MyWindow::on_scalingAllSlider_valueChanged(int value){
    if(value == 100)
        ui->scalingAllButton->setVisible(false);
    else
        ui->scalingAllButton->setVisible(true);
    parameter_scaledX = round(double(value))/100;
    parameter_scaledY = round(double(value))/100;
    parameter_scaledZ = round(double(value))/100;
    transformation();
}

void MyWindow::on_scalingCheckBox_toggled(bool checked){
    if (checked) {
        proporcje = true;
        ui->scalingAllSlider->setValue(ui->scalingXSlider->value());
        ui->scalingAllSlider->setVisible(true);
        ui->scalingXSlider->setVisible(false);
        ui->scalingYSlider->setVisible(false);
        ui->scalingZSlider->setVisible(false);
        ui->scalingXButton->setVisible(false);
        ui->scalingYButton->setVisible(false);
        ui->scalingZButton->setVisible(false);
    }
    else {
        proporcje = false;
        ui->scalingXSlider->setValue(ui->scalingAllSlider->value());
        ui->scalingYSlider->setValue(ui->scalingAllSlider->value());
        ui->scalingZSlider->setValue(ui->scalingAllSlider->value());
        ui->scalingAllSlider->setVisible(false);
        ui->scalingXSlider->setVisible(true);
        ui->scalingYSlider->setVisible(true);
        ui->scalingZSlider->setVisible(true);
        if(ui->scalingXSlider->value() == 100)
            ui->scalingXButton->setVisible(false);
        else
            ui->scalingXButton->setVisible(true);

        if(ui->scalingYSlider->value() == 100)
            ui->scalingYButton->setVisible(false);
        else
            ui->scalingYButton->setVisible(true);

        if(ui->scalingZSlider->value() == 100)
            ui->scalingZButton->setVisible(false);
        else
            ui->scalingZButton->setVisible(true);
    }
}



void MyWindow::on_rotationXSlider_valueChanged(int value){
    if(value == 0)
        ui->rotationXButton->setVisible(false);
    else
        ui->rotationXButton->setVisible(true);
    parameter_rotatedX = (double)value;
    transformation();
}


void MyWindow::on_rotationYSlider_valueChanged(int value){
    if(value == 0)
        ui->rotationYButton->setVisible(false);
    else
        ui->rotationYButton->setVisible(true);
    parameter_rotatedY = (double)value;
    transformation();
}


void MyWindow::on_rotationZSlider_valueChanged(int value){
    if(value == 0)
        ui->rotationZButton->setVisible(false);
    else
        ui->rotationZButton->setVisible(true);
    parameter_rotatedZ = (double)value;
    transformation();
}


void MyWindow::on_rzutowanieSlider_valueChanged(int value){
    if(value == 500)
        ui->rzutowanieXButton->setVisible(false);
    else
        ui->rzutowanieXButton->setVisible(true);
    parameter_view = value;
    transformation();
}


void MyWindow::on_shearingXSlider_valueChanged(int value){
    if(value == 0)
        ui->shearingXButton->setVisible(false);
    else
        ui->shearingXButton->setVisible(true);
    parameter_shearingX = -value/100.0;
    transformation();
}


void MyWindow::on_shearingYSlider_valueChanged(int value){
    if(value == 0)
        ui->shearingYButton->setVisible(false);
    else
        ui->shearingYButton->setVisible(true);
    parameter_shearingY = -value/100.0;
    transformation();
}


void MyWindow::on_shearingZSlider_valueChanged(int value){
    if(value == 0)
        ui->shearingZButton->setVisible(false);
    else
        ui->shearingZButton->setVisible(true);
    parameter_shearingZ = -value/100.0;
    transformation();
}


void MyWindow::on_rzutowanieXButton_clicked(){
    ui->rzutowanieSlider->setValue(500);
}


void MyWindow::on_translationXButton_clicked(){
    ui->translationXSlider->setValue(0);
}


void MyWindow::on_translationYButton_clicked(){
    ui->translationYSlider->setValue(0);
}


void MyWindow::on_translationZButton_clicked(){
    ui->translationZSlider->setValue(0);
}


void MyWindow::on_rotationXButton_clicked(){
    ui->rotationXSlider->setValue(0);
}


void MyWindow::on_rotationYButton_clicked(){
    ui->rotationYSlider->setValue(0);
}


void MyWindow::on_rotationZButton_clicked(){
    ui->rotationZSlider->setValue(0);
}

void MyWindow::on_scalingAllButton_clicked()
{
    ui->scalingAllSlider->setValue(100);
}

void MyWindow::on_scalingXButton_clicked(){
    ui->scalingXSlider->setValue(100);
}


void MyWindow::on_scalingYButton_clicked(){
    ui->scalingYSlider->setValue(100);
}


void MyWindow::on_scalingZButton_clicked(){
    ui->scalingZSlider->setValue(100);
}


void MyWindow::on_shearingXButton_clicked(){
    ui->shearingXSlider->setValue(0);
}


void MyWindow::on_shearingYButton_clicked(){
    ui->shearingYSlider->setValue(0);
}


void MyWindow::on_shearingZButton_clicked(){
    ui->shearingZSlider->setValue(0);
}


void MyWindow::on_colorAButton_clicked(){
    QColor color_in = QColorDialog::getColor(Qt::white,this);
    if (color_in.isValid()){
    colorA = color_in;
    }
    transformation();
}


void MyWindow::on_colorBButton_clicked(){
    QColor color_in = QColorDialog::getColor(Qt::white,this);
    if (color_in.isValid()){
    colorB = color_in;
    }
    transformation();
}


void MyWindow::on_colorCButton_clicked(){
    QColor color_in = QColorDialog::getColor(Qt::white,this);
    if (color_in.isValid()){
    colorC = color_in;
    }
    transformation();
}


void MyWindow::on_colorButton_clicked(){
    QColor color_in = QColorDialog::getColor(Qt::white,this);
    if (color_in.isValid()){
    colorW = color_in;
    }
    transformation();
}


void MyWindow::on_gruboscSlider_valueChanged(int value){
    gruboscpedzla = value;
    transformation();
}




