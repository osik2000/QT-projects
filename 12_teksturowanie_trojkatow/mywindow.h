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

#include <QFileDialog>


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
    QImage *img, *img2, *source_img, *imgPunkty, *imgPunkty2, *kopia;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    int szer, szer2;
    int wys, wys2;
    int poczX, poczX2;
    int poczY, poczY2;
    int wsp_x0, wsp_y0;
    int czypunkt = 0, ktorypunkt = 0;
    bool interpolacja = false;

    QString nazwa_pliku = "";

    // zmienna zapamietujaca kolor ktory bedzie narysowany
    QColor color = QColor::fromRgb(255,0,0,255);


    int licznikpunktow = 0; int licznikpunktow2 = 0;
    QVector<int> punkty_x, punkty_y;
    QVector<int> punkty_x2, punkty_y2;

    // Deklaracje funkcji

    void rysuj_piksel(int x, int y, QColor color, QString side);
    void rysuj_piksel_2(int x, int y, QColor color, QString side);
    void rysuj_pikselPunkty(int x, int y, QColor color, QString side);
    void rysuj_odcinek(double x0, double y0, double x1, double y1, QString side);
    void rysuj_punkt(int x, int y, QString side);
    void rysuj_punkty();
    void rysuj_trojkat(QString side);
    void dodaj_punkt(int x, int y, QString side);
    void aktualizuj_punkt(int i, int x, int y, QString side);
    void teksturuj();
    void czysc(QString side);
    void czysc_punkty();

    QColor pobierz_kolor(QImage *image, int x, int y, QString side);
    QColor pobierz_kolor_interpolacja(QImage *image, double x, double y, QString side);
    int ktore_okno(int x, int y);

    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent*);
    void on_exitButton_clicked();
    void on_loadButton_clicked();
    void on_resetButton_clicked();

    void on_interpolacjaCheckBox_stateChanged(int arg1);
};

#endif // MYWINDOW_H
