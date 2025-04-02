//contains crude decision function

#include "computer2.h"


Computer::Computer(Snake& s, Apple& a) : snake(s), apple(a)
{
    direction = 'R';
    collision = false;  
};


//introduce a heuristic to being crowding/folding the snake as in human games, with leaving a cell on the
//ends to allow escape. f(x) represents minimizing distance from snake head to snake tail,
//g(x) represents minimizing area of the snake, as in we imagine filled in cells to create a rectangle around the
//snake's width and height. g(x) may also need to handle avoiding self-collisions and wall-collisions


void Computer::decision()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1);
    double give = dist(gen); //for random move occurring x% of the time
    double give2 = dist(gen); //for choosing between random move and determined move
    

    if(give2 >= epsilon)
    {
    if(give <= 0.25)
        direction = 'R';
    else if(give > 0.25 && give <= 0.5)
        direction = 'L';
    else if(give > 0.5 && give <= 0.75)
        direction = 'U';
    else if(give > 0.75)
        direction = 'D';
    }
    else
    {
        //pre-computes best move by reduced distance, then sends this best move
        snake_head_y = snake.get_body().front().second;
        snake_head_x = snake.get_body().front().first;
        apple_y = apple.get_apple_loc().second;
        apple_x = apple.get_apple_loc().first;

        radius_1 = sqrt( pow((snake_head_x+1) - apple_x, 2) + pow(snake_head_y - apple_y, 2)); //right
        radius_2 = sqrt( pow((snake_head_x-1) - apple_x, 2) + pow(snake_head_y - apple_y, 2)); //left
        radius_3 = sqrt( pow(snake_head_x - apple_x, 2) + pow((snake_head_y+1) - apple_y, 2)); //up
        radius_4 = sqrt( pow(snake_head_x - apple_x, 2) + pow((snake_head_y-1) - apple_y, 2)); //down

        right = {{'R', radius_1}};
        left = {{'L', radius_2}};
        up = {{'U', radius_3}};
        down = {{'D', radius_4}};

        matches.push_back(right);
        matches.push_back(left);
        matches.push_back(up);
        matches.push_back(down);
           
        if(snake.direction == 'R')
        {
            choice = std::min({radius_2, radius_3, radius_4});
            for(auto b : matches)
                for(auto const& pair : b)
                    if(pair.second == choice)
                        direction = pair.first;
        }
        else if(snake.direction == 'L')
        {
            choice = std::min({radius_1, radius_3, radius_4});
            for(auto b : matches)
                for(auto const& pair : b)
                    if(pair.second == choice)
                        direction = pair.first;
        }
        else if(snake.direction == 'U')
        {
            choice = std::min({radius_1, radius_2, radius_4});
            for(auto b : matches)
                for(auto const& pair : b)
                    if(pair.second == choice)
                        direction = pair.first;
        }
        else if(snake.direction == 'D')
        {
            choice = std::min({radius_1, radius_2, radius_3});
            for(auto b : matches)
                for(auto const& pair : b)
                    if(pair.second == choice)
                        direction = pair.first;
        }
        matches.clear();
    }
    

    move = direction;
}

void Computer::self_collision()
{
    //make a copy of snake's body and then shift it pre-emptively to see

    std::vector<std::pair<int,int>> body_copy = snake.get_body();

    Computer::decision();

    if(move == 'R')//shifting:
    {
        std::pair<int, int> temp = body_copy.front();
        std::pair<int, int> temp2;
        body_copy.front().first = body_copy.front().first + 1;
        for(int i = 1; i<body_copy.size(); i++)
        {
            temp2 = body_copy[i];
            body_copy[i] = temp;
            temp = temp2;
        }
        for(int i = 1; i<body_copy.size(); i++)
        {
            if(body_copy.front() == body_copy[i])
                collision = true;
        }
    }
    else if(move == 'L')
    {
        std::pair<int, int> temp = body_copy.front();
        std::pair<int, int> temp2;
        body_copy.front().first = body_copy.front().first - 1;
        for(int i = 1; i<body_copy.size(); i++)
        {
            temp2 = body_copy[i];
            body_copy[i] = temp;
            temp = temp2;
        }
        for(int i = 1; i<body_copy.size(); i++)
        {
            if(body_copy.front() == body_copy[i])
                collision = true;
        }
    }
    else if(move == 'U')
    {
        std::pair<int, int> temp = body_copy.front();
        std::pair<int, int> temp2;
        body_copy.front().second = body_copy.front().second + 1;
        for(int i = 1; i<body_copy.size(); i++)
        {
            temp2 = body_copy[i];
            body_copy[i] = temp;
            temp = temp2;
        }
        for(int i = 1; i<body_copy.size(); i++)
        {
            if(body_copy.front() == body_copy[i])
                collision = true;
        }
    }
    else if(move == 'D')
    {
        std::pair<int, int> temp = body_copy.front();
        std::pair<int, int> temp2;
        body_copy.front().second = body_copy.front().second - 1;
        for(int i = 1; i<body_copy.size(); i++)
        {
            temp2 = body_copy[i];
            body_copy[i] = temp;
            temp = temp2;
        }
        for(int i = 1; i<body_copy.size(); i++)
        {
            if(body_copy.front() == body_copy[i])
                collision = true;
        }
    }


}


void Computer::policy_func()
{
    Computer::decision();

    double lambda;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1);
    double random_num = dist(gen);

    //if(random_num < lambda)
    
}
