#define _CRT_SECURE_NO_WARNINGS
#define NUM_OF_DISC 7
#define FRAME_RATE 0.1f

#define HORIZONTAL_MARGIN_BETWEEN_DISC 200
#define VERTICAL_MARGIN_BETWEEN_DISC 25


#define LEFT_MOST_DISC_POS 190
#define BOTTOM_DISC_POS 135

#include <stdio.h>
#include <bangtal.h>

using namespace std;
int move_queue[999][2];
int move_queue_size = 0;

int current_frame = 0;

ObjectID startButton;
TimerID timer1;
SceneID scene1;

ObjectID disc[NUM_OF_DISC];


bool gameStarted = false;

int holder[3][NUM_OF_DISC + 1];

void enqueue_move(int src, int dst);
void calc_hanoi(int n, int from, int to, int temp);
void mouseCallback(ObjectID object, int x, int y, MouseAction action);
void timerCallback(TimerID timer);
void moveDisck(int src, int dst);

int main(void)
{
    setMouseCallback(mouseCallback);
    setTimerCallback(timerCallback);

    scene1 = createScene("하노이타워", "back.png");

    startButton = createObject("start.png");
    locateObject(startButton, scene1, 590, 70);
    showObject(startButton);

    char image[30];
    for (int i = NUM_OF_DISC; i > 0; i--)
    {
        holder[0][i] = i;
        if (i % 7 != 0)
            sprintf(image, "plate%d.png", i % 7);
        else
            sprintf(image, "plate%d.png", 7);
        disc[i - 1] = createObject(image);
        if (i >= 8)
            scaleObject(disc[i - 1], 1.0f / ((i / 7) + 1)); //anneal the size
        locateObject(disc[i - 1], scene1, LEFT_MOST_DISC_POS + HORIZONTAL_MARGIN_BETWEEN_DISC, BOTTOM_DISC_POS + i * VERTICAL_MARGIN_BETWEEN_DISC);
        showObject(disc[i - 1]);
    }
    holder[0][0] = NUM_OF_DISC;

    timer1 = createTimer(FRAME_RATE);
    startGame(scene1);
    return 0;
}


void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{

    if (object == startButton) {
        if (gameStarted == true)
            endGame();
        hideObject(startButton);
        calc_hanoi(NUM_OF_DISC, 1, 2, 3);
        moveDisck(move_queue[current_frame][0], move_queue[current_frame][1]);
        current_frame++;
        gameStarted = true;
    }
}

void timerCallback(TimerID timer)
{
    if (timer == timer1)
    {
        moveDisck(move_queue[current_frame][0], move_queue[current_frame][1]);
        current_frame++;
    }

}


void enqueue_move(int src, int dst)
{
    move_queue[move_queue_size][0] = src;
    move_queue[move_queue_size][1] = dst;
    move_queue_size++;
}
void calc_hanoi(int n, int from, int to, int temp)    // n개의 원판을 from에서 to로 temp를 이용해서 옮긴다.
{
    if (n == 1)
    {
        enqueue_move(from, to);
    }
    else {
        calc_hanoi(n - 1, from, temp, to);
        enqueue_move(from, to);
        calc_hanoi(n - 1, temp, to, from);
    }
}


void moveDisck(int src, int dst)
{
    int src_top = (holder[src - 1][0]--);//stack top idx
    int dst_top = (++holder[dst - 1][0]);//stack top idx
    int src_idx = holder[src - 1][src_top];
    holder[dst - 1][dst_top] = src_idx;



    locateObject(disc[src_idx - 1], scene1, LEFT_MOST_DISC_POS + dst * HORIZONTAL_MARGIN_BETWEEN_DISC, BOTTOM_DISC_POS + dst_top * VERTICAL_MARGIN_BETWEEN_DISC);

    if (current_frame + 1 == move_queue_size)
    {
        setObjectImage(startButton, "end.png");
        showObject(startButton);
        return;
    }
    setTimer(timer1, FRAME_RATE);
    startTimer(timer1);
}