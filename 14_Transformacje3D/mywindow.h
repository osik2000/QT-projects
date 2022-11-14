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
    QImage *img;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    int szer;
    int wys;
    int poczX;
    int poczY;

    int gruboscpedzla = 2;

    double parameter_view = 500;

    double parameter_translatedX = 0;
    double parameter_translatedY = 0;
    double parameter_translatedZ = 0;
    double parameter_rotatedX = 0;
    double parameter_rotatedY = 0;
    double parameter_rotatedZ = 0;
    double parameter_scaledX = 1;
    double parameter_scaledY = 1;
    double parameter_scaledZ = 1;
    double parameter_shearingX = 0;
    double parameter_shearingY = 0;
    double parameter_shearingZ = 0;

    double matrix_translation[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    double matrix_rotationX[4][4]   = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    double matrix_rotationY[4][4]   = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    double matrix_rotationZ[4][4]   = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    double matrix_shearingX[4][4]   = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    double matrix_shearingY[4][4]   = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    double matrix_shearingZ[4][4]   = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    double matrix_scaling[4][4]     = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    double matrix_result[4][4]      = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    double matrix_temp[4][4]        = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    double matrix_p1[4] = {-60,-60,-60,1};
    double matrix_p2[4] = {60,-60,-60,1};
    double matrix_p3[4] = {60,-60,60,1};
    double matrix_p4[4] = {-60,-60,60,1};
    double matrix_p5[4] = {-60,60,-60,1};
    double matrix_p6[4] = {60,60,-60,1};
    double matrix_p7[4] = {60,60,60,1};
    double matrix_p8[4] = {-60,60,60,1};

    double  matrix_p1_3D_result[4] = {0,0,0,0};
    double  matrix_p2_3D_result[4] = {0,0,0,0};
    double  matrix_p3_3D_result[4] = {0,0,0,0};
    double  matrix_p4_3D_result[4] = {0,0,0,0};
    double  matrix_p5_3D_result[4] = {0,0,0,0};
    double  matrix_p6_3D_result[4] = {0,0,0,0};
    double  matrix_p7_3D_result[4] = {0,0,0,0};
    double  matrix_p8_3D_result[4] = {0,0,0,0};

   int  matrix_p1_2D_result[2] = {0,0};
   int  matrix_p2_2D_result[2] = {0,0};
   int  matrix_p3_2D_result[2] = {0,0};
   int  matrix_p4_2D_result[2] = {0,0};
   int  matrix_p5_2D_result[2] = {0,0};
   int  matrix_p6_2D_result[2] = {0,0};
   int  matrix_p7_2D_result[2] = {0,0};
   int  matrix_p8_2D_result[2] = {0,0};

    bool proporcje = false;

    // zmienna zapamietujaca kolor ktory bedzie narysowany
    QColor color = QColor::fromRgb(255,0,0,255);
    QColor colorA = QColor::fromRgb(128,0,0,255);
    QColor colorB = QColor::fromRgb(0,128,0,255);
    QColor colorC = QColor::fromRgb(0,0,128,255);
    QColor colorW = QColor::fromRgb(255,0,0,255);


    // Deklaracje funkcji

    void czysc();
    void rysuj_piksel(int x, int y, QColor color, QImage *image);
    void rysuj_wierzcholek(int x, int y, QColor color);
    void rysuj_odcinek(double x0, double y0, double x1, double y1, QColor color);
    QColor pobierz_kolor(QImage *image, int x, int y);
    void zeruj_macierze();
    void pomnoz_macierz(QString typ);
    void translation();
    void rotationX();
    void rotationY();
    void rotationZ();
    void shearingX();
    void shearingY();
    void shearingZ();
    void scaling();
    void transformation();
    void rzutuj_punkty();
    void rysuj_krawedzie();


    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:

    void on_exitButton_clicked();

    void paintEvent(QPaintEvent*);

    void on_resetButton_clicked();

    void on_translationXSlider_valueChanged(int value);
    void on_translationYSlider_valueChanged(int value);
    void on_scalingXSlider_valueChanged(int value);
    void on_scalingYSlider_valueChanged(int value);
    void on_scalingCheckBox_toggled(bool checked);
    void on_scalingAllSlider_valueChanged(int value);
    void on_rotationXSlider_valueChanged(int value);
    void on_rotationYSlider_valueChanged(int value);
    void on_rotationZSlider_valueChanged(int value);
    void on_scalingZSlider_valueChanged(int value);
    void on_rzutowanieSlider_valueChanged(int value);
    void on_translationZSlider_valueChanged(int value);
    void on_shearingXSlider_valueChanged(int value);
    void on_shearingYSlider_valueChanged(int value);
    void on_shearingZSlider_valueChanged(int value);
    void on_rzutowanieXButton_clicked();
    void on_translationXButton_clicked();
    void on_translationYButton_clicked();
    void on_translationZButton_clicked();
    void on_rotationXButton_clicked();
    void on_rotationYButton_clicked();
    void on_rotationZButton_clicked();
    void on_scalingXButton_clicked();
    void on_scalingYButton_clicked();
    void on_scalingZButton_clicked();
    void on_shearingXButton_clicked();
    void on_shearingYButton_clicked();
    void on_shearingZButton_clicked();
    void on_scalingAllButton_clicked();
    void on_colorAButton_clicked();
    void on_colorBButton_clicked();
    void on_colorCButton_clicked();
    void on_gruboscSlider_valueChanged(int value);
    void on_colorButton_clicked();
};

#endif // MYWINDOW_H
