#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "knight.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
//ignorowanie ostrzezen visual studio co do funkcji scanf i strncpy (nie sa bezpieczne)
#pragma warning( disable : 4703)
#pragma warning( disable : 4996)

const int square_size = 80 /*!< Wielkosc pol szachownicy w pikselach */,
board_margin_x = 20, /*!< Margines szachownicy x */
board_margin_y = 80; /*!< Margines szachownicy y */
const float FPS = 60; /*!< Klatki na sekunde */
int mouse_pos_x, /*!< Stale aktualizowana pozycja myszki x */
mouse_pos_y; /*!< Stale aktualizowana pozycja myszki y */
bool anim_button = false, /*!< Opisujacy mozliwosc wciskania przycisku animate */
calc_button = true, /*!< Opisujacy mozliwosc wciskania przycisku calculate */
click_button = true; /*!< Opisujacy mozliwosc zmiany pozycji konia */
/**
*@brief Funkcja ustawiania boole przyciskow ui
*/
void setSwitches(bool anim, bool calc, bool click) {
    anim_button = anim;
    calc_button = calc;
    click_button = click;
}
/**
*@brief Funkcja rysuje konia na podanej pozycji
*/
void drawKnight(ALLEGRO_BITMAP* all_player, int x, int y) {
    al_draw_bitmap(all_player, board_margin_x + 3 + (x * square_size), board_margin_y + 5 + (y * square_size), 0);
}
/**
*@brief Funkcja rysuje wszystkie pola szachownicy, takze odwiedzone
*/
void drawBoard(ALLEGRO_BITMAP* all_dark_square, ALLEGRO_BITMAP* all_white_square, ALLEGRO_BITMAP* all_visited_square)
{
    int i, j;
    for (i = 0; i < BoardSize; i++)
    {
        for (j = 0; j < BoardSize; j++)
        {
            if (j % 2 == 0)
            {
                if (i % 2 == 0)
                {
                    if (Visited(movesVisited, i, j))
                        al_draw_bitmap(all_visited_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                    else
                        al_draw_bitmap(all_white_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);

                }
                else
                {
                    if (Visited(movesVisited, i, j))
                        al_draw_bitmap(all_visited_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                    else
                        al_draw_bitmap(all_dark_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                }
            }
            else
            {
                if (i % 2 == 1)
                {
                    if (Visited(movesVisited, i, j))
                        al_draw_bitmap(all_visited_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                    else
                        al_draw_bitmap(all_white_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                }
                else
                {
                    if (Visited(movesVisited, i, j))
                        al_draw_bitmap(all_visited_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                    else
                        al_draw_bitmap(all_dark_square, i * square_size + board_margin_x, j * square_size + board_margin_y, 0);
                }
            }

        }

    }

}
/**
*@brief Funkcja rysuje przyciski ui i tekst na ekranie
*/
void drawUI(ALLEGRO_FONT* uiText, ALLEGRO_FONT* smallUIText)
{
    if (anim_button)
        al_draw_filled_rectangle(475, 80, 765, 155, al_map_rgb(119, 149, 86));
    else
        al_draw_filled_rectangle(475, 80, 765, 155, al_map_rgb(100, 100, 100));
    if (calc_button)
        al_draw_filled_rectangle(520, 180, 730, 255, al_map_rgb(119, 149, 86));
    else
        al_draw_filled_rectangle(520, 180, 730, 255, al_map_rgb(100, 100, 100));
    if (click_button)
        al_draw_filled_rectangle(board_margin_x, board_margin_y - 40, 350, board_margin_y, al_map_rgb(119, 149, 86));
    else
        al_draw_filled_rectangle(board_margin_x, board_margin_y - 40, 350, board_margin_y, al_map_rgb(100, 100, 100));

    al_draw_text(smallUIText, al_map_rgb(0, 0, 0), board_margin_x + 15, board_margin_y - 40, 0, "POSITION CHANGE");
    al_draw_text(uiText, al_map_rgb(0, 0, 0), 485, 80, 0, "ANIMATE");
    al_draw_text(uiText, al_map_rgb(0, 0, 0), 530, 180, 0, "SOLVE");
    if (solved)
        al_draw_text(smallUIText, al_map_rgb(0, 0, 0), 460, 300, 0, "SOLVED!");
    else
        al_draw_text(smallUIText, al_map_rgb(0, 0, 0), 460, 300, 0, "UNSOLVED :(");
    char text[30];
    sprintf(text, "MOVES:%d", movesFound + 1);
    al_draw_text(smallUIText, al_map_rgb(0, 0, 0), 430, 330, 0, text);
    sprintf(text, "RECURSIONS:%d", recursionsMade);
    al_draw_text(smallUIText, al_map_rgb(0, 0, 0), 430, 360, 0, text);
}
/**
*@brief Funkcja ustawia konia na szachownicy po podaniu pozycji myszy
*/
void setKnightPosition(int mouseX, int mouseY) {
    mouseX -= board_margin_x;
    mouseY -= board_margin_y;
    int x = mouseX / square_size;
    int y = mouseY / square_size;
    xKnightPos = x;
    yKnightPos = y;
    setSwitches(false, true, true);
}
/**
*@brief Funkcja main inicjuje pakiety allegro,
* tworzy ekran, wszystkie bitmapy i fonty.
*  Nastepnie rozpoczyna petle while true, w ktorej wyczekuje przycisku myszy,
* uruchamiajac odpowiednie przyciski.
*
*/

int main()
{
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_init_primitives_addon();

    ALLEGRO_EVENT_QUEUE* queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);

    ALLEGRO_BITMAP* all_player = al_load_bitmap("konik.png");
    ALLEGRO_BITMAP* all_white_square = al_load_bitmap("white.png");
    ALLEGRO_BITMAP* all_dark_square = al_load_bitmap("black.png");
    ALLEGRO_BITMAP* all_visited_square = al_load_bitmap("red.png");


    ALLEGRO_FONT* all_uiText = al_load_font("Pixeloid.ttf", 60, 0);
    ALLEGRO_FONT* all_smallUIText = al_load_font("Pixeloid.ttf", 30, 0);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    bool done = true;

    int curFrame = 0;
    int nextFrame = 30;
    int animMoveId = -1;

    while (done) {
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = false;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            //animacja konia
            if (animate) {
                if (++curFrame >= nextFrame)
                {
                    if (find_next(movesToAnimate, animMoveId, true) != -1)
                    {
                        xKnightPos = find_next(movesToAnimate, animMoveId, true);
                        yKnightPos = find_next(movesToAnimate, animMoveId, false);
                        movesVisited = insert_node(movesVisited, animMoveId, xKnightPos, yKnightPos);
                        curFrame = 0;
                        animMoveId++;
                    }
                    else
                    {
                        animate = false;
                        setSwitches(false, true, true);
                    }
                }

            }
        }
        //pobieranie pozycji myszki
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouse_pos_x = ev.mouse.x;
            mouse_pos_y = ev.mouse.y;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (ev.mouse.button & 1) {
                //przycisk animacji
                if ((mouse_pos_x >= 475 && mouse_pos_x <= 765) && (mouse_pos_y >= 80 & mouse_pos_y <= 155)) {
                    if (anim_button)
                    {
                        animate = true;
                        movesVisited = create_list(0, xKnightPos, yKnightPos);
                        curFrame = 0;
                        animMoveId = -1;
                        setSwitches(false, false, false);
                    }
                }
                //przycisk kalkulacji
                if ((mouse_pos_x >= 520 && mouse_pos_x <= 730) && (mouse_pos_y >= 180 & mouse_pos_y <= 255)) {
                    if (calc_button)
                    {
                        knightsTour();
                        remove_list(&movesVisited);
                        setSwitches(true, true, true);
                    }
                }
                //zmiana pozycji
                if ((mouse_pos_x >= board_margin_x && mouse_pos_x <= BoardSize * square_size + board_margin_x) && (mouse_pos_y >= board_margin_y && mouse_pos_y <= BoardSize * square_size + board_margin_y))
                {
                    if (click_button) {
                        setKnightPosition(mouse_pos_x, mouse_pos_y);
                        remove_list(&movesVisited);
                        setSwitches(false, true, true);
                    }
                }
            }

        }
        drawBoard(all_dark_square, all_white_square, all_visited_square);
        drawUI(all_uiText, all_smallUIText);
        drawKnight(all_player, xKnightPos, yKnightPos);
        al_flip_display();
        al_clear_to_color(al_map_rgb(60, 60, 60));

    }

    return 0;
}