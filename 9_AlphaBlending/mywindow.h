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


namespace Ui {
    class MyWindow;
}



struct layer {
    QImage *picture;
    int alpha;
    int type;
    bool active = false;
};

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



    // zmienna zapamietujaca kolor ktory bedzie narysowany
    QColor color = QColor::fromRgb(255,0,0,255);

    layer layers[6];

    int ktora_wybrana = 0;


    // Deklaracje funkcji

    void odswiez_warstwy();
    void blend(QImage tlo, QImage pierwszy_plan, int alpha, int mode);




    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:

    void on_exitButton_clicked();
    void rysuj_piksel(int x, int y);
    void paintEvent(QPaintEvent*);

    void on_ResetButton_clicked();
    void on_layerRadio_1_clicked();
    void on_layerRadio_2_clicked();
    void on_layerRadio_3_clicked();
    void on_layerRadio_4_clicked();
    void on_layerRadio_5_clicked();
    void on_layerRadio_6_clicked();
    void on_layerCheckBox_1_stateChanged(int arg1);
    void on_layerCheckBox_2_stateChanged(int arg1);
    void on_layerCheckBox_3_stateChanged(int arg1);
    void on_layerCheckBox_4_stateChanged(int arg1);
    void on_layerCheckBox_5_stateChanged(int arg1);
    void on_layerCheckBox_6_stateChanged(int arg1);
    void on_alphaModeBox_currentIndexChanged(int index);
    void on_alphaSlider_valueChanged(int value);
};

#endif // MYWINDOW_H
