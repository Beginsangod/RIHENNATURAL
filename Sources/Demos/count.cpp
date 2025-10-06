#include "count.h"

namespace nkentseu{

    Count::Count(const ApplicationProperties& props) : Application(props) {
        
    }

    void Count::Setup(){
        position.x = 100; 
        position = GetWindowHeight()/2;
    }
    
    void Count::Update(float deltaTime){
        position.y += gravity * deltaTime * 50;
        if(position.y >= GetWindowHeight() -  r){
            position.y -= friction * deltaTime * 50;
        }
    }

    void Count::Draw(){
        Background(graphics::Color::Black, true);
        Fill(graphics::Color::Green);
        Circle(position, r);
    }

}