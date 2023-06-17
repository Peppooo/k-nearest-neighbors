#include <iostream>
#include <vector>

using namespace std;

struct vec2 {
	double x,y;
	double distance_eu(vec2 p) {
		return sqrt(pow(x - p.x,2) + pow(y - p.y,2));
	}
	double distance_manh(vec2 p) {
		return abs(p.x - x)+abs(p.y - y);
	}
	double distance(vec2 p,bool e=true) {
		return e ? distance_eu(p) : distance_manh(p);
	}
	bool operator==(vec2& obj) {
		return ((x == obj.x) && (y == obj.y));
	}
};

struct set {
	vec2 p;
	double v; // value
	bool operator==(set& obj) {
		return (obj.p == p);
	}
};

void push_repeated(vector<set*>* dataset,set* t,int f) {
	vector<set*> r;
	for(int i = 0; i < f; i++) {
		r.push_back(t);
	}
	dataset->insert(dataset->begin(),r.begin(),r.end());
}

double average_set(vector<set*> d) {
	long double sum = 0;
	for(int i = 0; i < d.size(); i++) {
		if(d[i] != nullptr) {
			sum += d[i]->v;
		}
	}
	return sum / d.size();
}

double average(vector<double> d) {
	long double sum = 0;
	for(int i = 0; i < d.size(); i++) {
		sum += d[i];
	}
	return sum / d.size();
}

class knn {
public:
	vector<set> dataset;
	void add(vec2 p,double v) {
		dataset.push_back({p,v});
	}
private:
	set* closer_set(vec2 p,vector<set*> ignore = {}) {
		set* c = nullptr;
		double min_dist = (+INFINITY);
		for(int i = 0; i < dataset.size(); i++) {
			double c_dist = 0;
			for(int j = 0; j < ignore.size(); j++) {
				if(dataset[i] == (*ignore[j])) {
					goto skip_element;
				}
			}
			c_dist = dataset[i].p.distance_manh(p);
			if(c_dist < min_dist) {
				c = &dataset[i];
				min_dist = c_dist;
			}
		skip_element:continue;
		}
		return c;
	}
public:
	double predict(vec2 p,int k=3) {
		vector<set*> near_v;
		for(int i = 0; i < k; i++) {
			push_repeated(&near_v,closer_set(p,near_v),k - i);
		}
		return average_set(near_v);
	}
};

int main() {
	knn clf;

	for(double x = 1; x <= 10; x+=0.1) {
		for(double y = 1; y <= 10; y+=0.1) {
			clf.add({x,y}, x/y );
		}
	}

	const double x = 10.1834;
	const double y = 2.1239;

	vector<double> accuracy = {};

	for(double x = 1; x <= 10; x += 0.01) {
		for(double y = 1; y <= 10; y += 0.01) {
			double ob = x / y;
			double pr = clf.predict({x,y},3);
			accuracy.push_back( min(ob,pr)/max(ob,pr) );
		}
	}

	cout << average(accuracy);
	

	//cout << "predict: " << z_test << ", observed: " << x/y << ", y*z= " << y*z_test << ", x: " << x << endl;


	/*while(1) {
		SDL_PollEvent(&e);
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,0,0,0,255);

		buttons = SDL_GetMouseState(&dx,&dy);

		if(buttons == SDL_BUTTON_LEFT) {
			clf.add({(int)floor(dx / w_r),(int)floor(dy / h_r)},1);
		}
		if(buttons == SDL_BUTTON_RIGHT) {
			clf.add({(int)floor(dx / w_r),(int)floor(dy / h_r)},0);
		}
		for(int i = 0; i < clf.dataset.size(); i++) {
			SDL_SetRenderDrawColor(renderer,clf.dataset[i].v * 255,0,255,255);
			SDL_Rect r = {clf.dataset[i].p.x * w_r,clf.dataset[i].p.y * h_r,w_r,h_r};
			SDL_RenderFillRect(renderer,&r);
		}
		if(e.key.keysym.scancode == SDL_SCANCODE_P && e.type == SDL_KEYUP) {
			auto p = clf.predict({(int)floor(dx / w_r),(int)floor(dy / h_r)},3);
			clf.add({(int)floor(dx / w_r),(int)floor(dy / h_r)},p);
		}


		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_Rect r = {floor(dx / w_r)*w_r,floor(dy / h_r)*h_r,w_r,h_r};
		SDL_RenderDrawRect(renderer,&r);
		
		SDL_RenderPresent(renderer);
	}*/
}