#include "UI.h"

//widnow


WindowManager::WindowManager() :window(sf::VideoMode(1000, 1000), "Linear Aproximation"){
    reference_frame = nullptr;
    points = nullptr;
    Data = nullptr;
    LinearAprox = nullptr;
    LinearInter = nullptr;
}


WindowManager::~WindowManager() {
    delete reference_frame;
    delete points;
    delete Data;
    delete LinearAprox;
    delete LinearInter;
}

void WindowManager::run(int option) {

    Data = new data;
    Data->getAndSavePositon();
    Data->maxMinValue();
    reference_frame = new ReferenceFrame(*this, *Data);
    points = new Points;
    points->scale(window, *Data);

    if (option == 1) {
        Data->linearAprox();
        Data->uncertainty();
        LinearAprox = new LinearAproximation;
        LinearAprox->setLine(window, *Data);
    }
    if (option == 2){
        LinearInter = new LinearInterpolation;
        LinearInter->setLine(window, *Data);
    }
    if (option == 3) {
        Data->linearAprox();
        Data->uncertainty();
        LinearAprox = new LinearAproximation;
        LinearAprox->setLine(window, *Data);
        LinearInter = new LinearInterpolation;
        LinearInter->setLine(window, *Data);
    }
    //test
    Data->test();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        reference_frame->draw(window);
        points->draw(window, *Data);
        if(option == 1) LinearAprox->draw(window);
        if (option == 2) LinearInter->draw(window);
        if (option == 3) {
            LinearAprox->draw(window);
            LinearInter->draw(window);
        }
        window.display();
    }
}

sf::Vector2u WindowManager::getSize() {
    
    return window.getSize();
}
//window


//reference frame
ReferenceFrame::ReferenceFrame(WindowManager& Window, data& data) {
    sf::Vector2u Window_Size = Window.getSize();
    sf::Vector2f window_size(static_cast<float>(Window_Size.x), static_cast<float>(Window_Size.y));
    if (!font.loadFromFile("YOUR_FONT_PATH")) {
        std::cout << "Cannot load font" << std::endl;
    }
    else {
        text.setCharacterSize(15);
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
    }
    float margin = window_size.x / 50;

    //6 - srednica poj punktu, 1 jednostka calk = 6 px
    float x_range = data.x.max - data.x.min;
    float y_range = data.y.max - data.y.min;

    if (x_range == 0) x_range = 1;
    if (y_range == 0) y_range = 1;

    float scale_x = (window_size.x - 2 * margin) / x_range;
    float scale_y = (window_size.y - 2 * margin) / y_range;

    float zero_x = margin - data.x.min * scale_x;  // Gdzie wypada X=0
    float zero_y = window_size.y - margin + data.y.min * scale_y; //gdzie wypada y = 0 Odwrócenie osi Y 



    x_axis[0] = sf::Vertex(sf::Vector2f(zero_x, margin), sf::Color::Black);
    x_axis[1] = sf::Vertex(sf::Vector2f(zero_x, window_size.y - margin), sf::Color::Black);

    y_axis[0] = sf::Vertex(sf::Vector2f(margin, zero_y), sf::Color::Black);
    y_axis[1] = sf::Vertex(sf::Vector2f(window_size.x - margin, zero_y),sf::Color::Black);

    graduation.setSize(sf::Vector2f(10,2));
    graduation.setFillColor(sf::Color::Black);
    sf::Vector2f size_g_s = graduation.getSize();
    graduation.setOrigin(size_g_s.x / 2, size_g_s.y / 2);

    graduation_scale_y.clear();
    for (int i = 0; i <= 20; ++i) {
        float data_value = data.y.min + i * (y_range / 20); // równomierne wartoœci danych
        float tick_y = (data_value - data.y.min) * scale_y + margin; // przekszta³cenie na ekran

        sf::RectangleShape tick = graduation;
        tick.setPosition(zero_x, tick_y);
        graduation_scale_y.push_back(tick);
    }

    graduation_scale_x.clear();
    graduation.rotate(90);
    for (int i = 0; i <= 20; ++i) {
        float data_value = data.x.min + i * (x_range / 20); // równomierne wartoœci danych
        float tick_x = (data_value - data.x.min) * scale_x + margin; // przekszta³cenie na ekran

        sf::RectangleShape tick = graduation;
        tick.setPosition(tick_x, zero_y);
        graduation_scale_x.push_back(tick);
    }
    
    num_scale_x.clear();
    for (int i = 0; i <= 20; ++i) {
        int data_value = data.x.min + i * (x_range / 20);
        text.setString(std::to_string(data_value));
        float tick_x = (data_value - data.x.min) * scale_x + margin;
        text.setPosition(tick_x, zero_y);
        num_scale_x.push_back(text);
     }
    
    num_scale_y.clear();
    for (int i = 0; i <= 20; ++i) {
        int data_value = data.y.min + i * (y_range / 20); 
        text.setString(std::to_string(data_value));
        float tick_y = window_size.y - margin - (data_value - data.y.min) * scale_y;
        text.setPosition(zero_x, tick_y);
        num_scale_y.push_back(text);
    }



}

void ReferenceFrame::draw(sf::RenderWindow& window) {
    window.draw(x_axis, 2, sf::Lines);
    window.draw(y_axis, 2, sf::Lines);
    for (auto& it : graduation_scale_x) {
        window.draw(it);
    }
    for (auto& it : graduation_scale_y) {
        window.draw(it);
    }
    for (auto& it : num_scale_x) {
        window.draw(it);
    }
    for (auto& it : num_scale_y) {
        window.draw(it);
    }
}


Points::Points() {
    point.setRadius(3);
    point.setOrigin(1,5);
    point.setFillColor(sf::Color::Blue);
}

void Points::scale(sf::RenderWindow& window, data& data) {
    sf::Vector2u Window_Size = window.getSize();
    sf::Vector2f window_size(static_cast<float>(Window_Size.x), static_cast<float>(Window_Size.y));
    float margin = window_size.x / 50;

    float x_range = data.x.max - data.x.min;
    float y_range = data.y.max - data.y.min;

    float scale_x = (window_size.x - 2 * margin) / x_range;
    float scale_y = (window_size.y - 2 * margin) / y_range;

    float zero_x = margin - data.x.min * scale_x;  // Gdzie wypada X=0
    float zero_y = window_size.y - margin + data.y.min * scale_y; //gdzie wypada y = 0 Odwrócenie osi 

    for (auto& it : data.points) {
        positionUI.x = (it.x - data.x.min) * scale_x + margin;
        positionUI.y = window_size.y - margin - (it.y - data.y.min) * scale_y;
        positionsUI.push_back(positionUI);
    }
}

void Points::draw(sf::RenderWindow& window, data& data) {

    for (auto& it : positionsUI) {
        point.setPosition(it.x, it.y);
        window.draw(point);
    }

}

void LinearAproximation::setLine(sf::RenderWindow& window, data& data) {

    sf::Vector2u Window_Size = window.getSize();
    sf::Vector2f window_size(static_cast<float>(Window_Size.x), static_cast<float>(Window_Size.y));
    float margin = window_size.x / 50;

    float x_range = data.x.max - data.x.min;
    float y_range = data.y.max - data.y.min;

    float scale_x = (window_size.x - 2 * margin) / x_range;
    float scale_y = (window_size.y - 2 * margin) / y_range;

    float zero_x = margin - data.x.min * scale_x;
    float zero_y = window_size.y - margin + data.y.min * scale_y;


    float x1_real = static_cast<float>(data.x.min);
    float y1_real = static_cast<float>(data.predictedValue(data.x.min));
    float x2_real = static_cast<float>(data.x.max);
    float y2_real = static_cast<float>(data.predictedValue(data.x.max));

    float x1 = (x1_real - data.x.min) * scale_x + margin;
    float y1 = window_size.y - margin - (y1_real - data.y.min) * scale_y;
    float x2 = (x2_real - data.x.min) * scale_x + margin;
    float y2 = window_size.y - margin - (y2_real - data.y.min) * scale_y;


    line[0] =  sf::Vertex(sf::Vector2f(x1, y1), sf::Color::Red);
    line[1] = sf::Vertex(sf::Vector2f(x2, y2), sf::Color::Red);

}

void LinearAproximation::draw(sf::RenderWindow& window) {
    window.draw(line, 2, sf::Lines);
}



void LinearInterpolation::setLine(sf::RenderWindow& window, data& data) {

    sf::Vector2u Window_Size = window.getSize();
    sf::Vector2f window_size(static_cast<float>(Window_Size.x), static_cast<float>(Window_Size.y));
    float margin = window_size.x / 50;

    float x_range = data.x.max - data.x.min;
    float y_range = data.y.max - data.y.min;

    float scale_x = (window_size.x - 2 * margin) / x_range;
    float scale_y = (window_size.y - 2 * margin) / y_range;

    float zero_x = margin - data.x.min * scale_x;
    float zero_y = window_size.y - margin + data.y.min * scale_y;

    data.sort();

    for (int i = 0; i < data.pointsSize() -1 ; i++) {
        float x1_r, y1_r, x2_r, y2_r;
        x1_r = data.getX(i);
        y1_r = data.getY(i);
        
            x2_r = data.getX(i + 1);
            y2_r = data.getY(i + 1);
        
        float x1 = (x1_r - data.x.min) * scale_x + margin;
        float y1 = window_size.y - margin - (y1_r - data.y.min) * scale_y;
        float x2 = (x2_r - data.x.min) * scale_x + margin;
        float y2 = window_size.y - margin - (y2_r - data.y.min) * scale_y;

        lines.push_back(sf::Vertex(sf::Vector2f(x1, y1), sf::Color::Green));
        lines.push_back(sf::Vertex(sf::Vector2f(x2, y2), sf::Color::Green));

    }

    

}

void LinearInterpolation::draw(sf::RenderWindow & window) {
    if (!lines.empty())
        window.draw(&lines[0], lines.size(), sf::Lines);
}

