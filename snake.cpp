#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>

using namespace std;
Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
double lastUpdateTime = 0;
int score = 0;
int timer = 10;
bool eventTriggered(double interval){
	double currentTime = GetTime();
	if(currentTime - lastUpdateTime >= interval){
		lastUpdateTime = currentTime;
		return true;
	}else{
		return false;
	}
}

class Snake{
	public:
		deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
		Vector2 direction = {1, 0};	

		void Draw(){
			for(int i = 0; i < body.size(); i++){
				float x = body[i].x;
				float y = body[i].y;
				Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
				DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}		
	}
		void Update(){
			body.pop_back();
			body.push_front(Vector2Add(body[0], direction));
			if(body[0].x < 0){
				body[0].x = cellCount - 1;	
			}else if(body[0].x >= cellCount){
				body[0].x = 0;
			}
			else if(body[0].y < 0){
				body[0].y = cellCount - 1;
			}
			else if(body[0].y >= cellCount){
				body[0].y = 0;
			}
	}
		void Eat(){		
			score++;
			cout << score << endl;
			Vector2 tail = body.back();

			Vector2 beforeTail = body[body.size() - 2];
			Vector2 direction = Vector2Subtract(tail, beforeTail);
	
			body.push_back(Vector2Add(tail, direction));
	}	

		void gameOver(){
			DrawText(("Game Over"), 280, 250, 50, RED);
			DrawText(("Press F to restart!"), 265, 350, 30, RED);
			EndDrawing();
			std::this_thread::sleep_for(std::chrono::seconds(10));
			if(IsKeyPressed(KEY_F)){
				CloseWindow();		
			}
			CloseWindow();
	}
};

class Food {
    public:
        Texture2D texture;
	Vector2 position;
	Food(){
		Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();	
	}	
	~Food(){
		UnloadTexture(texture);
}
	void Draw() {
            DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}
	Vector2 GenerateRandomPos(){
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{x, y};
	}
};


int main() {
	InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake Game");
	SetTargetFPS(60);
	
	Food food = Food();
	Snake snake = Snake();

	while (!WindowShouldClose()) {
        BeginDrawing();

	if(eventTriggered(0.2)){
		snake.Update();
	}
	if(IsKeyPressed(KEY_UP)){
		snake.direction = {0, -1};
	}
	else if(IsKeyPressed(KEY_DOWN)){
		snake.direction = {0, 1};
	}
	else if(IsKeyPressed(KEY_LEFT)){
		snake.direction = {-1, 0};
	}
	else if(IsKeyPressed(KEY_RIGHT)){
		snake.direction = {1, 0};
	}
	ClearBackground(green);
        food.Draw();
	snake.Draw();
	if(snake.body[0].x == food.position.x && snake.body[0].y == food.position.y){
		snake.Eat();
		food.position = food.GenerateRandomPos();
	}
	DrawText(TextFormat("Score: %03i", score), 280, 30, 40, GRAY);	        	
	
	for(int i = 1; i < snake.body.size(); i++){
		if(snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y){
			snake.gameOver();
		}
	}
	
	EndDrawing();
	
    }
    CloseWindow();
    return 0;
}

