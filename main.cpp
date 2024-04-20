#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

const unsigned int gridHeight=1000, gridLength=1000;

namespace la{

	std::vector<std::vector<double>> make_E(const unsigned int dim){
		std::vector<std::vector<double>> E_temp(dim,std::vector<double>(dim,0));
		for(std::size_t i=0; i<dim; ++i) E_temp[i][i] = 1;
		return E_temp;
	}

	std::vector<std::vector<double>> make_rot(const unsigned int dim, const unsigned int a, const unsigned int b, const double angle){
		std::vector<std::vector<double>> rot(dim, std::vector<double>(dim,0));
		for(std::size_t i=0; i<dim; ++i) rot[i][i]=1;
		rot[b][b] = rot[a][a] = std::cos(angle);
		rot[a][b] = -std::sin(angle);
		rot[b][a] = std::sin(angle);
		return rot;
	}

	void rot_adjust(std::vector<std::vector<double>> &rot, const unsigned int dim, const unsigned int a, const unsigned int b, const double angle){
		if(rot.size() != dim || (rot[0].size()>0 && rot[0].size() != rot.size())){
			sf::sleep(sf::milliseconds(5000));
			throw std::invalid_argument("Invalid rot size");
		}
		rot[b][b] = rot[a][a] = std::cos(angle);
		rot[a][b] = -std::sin(angle);
		rot[b][a] = std::sin(angle);
	}

	double scal(const std::vector<double> &A, const std::vector<double> &B){
		if(A.size()!=B.size()){
			sf::sleep(sf::milliseconds(5000));
			throw std::invalid_argument("Scalar mult: not the same size");
		}
		double res = 0;
		for(std::size_t i=0; i<A.size(); ++i) res+=A[i]*B[i];
		return res;
	}

	void apply_operator(std::vector<std::vector<double>> &A, std::vector<double> &B){
		if(B.size()!=A[0].size()){
			std::cout<<"Not the same size\n";
			sf::sleep(sf::milliseconds(5000));
			throw std::invalid_argument("Apply operator: Not the same size");
		}
		std::vector<double> C(B.size());
		for(std::size_t i=0; i<C.size(); ++i) C[i] = scal(A[i],B);
		for(std::size_t i=0; i<C.size(); ++i) B[i] = C[i];
	}

	template<int dim>
	struct Point{
		std::vector<double> dot;

		static std::vector<std::vector<double>> E;
		Point():dot(dim,0){
			if(!isUnitMatrixInitialized){
				initializeUnitMatrix();
				isUnitMatrixInitialized = true;
			}
		}
		Point(const Point<dim> &&A):dot(A.dot){
			if(!isUnitMatrixInitialized){
				initializeUnitMatrix();
				isUnitMatrixInitialized = true;
			}
		}
		Point(const Point<dim> &A):dot(A.dot){
			if(!isUnitMatrixInitialized){
				initializeUnitMatrix();
				isUnitMatrixInitialized = true;
			}
		}
		Point(const std::vector<double> &init):dot(dim,0){
			if(!isUnitMatrixInitialized){
				initializeUnitMatrix();
				isUnitMatrixInitialized = true;
			}

			if(dim<init.size()){
				int i=0;

				for(double tmp:init){
					if(i>=dim) break;
					dot[i++] = tmp;
				}
			}

			else{
				int i=0;

				for(double tmp:init){
					dot[i++] = tmp;
				}
			}
		}
		Point(const std::initializer_list<double> &&init):dot(dim,0){
			if(!isUnitMatrixInitialized){
				initializeUnitMatrix();
				isUnitMatrixInitialized = true;
			}

			if(dim<init.size()){
				int i=0;

				for(double tmp:init){
					if(i>=dim) break;
					dot[i++] = tmp;
				}
			}

			else{
				int i=0;

				for(double tmp:init){
					dot[i++] = tmp;
				}
			}
		}

		unsigned int get_dim(){
			return dim;
		}

		double& operator[](const std::size_t index){
			if(index<0||index>=dot.size()){
				sf::sleep(sf::milliseconds(5000));
				throw std::out_of_range("Point error: index out of range"); // Exception bro
			}
			return dot[index];
		}

		void operator=(const Point<dim> &&A){
			dot = A.dot;
		}

		void operator=(const Point<dim> &A){
			dot = A.dot;
		}

		Point<dim> if_rotate(const unsigned int a, const unsigned int b, const double angle){
			rot_adjust(E, dim, a, b, angle);
			Point<dim> new_point(*this);
			apply_operator(E, new_point.dot);
			rot_adjust(E, dim, a, b, 0);
			return new_point;
		}

		std::vector<double> projection(const std::vector<double>& normal) const {
		    double dotProduct = scal(dot, normal);
		    std::vector<double> projection(dot.size());
		    for(std::size_t i = 0; i < dot.size(); ++i) {
		        projection[i] = dot[i] - dotProduct * normal[i];
		    }
		    return projection;
		}

		std::vector<double> projection(const std::vector<double>&& normal) const {
		    double dotProduct = scal(dot, normal);
		    std::vector<double> projection(dot.size());
		    for(std::size_t i = 0; i < dot.size(); ++i) {
		        projection[i] = dot[i] - dotProduct * normal[i];
		    }
		    return projection;
		}

	private:

		static void initializeUnitMatrix() {
	        E.resize(dim, std::vector<double>(dim, 0));

	        for (std::size_t i = 0; i < dim; ++i) {
	            E[i][i] = 1;
	        }
	    }

		static bool isUnitMatrixInitialized;
	};

	template<int dim>
	std::vector<std::vector<double>> Point<dim>::E;

	template<int dim>
	bool Point<dim>::isUnitMatrixInitialized = false;

	template<unsigned int dim>
	struct Figure{
		std::vector<Point<dim>> skel;
		std::vector<std::pair<std::size_t, std::size_t>> edges;
		Figure(Figure<dim> &init):skel(init.skel),edges(init.edges){}
		Figure(Figure<dim> &&init):skel(init.skel),edges(init.edges){}
		Figure(std::vector<std::pair<std::size_t, std::size_t>> &&graph):edges(graph){}
		Figure(std::vector<std::pair<std::size_t, std::size_t>> &graph):edges(graph){}
		Figure(std::initializer_list<Point<dim>> &&init):skel(init){}
		Figure(std::initializer_list<Point<dim>> &&init, std::initializer_list<std::pair<std::size_t,std::size_t>> &&graph):skel(init),edges(graph){}
		Figure(std::initializer_list<Point<dim>> &&init, std::vector<std::pair<std::size_t, std::size_t>> graph):skel(init),edges(graph){}
		Figure(std::vector<Point<dim>> &init):skel(init){}
		Figure(std::vector<Point<dim>> &init, std::vector<std::pair<std::size_t, std::size_t>> &graph):skel(init),edges(graph){}
		void add_edges(std::vector<std::pair<std::size_t, std::size_t>> &&graph){
			edges = graph;
		}
		void add_edge(std::pair<std::size_t,std::size_t> &&edge){
			edges.emplace_back(edge);
		}
		void add_edge(std::pair<std::size_t,std::size_t> &edge){
			edges.emplace_back(edge);
		}
		void add_edges(std::initializer_list<std::pair<std::size_t,std::size_t>> &&graph){
			edges = graph;
		}
		unsigned int get_dim(){
			return dim;
		}
		Figure<dim> if_rotate(const unsigned int a, const unsigned int b, const double angle){
			Figure<dim> new_fig(*this);
			for(auto &i:new_fig.skel){
				i = i.if_rotate(a,b,angle);
			}
			return new_fig;
		}

		void print_skel(){
			std::cout<<"Skel: "<<skel.size()<<'\n';
			for(const auto& point : skel){
				for(const auto& tmp : point.dot) std::cout<<tmp<<' ';
				std::cout<<'\n';
			}
		}

		void print_edges(){
			std::cout<<"Edges: "<<edges.size()<<'\n';
			for(const auto& edge : edges)
				std::cout<<edge.first<<' '<<edge.second<<'\n';
		}

	};

	template<int dim>
	Figure<dim> generate_cube(const double side){
		unsigned int N = 1<<dim;
		std::vector<Point<dim>> skel;
		std::vector<std::pair<std::size_t, std::size_t>> edges;
		std::vector<std::vector<bool>> graph(N,std::vector<bool>(N,false));
		Point<dim> temporary;
		for(std::size_t i=0; i<N; ++i){
			for(std::size_t j=0; j<dim; ++j){
				if((i>>j)&1 == 1){
					temporary[j]=-side;
					graph[i][i-(1<<j)] = true;
					graph[i-(1<<j)][i] = true;
				}
				else{
					temporary[j]=side;
					graph[i][i+(1<<j)] = true;
					graph[i+(1<<j)][i] = true;
				}
			}
			skel.emplace_back(temporary);
		}
		for(std::size_t i=0; i<N; ++i){
			for(std::size_t j=0; j<i; ++j){
				if(graph[i][j]) edges.emplace_back(i,j);
			}
		}
		return Figure<dim>(skel,edges);
	}

	void generate_texture_circle(sf::Uint8* pixels, unsigned int a, unsigned int b){
		unsigned int R = a/2;
		for(std::size_t y=0; y<a; ++y){
			for(std::size_t x=0; x<b; ++x){
				unsigned int index = (y*a+x)*4;
				if(x*x + y*y <= R*R){
					unsigned int index = (y*a+x)*4;
					pixels[index] = 0;
					pixels[index+1] = 255;
					pixels[index+2] = 0;
					pixels[index+3] = 255;
				}
				else{
					pixels[index] = 0;
					pixels[index+1] = 0;
					pixels[index+2] = 0;
					pixels[index+3] = 0;
				}
			}
		}
	}

}

template<int dim>
la::Figure<2> projection2d(la::Figure<dim> &&fig){
	if(fig.get_dim()<2){
		sf::sleep(sf::milliseconds(5000));
		throw std::invalid_argument("projection2d: can't make projection of an object which dim is under 2");
	}
	la::Figure<2> result(std::move(fig.edges));
	
	for (auto& point : fig.skel) {
        // Здесь мы не можем избежать копирования, так как меняем размерность
        std::vector<double> newCoords{point[0], point[1]};
        result.skel.emplace_back(la::Point<2>(newCoords));
    }
    return result;
}

template<>
la::Figure<2> projection2d<2>(la::Figure<2> &&fig){
	return std::move(fig);
}

void draw_points(sf::Uint8* pixels, la::Figure<2> &&fig, int radius = 3) {
    const int width = gridLength;
    const int height = gridHeight;
    const sf::Uint8 green[4] = {0, 255, 0, 255}; // Зелёный цвет

    const int offsetX = 200; // Смещение по оси X
    const int offsetY = 200; // Смещение по оси Y

    for (const auto& point : fig.skel) {
        // Преобразование координат точки с учетом смещения
        int x = static_cast<int>(point.dot[0]) + offsetX;
        int y = static_cast<int>(point.dot[1]) + offsetY;

        // Закрашиваем квадрат вокруг точки
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int newX = x + dx;
                int newY = y + dy;

                // Проверяем, что новые координаты находятся в пределах изображения
                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                    int index = (newY * width + newX) * 4;
                    std::copy(green, green + 4, pixels + index);
                }
            }
        }
    }
}

void draw_points(sf::Uint8* pixels, la::Figure<2> &fig, int radius = 3) {
    const int width = gridLength;
    const int height = gridHeight;
    const sf::Uint8 green[4] = {0, 255, 0, 255}; // Зелёный цвет

    const int offsetX = 200; // Смещение по оси X
    const int offsetY = 200; // Смещение по оси Y

    for (const auto& point : fig.skel) {
        // Преобразование координат точки с учетом смещения
        int x = static_cast<int>(point.dot[0]) + offsetX;
        int y = static_cast<int>(point.dot[1]) + offsetY;

        // Закрашиваем квадрат вокруг точки
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int newX = x + dx;
                int newY = y + dy;

                // Проверяем, что новые координаты находятся в пределах изображения
                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                    int index = (newY * width + newX) * 4;
                    std::copy(green, green + 4, pixels + index);
                }
            }
        }
    }
}

void draw_figure2d(sf::Sprite &delta, sf::RenderWindow &window, la::Figure<2> &&fig, const unsigned int N){
	std::vector<double> direction(2);
	unsigned int offsetX = 400;
	unsigned int offsetY = 400;
	for(const auto& edge : fig.edges){
		unsigned int newX = fig.skel[edge.first][0] + offsetX;
		unsigned int newY = fig.skel[edge.first][1] + offsetY;
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
	unsigned int offsetX = 400;
	unsigned int offsetY = 400;
	for(const auto& edge : fig.edges){
		unsigned int newX = fig.skel[edge.first][0] + offsetX;
		unsigned int newY = fig.skel[edge.first][1] + offsetY;
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

    	draw_figure2d(sprite, window, projection2d<4>(cube4d.if_rotate(0,1,angle).if_rotate(1,2,angle).if_rotate(0,2,angle).if_rotate(1,3,angle)), 30);
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
