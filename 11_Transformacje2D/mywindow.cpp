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
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            matrix_translation  [i][j]= 0;
            matrix_rotation     [i][j]= 0;
            matrix_scaling      [i][j]= 0;
            matrix_shearing     [i][j]= 0;
            matrix_temp         [i][j]= 0;
            matrix_result       [i][j]= 0;
        }
    }
}

void MyWindow::pomnoz_macierz(QString typ){
    double macierz[3][3];
    double temp[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    double wynik_temp;

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if (typ == "rotation") macierz[i][j] = matrix_rotation[i][j];
            if (typ == "scaling") macierz[i][j] = matrix_scaling[i][j];
            if (typ == "shearing") macierz[i][j] = matrix_shearing[i][j];
            if (typ == "translation") macierz[i][j] = matrix_translation[i][j];
        }
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            wynik_temp = 0;
            for(int k=0; k<3; k++){
                wynik_temp = wynik_temp + matrix_result[i][k] * macierz[k][j];
            }
            temp[i][j] = wynik_temp;
        }
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            matrix_result[i][j] = temp[i][j];
        }
    }
}

QColor MyWindow::pobierz_kolor_interpolacja(QImage *image, double dx, double dy){

    QColor out;
    if((int)dx<szer && (int)dy < wys && (int)dx >= 0 && (int)dy >= 0){

        if(dx>szer-2)dx = dx-1;
        if(dy>wys-2)dy = dy-1;

        unsigned char *ptr;
        ptr = image->bits();

        int Ax,Ay;
        int Bx,By;
        int Cx,Cy;
        int Dx,Dy;
        double a=0,b=0;

        uchar bA,bB,bC,bD;
        uchar gA,gB,gC,gD;
        uchar rA,rB,rC,rD;

        uchar bluee, greeen, reed;

        Ax = floor(dx);    Ay = ceil(dy);
        Bx = ceil(dx);    By = ceil(dy);
        Cx = ceil(dx);    Cy = floor(dy);
        Dx = floor(dx);    Dy = floor(dy);

        bA = ptr[szer*4*Ay + 4*Ax];        bB = ptr[szer*4*By + 4*Bx];
        gA = ptr[szer*4*Ay + 4*Ax + 1];    gB = ptr[szer*4*By + 4*Bx + 1];
        rA = ptr[szer*4*Ay + 4*Ax + 2];    rB = ptr[szer*4*By + 4*Bx + 2];

        bC = ptr[szer*4*Cy + 4*Cx];        bD = ptr[szer*4*Dy + 4*Dx];
        gC = ptr[szer*4*Cy + 4*Cx + 1];    gD = ptr[szer*4*Dy + 4*Dx + 1];
        rC = ptr[szer*4*Cy + 4*Cx + 2];    rD = ptr[szer*4*Dy + 4*Dx + 2];

        a = dx - floor(dx);
        b = dy - floor(dy);

        bluee = b * ((1.0-a)*bA + a*bB) + (1.0 - b) * ((1.0-a)*bD + a*bC);
        greeen = b * ((1.0-a)*gA + a*gB) + (1.0 - b) * ((1.0-a)*gD + a*gC);
        reed = b * ((1.0-a)*rA + a*rB) + (1.0 - b) * ((1.0-a)*rD + a*rC);

        out.setBlue(bluee);
        out.setGreen(greeen);
        out.setRed(reed);
      //  out.setAlpha(ptr[szerr*4*y + 4*x + 3]);
        return out;
    }
    else return QColor::fromRgb(1,1,1,255);
}

void MyWindow::translation(){
    matrix_translation[0][0] = 1;
    matrix_translation[0][1] = 0;
    matrix_translation[0][2] = parameter_translatedX;
    matrix_translation[1][0] = 0;
    matrix_translation[1][1] = 1;
    matrix_translation[1][2] = -parameter_translatedY;
    matrix_translation[2][2] = 1;
}

void MyWindow::rotation(){
    matrix_rotation[0][0] = cos(parameter_rotated);
    matrix_rotation[0][1] = -sin(parameter_rotated);
    matrix_rotation[0][2] = (szer/2.0)*(1-cos(parameter_rotated))+(wys/2.0)*(sin(parameter_rotated));
    matrix_rotation[1][0] = sin(parameter_rotated);
    matrix_rotation[1][1] = cos(parameter_rotated);
    matrix_rotation[1][2] = (wys/2.0)*(1-cos(parameter_rotated))-(szer/2.0)*(sin(parameter_rotated));
    matrix_rotation[2][2] = 1;
}

void MyWindow::scaling(){
    matrix_scaling[0][0] = parameter_scaledX;
    matrix_scaling[0][1] = 0;
    matrix_scaling[0][2] = (1 - parameter_scaledX) * (szer/2.0);
    matrix_scaling[1][0] = 0;
    matrix_scaling[1][1] = parameter_scaledY;
    matrix_scaling[1][2] = (1 - parameter_scaledY) * (wys/2.0);
    matrix_scaling[2][2] = 1;
}

void MyWindow::shearing(){
    matrix_shearing[0][0] = 1 + parameter_shearedX * parameter_shearedY;
    matrix_shearing[0][1] = parameter_shearedX;
    matrix_shearing[0][2] = parameter_shearedX * parameter_shearedY * (-(szer/2.0)) - (parameter_shearedX * (wys/2.0));
    matrix_shearing[1][0] = parameter_shearedY;
    matrix_shearing[1][1] = 1;
    matrix_shearing[1][2] = parameter_shearedY * (-(wys/2.0));
    matrix_shearing[2][2] = 1;
}

void MyWindow::transformation(){

    double tx,ty;

    czysc();

    rotation();
    scaling();
    shearing();
    translation();

//qDebug()<<"rotation:";
//    for(int i = 0; i<3; i++)
//        for(int j = 0; j<3; j++){
//            qDebug()<<"["<<i<<"]["<<j<<"]:"<< matrix_rotation[i][j];
//        }
//    qDebug()<<"translation:";
//    for(int i = 0; i<3; i++)
//        for(int j = 0; j<3; j++){
//            qDebug()<<"["<<i<<"]["<<j<<"]:"<< matrix_translation[i][j];
//        }
//    qDebug()<<"shearing:";
//    for(int i = 0; i<3; i++)
//        for(int j = 0; j<3; j++){
//            qDebug()<<"["<<i<<"]["<<j<<"]:"<< matrix_shearing[i][j];
//        }
//    qDebug()<<"scaling:";
//    for(int i = 0; i<3; i++)
//        for(int j = 0; j<3; j++){
//            qDebug()<<"["<<i<<"]["<<j<<"]:"<< matrix_scaling[i][j];
//        }

    matrix_result[0][0] = 1;matrix_result[0][1] = 0;matrix_result[0][2] = 0;
    matrix_result[1][0] = 0;matrix_result[1][1] = 1;matrix_result[1][2] = 0;
    matrix_result[2][2] = 1;

    pomnoz_macierz("rotation");
    pomnoz_macierz("scaling");
    pomnoz_macierz("shearing");
    pomnoz_macierz("translation");

    for(int iy = 0; iy<wys; iy++)
        for(int ix = 0; ix<szer; ix++){
            tx = matrix_result[0][0] * ix + matrix_result[0][1] * iy + matrix_result[0][2];
            ty = matrix_result[1][0] * ix + matrix_result[1][1] * iy + matrix_result[1][2];

            if(tx >= 0 && ty >= 0 && tx < szer && ty < wys){
                rysuj_piksel(ix,iy, pobierz_kolor_interpolacja(source_img,tx,ty), img);
            }
        }

    zeruj_macierze();
    update();
}

void MyWindow::on_loadButton_clicked(){
   nazwa_pliku = QFileDialog::getOpenFileName(this,
    tr("Zaladuj Obraz"), "..\\11_Transformacje2D", tr("Typy plikow graficznych (*.png *.jpg *.bmp)"));
   if (nazwa_pliku != ""){
       source_img = new QImage(nazwa_pliku);
       *source_img = source_img->scaled(szer,wys);
       *img = *source_img;
       update();

       ui->rotationSlider->setEnabled(true);
       ui->scalingAllSlider->setEnabled(true);
       ui->scalingCheckBox->setEnabled(true);
       ui->scalingXSlider->setEnabled(true);
       ui->scalingYSlider->setEnabled(true);
       ui->shearingXSlider->setEnabled(true);
       ui->shearingYSlider->setEnabled(true);
       ui->translationXSlider->setEnabled(true);
       ui->translationYSlider->setEnabled(true);
   }
}

void MyWindow::on_resetButton_clicked(){
    if(nazwa_pliku!="") {

        *img = *source_img;
        zeruj_macierze();

        ui->translationXSlider->setValue(0);
        ui->translationYSlider->setValue(0);
        ui->rotationSlider->setValue(0);
        ui->scalingXSlider->setValue(100);
        ui->scalingYSlider->setValue(100);
        ui->scalingAllSlider->setValue(100);
        ui->shearingXSlider->setValue(0);
        ui->shearingYSlider->setValue(0);
        update();
    }
}

void MyWindow::on_translationXSlider_valueChanged(int value){
    parameter_translatedX = -value;
    transformation();
}

void MyWindow::on_translationYSlider_valueChanged(int value){
    parameter_translatedY = value;
    transformation();
}

void MyWindow::on_rotationSlider_valueChanged(int value){
    parameter_rotated = M_PI*double(-value) / 180;
    transformation();
}

void MyWindow::on_scalingXSlider_valueChanged(int value){
    parameter_scaledX = 100.0 / double(value);
    if(proporcje)
        ui->scalingYSlider->setValue(value);
    transformation();
}

void MyWindow::on_scalingYSlider_valueChanged(int value){
    parameter_scaledY = 100.0 / double(value);
    if(proporcje)
        ui->scalingXSlider->setValue(value);
    transformation();
}

void MyWindow::on_shearingXSlider_valueChanged(int value){
    parameter_shearedX = double(value) / 100;
    transformation();
}

void MyWindow::on_shearingYSlider_valueChanged(int value){
    parameter_shearedY = double(value) / 100;
    transformation();
}

void MyWindow::on_scalingCheckBox_toggled(bool checked){
    if (checked) {
        ui->scalingAllSlider->setValue(ui->scalingXSlider->value());
        ui->scalingAllSlider->setVisible(true);
        ui->scalingXSlider->setVisible(false);
        ui->scalingYSlider->setVisible(false);
    }
    else {
        ui->scalingXSlider->setValue(ui->scalingAllSlider->value());
        ui->scalingYSlider->setValue(ui->scalingAllSlider->value());
        ui->scalingAllSlider->setVisible(false);
        ui->scalingXSlider->setVisible(true);
        ui->scalingYSlider->setVisible(true);
    }
}

void MyWindow::on_scalingAllSlider_valueChanged(int value){
    parameter_scaledX = 100.0 / double(value);
    parameter_scaledY = 100.0 / double(value);
    transformation();
}
