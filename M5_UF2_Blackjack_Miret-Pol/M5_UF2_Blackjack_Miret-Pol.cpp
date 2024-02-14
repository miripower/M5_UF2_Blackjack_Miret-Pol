#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <string>
using namespace std;

//Variables:
const int palos = 4;
const int cartas = 13;
const int limiteCrupier = 17;
const int maximoPuntos = 21;
int totalCartasJugador = 0;
int totalCartasCrupier = 0;
int balance;
int apuesta;
int cartaJugador1, cartaJugador2, cartaCrupier1;
int numeroPartida;
int arrayCartas[palos][cartas];

//Construir el array de cada partida:
void buildArrayCartas() {
    for (int i = 0; i < palos; ++i) {
        for (int j = 0; j < cartas; ++j) {
            arrayCartas[i][j] = (j + 1 <= 10) ? (j + 1) : 10;
        }
    }
}

//Obtener el nombre del Palo:
string obtenerNombrePalo(int palo) {
    switch (palo) {
    case 1:
        return "\x1B[31mDiamantes\x1B[37m";
    case 2:
        return "Picas";
    case 3:
        return "\x1B[31mCorazones\x1B[37m";
    case 4:
        return "Treboles";
    default:
        return "Desconocido";
    }
}

//Obtener el nombre de la carta en caso de As, J, Q y K:
string obtenerNombreCarta(int numeroCarta) {
    switch (numeroCarta) {
    case 1:
        return "As";
    case 11:
        return "J";
    case 12:
        return "Q";
    case 13:
        return "K";
    default:
        return to_string(numeroCarta);
    }
}

//Comprobar el valor de la carta en caso de que sea As:
int obtenerValorCarta(int numeroCarta, bool esCrupier = false) {
    if (numeroCarta == 1 && esCrupier && totalCartasCrupier <= 10) {
        return 11;
    }
    else if (numeroCarta == 1 && esCrupier && totalCartasCrupier > 10) {
        return 1;
    }
    else if (numeroCarta == 1 && esCrupier == false) {
        int valorAs;
        do {
            cout << "Elige el valor del AS (1/11): ";
            cin >> valorAs;
            if (valorAs != 1 && valorAs != 11) {
                cout << "Valor incorrecto. Debes elegir 1 o 11.\n";
            }
        } while (valorAs != 1 && valorAs != 11);
        return valorAs;
    }
    else {
        return numeroCarta;
    }
}

//Generar la carta y el palo. Verificar que no sea repetible:
void obtenerCarta(int& numeroCarta, bool esCrupier = false) {
    int paloRand, numCartaRand;

    do {
        paloRand = rand() % palos;
        numCartaRand = rand() % cartas;

    } while (arrayCartas[paloRand][numCartaRand] == 0);

    int palo = paloRand + 1;
    int numCarta = arrayCartas[paloRand][numCartaRand];

    cout << "Carta: " << obtenerNombreCarta(numCartaRand + 1) << " de " << obtenerNombrePalo(palo) << "\n";

    arrayCartas[paloRand][numCartaRand] = 0;

    numeroCarta = obtenerValorCarta(numCarta, esCrupier);
}

//Repartir las cartas al jugador y al crupier:
void repartirCartas(int& totalCartas, bool esCrupier = false) {
    int nuevaCarta;
    bool repartiendo = true;
    if (esCrupier == false) {
        cout << "\x1B[35mTu total de cartas: " << totalCartasJugador << "\x1B[37m\n";
        while (repartiendo && totalCartas < 21) {
            string opcion;
            cout << "Quieres una carta? (S/N): ";
            cin >> opcion;
            cout << "\n";
            if (opcion == "S" || opcion == "s") {
                obtenerCarta(nuevaCarta, false);
                totalCartas += nuevaCarta;
                cout << "\x1B[35mTu total de cartas: " << totalCartasJugador << "\x1B[37m\n";
            }
            else if (opcion == "N" || opcion == "n") {
                repartiendo = false;
            }
            else {
                cout << "Valor introducido incorrecto, porfavor introduce (S/N)\n";
            }
        }
    }
    else if (esCrupier == true) {
        while (totalCartasCrupier < 17) {
			cout << "\nEl crupier pide una carta" << endl;
			obtenerCarta(nuevaCarta, true);
			totalCartas += nuevaCarta;

			this_thread::sleep_for(chrono::seconds(1));
			cout << "\x1B[34mTotal cartas Crupier: " << totalCartasCrupier << "\x1B[37m" << endl;

			this_thread::sleep_for(chrono::seconds(1));  
        }
    }
}

//Verificar los totales finales del jugador y el crupier para determinar el ganador:
void verificarTotales(int totalCartasJugador, int totalCartasCrupier) {
    cout << "\n\x1B[35mTotal cartas jugador: " << totalCartasJugador << "\x1B[37m\n";
    cout << "\x1B[34mTotal cartas Crupier: " << totalCartasCrupier << "\x1B[37m\n";
    balance -= apuesta;
    if ((cartaJugador1 + cartaJugador2) == maximoPuntos && totalCartasCrupier != maximoPuntos) {
        balance += static_cast<int>(apuesta * 2.5);
        cout << "\nBlackjack! Has ganado \x1B[32m$" << apuesta * 1.5 << "\x1B[37m" << endl;
        cout << "Balance: \x1B[32m$" << balance << "\x1B[37m" << endl;
    } 
    else if (totalCartasJugador == maximoPuntos && totalCartasCrupier != maximoPuntos) {
        balance += apuesta * 2;
        cout << "\nHas ganado \x1B[32m$" << apuesta * 2 << "\x1B[37m" << endl;
        cout << "Balance: \x1B[32m$" << balance << "\x1B[37m" << endl;
    }
    else if (totalCartasJugador > totalCartasCrupier && totalCartasJugador <= maximoPuntos) {
        balance += apuesta * 2;
        cout << "\nHas ganado \x1B[32m$" << apuesta * 2 << "\x1B[37m" << endl;
        cout << "Balance: \x1B[32m$" << balance << "\x1B[37m" << endl;
    }
    else if (totalCartasJugador < totalCartasCrupier && totalCartasCrupier > maximoPuntos && totalCartasJugador <= maximoPuntos) {
        balance += apuesta * 2;
        cout << "\nHas ganado \x1B[32m$" << apuesta * 2 << "\x1B[37m" << endl;
        cout << "Balance: \x1B[32m$" << balance << "\x1B[37m" << endl;
    }
    else if (totalCartasJugador > maximoPuntos) {
        cout << "\nEl Crupier gana... Has perdido \x1B[32m$" << apuesta << "\x1B[37m" << endl;
        cout << "Balance: \x1B[32m$" << balance << "\x1B[37m" << endl;
    }
    else {
        cout << "\nEl Crupier gana... Has perdido \x1B[32m$" << apuesta << "\x1B[37m" << endl;
        cout << "Balance: \x1B[32m$" << balance << "\x1B[37m" << endl;
    }
}

//Codigo main con todas las funciones y un bucle do-while
int main() {
    srand(time(0));
    system("Title Casino Miret - Blackjack");

    balance = 100;
    apuesta = 0;
    string continuarJugando;
    numeroPartida = 0;

    cout << "/------------------------\\" << endl;
    cout << "|Bienvenido al Blackjack!|" << endl;
    cout << "\\------------------------/" << endl;
    cout << "\n";

    do {
        buildArrayCartas();

        cout << "Numero de partidas: \x1B[33m" << numeroPartida << "\x1B[37m\n\n";
        cout << "Tu balance actual es: \x1B[32m$" << balance << "\x1B[37m" << endl;
        cout << "Cuanto dinero quieres apostar?: \x1B[32m";
        cin >> apuesta;
        cout << "\x1B[37m\n";

        if (apuesta > balance || apuesta <= 0) {
            cout << "Apuesta no valida. Debes apostar una cantidad valida\n" << endl;
            continuarJugando = "s";
            continue;
        }

        obtenerCarta(cartaJugador1, false);
        obtenerCarta(cartaJugador2, false);
        totalCartasJugador = cartaJugador1 + cartaJugador2;

        cout << "\x1B[35mTu total de cartas: " << totalCartasJugador << "\x1B[37m\n\n";

        obtenerCarta(cartaCrupier1, true);
        totalCartasCrupier = cartaCrupier1;

        cout << "\x1B[34mTotal cartas Crupier: " << totalCartasCrupier << "\x1B[37m\n\n";

        repartirCartas(totalCartasJugador, false);
        repartirCartas(totalCartasCrupier, true);

        verificarTotales(totalCartasJugador, totalCartasCrupier);
        cout << "-----------------------" << endl;

        if (balance <= 0) {
            this_thread::sleep_for(chrono::seconds(3));
            continuarJugando = "n";
        }
        else {
            do {
                cout << "Quieres seguir jugando? (S/N): ";
                    cin >> continuarJugando;
                    cout << "\n";
                    if (continuarJugando != "N" && continuarJugando != "n" && continuarJugando != "S" && continuarJugando != "s") {
                        cout << "Valor incorrecto. Debes introducir (S/N)\n";
                    }
            } while (continuarJugando != "N" && continuarJugando != "n" && continuarJugando != "S" && continuarJugando != "s");
        }
        numeroPartida++;
        system("cls");

    } while (balance > 0 && (continuarJugando == "S" || continuarJugando == "s"));
    
    if (balance <= 0) {
        cout << "Has jugado un total de \x1B[33m" << numeroPartida << "\x1B[37m partida/s\n" << endl;
        cout << "Lo has perdido todo...\nBueno, casi todo, aun puedes vender la casa para volver a jugar :D\nEsperamos verte pronto en nuestro Casino \x1B[31m<3\x1B[37m" << endl;
    }
    else {
        cout << "Has jugado un total de \x1B[33m" << numeroPartida << "\x1B[37m partida/s\n" << endl;
        cout << "Fin del juego. Tu balance final es: \x1B[32m$" << balance << "\x1B[37m" << endl;
    }

    return 0;
}