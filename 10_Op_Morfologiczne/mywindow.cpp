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


//funkcja powoduje zamalowanie piksela danym kolorem.

void MyWindow::rysuj_piksel(int x, int y, QColor color){
    if(x<szer && y < wys && x >= 0 && y >= 0){
        unsigned char *ptr;
        ptr = img->bits();
        ptr[szer*4*y + 4*x] = color.blue();
        ptr[szer*4*y + 4*x + 1] = color.green();
        ptr[szer*4*y + 4*x + 2] = color.red();
    }
}


/*
                if (pobierz_kolor(kopia, x, y-i) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x, y+i) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x-i, y) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x+i, y) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x-i, y-i) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x-i, y+i) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x+i, y-i) == kol) sasiedzi++;
                if (pobierz_kolor(kopia, x+i, y+i) == kol) sasiedzi++;
                */

bool MyWindow::czy_sasiedzi(int x, int y, QColor kol){
    int sasiedzi = 0;

    if(x<szer && y < wys && x >= 0 && y >= 0){
        for(int mx = x-(int)(maska/2); mx <= x+(int)(maska/2); mx++){
            for(int my = y-(int)(maska/2); my <= y+(int)(maska/2); my++){
                if(!(mx == x && my == y)){
                    if (mx<szer && my < wys && mx >= 0 && my >= 0)
                        if(pobierz_kolor(kopia, mx, my) == kol) sasiedzi++;
                }
            }
        }
    }
    if (sasiedzi)
        return true;
    else
        return false;
}

void MyWindow::tobinary(QImage *colored){

    QColor temp = QColor::fromRgb(0,0,0,255);

    unsigned char *ptr;

    ptr = colored->bits();

    int i,j;


    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            temp = pobierz_kolor(colored, j,i);
             uchar v = temp.red() * 0.2126 + temp.green() * 0.7152 + temp.blue() * 0.0722 ;
            ptr[szer*4*i + 4*j]= v > 128 ? 255 : 0;
            ptr[szer*4*i + 4*j + 1] = v > 128 ? 255 : 0;
            ptr[szer*4*i + 4*j + 2] = v > 128 ? 255 : 0;
        }
    }

}

void MyWindow::dylatacja(){
    *kopia = *img;
    for(int x=0; x<szer; x++){
        for(int y=0; y<wys; y++){
            if (czy_sasiedzi(x,y, czarny)) rysuj_piksel(x,y, czarny);
            else rysuj_piksel(x,y, bialy);
        }
    }
}

void MyWindow::erozja(){
    *kopia = *img;
    for(int x=0; x<szer; x++){
        for(int y=0; y<wys; y++){
            if (czy_sasiedzi(x,y, bialy)) rysuj_piksel(x,y, bialy);
        }
    }
}



void MyWindow::on_loadButton_clicked()
{
   nazwa_pliku = QFileDialog::getOpenFileName(this,
    tr("Zaladuj Obraz"), "..\\10_Op_Morfologiczne", tr("Typy plikow graficznych (*.png *.jpg *.bmp)"));
   if (nazwa_pliku != ""){
       source_img = new QImage(nazwa_pliku);
       *source_img = source_img->scaled(szer,wys);
       *img = *source_img;
       tobinary(img);
       *source_img = *img;
       update();
   }
}


void MyWindow::on_resetButton_clicked()
{
    *img = *source_img;
    update();
}


void MyWindow::on_dylatacjaButton_clicked()
{
    if(nazwa_pliku != ""){
        dylatacja();
        update();
    }
}


void MyWindow::on_erozjaButton_clicked()
{
    if(nazwa_pliku != ""){
        erozja();
        update();
    }
}


void MyWindow::on_otwarcieButton_clicked()
{
    if(nazwa_pliku != ""){
        erozja();
        dylatacja();
        update();
    }
}


void MyWindow::on_domkniecieButton_clicked()
{
    if(nazwa_pliku != ""){
        dylatacja();
        erozja();
        update();
    }
}


void MyWindow::on_maskSpinBox_valueChanged(int arg1)
{
    maska = arg1;
}

