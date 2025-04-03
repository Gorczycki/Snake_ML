#include "computer2.h"


Computer::Computer(Snake& s, Apple& a) : snake(s), apple(a)
{
    direction = 'R';
    collision = false;  
    lambda = 1;
};


//introduce a heuristic to being crowding/folding the snake as in human games, with leaving a cell on the
//ends to allow escape. f(x) represents minimizing distance from snake head to snake tail,
//g(x) represents minimizing area of the snake, as in we imagine filled in cells to create a rectangle around the
//snake's width and height. g(x) may also need to handle avoiding self-collisions and wall-collisions


void Computer::decision() //crude
{
    snake_head_y = snake.get_body().front().second;
    snake_head_x = snake.get_body().front().first;
    apple_y = apple.get_apple_loc().second;
    apple_x = apple.get_apple_loc().first;

    std::vector<std::pair<char, double>> valid_moves;

    if (!self_collision('R'))
        valid_moves.push_back({'R', sqrt(pow((snake_head_x+1) - apple_x, 2) + pow(snake_head_y - apple_y, 2))});
    if (!self_collision('L'))
        valid_moves.push_back({'L', sqrt(pow((snake_head_x-1) - apple_x, 2) + pow(snake_head_y - apple_y, 2))});
    if (!self_collision('U'))
        valid_moves.push_back({'U', sqrt(pow(snake_head_x - apple_x, 2) + pow((snake_head_y+1) - apple_y, 2))});
    if (!self_collision('D'))
        valid_moves.push_back({'D', sqrt(pow(snake_head_x - apple_x, 2) + pow((snake_head_y-1) - apple_y, 2))});

    if (valid_moves.empty())
    {
        direction = 'X'; //invalid
        return;
    }

    auto best_move = *std::min_element(valid_moves.begin(), valid_moves.end(),
                                       [](const std::pair<char, double>& a, const std::pair<char, double>& b) {
                                           return a.second < b.second;
                                       });

    direction = best_move.first;
    move = direction;
}


bool Computer::self_collision(char move_) //crude checker
{
    std::vector<std::pair<int, int>> body_copy = snake.get_body();
    int next_x = body_copy.front().first;
    int next_y = body_copy.front().second;

    bool ans = false;

    // Simulate the move
    if (move_ == 'R') next_x += 1;
    else if (move_ == 'L') next_x -= 1;
    else if (move_ == 'U') next_y += 1;
    else if (move_ == 'D') next_y -= 1;

    // Check if the new head position matches any body segment
    for (size_t i = 1; i < body_copy.size(); i++)
    {
        if (body_copy[i].first == next_x && body_copy[i].second == next_y)
            ans = true;
    }

    return ans;
}




std::vector<double> Computer::compute_state()
{
    int snake_head_x = snake.get_body().front().first;
    int snake_head_y = snake.get_body().front().second;

    int apple_x = apple.get_apple_loc().first;
    int apple_y = apple.get_apple_loc().second;

    std::vector<double> state;

    // feature 1
    state.push_back(snake_head_x);
    state.push_back(snake_head_y);

    //feature 2
    state.push_back(apple_x - snake_head_x);  
    state.push_back(apple_y - snake_head_y);  

    // Feature3
    state.push_back(self_collision('R') ? 1.0 : 0.0);  
    state.push_back(self_collision('L') ? 1.0 : 0.0);  
    state.push_back(self_collision('U') ? 1.0 : 0.0); 
    state.push_back(self_collision('D') ? 1.0 : 0.0);  

   //feature 4
    if (snake.direction == 'R') {
        state.push_back(1.0);  
        state.push_back(0.0);  
        state.push_back(0.0);
        state.push_back(0.0);  
    }
    else if (snake.direction == 'L') {
        state.push_back(0.0);  
        state.push_back(1.0);  
        state.push_back(0.0);
        state.push_back(0.0);  
    }
    else if (snake.direction == 'U') {
        state.push_back(0.0);  
        state.push_back(0.0);  
        state.push_back(1.0);
        state.push_back(0.0);  
    }
    else if (snake.direction == 'D') {
        state.push_back(0.0);  
        state.push_back(0.0);  
        state.push_back(0.0);
        state.push_back(1.0);  
    }

    //feature 5
    int grid_width = 14;  
    int grid_height = 16; 
    
    state.push_back(snake_head_x); //dist to left wall
    state.push_back(grid_width - snake_head_x);  // dist to right wall
    state.push_back(snake_head_y);  // dost to top wall
    state.push_back(grid_height - snake_head_y); // dist to bottom wall

    //feature 6 (snake length)
    state.push_back(snake.get_body().size());

    return state;
}



void Computer::valid_move()
{
    Computer::self_collision(char move);
    while(collision)
    {
        for(auto const& pair : valids)
            if(pair.second == false)
            {
                move = pair.first;
            }
    }
    
}


void Computer::policy_func()
{
    std::vector<double> state = compute_state();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1);
    double random_num = dist(gen);

    //if(random_num >= lambda)
    //{
    //    //move = learned_policy_action();
    //}
    


}
