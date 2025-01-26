#include <cmath>
#include "include/linalg.h"

constexpr unsigned int gridHeight=1000, gridLength=1000;

void draw_points(sf::Uint8* pixels, la::Figure<2> &&fig, int radius = 3) {
	constexpr int width = gridLength;
	constexpr int height = gridHeight;
	constexpr sf::Uint8 green[4] = {0, 255, 0, 255}; // Зелёный цвет

	for (const auto& point : fig.skel) {
		constexpr int offsetY = 200;
		constexpr int offsetX = 200;
	    // Преобразование координат точки с учетом смещения
        int x = static_cast<int>(point.dot[0]) + offsetX;
        int y = static_cast<int>(point.dot[1]) + offsetY;

        // Закрашиваем квадрат вокруг точки
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
	            const int newX = x + dx;
	            const int newY = y + dy;

                // Проверяем, что новые координаты находятся в пределах изображения
                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
	                const int index = (newY * width + newX) * 4;
                    std::copy(green, green+4, pixels + index);
                }
            }
        }
    }
}

void draw_points(sf::Uint8* pixels, la::Figure<2> &fig, int radius = 3) {
	constexpr int width = gridLength;
	constexpr int height = gridHeight;
	constexpr sf::Uint8 green[4] = {0, 255, 0, 255}; // Зелёный цвет

	for (const auto& point : fig.skel) {
		constexpr int offsetY = 200;
		constexpr int offsetX = 200;
	    // Преобразование координат точки с учетом смещения
        int x = static_cast<int>(point.dot[0]) + offsetX;
        int y = static_cast<int>(point.dot[1]) + offsetY;

        // Закрашиваем квадрат вокруг точки
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
	            const int newX = x + dx;
                const int newY = y + dy;

                // Проверяем, что новые координаты находятся в пределах изображения
                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                    const int index = (newY * width + newX) * 4;
                    std::copy(green, green + 4, pixels + index);
                }
            }
        }
    }
}

void draw_figure2d(sf::Sprite &delta, sf::RenderWindow &window, la::Figure<2> &&fig, const unsigned int N){
	std::vector<double> direction(2);
	for(const auto& edge : fig.edges){
		constexpr unsigned int offsetY = 400;
		constexpr unsigned int offsetX = 400;
		const unsigned int newX = fig.skel[edge.first][0] + offsetX;
		const unsigned int newY = fig.skel[edge.first][1] + offsetY;
		for(std::size_t i=0; i<2; ++i)
			direction[i] = (fig.skel[edge.second][i] - fig.skel[edge.first][i])/N;
		for(std::size_t i=0; i<=N; ++i){
			delta.setPosition(std::round(newX+direction[0]*i),std::round(newY+direction[1]*i));
			window.draw(delta);
		}
	}
}

void draw_figure2d(sf::Sprite &delta, sf::RenderWindow &window, la::Figure<2> &fig, const unsigned int N){
	std::vector<double> direction(2);
	for(const auto& edge : fig.edges){
		constexpr unsigned int offsetY = 400;
		constexpr unsigned int offsetX = 400;
		const unsigned int newX = fig.skel[edge.first][0] + offsetX;
		const unsigned int newY = fig.skel[edge.first][1] + offsetY;
		for(std::size_t i=0; i<2; ++i)
			direction[i] = (fig.skel[edge.second][i] - fig.skel[edge.first][i])/N;
		for(std::size_t i=0; i<=N; ++i){
			delta.setPosition(std::round(newX+direction[0]*i),std::round(newY+direction[1]*i));
			window.draw(delta);
		}
	}
}

int main(){
	sf::RenderWindow window(sf::VideoMode(gridLength,gridHeight), "My first program");

    sf::Texture texture;

    unsigned int textureLength = 10;
    unsigned int textureHeight = 10;

    if(!texture.create(textureLength, textureHeight)){
    	sf::sleep(sf::milliseconds(5000));
    	throw std::runtime_error("Couldn't create the texture");
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Uint8* pixels = new sf::Uint8[textureHeight*textureLength*4];
    la::generate_texture_circle(pixels,textureLength,textureHeight);

    texture.update(pixels);

    // la::Figure<3> cube({{50,50,-50},{-50,50,-50},{-50,-50,-50},{50,-50,-50},{50,50,50},{-50,50,50},{-50,-50,50},{50,-50,50}},{{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}});

    std::cout<<"Pre gen\n";

    la::Figure<4> cube4d(la::generate_cube<4>(50));

    std::cout<<"After gen\n";

    cube4d.print_skel();
    cube4d.print_edges();

    double angle = 0;

    while(window.isOpen()){
    	sf::Event event;
    	while(window.pollEvent(event)){
    		if(event.type == sf::Event::Closed){
                window.close();
            }
    	}

    	window.clear();

	    // texture.update(pixels);

	    // draw_points(pixels,projection2d<3>(cube.if_rotate(0,1,angle).if_rotate(1,2,angle).if_rotate(0,2,angle)));

    	draw_figure2d(sprite, window, la::projection2d<4>(cube4d.if_rotate(0,1,angle).if_rotate(1,2,angle).if_rotate(0,2,angle).if_rotate(1,3,angle)), 30);
    	// draw_figure2d(sprite, window, projection2d<4>(std::move(cube4d)), 30);
    	window.display();
    	angle+=0.01;
    	sf::sleep(sf::milliseconds(10));
    }
	return 0;
}
/*
g++ -c main.cpp -IC:\Users\tsvet\SFML-2.6.1\include
g++ main.o -o main -LC:\Users\tsvet\SFML-2.6.1\lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32
*/
