#ifndef LINEAR_ALGEBRA
#define LINEAR_ALGEBRA

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

namespace la{
	std::vector<std::vector<double>> make_E(const unsigned int);

	std::vector<std::vector<double>> make_rot(const unsigned int, const unsigned int, const unsigned int, const double);

	void rot_adjust(std::vector<std::vector<double>> &rot, const unsigned int dim, const unsigned int a, const unsigned int b, const double angle);

	double scal(const std::vector<double> &A, const std::vector<double> &B);

	void apply_operator(std::vector<std::vector<double>> &A, std::vector<double> &B);

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
		Point(Point &&A) noexcept :dot(std::move(A.dot)){
			if(!isUnitMatrixInitialized){
				initializeUnitMatrix();
				isUnitMatrixInitialized = true;
			}
		}
		Point(const Point &A):dot(A.dot){
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

				for(const double tmp:init){
					if(i>=dim) break;
					dot[i++] = tmp;
				}
			}

			else{
				int i=0;

				for(const double tmp:init){
					dot[i++] = tmp;
				}
			}
		}

		static unsigned int get_dim(){
			return dim;
		}

		double& operator[](const int index){
			if(index<0||index>=dot.size()){
				sf::sleep(sf::milliseconds(5000));
				throw std::out_of_range("Point error: index out of range"); // Exception bro
			}
			return dot[index];
		}

		Point& operator=(Point<dim> &&A) noexcept {
			dot = std::move(A.dot);
			return *this;
		}

		Point& operator=(const Point<dim> &A){
			dot = A.dot;
			return *this;
		}

		Point if_rotate(const unsigned int a, const unsigned int b, const double angle){
			rot_adjust(E, dim, a, b, angle);
			Point new_point(*this);
			apply_operator(E, new_point.dot);
			rot_adjust(E, dim, a, b, 0);
			return new_point;
		}

		std::vector<double> projection(const std::vector<double>& normal) const {
			const double dotProduct = scal(dot, normal);
			std::vector<double> projection(dot.size());
			for(std::size_t i = 0; i < dot.size(); ++i) {
			    projection[i] = dot[i] - dotProduct * normal[i];
			}
			return projection;
		}

		std::vector<double> projection(const std::vector<double>&& normal) const {
			const double dotProduct = scal(dot, normal);
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

		Figure(Figure &&init) noexcept :skel(std::move(init.skel)),edges(std::move(init.edges)){}
		Figure(Figure &init):skel(init.skel),edges(init.edges){}
		explicit Figure(std::vector<std::pair<std::size_t, std::size_t>> &&graph):edges(std::move(graph)){}
		explicit Figure(const std::vector<std::pair<std::size_t, std::size_t>> &graph):edges(graph){}
		Figure(std::initializer_list<Point<dim>> &&init):skel(init){}
		Figure(std::initializer_list<Point<dim>> &&init, std::initializer_list<std::pair<std::size_t,std::size_t>> &&graph):skel(init),edges(graph){}
		Figure(std::initializer_list<Point<dim>> &&init, std::vector<std::pair<std::size_t, std::size_t>> graph):skel(std::move(init)),edges(std::move(graph)){}
		Figure(std::vector<Point<dim>> &init):skel(init){}
		Figure(std::vector<Point<dim>> &init, const std::vector<std::pair<std::size_t, std::size_t>> &graph):skel(init),edges(graph){}
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

		static unsigned int get_dim(){
			return dim;
		}
		Figure if_rotate(const unsigned int a, const unsigned int b, const double angle){
			Figure new_fig(*this);
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
				if(((i>>j)&1) == 1){
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

	void generate_texture_circle(sf::Uint8* pixels, unsigned int a, unsigned int b);

	template<int dim>
	Figure<2> projection2d(la::Figure<dim> &&fig){
		if(fig.get_dim()<2){
			sleep(sf::milliseconds(5000));
			throw std::invalid_argument("projection2d: can't make projection of an object which dim is under 2");
		}
		Figure<2> result(std::move(fig.edges));

		for (auto& point : fig.skel) {
			// Здесь мы не можем избежать копирования, так как меняем размерность
			std::vector<double> newCoords{point[0], point[1]};
			result.skel.emplace_back(newCoords);
		}
		return result;
	}

	template<>
	Figure<2> projection2d<2>(Figure<2> &&fig);

}



#endif