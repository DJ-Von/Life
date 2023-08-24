#include <random>
#include <iostream>
#include <unistd.h>

using namespace std;
struct cell{
	unsigned live: 1;
};

#define WIDTH 20
#define HEIGHT 20

void create_world(cell world[][HEIGHT]){
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<> dis(1, 10000);

	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			unsigned int num = dis(rng);
			if(num % 2 == 0){
				world[i][j].live = 1;
			} else {
				world[i][j].live = 0;
			}
		}
	}
}

int get_live_count(cell world[][HEIGHT]){
	int count = 0;
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			if (world[i][j].live == 1){
				count++;
			}
		}
	}
	return count;
}

void get_neighbors_coords(int x, int y, int neighbors[][2]){
	int k = 0;
	for(int i = x - 1; i <= x + 1; i++){
		for(int j = y - 1; j <= y + 1; j++){
			if(i != x && j != y){
				neighbors[k][0] = i;
				neighbors[k][1] = j;
				k++;
			}
		}
	}
}

int count_live_neighbors(int x, int y, cell world[][HEIGHT]){
	int count = 0;
	int _x, _y;
	int neighbors_coords[8][2];

	get_neighbors_coords(x, y, neighbors_coords);

	for(int i = 0; i < 8; i++){
		_x = neighbors_coords[i][0];
		_y = neighbors_coords[i][1];
		if (_x < 0 || _y < 0 || _x >= WIDTH || _y >= HEIGHT){
			continue;
		}

		if(world[_x][_y].live == 1){
			count++;
		}
	}

	return count;
}

void next_generation(cell world[][HEIGHT], cell prev_world[][HEIGHT]){
	int live_neighbors;
	cell c;

	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			c = prev_world[i][j];
			live_neighbors = count_live_neighbors(i, j, prev_world);

			if(c.live == 0){
				if(live_neighbors == 3){
					world[i][j].live = 1;
				}
			} else {
				if(live_neighbors < 2 || live_neighbors > 3){
					world[i][j].live = 0;
				}
			}
		}
	}
}

void copy_world(cell source_world[][HEIGHT], cell destination_world[][HEIGHT]){
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < WIDTH; j++){
			destination_world[i][j] = source_world[i][j];
		}
	}
}

int compare_world(cell world_1[][HEIGHT], cell world_2[][HEIGHT]){
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < WIDTH; j++){
			if(world_1[i][j].live != world_2[i][j].live){
				return -1;
			}
		}
	}

	return 0;	
}

void print_world(cell world[][HEIGHT]){
    unsigned int i, j;
    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            if (world[i][j].live == 1) {
                cout << '*';
            } else {
                cout << ' ';
            }
            cout << ' ';
        }
        cout << endl;
    }
}

int main(){
	cell world[WIDTH][HEIGHT];
	cell prev_world[WIDTH][HEIGHT];

	create_world(world);
	int live_cells = -1;
	bool optimal = false;

	do{
		system("clear");
		print_world(world);
		copy_world(world, prev_world);
		next_generation(world, prev_world);

		optimal = compare_world(world, prev_world) == 0;
		live_cells = get_live_count(world);

		if(optimal){
			cout << "Stable config found!" << endl;
		}

		if(live_cells == 0){
			cout << "All cells died!" << endl;
		}

		sleep(1);
	} while(live_cells != 0 && !optimal);

	

	return 0;

}