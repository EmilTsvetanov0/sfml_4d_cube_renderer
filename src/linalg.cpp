#include "linalg.h"

namespace la{

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

	template<>
	Figure<2> projection2d<2>(Figure<2> &&fig){
		return std::move(fig);
	}

}