#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 35;

typedef vector<vector<int>> vij;

bool isValido(int x, int y, int vis[MAXN][MAXN]){
    if(x < 2 || x >=30 || y <2 || y >=30) return false;
    if(vis[x][y]) return false;
    return true;
}

int getRand(int a, int b){
    return a + rand() % (b-a);
} 

void getNewCaminho(vij& matriz){
    for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
            matriz[i][j] = getRand(2, 29);
}

void desenhaGrid(sf::RenderWindow& window, vij& matriz, vij& caminho){
    sf::Color cinza(128, 128, 128);
    for (int i = 2; i < 30; i++) {
        for (int j = 2; j < 30; j++) {
            sf::RectangleShape rectangle(sf::Vector2f(30, 30));
            rectangle.setPosition(i * 30, j * 30);
            rectangle.setOutlineThickness(1);
            rectangle.setOutlineColor(sf::Color::Black);

            if((caminho[0][0] == i && caminho[0][1] == j) ||
                (caminho[1][0] == i && caminho[1][1] == j))
                rectangle.setFillColor(sf::Color::Blue);
            else if(matriz[i][j] == 0)
                rectangle.setFillColor(sf::Color::White);
            else if(matriz[i][j] == 1)
                rectangle.setFillColor(cinza);
            else if(matriz[i][j] == 2)
                rectangle.setFillColor(sf::Color::Green);
            else 
                rectangle.setFillColor(sf::Color::Red);

            window.draw(rectangle);
        }
    }
}

void bfs(sf::RenderWindow& window, vij& matriz, vij& caminho){
    queue< pair<int, int> > fila;
    fila.push(make_pair(caminho[0][0], caminho[0][1]));
    int visistado[MAXN][MAXN];
    vector<vector<pair<int, int>>> pai(MAXN, vector<pair<int, int>>(MAXN, make_pair(0,0)));
    memset(visistado, 0, sizeof visistado);
    int x, y;
    int dx[] = {-1, 0, 1, 0}, dy[] = {0, 1, 0, -1};
    while(!fila.empty()){
        x = fila.front().first;
        y = fila.front().second;
        fila.pop();
        if(caminho[1][0] == x && caminho[1][1] == y) break;
        matriz[x][y] = 2;
        visistado[x][y] = 1;
        for(int i=0; i<4; i++){
            if(isValido(x+dx[i], y+dy[i], visistado) && matriz[ x+dx[i] ][ y+dy[i] ] == 0){
                matriz[ x+dx[i] ][ y+dy[i] ] = 3;
                visistado[ x+dx[i] ][ y+dy[i] ] = 1;
                pai[ x+dx[i] ][ y+dy[i] ] = make_pair(x, y);
                fila.push(make_pair(x+dx[i], y+dy[i]));
            }
        }
        window.clear();
        desenhaGrid(window, matriz, caminho);
        window.display();
        sf::sleep(sf::milliseconds(30));
    }

    if(visistado[ caminho[1][0] ][ caminho[1][1] ]){// consegui chegar
        // vamos mudar a cor das celulas que estão na fila inicialmente
        while(!fila.empty()){
            x = fila.front().first;
            y = fila.front().second;
            fila.pop();
            matriz[x][y] = 0;
        }
        x = caminho[1][0], y = caminho[1][1];
        cout << caminho[1][0] << ' ' << caminho[1][1] << '\n';
        cout << x << ' ' << y << '\n';
        while(x != caminho[0][0] || y != caminho[0][1]){
            int x1 = x;
            x = pai[x1][y].first;
            y = pai[x1][y].second;
            matriz[x][y] = 3;
        }
    }
}

int main() {
    vij matriz(MAXN, vector<int>(MAXN, 0)); // matriz das posisções no mapa
    vij caminho(2, vector<int>(2, 0)); // ponto inicial/final
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Mapa 30x30"); // GUI/tela

    // cria um objeto de botão de inicio
    sf::RectangleShape buttonInicio(sf::Vector2f(110, 50));
    buttonInicio.setPosition(800, 930);
    buttonInicio.setFillColor(sf::Color::Blue);

    // cria um objeto de texto para o rótulo do botão de inicio
    sf::Font font;
    font.loadFromFile("Fontes/Roboto-Black.ttf");
    sf::Text buttonLabelInicio("Iniciar", font, 30);
    buttonLabelInicio.setPosition(buttonInicio.getPosition().x + 10, buttonInicio.getPosition().y + 5);
    buttonLabelInicio.setFillColor(sf::Color::Black);

    // cria um objeto de botão de limpar
    sf::RectangleShape buttonLimpar(sf::Vector2f(115, 50));
    buttonLimpar.setPosition(600, 930);
    buttonLimpar.setFillColor(sf::Color::Blue);

    // cria um objeto de texto para o rótulo do botão de limpar
    sf::Text buttonLabelLimpar("Limpar", font, 30);
    buttonLabelLimpar.setPosition(buttonLimpar.getPosition().x + 10, buttonLimpar.getPosition().y + 5);
    buttonLabelLimpar.setFillColor(sf::Color::Black);

    srand(time(NULL));
    getNewCaminho(caminho);

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
                if(!((i == caminho[0][0] && j == caminho[0][1]) || 
                    (i == caminho[1][0] && j == caminho[1][1]))) // não é inicio/fim
                    matriz[i][j] = !matriz[i][j];
            }

            // verifica se o botão iniciar foi pressionado
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                if (buttonInicio.getGlobalBounds().contains(worldPosition)) {
                    bfs(window, matriz, caminho);
                }
            }

            // verifica se o botão limpar foi precionado
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                if(buttonLimpar.getGlobalBounds().contains(worldPosition)){
                    for(int i=0; i<MAXN; i++)
                        for(int j=i; j < MAXN; j++)
                            matriz[i][j] = matriz[j][i] = 0;
                    getNewCaminho(caminho);
                }
            }
        }

        window.clear();

        // desenha o grid
        desenhaGrid(window, matriz, caminho);

        // desenha o botão e o rótulo do botão de inicio
        window.draw(buttonInicio);
        window.draw(buttonLabelInicio);
        // desenha o botão e o rótulo do botão de limpar
        window.draw(buttonLimpar);
        window.draw(buttonLabelLimpar);
        window.display();
    }

    return 0;
}
