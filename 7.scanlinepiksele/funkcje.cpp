
void MyWindow::rysuj_piksel(int x, int y){
    int kolor = 0;
    unsigned char *ptr;
    ptr = img->bits();
    // Ustawiamy kolor kliknietego piksela na bialy lub czarny
    ptr[szer*4*y + 4*x] = kolor;
    ptr[szer*4*y + 4*x + 1] = kolor;
    ptr[szer*4*y + 4*x + 2] = kolor;
}

void MyWindow::rysuj_odcinek(int x0, int y0, int x1, int y1){

    int x,y;
    double a,b;

    if (x1<x0){
        int temp = x0;
        x0 = x1;
        x1 = temp;
    }

    if (x1-x0 != 0) {
        a = (y1-y0)/(x1-x0);
        b = y0 - a*x0;
    }



    // Sprawdzamy czy klikniecie nastapilo w granicach rysunku
    if ((x0>=0)&&(y0>=0)&&(x0<szer)&&(y0<wys))
    {


        if(x1-x0==0){
            x = x1;
            if (y1<y0){
                int temp = y0;
                y0 = y1;
                y1 = temp;
            }

            for (y=y0; y<=y1; y++){
                rysuj_piksel(x,y);
            }


        }
        else{
            for (x=x0; x<=x1; x++){

                y = (int)round(a * x + b);
                rysuj_piksel(x,y);
            }
        }
    }


    // Odswiezamy komponent
    update();


}



// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    int x0 = event->position().x();
    int y0 = event->position().y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

    x0 -= poczX;
    y0 -= poczY;


    wsp_x0 = x0;
    wsp_y0 = y0;

}

void MyWindow::mouseReleaseEvent(QMouseEvent *event){

    // Pobieramy wspolrzedne punktu klikniecia
    int x1 = event->position().x();
    int y1 = event->position().y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

    x1 -= poczX;
    y1 -= poczY;

    rysuj_odcinek(wsp_x0,wsp_y0,x1,y1);


}