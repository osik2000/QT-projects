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
    QImage *img, *source_img, *kopia;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    int szer;
    int wys;
    int poczX;
    int poczY;

    double parameter_translatedX = 0;
    double parameter_translatedY = 0;
    double parameter_rotated = 0;
    double parameter_scaledX = 1;
    double parameter_scaledY = 1;
    double parameter_shearedX = 0;
    double parameter_shearedY = 0;

    double matrix_translation[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    double matrix_rotation[3][3]    = {{0,0,0},{0,0,0},{0,0,0}};
    double matrix_scaling[3][3]     = {{1,0,0},{0,1,0},{0,0,1}};
    double matrix_shearing[3][3]    = {{1,0,0},{0,1,0},{0,0,1}};
    double matrix_temp[3][3]        = {{0,0,0},{0,0,0},{0,0,0}};
    double matrix_result[3][3]      = {{1,0,0},{0,1,0},{0,0,1}};


    bool proporcje = false;

    QString nazwa_pliku = "";

    // zmienna zapamietujaca kolor ktory bedzie narysowany
    QColor color = QColor::fromRgb(255,0,0,255);
    QColor czarny = QColor::fromRgb(0,0,0,255);



    // Deklaracje funkcji

    void czysc();
    void rysuj_piksel(int x, int y, QColor color, QImage *image);
    void tobinary(QImage *colored);
    QColor pobierz_kolor(QImage *image, int x, int y);
    QColor pobierz_kolor_interpolacja(QImage *image, double dx, double dy);
    bool czy_sasiedzi(int x, int y, QColor kol);
    void zeruj_macierze();
    void pomnoz_macierz(QString typ);
    void translation();
    void rotation();
    void scaling();
    void shearing();
    void transformation();


    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:

    void on_exitButton_clicked();

    void paintEvent(QPaintEvent*);

    void on_loadButton_clicked();
    void on_resetButton_clicked();

    void on_translationXSlider_valueChanged(int value);
    void on_translationYSlider_valueChanged(int value);
    void on_rotationSlider_valueChanged(int value);
    void on_scalingXSlider_valueChanged(int value);
    void on_scalingYSlider_valueChanged(int value);
    void on_shearingXSlider_valueChanged(int value);
    void on_shearingYSlider_valueChanged(int value);
    void on_scalingCheckBox_toggled(bool checked);
    void on_scalingAllSlider_valueChanged(int value);
};

#endif // MYWINDOW_H
