// Plik naglowkowy klasy MyWindow
// Obiekt tej klasy to glowne okno naszej aplikacji
// Szkielet tego pliku jest tworzony przez QtCreator
// Mozemy do niego dodac deklaracje wlasnych pol i metod

#ifndef MYWINDOW_H
#define MYWINDOW_H

// Dolaczamy plik naglowkowy klasy QMainWindow,
// Klasa QMainWindow posiada swoj wlasny layout.
// latwo mozna do niej dodac pasek menu, widzety dokujace,
// pasek narzedzi i pasek statusu. Na srodku okna
// wyswietlanego przez QMainWindow znajduje sie obszar,
// ktory mozna wypelnic roznymi widgetami.
#include <QMainWindow>

// QPainter to klasa umozliwiajaca niskopoziomowe rysowanie
// na elementach GUI
#include <QPainter>

// QImage to klasa pozwalajaca na niezalezna od sprzetu reprezentacje obrazu.
// Pozwala na bezposredni dostep do poszczegolnych pikseli,
// Bedziemy jej uzywali do tworzenia i przechowywania
// naszych rysunkow
#include <QImage>

// QMouseEvent to klasa obslugujaca zdarzenia zwiazane z myszka
// klikniecia, ruch myszka itp.
#include <QMouseEvent>

#include <QColorDialog>

#include <QStack>


namespace Ui {
    class MyWindow;
}

// MyWindow jest podklasa klasy QMainWindow.
class MyWindow : public QMainWindow
{
    // Q_OBJECT jest to makro, ktore musi sie znajdowac
    // we wszystkich klasach definiujacych wlasne sygnaly i sloty
    // W naszej klasie nie jest ono potrzebne,
    // ale QtCreator dodaje je automatycznie do kazdej klasy.
    Q_OBJECT

public:
    // Typowa deklaracja konstruktora w Qt.
    // Parametr "parent" okresla rodzica komponenetu.
    // W przypadku naszej klasy parametr ten wskazuje na null
    // co oznacza, ze komponenet nie ma rodzica, jest to
    // komponenet najwyzszego poziomu
    explicit MyWindow(QWidget *parent = 0);

    // Deklaracja destruktora
    ~MyWindow();

private:
    // QtCreator pozwala na tworzenie GUI za pomoca graficznego kreatora.
    // Skladniki interfejsu i ich wlasciwosci zapisane sa wowczas
    // w pliku XML "nazwa_klasy.ui"
    // Do poszczegolnych elementow GUI odwolujemy sie za pomoca zmiennej "ui"
    Ui::MyWindow *ui;

    // Pole przechowujace obrazek
    QImage *img, *img2, *imgKrzywe;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    int szer;
    int wys;
    int poczX;
    int poczY;

    // Pola przechowujace wspolrzedne punktow potrzebnych do rysowania
    int wsp_x0,wsp_y0;
    int wsp_x1,wsp_y1;
    int wsp_x2,wsp_y2;
    int wsp_x3,wsp_y3;

    // rozroznienie w jaki sposob program ma rysowac

    int czyolowek = 0 ;
    int czyodcinek = 1;
    int czyokrag = 0;
    int czydynamiczne = 1;
    int czyelipsa = 0;
    int czykrzywabeziera = 0;
    int czykrzywabsklejana = 0;
    int czyfloodfill = 0;
    int czyscanline = 0;
    int czyrysowanie = 1;
    int czyprzemieszczanie = 0;
    int czyusuwanie = 0;
    int czymogemalowac = 1;
    int czyolowekwewnatrz = 1;
    int gruboscpedzla = 1;

    int ilewierzcholkow = 3;

    int licznikbeziera = 0;
    int temp_licznikbeziera = 0;
    int licznikpunktow = 0;
    int ktorypunkt;
    int czypunkt = -1;

    // tablica zmiennych do okreslania koloru rysowania

    QColor color = QColor::fromRgb(255,0,0,255);

    // wektor do przechowywania punktow potrzebnych do rysowania krzywej lub wielokatow

    QVector<int> punkty_x, punkty_y;
    QVector<int> punkty_sl_x, punkty_sl_y;



    // Deklaracje funkcji
    void czysc();
    void czyscKrzywe();
    void rysuj_odcinek(double x0, double y0, double x1, double y1);
    void rysuj_okrag(double x0, double y0, double x1, double y1);
    void rysuj_elipsa(double x0, double y0, double x1, double y1);
    void rysuj_krzywa_beziera(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    void rysuj_krzywa_b_sklejana(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    void rysuj_piksel(int x, int y);
    void rysuj_pikselKrzywe(int x, int y);
    void rysuj_punkt(int x, int y);
    void ustaw_kolor(QColor color);
    QColor pobierz_kolor(int x, int y);
    void zamaluj_piksel(int x, int y, QColor kolorek);
    void dodaj_punkt(int x, int y);
    void aktualizuj_punkt(int i, int x, int y);
    void aktualizuj_krzywa_beziera();
    void aktualizuj_krzywa_b_sklejana();
    void wypelnij_floodfill(int x,int y, QColor stary, QColor nowy);
    void wypelnij_scanline();

    int  wylicz_iks_przeciecia(int y, int x0, int y0, int x1, int y1);



    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:

    void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);

    void on_odcinekButton_clicked();
    void on_okragButton_clicked();
    void on_checkBox_stateChanged();
    void on_checkBox_2_stateChanged(int arg1);
    void on_elipsaButton_clicked();
    void on_krzywaBezieraButton_clicked();
    void on_rysowanieButton_clicked();
    void on_przemieszczenieButton_clicked();
    void on_usuniecieButton_clicked();
    void on_IloscWierzcholkowSpinBox_valueChanged(int arg1);
    void on_krzywaBsklejanaButton_clicked();
    void on_kolorButton_clicked();
    void on_floodFillButton_clicked();
    void on_scanLineButton_clicked();
    void on_olowekButton_clicked();
    void on_gruboscSpinBox_valueChanged(int arg1);
};

#endif // MYWINDOW_H
