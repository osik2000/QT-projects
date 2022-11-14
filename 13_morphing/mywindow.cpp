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

    szer2 = ui->rysujFrame_2->width();
    wys2 = ui->rysujFrame_2->height();
    poczX2 = ui->rysujFrame_2->x();
    poczY2 = ui->rysujFrame_2->y();

    szer3 = ui->rysujFrame_3->width();
    wys3 = ui->rysujFrame_3->height();
    poczX3 = ui->rysujFrame_3->x();
    poczY3 = ui->rysujFrame_3->y();

    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).

    img = new QImage(szer,wys,QImage::Format_RGB32);
    img2 = new QImage(szer,wys,QImage::Format_RGB32);
    img3 = new QImage(szer,wys,QImage::Format_RGB32);

    kopia = new QImage(szer,wys,QImage::Format_RGB32);

    source_img = new QImage(szer,wys,QImage::Format_RGB32);
    source_img2 = new QImage(szer,wys,QImage::Format_RGB32);

    imgPunkty = new QImage(szer,wys,QImage::Format_ARGB32);
    imgPunkty2 = new QImage(szer,wys,QImage::Format_ARGB32);


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
    p.drawImage(poczX,poczY,*imgPunkty);
    p.drawImage(poczX2,poczY2,*img2);
    p.drawImage(poczX2,poczY2,*imgPunkty2);
    p.drawImage(poczX3,poczY3,*img3);
}


QColor MyWindow::pobierz_kolor(QImage *image, int x, int y, QString side){
    QColor out;
    int szerr, wyss;
    if(side=="left") {
        szerr = szer;
        wyss = wys;
    } else {
        szerr = szer2;
        wyss = wys2;
    }

    if(x<szerr && y < wyss && x >= 0 && y >= 0){
        unsigned char *ptr;
        ptr = image->bits();
        out.setBlue(ptr[szerr*4*y + 4*x]);
        out.setGreen(ptr[szerr*4*y + 4*x + 1]);
        out.setRed(ptr[szerr*4*y + 4*x + 2]);
      //  out.setAlpha(ptr[szerr*4*y + 4*x + 3]);
        return out;
    }
    else return QColor::fromRgb(1,1,1,255);
}


QColor MyWindow::pobierz_kolor_interpolacja(QImage *image, double dx, double dy, QString side){
    QColor out;
    int szerr, wyss;
    if(side=="left") {
        szerr = szer;
        wyss = wys;
    } else {
        szerr = szer2;
        wyss = wys2;
    }
    if((int)dx<szerr && (int)dy < wyss && (int)dx >= 0 && (int)dy >= 0){

        unsigned char *ptr;
        ptr = image->bits();

        int Ax,Ay;
        int Bx,By;
        int Cx,Cy;
        int Dx,Dy;
        double a,b;

        uchar bA,bB,bC,bD;
        uchar gA,gB,gC,gD;
        uchar rA,rB,rC,rD;

        uchar bluee, greeen, reed;

        Ax = floor(dx);    Ay = ceil(dy);
        Bx = ceil(dx);    By = ceil(dy);
        Cx = ceil(dx);    Cy = floor(dy);
        Dx = floor(dx);    Dy = floor(dy);

        bA = ptr[szerr*4*Ay + 4*Ax];        bB = ptr[szerr*4*By + 4*Bx];
        gA = ptr[szerr*4*Ay + 4*Ax + 1];    gB = ptr[szerr*4*By + 4*Bx + 1];
        rA = ptr[szerr*4*Ay + 4*Ax + 2];    rB = ptr[szerr*4*By + 4*Bx + 2];

        bC = ptr[szerr*4*Cy + 4*Cx];        bD = ptr[szerr*4*Dy + 4*Dx];
        gC = ptr[szerr*4*Cy + 4*Cx + 1];    gD = ptr[szerr*4*Dy + 4*Dx + 1];
        rC = ptr[szerr*4*Cy + 4*Cx + 2];    rD = ptr[szerr*4*Dy + 4*Dx + 2];

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

//funkcja powoduje zamalowanie piksela danym kolorem.

void MyWindow::rysuj_piksel(int x, int y, QColor color, QString side){
    if(side=="left"){
        if(x<szer && y < wys && x >= 0 && y >= 0){
            unsigned char *ptr;
            ptr = img->bits();
            ptr[szer*4*y + 4*x] = color.blue();
            ptr[szer*4*y + 4*x + 1] = color.green();
            ptr[szer*4*y + 4*x + 2] = color.red();
        }
    }
    else if(side=="right"){
            if(x<szer2 && y < wys2 && x >= 0 && y >= 0){
                unsigned char *ptr;
                ptr = img2->bits();
                ptr[szer*4*y + 4*x] = color.blue();
                ptr[szer*4*y + 4*x + 1] = color.green();
                ptr[szer*4*y + 4*x + 2] = color.red();
            }
    }
    else if(side=="bottom"){
            if(x<szer2 && y < wys2 && x >= 0 && y >= 0){
                unsigned char *ptr;
                ptr = img3->bits();
                ptr[szer*4*y + 4*x] = color.blue();
                ptr[szer*4*y + 4*x + 1] = color.green();
                ptr[szer*4*y + 4*x + 2] = color.red();
            }
    }
}


void MyWindow::rysuj_piksel_2(int x, int y, QColor color, QString side){
    if(side=="left"){
        if(x<szer && y < wys && x >= 0 && y >= 0){
            unsigned char *ptr;
            ptr = imgPunkty->bits();
            ptr[szer*4*y + 4*x] = color.blue();
            ptr[szer*4*y + 4*x + 1] = color.green();
            ptr[szer*4*y + 4*x + 2] = color.red();
            ptr[szer*4*y + 4*x + 3] = 255;
        }
    }
    else if(side=="right"){
            if(x<szer2 && y < wys2 && x >= 0 && y >= 0){
                unsigned char *ptr;
                ptr = imgPunkty2->bits();
                ptr[szer*4*y + 4*x] = color.blue();
                ptr[szer*4*y + 4*x + 1] = color.green();
                ptr[szer*4*y + 4*x + 2] = color.red();
                ptr[szer*4*y + 4*x + 3] = 255;
            }
    }
    else if(side=="bottom"){
            if(x<szer3 && y < wys3 && x >= 0 && y >= 0){
                unsigned char *ptr;
                ptr = img3->bits();
                ptr[szer*4*y + 4*x] = color.blue();
                ptr[szer*4*y + 4*x + 1] = color.green();
                ptr[szer*4*y + 4*x + 2] = color.red();
                ptr[szer*4*y + 4*x + 3] = color.alpha();
            }
    }

}

void MyWindow::rysuj_odcinek(double x0, double y0, double x1, double y1, QString side){

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
            rysuj_piksel_2(x,y,color,side);
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
                rysuj_piksel_2(x,y,color,side);
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
                rysuj_piksel_2(x,y,color,side);
            }
        }
    }
    // Odswiezamy komponent
    update();
}

void MyWindow::rysuj_pikselPunkty(int x, int y, QColor color, QString side){
    if(side=="left"){
        if(x<szer && y < wys && x >= 0 && y >= 0){
            unsigned char *ptrPunkty;
            ptrPunkty = imgPunkty->bits();

            ptrPunkty[szer*4*y + 4*x] = color.blue();
            ptrPunkty[szer*4*y + 4*x + 1] = color.green();
            ptrPunkty[szer*4*y + 4*x + 2] = color.red();
            ptrPunkty[szer*4*y + 4*x + 3] = 255;
        }
    }
    else if(side=="right"){
        if(x<szer2 && y < wys2 && x >= 0 && y >= 0){
            unsigned char *ptrPunkty;
            ptrPunkty = imgPunkty2->bits();

            ptrPunkty[szer*4*y + 4*x] = color.blue();
            ptrPunkty[szer*4*y + 4*x + 1] = color.green();
            ptrPunkty[szer*4*y + 4*x + 2] = color.red();
            ptrPunkty[szer*4*y + 4*x + 3] = 255;
        }
    }
}

void MyWindow::rysuj_punkt(int x, int y, QString side){

    for (int ix = x-2; ix<=x+2; ix++){
        for(int iy = y-2; iy<=y+2; iy++){
            rysuj_pikselPunkty(ix, iy, color, side);
        }
    }
    update();
}

void MyWindow::dodaj_punkt(int x, int y, QString side){

    if(side=="left"){
        licznikpunktow++;
        punkty_x.push_back(x);
        punkty_y.push_back(y);
    }
    else if(side=="right"){
        licznikpunktow2++;
        punkty_x2.push_back(x);
        punkty_y2.push_back(y);
    }
    rysuj_punkty();
}


void MyWindow::aktualizuj_punkt(int i, int x, int y, QString side){
    if(side=="left"){
        punkty_x.replace(i, x);
        punkty_y.replace(i, y);
    }
    else if(side=="right"){
        punkty_x2.replace(i,x);
        punkty_y2.replace(i,y);
    }
    rysuj_punkty();
}

int MyWindow::ktore_okno(int x, int y){
    if(x>=poczX && x<poczX+szer && y>=poczY && y<poczY+wys) return 1;
        else if(x>=poczX2 && x<poczX2+szer2 && y>=poczY2 && y<poczY2+wys2) return 2;
    else return 0;
}


void MyWindow::on_loadButton_clicked(){
   nazwa_pliku = QFileDialog::getOpenFileName(this,
    tr("Zaladuj Obraz"), "..\\13_morphing", tr("Typy plikow graficznych (*.png *.jpg *.bmp)"));
   if (nazwa_pliku != ""){

       source_img = new QImage(nazwa_pliku);
       *source_img = source_img->scaled(szer,wys);
       *img = *source_img;
       update();
   }

   if(nazwa_pliku != "" && nazwa_pliku2 != "") ui->resetButton->setEnabled(true);
}


void MyWindow::on_loadButton_2_clicked(){
    nazwa_pliku2 = QFileDialog::getOpenFileName(this,
     tr("Zaladuj Obraz"), "..\\13_morphing", tr("Typy plikow graficznych (*.png *.jpg *.bmp)"));
    if (nazwa_pliku2 != ""){
        ui->resetButton->setEnabled(true);
        source_img2 = new QImage(nazwa_pliku2);
        *source_img2 = source_img2->scaled(szer2,wys2);
        *img2 = *source_img2;
        update();
    }
    if(nazwa_pliku != "" && nazwa_pliku2 != "") ui->resetButton->setEnabled(true);
}



void MyWindow::czysc_punkty(){

    unsigned char *ptrPunkty, *ptrPunkty2;
    ptrPunkty = imgPunkty->bits();
    ptrPunkty2 = imgPunkty2->bits();
    int i,j;

    for(i=0; i<wys; i++){
        for(j=0; j<=szer; j++){
            ptrPunkty[szer*4*i + 4*j] = 0; // Skladowa BLUE
            ptrPunkty[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptrPunkty[szer*4*i + 4*j + 2] = 0; // Skladowa RED
            ptrPunkty[szer*4*i + 4*j + 3] = 0; // Skladowa ALPHA
        }
    }
    for(i=0; i<wys2; i++){
        for(j=0; j<=szer2; j++){
            ptrPunkty2[szer*4*i + 4*j] = 0; // Skladowa BLUE
            ptrPunkty2[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptrPunkty2[szer*4*i + 4*j + 2] = 0; // Skladowa RED
            ptrPunkty2[szer*4*i + 4*j + 3] = 0; // Skladowa ALPHA
        }
    }
    update();
}

void MyWindow::rysuj_punkty(){


    czysc_punkty();

    if(licznikpunktow == 3 && licznikpunktow2 == 3) {
        isrunable = true;
        ui->runButton->setEnabled(true);
    }


    for(int i=0; i<licznikpunktow; i++){
        rysuj_punkt(punkty_x[i],punkty_y[i], "left");
    }
    for(int i=0; i<licznikpunktow2; i++){
        rysuj_punkt(punkty_x2[i],punkty_y2[i], "right");
    }
    if(licznikpunktow == 3){
        rysuj_trojkat("left");
    }
    if(licznikpunktow2 == 3){
        rysuj_trojkat("right");
    }

    update();
}

void MyWindow::rysuj_trojkat(QString side){
    if(side == "left"){
        for(int i = 0; i<punkty_x.size()-1; i++){
            rysuj_odcinek(punkty_x[i],punkty_y[i],punkty_x[i+1],punkty_y[i+1], "left");
        }
        rysuj_odcinek(punkty_x[0],punkty_y[0],punkty_x[2],punkty_y[2], "left");
    }
    else if(side == "right"){
        for(int i = 0; i<punkty_x2.size()-1; i++){
            rysuj_odcinek(punkty_x2[i],punkty_y2[i],punkty_x2[i+1],punkty_y2[i+1], "right");
        }
        rysuj_odcinek(punkty_x2[0],punkty_y2[0],punkty_x2[2],punkty_y2[2], "right");
    }

}

QColor MyWindow::blend(QColor c1, QColor c2, int n){

    int r,g,b,a;

    double frame = (double)n/(double)frames;

   r = (double)(1 - frame)*c1.red()   + frame * c2.red() ;
   g = (double)(1 - frame)*c1.green() + frame * c2.green() ;
   b = (double)(1 - frame)*c1.blue()  + frame * c2.blue() ;
   a = (double)(1 - frame)*c1.alpha() + frame * c2.alpha() ;

   return QColor::fromRgb(r,g,b,a);
}

void MyWindow::teksturuj(int n){

    if(teksturowanie_tlo == 0) czysc("bottom");
    if(teksturowanie_tlo == 1) *img3 = *source_img;
    if(teksturowanie_tlo == 2) *img3 = *source_img2;

    int punkty_posrednie_x[3];
    int punkty_posrednie_y[3];

    double frame = (double)n/(double)frames;

    for(int i=0; i<3; i++){

        double x,x2,y,y2;

        x = punkty_x[i];
        y = punkty_y[i];
        x2 = punkty_x2[i];
        y2 = punkty_y2[i];

        punkty_posrednie_x[i] = ((1.0 - frame)* x) + (frame * x2);
        punkty_posrednie_y[i] = ((1.0 - frame)* y) + (frame * y2);
    }

    double u = 0, v = 0, w = 0, mianownik;

    int minx  = punkty_posrednie_x[0],
        maxx  = punkty_posrednie_x[0],
        miny  = punkty_posrednie_y[0],
        maxy  = punkty_posrednie_y[0];

    for(int i = 1; i < 3; i++){

        if(punkty_posrednie_x[i] < minx) minx = punkty_posrednie_x[i];
        if(punkty_posrednie_y[i] < miny) miny = punkty_posrednie_y[i];
        if(punkty_posrednie_x[i] > maxx) maxx = punkty_posrednie_x[i];
        if(punkty_posrednie_y[i] > maxy) maxy = punkty_posrednie_y[i];
    }

    mianownik = (((punkty_posrednie_x[1] - punkty_posrednie_x[0]) * (punkty_posrednie_y[2] - punkty_posrednie_y[0])) - ((punkty_posrednie_y[1] - punkty_posrednie_y[0]) * (punkty_posrednie_x[2] - punkty_posrednie_x[0])));

        for(int iy = miny; iy < maxy; iy++){
            for(int ix = minx; ix < maxx; ix++){
                v = ((( (ix - punkty_posrednie_x[0]) * (punkty_posrednie_y[2] - punkty_posrednie_y[0]) ) - ((punkty_posrednie_x[2] - punkty_posrednie_x[0]) * ((iy - punkty_posrednie_y[0])))) / mianownik);
                w = (( ( (punkty_posrednie_x[1] - punkty_posrednie_x[0]) * (iy - punkty_posrednie_y[0]) ) - ((punkty_posrednie_y[1] - punkty_posrednie_y[0]) * (ix - punkty_posrednie_x[0]))) / mianownik);
                u = 1 - v - w;

                double xt, yt, xt2, yt2;

                if(u > 0 && u < 1 && v > 0 && v < 1 && w > 0 && w < 1){

                    QColor colort, colort2;
                    xt = u * punkty_x[0] + v * punkty_x[1] + w * punkty_x[2];
                    yt = u * punkty_y[0] + v * punkty_y[1] + w * punkty_y[2];
                    colort = pobierz_kolor_interpolacja(source_img, xt, yt, "left");


                    xt2 = u * punkty_x2[0] + v * punkty_x2[1] + w * punkty_x2[2];
                    yt2 = u * punkty_y2[0] + v * punkty_y2[1] + w * punkty_y2[2];
                    colort2 = pobierz_kolor_interpolacja(source_img2, xt2, yt2, "right");

                    rysuj_piksel(ix,iy,blend(colort,colort2, n),"bottom");
                }
            }
        }
        //obwodka

        rysuj_odcinek(punkty_posrednie_x[0],punkty_posrednie_y[0],punkty_posrednie_x[1],punkty_posrednie_y[1], "bottom");
        rysuj_odcinek(punkty_posrednie_x[1],punkty_posrednie_y[1],punkty_posrednie_x[2],punkty_posrednie_y[2], "bottom");
        rysuj_odcinek(punkty_posrednie_x[2],punkty_posrednie_y[2],punkty_posrednie_x[0],punkty_posrednie_y[0], "bottom");

        repaint();

}


void MyWindow::czysc(QString side = ""){

    unsigned char *ptr3;
    ptr3 = img3->bits();
    int i,j;

    if (side != "bottom"){

        czysc_punkty();
        licznikpunktow = 0;
        licznikpunktow2 = 0;
        unsigned char *ptr, *ptr2;
        ptr = img->bits();
        ptr2 = img2->bits();

        for(i=0; i<wys; i++){
            for(j=0; j<=szer; j++){
                ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
                ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
            }
        }

        for(i=0; i<wys2; i++){
            for(j=0; j<=szer2; j++){
                ptr2[szer*4*i + 4*j] = 0; // Skladowa BLUE
                ptr2[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                ptr2[szer*4*i + 4*j + 2] = 0; // Skladowa RED
            }
        }

    }

    for(i=0; i<wys3; i++){
        for(j=0; j<=szer3; j++){
            ptr3[szer*4*i + 4*j] = 0; // Skladowa BLUE
            ptr3[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptr3[szer*4*i + 4*j + 2] = 0; // Skladowa RED
        }
    }

    update();
}



void MyWindow::on_resetButton_clicked()
{

    czysc();
    nazwa_pliku="";
    nazwa_pliku2="";
    czypunkt = 0;
    punkty_x.clear();
    punkty_y.clear();
    punkty_x2.clear();
    punkty_y2.clear();
    update();
    ui->runButton->setEnabled(false);
    ui->resetButton->setEnabled(false);

}

void MyWindow::mousePressEvent(QMouseEvent *event){

    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->position().x();
    int y = event->position().y();

    if (ktore_okno(x,y) == 1){
        x -= poczX;
        y -= poczY;
        if(licznikpunktow > 0){
            double dlugosc_odcinka;
            czypunkt = -1;
            for(int i = 0; i<punkty_x.size(); i++){
                dlugosc_odcinka = sqrt( pow(x - punkty_x[i],2) + pow(y - punkty_y[i],2) );
                if (dlugosc_odcinka<=5){
                    czypunkt = 1; ktorypunkt = i;
                }
            }
            if(czypunkt == -1) czypunkt = 0;
        }
        if(licznikpunktow < 3 && czypunkt == 0){
            dodaj_punkt(x,y,"left");
        }
    }
    else if (ktore_okno(x,y) == 2){
        x -= poczX2;
        y -= poczY2;
        if(licznikpunktow2 > 0){
            double dlugosc_odcinka;
            czypunkt = -1;
            for(int i = 0; i<punkty_x2.size(); i++){
                dlugosc_odcinka = sqrt( pow(x - punkty_x2[i],2) + pow(y - punkty_y2[i],2) );
                if (dlugosc_odcinka<=5){
                    czypunkt = 1; ktorypunkt = i;
                }
            }
            if(czypunkt == -1) czypunkt = 0;
        }
        if(licznikpunktow2 < 3 && czypunkt == 0){
            dodaj_punkt(x,y,"right");
        }
    }



}


void MyWindow::mouseMoveEvent(QMouseEvent *event){

    if(czypunkt){

        // Pobieramy wspolrzedne punktu klikniecia
        int x = event->position().x();
        int y = event->position().y();
        if (ktore_okno(x,y) == 1){
            x -= poczX;
            y -= poczY;
            if(x>=0 && y >= 0 && x < szer && y < wys){

                if (x < 0) x = 0;

                else if (x > szer) x = szer;

                if (y > wys) y = wys;

                else if (y<0) y = 0;

                aktualizuj_punkt(ktorypunkt, x, y, "left");

             }
            else czypunkt = 0;
        }
        else if (ktore_okno(x,y) == 2){
            x -= poczX2;
            y -= poczY2;
            if(x>=0 && y >= 0 && x < szer2 && y < wys2){

                if (x < 0) x = 0;
                else if (x > szer2) x = szer2;
                if (y > wys2)  y = wys2;
                else if (y<0)  y = 0;

                aktualizuj_punkt(ktorypunkt, x, y, "right");
             }
            else czypunkt = 0;
        }
        else czypunkt = 0;
    }

}

void MyWindow::mouseReleaseEvent(QMouseEvent *event){

    int x = event->position().x();
    int y = event->position().y();


        // Pobieramy wspolrzedne punktu klikniecia


        if (ktore_okno(x,y) == 1){
            // Sa to wspolrzedne wzgledem glownego okna,
            // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku
            x -= poczX;
            y -= poczY;

            if(x>=0 && y >= 0 && x < szer && y < wys){
                if(czypunkt){
                    if (x < 0) x = 0;
                    else if (x > szer) x = szer;
                    if (y > wys) y = wys;
                    else if (y<0) y = 0;
                    aktualizuj_punkt(ktorypunkt, x, y, "left");
                }
            }
        }
        else if (ktore_okno(x,y) == 2){
            // Sa to wspolrzedne wzgledem glownego okna,
            // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku
            x -= poczX2;
            y -= poczY2;
            if(x>=0 && y >= 0 && x < szer2 && y < wys2){
                if(czypunkt){
                    if (x < 0) x = 0;
                    else if (x > szer2) x = szer2;
                    if (y > wys2) y = wys2;
                    else if (y<0) y = 0;
                    aktualizuj_punkt(ktorypunkt, x, y, "right");
                }
            }
        }
        czypunkt = 0;
}


void MyWindow::run(){

    for(int n = 0; n<=frames; n++){
        end = QTime::currentTime().addMSecs(40 / speed);
        teksturuj(n);
        delay();
    }

}


void MyWindow::delay (){
    while(QTime::currentTime()<end)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}




void MyWindow::on_frameSlider_valueChanged(int value)
{
    frames = value;
    ui->frameValueLabel->setText(QString::number(value));

    double czas = (double)frames/(25.0 * speed);
    czas = round(czas * 100)/100;
    ui->speedValueLabel->setText(QString::number(czas)+"s");
}

void MyWindow::on_speedSlider_valueChanged(int value)
{
    speed = (double)round(value)/100;
    double czas = (double)frames/(25.0 * speed);
    czas = round(czas * 100)/100;
    ui->speedValueLabel->setText(QString::number(czas)+"s");

}


void MyWindow::on_runButton_clicked()
{
    ui->resetButton->setEnabled(false);
    ui->runButton->setEnabled(false);
    if(isrunable) run();
    ui->resetButton->setEnabled(true);
    ui->runButton->setEnabled(true);
}


void MyWindow::on_resetBottomButton_clicked()
{
    czysc("bottom");
}


void MyWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index !=0){

        if(index == 1){
            teksturowanie_tlo = 0;
            czysc("bottom");
        }
        if(index == 2){
            teksturowanie_tlo = 1;
            if(nazwa_pliku != "") {
                *img3 = *source_img;
                update();
            }
        }
        if(index == 3){
            teksturowanie_tlo = 2;
            if(nazwa_pliku2 != ""){
                *img3 = *source_img2;
                update();
            }
        }

    } else ui->comboBox->setCurrentIndex(1);
}



