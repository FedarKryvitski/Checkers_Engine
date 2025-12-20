#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "GameController.h"
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <condition_variable>
#include <atomic>

#define mytype unsigned char

using std::cout;
using std::endl;
using std::mutex;
using std::condition_variable;
using std::atomic_bool;

const unsigned short PORT = 5000;
using namespace sf;

#define TIME 600

enum Type {
    INIT,
    MOVEREQ,
    DRAWREQ,
    RESIGN
};

enum GAME_RESULT {
    FIRST_WIN,
    SECOND_WIN,
    FIRST_DISCONNECT,
    SECOND_DISCONNECT,
    THERE_DRAW,
    GAME_IS_GOING,
    TIMEOUT_RESULT
};

struct Client {
    int id;
    TcpSocket* socket;
    bool draw;
    int time;
};

std::vector<Client> clients;

typedef struct {
    int* time1;
    int* time2;
    bool* turn;
    atomic_bool* gameActive;
    condition_variable* cv;
    mutex* mtx;
    Client* first;
    Client* second;
    GameController* control;
} ClockData;

void handleTime(ClockData& data) {
    while (*(data.gameActive)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<mutex> lock(*(data.mtx));
        if (*(data.turn)) {
            (*(data.time1))--;
            if (*(data.time1) == 0) {
                *(data.gameActive) = false;
                data.cv->notify_one();

                Packet packet;
                packet << (uint8_t)MOVEREQ << (uint8_t)LOSE << 0 << 0 << 0 << 0;
                data.first->socket->send(packet);
                packet.clear();
                packet << (uint8_t)MOVEREQ << (uint8_t)WIN << 0 << 0 << 0 << 0;
                data.second->socket->send(packet);
                break;
            }
        }
        else {
            (*(data.time2))--;
            if (*(data.time2) == 0) {
                *(data.gameActive) = false;
                data.cv->notify_one();

                Packet packet;
                packet << (uint8_t)MOVEREQ << (uint8_t)WIN << 0 << 0 << 0 << 0;
                data.first->socket->send(packet);
                packet.clear();
                packet << (uint8_t)MOVEREQ << (uint8_t)LOSE << 0 << 0 << 0 << 0;
                data.second->socket->send(packet);
                break;
            }
        }
    }
}

void resignOrDraw(Client& first, Client& second, std::atomic<bool>& secondResign, std::atomic<bool>& theredraw) {
    Packet packet;
    uint8_t status;
    while (second.socket->receive(packet) == Socket::Done) {
        packet >> status;
        if (status == DRAWREQ) {
            second.draw = true;
            if (!first.draw) {
                packet.clear();
                packet << (uint8_t)DRAWREQ;
                first.socket->send(packet);
            }
            else {
                theredraw = true;
                packet.clear();
                packet << (uint8_t)MOVEREQ << (uint8_t)DRAW << 0 << 0 << 0 << 0;
                first.socket->send(packet);
                second.socket->send(packet);
            }
        }
        else if (status == RESIGN) {
            secondResign = true;
            Packet packet1, packet2;
            packet1 << (uint8_t)MOVEREQ << (uint8_t)WIN << 0 << 0 << 0 << 0;
            packet2 << (uint8_t)MOVEREQ << (uint8_t)LOSE << 0 << 0 << 0 << 0;
            first.socket->send(packet1);
            second.socket->send(packet2);
        }
    }
}

GAME_RESULT handleMove(GameController& control, Client& tfirst, Client& tsecond) {
    Packet packet;
    mytype x1, y1, x2, y2;
    uint8_t type;
    uint8_t result;

    Client first = tfirst, second = tsecond;

    if (!control.turn) {
        std::swap(first, second);
    }

    std::atomic<bool> secondResign(false);
    std::atomic<bool> theredraw(false);
    sf::Thread thread(std::bind(&resignOrDraw, std::ref(first), std::ref(second), std::ref(secondResign), std::ref(theredraw)));
    thread.launch();

    if (first.socket->receive(packet) == Socket::Done) {
        thread.terminate();
        if (secondResign) {
            return (control.turn ? SECOND_DISCONNECT : FIRST_DISCONNECT);
        }
        if (theredraw) {
            return THERE_DRAW;
        }

        packet >> type;

        if (type == RESIGN) {
            Packet packet1, packet2;
            packet1 << (uint8_t)MOVEREQ << (uint8_t)LOSE << 0 << 0 << 0 << 0;
            packet2 << (uint8_t)MOVEREQ << (uint8_t)WIN << 0 << 0 << 0 << 0;
            first.socket->send(packet1);
            second.socket->send(packet2);
            return (control.turn ? SECOND_WIN : FIRST_WIN);
        }
        else if (type == DRAWREQ) {

            if (control.turn) {
                tfirst.draw = true;
            }
            else {
                tsecond.draw = true;
            }


            if (second.draw) {
                packet.clear();
                packet << (uint8_t)MOVEREQ << (uint8_t)DRAW << 0 << 0 << 0 << 0;
                first.socket->send(packet);
                second.socket->send(packet);
                return THERE_DRAW;
            }
            else {
                packet.clear();
                packet << (uint8_t)DRAWREQ;
                second.socket->send(packet);
            }
        }
        else {
            tfirst.draw = false;
            tsecond.draw = false;

            packet >> x1 >> y1 >> x2 >> y2;

            result = control.PlayerMove(x1, y1, x2, y2);
            Packet packet1, packet2;

            if (result != INVALID_COORD) {
                if (result == WIN) {
                    packet1 << (uint8_t)MOVEREQ << (uint8_t)WIN << x1 << y1 << x2 << y2;
                    packet2 << (uint8_t)MOVEREQ << (uint8_t)LOSE << x1 << y1 << x2 << y2;
                    return (control.turn ? SECOND_WIN : FIRST_WIN);
                }
                else {
                    packet1 << (uint8_t)MOVEREQ << result << x1 << y1 << x2 << y2;
                    packet2 = packet1;
                }
                first.socket->send(packet1);
                second.socket->send(packet2);
            }
            else {
                packet1 << (uint8_t)MOVEREQ << (uint8_t)INVALID_COORD << x1 << y1 << x2 << y2;
                first.socket->send(packet1);
            }
        }
        return GAME_IS_GOING;
    }
    thread.terminate();
    return (GAME_RESULT)(3 - control.turn);
}

void handleGame(int index1, int index2) {
    Packet packet;
    Client first = clients[index1];
    Client second = clients[index2];
    first.draw = false;
    first.time = TIME;
    second.draw = false;
    second.time = TIME;

    GameController control;
    ClockData clockdata;
    clockdata.time1 = &first.time;
    clockdata.time2 = &second.time;
    clockdata.turn = &control.turn;

    atomic_bool gameActive(true);
    clockdata.gameActive = &gameActive;
    condition_variable cv;
    clockdata.cv = &cv;
    mutex mtx;
    clockdata.mtx = &mtx;
    clockdata.first = &first;
    clockdata.second = &second;
    clockdata.control = &control;

    packet << (uint8_t)INIT << (uint8_t)1 << TIME;
    first.socket->send(packet);
    packet.clear();

    packet << (uint8_t)INIT << (uint8_t)0 << TIME;
    second.socket->send(packet);
    packet.clear();

    Thread timeThread(&handleTime, std::ref(clockdata));

    GAME_RESULT status = GAME_IS_GOING;
    while (status == GAME_IS_GOING) {
        {
            std::lock_guard<mutex> lock(mtx);
            status = handleMove(control, first, second);
        }

        std::unique_lock<mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::milliseconds(1), [&gameActive] { return !gameActive; });

        if (!gameActive) {
            status = TIMEOUT_RESULT;
        }
    }

    if (status == FIRST_DISCONNECT) {
        packet.clear();
        packet << (uint8_t)MOVEREQ << (uint8_t)WIN << 0 << 0 << 0 << 0;
        second.socket->send(packet);
    }
    else if (status == SECOND_DISCONNECT) {
        packet.clear();
        packet << (uint8_t)MOVEREQ << (uint8_t)WIN << 0 << 0 << 0 << 0;
        first.socket->send(packet);
    }

    timeThread.terminate();
    delete first.socket;
    delete second.socket;
}

int main() {
    TcpSocket* socket = new TcpSocket;

    TcpListener listener;
    if (listener.listen(PORT) != Socket::Done) {
        return 1;
    }

    cout << "Server started, listening on port " << PORT << std::endl;

    while (true) {
        if (listener.accept(*socket) == Socket::Done) {
            Client newClient;
            newClient.id = clients.size();
            newClient.socket = socket;
            clients.push_back(newClient);
            cout << "Accepted new client: " << socket->getRemoteAddress() << "  " << socket->getRemotePort() << endl;
            if (clients.size() % 2 == 0) {
                std::thread(&handleGame, clients.size() - 2, clients.size() - 1).detach();
            }
            socket = new TcpSocket;
        }
    }
    return 0;
}

