#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <queue>

using namespace std;

const int MAXN = 35;

bool isValido(int x, int y, int vis[MAXN][MAXN]){
    if(x < 2 || x >=30 || y <2 || y >=30) return false;
    if(vis[x][y]) return false;
    return true;
}

void desenhaGrid(sf::RenderWindow& window, int matriz[MAXN][MAXN]){
    for (int i = 2; i < 30; i++) {
        for (int j = 2; j < 30; j++) {
            sf::RectangleShape rectangle(sf::Vector2f(30, 30));
            rectangle.setPosition(i * 30, j * 30);
            rectangle.setOutlineThickness(1);
            rectangle.setOutlineColor(sf::Color::White);
                
            if(matriz[i][j] == 0)
                rectangle.setFillColor(sf::Color::Black);
            else if(matriz[i][j] == 1)
                rectangle.setFillColor(sf::Color::Green);
            else 
                rectangle.setFillColor(sf::Color::Red);

            window.draw(rectangle);
        }
    }
}

void bfs(sf::RenderWindow& window, int matriz[MAXN][MAXN]){
    queue< pair<int, int> > fila;
    fila.push(make_pair(0, 0));
    int visistado[MAXN][MAXN];
    memset(visistado, 0, sizeof visistado);
    int x, y;
    int dx[] = {-1, 0, 1, 0}, dy[] = {0, 1, 0, -1};
    cout << " chama a bfs\n";
    while(!fila.empty()){
        x = fila.front().first;
        y = fila.front().second;
        fila.pop();
        matriz[x][y] = 1;
        visistado[x][y] = 1;
        for(int i=0; i<4; i++){
            if(isValido(x+dx[i], y+dy[i], visistado)){
                matriz[ x+dx[i] ][ y+dy[i] ] = 2;
                visistado[ x+dx[i] ][ y+dy[i] ] = 1;
                fila.push(make_pair(x+dx[i], y+dy[i]));
            }
        }
        desenhaGrid(window, matriz);
    }
}

int main() {
    int matriz[MAXN][MAXN]; // matriz das posisções no mapa
    memset(matriz, 0, sizeof matriz); // inicialmente todas as posições etsão livres
    
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Mapa 30x30");

    // cria um objeto de botão
    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setPosition(800, 930);
    button.setFillColor(sf::Color::Blue);

    // cria um objeto de texto para o rótulo do botão
    sf::Font font;
    font.loadFromFile("/home/esrael/Documentos/Projeto/Fontes/Roboto-Black.ttf");
    sf::Text buttonLabel("Iniciar", font, 30);
    buttonLabel.setPosition(button.getPosition().x + 10, button.getPosition().y + 5);
    buttonLabel.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // adiciona evento de clique do mouse
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                int i = worldPosition.x / 30;
                int j = worldPosition.y / 30;
                matriz[i][j] = !matriz[i][j];
            }

            // verifica se o botão foi pressionado
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                if (button.getGlobalBounds().contains(worldPosition)) {
                    button.setFillColor(sf::Color::Blue);
                    bfs(window, matriz);
                }
            }
        }

        window.clear();

        // desenha o grid
        desenhaGrid(window, matriz);

        // desenha o botão e o rótulo do botão
        window.draw(button);
        window.draw(buttonLabel);

        window.display();
    }

    return 0;
}
