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
    img2 = new QImage(szer,wys,QImage::Format_RGB32);
    imgKrzywe = new QImage(szer,wys,QImage::Format_ARGB32_Premultiplied);

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
    p.drawImage(poczX,poczY,*imgKrzywe);
}


// Funkcja (slot) wywolywana po nacisnieciu przycisku "Czysc" (cleanButton)
void MyWindow::on_cleanButton_clicked(){
    // Funkcja czysci (zamalowuje na bialo) obszar rysowania
    // definicja znajduje sie ponizej
    czysc();
    czyscKrzywe();

    punkty_sl_x.clear();
    punkty_sl_y.clear();

    for(int i = 0; i<licznikpunktow; i++){
        punkty_x.pop_back();
        punkty_y.pop_back();
    }
    licznikbeziera = 0;
    licznikpunktow = 0;

    // Funkcja "update()" powoduje ponowne "namalowanie" calego komponentu
    // Wywoluje funkcje "paintEvent"
    update();
}

// Funkcja powoduje wyczyszczenie (zamalowanie na bialo)
// obszaru rysowania
void MyWindow::czysc(){
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;

    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();

    int i,j;

    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<=szer; j++)
        {
            ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
        }
    }
}

    void MyWindow::czyscKrzywe(){
        // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
        unsigned char *ptrKrzywe;

        // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
        ptrKrzywe = imgKrzywe->bits();

        int i,j;

        // Przechodzimy po wszystkich wierszach obrazu
        for(i=0; i<wys; i++)
        {
            // Przechodzimy po pikselach danego wiersza
            // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
            for(j=0; j<=szer; j++)
            {
                ptrKrzywe[szer*4*i + 4*j] = 0; // Skladowa BLUE
                ptrKrzywe[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                ptrKrzywe[szer*4*i + 4*j + 2] = 0; // Skladowa RED
                ptrKrzywe[szer*4*i + 4*j + 3] = 0; // Skladowa ALPHA
            }
        }
}

void MyWindow::ustaw_kolor(QColor color_in){
    color = color_in;
}

QColor MyWindow::pobierz_kolor(int x, int y){
    if(x<szer && y < wys && x > 0 && y > 0){
        QColor pobrany;
        unsigned char *ptr;
        ptr = img->bits();
        int r,g,b;
        r = ptr[szer*4*y + 4*x + 2];
        g = ptr[szer*4*y + 4*x + 1];
        b = ptr[szer*4*y + 4*x];
        pobrany = QColor::fromRgb(r,g,b,255);
        return pobrany;
    }
    else return QColor::fromRgb(0,0,0,255);
}

void MyWindow::zamaluj_piksel(int x, int y, QColor kolorek){
    if(x<szer && y < wys && x >= 0 && y >= 0){
        unsigned char *ptr;
        ptr = img->bits();
        ptr[szer*4*y + 4*x + 2] = kolorek.red();
        ptr[szer*4*y + 4*x + 1] = kolorek.green();
        ptr[szer*4*y + 4*x] = kolorek.blue();
    }
}

void MyWindow::rysuj_piksel(int x, int y){
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

void MyWindow::rysuj_pikselKrzywe(int x, int y){
    if(x<szer && y < wys && x >= 0 && y >= 0 && gruboscpedzla == 1){
        unsigned char *ptrKrzywe;
        ptrKrzywe = imgKrzywe->bits();

        ptrKrzywe[szer*4*y + 4*x] = color.blue();
        ptrKrzywe[szer*4*y + 4*x + 1] = color.green();
        ptrKrzywe[szer*4*y + 4*x + 2] = color.red();
        ptrKrzywe[szer*4*y + 4*x + 3] = 255;
    }
    else if (gruboscpedzla>1){
        unsigned char *ptrKrzywe;
        ptrKrzywe = imgKrzywe->bits();

        for (int iy = gruboscpedzla; iy>=0; iy--)
            for (int ix = gruboscpedzla; ix>=0; ix--){
                if(x+ix-(gruboscpedzla/2)<szer && y+iy-(gruboscpedzla/2) < wys && x+ix-(gruboscpedzla/2) > 0 && y+iy-(gruboscpedzla/2) > 0){
                    ptrKrzywe[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2))] = color.blue();
                    ptrKrzywe[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2)) + 1] = color.green();
                    ptrKrzywe[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2)) + 2] = color.red();
                    ptrKrzywe[szer*4*(y+iy-(gruboscpedzla/2)) + 4*(x+ix-(gruboscpedzla/2)) + 3] = 255;
                }
        }

    }
}

void MyWindow::rysuj_odcinek(double x0, double y0, double x1, double y1){

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
            rysuj_piksel(x,y);
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
                rysuj_piksel(x,y);
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
                rysuj_piksel(x,y);
            }
        }
    }
    // Odswiezamy komponent
    update();
}

void MyWindow::rysuj_okrag(double x0, double y0, double x1, double y1){

    double y,r;

    r = sqrt(pow((x1-x0),2)+pow(y1-y0,2));

    for (int x=0; x<=x0+((sqrt(2)/2)*r); x++){
        y = sqrt(r*r - x*x);
        rysuj_piksel(x0+x,round(y)+y0);
        rysuj_piksel(x0+x,-round(y)+y0);
        rysuj_piksel(x0-x,round(y)+y0);
        rysuj_piksel(x0-x,-round(y)+y0);
        rysuj_piksel(round(y)+x0,y0+x);
        rysuj_piksel(-round(y)+x0,y0+x);
        rysuj_piksel(round(y)+x0,y0-x);
        rysuj_piksel(-round(y)+x0,y0-x);
    }
    update(); // Odswiezamy komponent
}

void MyWindow::rysuj_elipsa(double x0, double y0, double x1, double y1){
        double x, y, temp_x, temp_y;
        double iterator = 2*M_PI / ilewierzcholkow;

        double a = sqrt(pow((x1-x0),2));
        double b = sqrt(pow((y1-y0),2));

        for (double t=0; t<=2*M_PI+iterator; t+=iterator){
            x = x0 + round(a * cos(t));
            y = y0 + round(b * sin(t));

            if (t == 0){
                temp_x = x;
                temp_y = y;
            }
            rysuj_odcinek(temp_x,temp_y,x,y);
            temp_x = x;
            temp_y = y;
        }
        update(); // Odswiezamy komponent
}

void MyWindow::dodaj_punkt(int x, int y){
    licznikpunktow++;
    punkty_x.push_back(x);
    punkty_y.push_back(y);
    rysuj_punkt(x,y);
}


void MyWindow::aktualizuj_punkt(int i, int x, int y){
    punkty_x.replace(i, x);
    punkty_y.replace(i, y);
}

void MyWindow::aktualizuj_krzywa_beziera(){

    czyscKrzywe();

    for (int i=0; i<licznikpunktow; i++){
        QColor temp = color;
        if(i>licznikpunktow-2) ustaw_kolor(QColor::fromRgb(0,255,0,255));
        rysuj_punkt(punkty_x[i],punkty_y[i]);
        ustaw_kolor(temp);

        if(i==0){
        wsp_x0 = punkty_x[i];
        wsp_y0 = punkty_y[i];
        }
        else if(i==1){
        wsp_x1 = punkty_x[i];
        wsp_y1 = punkty_y[i];
        }
        else if(i==2){
        wsp_x2 = punkty_x[i];
        wsp_y2 = punkty_y[i];
        }
        else if(i==3){
        wsp_x3 = punkty_x[i];
        wsp_y3 = punkty_y[i];
        rysuj_krzywa_beziera(wsp_x0, wsp_y0, wsp_x1, wsp_y1, wsp_x2, wsp_y2, wsp_x3, wsp_y3);
        wsp_x0 = wsp_x3;
        wsp_y0 = wsp_y3;
        }
        else if((i-1)%3 == 0){
            wsp_x1 = punkty_x[i];
            wsp_y1 = punkty_y[i];
            licznikbeziera = 3;
        }
        else if((i-1)%3 == 1){
            wsp_x2 = punkty_x[i];
            wsp_y2 = punkty_y[i];
            licznikbeziera = 2;
        }
        else if((i-1)%3 == 2){
            wsp_x3 = punkty_x[i];
            wsp_y3 = punkty_y[i];
            rysuj_krzywa_beziera(wsp_x0, wsp_y0, wsp_x1, wsp_y1, wsp_x2, wsp_y2, wsp_x3, wsp_y3);
            wsp_x0 = wsp_x3;
            wsp_y0 = wsp_y3;
            licznikbeziera = 1;
        }
    }
}

void MyWindow::rysuj_punkt(int x, int y){

    for (int ix = x-2; ix<=x+2; ix++){
        for(int iy = y-2; iy<=y+2; iy++){
            rysuj_pikselKrzywe(ix, iy);
        }
    }
    update();
}

void MyWindow::rysuj_krzywa_beziera(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3){
    double bx,by;
    for (double t = 0; t<=1; t+=0.001){
        bx = (((1-t)*(1-t)*(1-t)*x0) + 3*((1-t)*(1-t)*t*x1) + 3*((1-t)*t*t*x2) + (t*t*t*x3));
        by = (((1-t)*(1-t)*(1-t)*y0) + 3*((1-t)*(1-t)*t*y1) + 3*((1-t)*t*t*y2) + (t*t*t*y3));
        rysuj_pikselKrzywe(bx, by);
    }
}

void MyWindow::rysuj_krzywa_b_sklejana(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3){
    double bx,by;
    for (double t = 0; t<=1; t+=0.001){
        bx = (((-pow(t,3)+3*pow(t,2)-3*t+1)/6)*x0) + (((3*pow(t,3)-6*pow(t,2)+4)/6)*x1) + (((-3*pow(t,3)+3*pow(t,2)+3*t+1)/6)*x2) + ((pow(t,3)/6)*x3);
        by = (((-pow(t,3)+3*pow(t,2)-3*t+1)/6)*y0) + (((3*pow(t,3)-6*pow(t,2)+4)/6)*y1) + (((-3*pow(t,3)+3*pow(t,2)+3*t+1)/6)*y2) + ((pow(t,3)/6)*y3);
        rysuj_pikselKrzywe(bx, by);
    }
}

void MyWindow::aktualizuj_krzywa_b_sklejana(){

    czyscKrzywe();

        for(int i=0; i<licznikpunktow; i++){
            QColor temp = color;
            if(i>licznikpunktow-2) ustaw_kolor(QColor::fromRgb(0,255,0,255));
            rysuj_punkt(punkty_x[i],punkty_y[i]);
            ustaw_kolor(temp);
        }

    if(licznikpunktow >= 4){

        for (int i=3; i<licznikpunktow; i++){

            wsp_x0 = punkty_x[i-3];
            wsp_y0 = punkty_y[i-3];
            wsp_x1 = punkty_x[i-2];
            wsp_y1 = punkty_y[i-2];
            wsp_x2 = punkty_x[i-1];
            wsp_y2 = punkty_y[i-1];
            wsp_x3 = punkty_x[i];
            wsp_y3 = punkty_y[i];
            rysuj_krzywa_b_sklejana(wsp_x0, wsp_y0, wsp_x1, wsp_y1, wsp_x2, wsp_y2, wsp_x3, wsp_y3);

        }
    }
}


void MyWindow::wypelnij_floodfill(int x0,int y0, QColor stary, QColor nowy){
    QStack<int> Qx, Qy;
    int x,y;

    Qx.push(x0);
    Qy.push(y0);

    if ( stary == nowy){
        czymogemalowac = 1;
        return;
    }

    while (!Qx.isEmpty() && !Qy.isEmpty()){
        x = Qx.pop();
        y = Qy.pop();
        if (pobierz_kolor(x,y) == stary){
            int w = x;
            int e = x;
            while (pobierz_kolor(w,y) == stary && w<szer && w>0) w--;
            while (pobierz_kolor(e,y) == stary && e<szer && e>0) e++;
            for (int i = w+1; i<e; i++) if(i<szer && y < wys && i >= 0 && y >= 0) zamaluj_piksel(i,y, nowy);
            for (int i = w+1; i<e; i++){
                if(i<szer && y < wys && i >= 0 && y >= 0){
                    if ((y+1 < wys) && pobierz_kolor(i,y+1) == stary){
                        Qx.push(i);
                        Qy.push(y+1);
                    }
                    if ((y-1 >= 0) && pobierz_kolor(i,y-1) == stary){
                        Qx.push(i);
                        Qy.push(y-1);
                    }
                }
            }
        }
    }
    update();
    czymogemalowac = 1;
}

void MyWindow::wypelnij_scanline(){

    int y_max = punkty_sl_y[0];
    int y_min = punkty_sl_y[0];
    bool czymalowac = false;

    QVector<int> iksy_przeciecia;

     if (punkty_sl_x.size()>0){ // znajdowanie najwyzszego i najnizszego wierzcholka
        for(int i = 1; i<punkty_sl_x.size(); i++){
            if(punkty_sl_y[i] > y_max)
                y_max = punkty_sl_y[i];
            if(punkty_sl_y[i] < y_min)
                y_min = punkty_sl_y[i];
        }
     }

     /*czy sie przrecina ->

     w czy sie przecina sprawdzam czy wierzcholek

             ->

             czy to jest ekstremum(2 razy wtedy)
             else 1raz

     else 1raz


     ekstr- wyzej niz -1 i +1 lub nizej
     */

    if (punkty_sl_x.size()>1){
        for(int iy = y_min; iy <= y_max; iy++){
                for(int i = 1; i<punkty_sl_x.size(); i++){
                        if((iy < punkty_sl_y[i] && iy >= punkty_sl_y[i-1]) || (iy > punkty_sl_y[i] && iy <= punkty_sl_y[i-1])){



                            if((iy == punkty_sl_y[i])){

                                int before_i, after_i;

                                if(i==0) before_i = punkty_sl_x.size()-1;
                                else before_i = i-1;
                                if(i==punkty_sl_x.size()-1) after_i = 0;
                                else after_i = i+1;

                                if((iy > punkty_sl_y[before_i] && iy > punkty_sl_y[after_i]) || (iy < punkty_sl_y[before_i] && iy < punkty_sl_y[after_i])){ //czyekstr
                                    int iks_2 = wylicz_iks_przeciecia(iy, punkty_sl_x[i], punkty_sl_y[i], punkty_sl_x[before_i], punkty_sl_y[before_i]);
                                    iksy_przeciecia.push_back(iks_2);
                                    iksy_przeciecia.push_back(iks_2);
                                }

                            }
                            else{
                                int iks = wylicz_iks_przeciecia(iy, punkty_sl_x[i], punkty_sl_y[i], punkty_sl_x[i-1], punkty_sl_y[i-1]);
                                iksy_przeciecia.push_back(iks);
                            }
                        }

                }

                std::sort(iksy_przeciecia.begin(), iksy_przeciecia.end());

                for(int i = 1; i< iksy_przeciecia.size(); i+=2){

                    rysuj_odcinek(iksy_przeciecia[i], iy, iksy_przeciecia[i-1], iy);
                }

                iksy_przeciecia.clear();

        }

    }
    punkty_sl_x.clear();
    punkty_sl_y.clear();
}


int MyWindow::wylicz_iks_przeciecia(int y, int x0, int y0, int x1, int y1){
    return (x0 + (y-y0)*((double)(x1-x0)/(double)(y1-y0)));
}


// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{

    //kopia planszy przy rysowaniu dynamicznym
    if (czydynamiczne) *img2=*img;

    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->position().x();
    int y = event->position().y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

    x -= poczX;
    y -= poczY;

    if(czykrzywabeziera == 0){
        wsp_x0 = x;
        wsp_y0 = y;
    }

    if (czykrzywabeziera) {
        if(czyrysowanie && x>=0 && y >= 0 && x < szer && y < wys){
            licznikbeziera++;
            dodaj_punkt(x,y);

            if(licznikbeziera == 1){
                wsp_x0 = x;
                wsp_y0 = y;
            }
            if (licznikbeziera == 2) {
                wsp_x1 = x;
                wsp_y1 = y;
            }
            if (licznikbeziera == 3) {
                    wsp_x2 = x;
                    wsp_y2 = y;
            }
            if (licznikbeziera == 4) {
                wsp_x3 = x;
                wsp_y3 = y;
                aktualizuj_krzywa_beziera();
                wsp_x0 = wsp_x3;
                wsp_y0 = wsp_y3;
             licznikbeziera = 1;

            }
            aktualizuj_krzywa_beziera();
        }
        else if(czyprzemieszczanie || czyusuwanie){
            double dlugosc_odcinka;
            czypunkt = -1;
            for(int i = 0; i<licznikpunktow; i++){
                dlugosc_odcinka = sqrt( pow(x - punkty_x[i],2) + pow(y - punkty_y[i],2) );
                if (dlugosc_odcinka<1){
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<2) {
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<3) {
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<4) {
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<5) {
                    czypunkt =1; ktorypunkt = i;
                }
            }
            if(czypunkt == -1) czypunkt = 0;
        }
    }

    else if(czykrzywabsklejana){

        if(czyrysowanie && x>=0 && y >= 0 && x < szer && y < wys){
            dodaj_punkt(x,y);

            aktualizuj_krzywa_b_sklejana();
        }
        else if(czyprzemieszczanie || czyusuwanie){
            double dlugosc_odcinka;
            czypunkt = -1;
            for(int i = 0; i<licznikpunktow; i++){
                dlugosc_odcinka = sqrt( pow(x - punkty_x[i],2) + pow(y - punkty_y[i],2) );
                if (dlugosc_odcinka<1){
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<2) {
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<3) {
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<4) {
                    czypunkt =1; ktorypunkt = i;
                }
                else if (dlugosc_odcinka<5) {
                    czypunkt =1; ktorypunkt = i;
                }
            }
            if(czypunkt == -1) czypunkt = 0;
        }
    }

    else if(czyscanline){


        if(x>=0 && y >= 0 && x < szer && y < wys){
            punkty_sl_x.push_back(x);
            punkty_sl_y.push_back(y);
        }

        int wielokat = punkty_sl_x.size();
        czypunkt = 0;

            if(wielokat >= 3){

                double dlugosc_odcinka = sqrt( pow(x - punkty_sl_x[0],2) + pow(y - punkty_sl_y[0],2) );
                if (dlugosc_odcinka<5){
                    czypunkt =1;
                    punkty_sl_x.replace(wielokat-1,punkty_sl_x[0]);
                    punkty_sl_y.replace(wielokat-1,punkty_sl_y[0]);
                }
            }
            if(wielokat>=2){
                rysuj_odcinek(punkty_sl_x[wielokat-2],punkty_sl_y[wielokat-2],punkty_sl_x[wielokat-1],punkty_sl_y[wielokat-1]);
            }

            if(czypunkt) wypelnij_scanline();
    }

    else if(czyfloodfill && czymogemalowac){
        czymogemalowac = 0;
        wypelnij_floodfill(x, y, pobierz_kolor(x,y), color);
    }

    else if (czyolowek){
        if(x>=0 && y >= 0 && x < szer && y < wys)
            czyolowekwewnatrz = 1;
        else
            czyolowekwewnatrz = 0;
        wsp_x0 = x;
        wsp_y0 = y;
        rysuj_piksel(x,y);
        update();
    }
}


void MyWindow::mouseMoveEvent(QMouseEvent *event){

    if(wsp_x0>=0 && wsp_y0 >= 0 && wsp_x0 < szer && wsp_y0 < wys){

        // Pobieramy wspolrzedne punktu klikniecia
        int x = event->position().x();
        int y = event->position().y();

        // Sa to wspolrzedne wzgledem glownego okna,
        // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

        x -= poczX;
        y -= poczY;
        wsp_x1 = x;
        wsp_y1 = y;
        if (czydynamiczne && !czyfloodfill && !czyolowek){
            *img=*img2;
            if(czyodcinek)
                rysuj_odcinek(wsp_x0,wsp_y0,wsp_x1,wsp_y1);
            else if(czyokrag)
                rysuj_okrag(wsp_x0,wsp_y0,wsp_x1,wsp_y1);
            else if(czyelipsa){
                rysuj_elipsa(wsp_x0,wsp_y0,wsp_x1,wsp_y1);
            }
            else if((czykrzywabeziera || czykrzywabsklejana) && czypunkt && czyprzemieszczanie){
                if (x < 0)
                    x = 0;
                else if (x > szer)
                    x = szer;
                if (y > wys)
                    y = wys;
                else if (y<0)
                    y = 0;
                aktualizuj_punkt(ktorypunkt, x, y);
                if (czykrzywabeziera)
                    aktualizuj_krzywa_beziera();
                else if (czykrzywabsklejana)
                    aktualizuj_krzywa_b_sklejana();
            }
        }
    }
    if (czyolowek && czyolowekwewnatrz){
        int x = event->position().x();
        int y = event->position().y();
        x -= poczX;
        y -= poczY;
      rysuj_odcinek(wsp_x0,wsp_y0,x,y);
      wsp_x0 = x;
      wsp_y0 = y;
    }
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event){

    if(wsp_x0>=0 && wsp_y0 >= 0 && wsp_x0 < szer && wsp_y0 < wys){

        // Pobieramy wspolrzedne punktu klikniecia
        int x = event->position().x();
        int y = event->position().y();

        // Sa to wspolrzedne wzgledem glownego okna,
        // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku
        x -= poczX;
        y -= poczY;
        wsp_x1 = x;
        wsp_y1 = y;

        if(czyodcinek)
            rysuj_odcinek(wsp_x0,wsp_y0,wsp_x1,wsp_y1);
        else if(czyokrag)
            rysuj_okrag(wsp_x0,wsp_y0,wsp_x1,wsp_y1);
        else if(czyelipsa)
            rysuj_elipsa(wsp_x0,wsp_y0,wsp_x1,wsp_y1);
        else if(czykrzywabeziera || czykrzywabsklejana){
            if(czyprzemieszczanie && czypunkt){
                if (x < 0) x = 0;
                else if (x > szer) x = szer;
                if (y > wys) y = wys;
                else if (y<0) y = 0;
                aktualizuj_punkt(ktorypunkt, x, y);
                if (czykrzywabeziera) aktualizuj_krzywa_beziera();
                else if (czykrzywabsklejana) aktualizuj_krzywa_b_sklejana();
            }
            else if(czyusuwanie && czypunkt){
                double dlugosc_odcinka;
                czypunkt = -1;
                dlugosc_odcinka = sqrt( pow(x - punkty_x[ktorypunkt],2) + pow(y - punkty_y[ktorypunkt],2) );
                if (dlugosc_odcinka <= 5) {
                    punkty_x.takeAt(ktorypunkt);
                    punkty_y.takeAt(ktorypunkt);
                    licznikpunktow--;
                    if(licznikpunktow == 0){
                        czyscKrzywe();
                        update();
                    }
                    else {
                        if (czykrzywabeziera) aktualizuj_krzywa_beziera();
                        else if (czykrzywabsklejana) aktualizuj_krzywa_b_sklejana();
                    }
                    ktorypunkt = -1;
                }
                if(czypunkt == -1)
                    czypunkt = 0;
            }
        }
    }

}

void MyWindow::on_kolorButton_clicked()
{
    QColor color_in = QColorDialog::getColor(Qt::white,this);
    if (color_in.isValid()){
    color = color_in;
    }
}

void MyWindow::on_olowekButton_clicked()
{
    czyolowek = 1;
    czyodcinek = 0;
    czyokrag = 0;
    czyelipsa = 0;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 0;
    czyfloodfill = 0;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(false);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setEnabled(true);
}

void MyWindow::on_odcinekButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 1;
    czyokrag = 0;
    czyelipsa = 0;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 0;
    czyfloodfill = 0;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(false);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setEnabled(true);
}

void MyWindow::on_okragButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 0;
    czyokrag = 1;
    czyelipsa = 0;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 0;
    czyfloodfill = 0;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(false);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setEnabled(true);
}

void MyWindow::on_elipsaButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 0;
    czyokrag = 0;
    czyelipsa = 1;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 0;
    czyfloodfill = 0;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(false);
    ui->IloscWierzcholkowSpinBox->setEnabled(true);
    ui->IloscWierzcholkow->setEnabled(true);
    ui->checkBox->setEnabled(true);
}

void MyWindow::on_krzywaBezieraButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 0;
    czyokrag = 0;
    czyelipsa = 0;
    czykrzywabeziera = 1;
    czykrzywabsklejana = 0;
    czyfloodfill = 0;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(true);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setEnabled(true);
}

void MyWindow::on_krzywaBsklejanaButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 0;
    czyokrag = 0;
    czyelipsa = 0;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 1;
    czyfloodfill = 0;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(true);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setEnabled(true);
}

void MyWindow::on_floodFillButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 0;
    czyokrag = 0;
    czyelipsa = 0;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 0;
    czyfloodfill = 1;
    czyscanline = 0;
    ui->groupBox_3->setEnabled(false);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->checkBox->setChecked(false);
}

void MyWindow::on_scanLineButton_clicked()
{
    czyolowek = 0;
    czyodcinek = 0;
    czyokrag = 0;
    czyelipsa = 0;
    czykrzywabeziera = 0;
    czykrzywabsklejana = 0;
    czyfloodfill = 0;
    czyscanline = 1;
    ui->groupBox_3->setEnabled(false);
    ui->IloscWierzcholkowSpinBox->setEnabled(false);
    ui->IloscWierzcholkow->setEnabled(false);
    ui->checkBox->setChecked(false);
    ui->checkBox->setEnabled(false);
}

void MyWindow::on_checkBox_stateChanged()
{
    czydynamiczne = abs(czydynamiczne-1);
}

void MyWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1){
        ui->olowekButton->setEnabled(true);
        ui->gruboscSpinBox->setEnabled(true);
    }
    else{
        ui->olowekButton->setEnabled(false);
        ui->gruboscSpinBox->setEnabled(false);
        ui->gruboscSpinBox->setValue(1);
    }
}

void MyWindow::on_rysowanieButton_clicked()
{
    czyrysowanie = 1;
    czyprzemieszczanie = 0;
    czyusuwanie = 0;
}

void MyWindow::on_przemieszczenieButton_clicked()
{
    czyrysowanie = 0;
    czyprzemieszczanie = 1;
    czyusuwanie = 0;
    temp_licznikbeziera = licznikbeziera;
}

void MyWindow::on_usuniecieButton_clicked()
{
   // if (czyrysowanie == 1) temp_licznikbeziera = licznikbeziera;
    czyrysowanie = 0;
    czyprzemieszczanie = 0;
    czyusuwanie = 1;
}

void MyWindow::on_IloscWierzcholkowSpinBox_valueChanged(int arg1)
{
    ilewierzcholkow = arg1;
}

void MyWindow::on_gruboscSpinBox_valueChanged(int arg1)
{
    gruboscpedzla = arg1;
}
