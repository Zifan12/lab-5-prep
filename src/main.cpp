#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const float CYCLE_SECONDS = 2.0f;
const float GRAPH_LEFT = 100.0f;
const float GRAPH_RIGHT = 700.0f;
const float GRAPH_TOP = 620.0f;
const float GRAPH_BOTTOM = 770.0f;

sf::Clock animclock;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }
        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyEvent->code) {
                case sf::Keyboard::Key::Num1:
                    tween = [](float a, float b, float t) {
                        float easedT = 1.0f - std::cos((t * 3.14159265f) / 2.0f);
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num2:
                    tween = [](float a, float b, float t) {
                        float easedT = t * t;
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num3:
                    tween = [](float a, float b, float t) {
                        float easedT = t * t * t;
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num4:
                    tween = [](float a, float b, float t) {
                        float easedT = (t == 1) ? 1 : 1 - std::pow(2.0f, -10.0f * t);
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num5:
                    tween = [](float a, float b, float t) {
                        float easedT = (t < 0.5f) ? 8.0f * t * t * t * t
                                                  : 1 - std::pow(-2.0f * t + 2.0f, 4) / 2.0f;
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num6:
                    tween = [](float a, float b, float t) {
                        float easedT = 1.0f - std::pow(1.0f - t, 3.0f);
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num7:
                    tween = [](float a, float b, float t) {
                        float easedT =
                            (t < 0.5f)
                                ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2))) / 2.0f
                                : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2)) + 1.0f) / 2.0f;
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num8:
                    tween = [](float a, float b, float t) {
                        const float c1 = 1.70158f;
                        const float c3 = c1 + 1.0f;
                        float easedT = c3 * t * t * t - c1 * t * t;
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
                case sf::Keyboard::Key::Num9:
                    tween = [](float a, float b, float t) {
                        float easedT = 1.0f - std::sqrt(1 - std::pow(t, 2));
                        return (1 - easedT) * a + easedT * b;
                    };
                    break;
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    float seconds = animclock.getElapsedTime().asSeconds();
    float wrapped =
        std::fmod(seconds, CYCLE_SECONDS);  // wrap growing time into repeating [0, CYCLE_SECONDS)
    float t = wrapped / CYCLE_SECONDS;      // scale into [0,1) for tween

    float x = tween(0.0f, 800.0f, t);
    float y = WINDOW_HEIGHT / 3.0f;

    sf::CircleShape circle(20.0f);
    circle.setPosition(sf::Vector2f(x, y));
    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    for (int i = 0; i <= 100; i++) {
        float x = i / 100.0f;  // So x goes 0.00, 0.01, 0.02, ... 1.00
        float y = tween(0.0f, 1.0f, x);

        float screenX = GRAPH_LEFT + x * (GRAPH_RIGHT - GRAPH_LEFT);
        float screenY = GRAPH_BOTTOM - y * (GRAPH_BOTTOM - GRAPH_TOP);

        sf::CircleShape point(2.0f);
        point.setPosition(sf::Vector2f(screenX, screenY));
        window.draw(point);
    }

    float curveY = tween(0.0f, 1.0f, t);
    float dotScreenX = GRAPH_LEFT + t * (GRAPH_RIGHT - GRAPH_LEFT);
    float dotScreenY = GRAPH_BOTTOM - curveY * (GRAPH_BOTTOM - GRAPH_TOP);
    sf::CircleShape currentDot(5.0f);
    currentDot.setPosition(sf::Vector2f(dotScreenX, dotScreenY));
    window.draw(currentDot);

    sf::RectangleShape xAxis(sf::Vector2f(GRAPH_RIGHT - GRAPH_LEFT, 2.0f));
    xAxis.setPosition(sf::Vector2f(GRAPH_LEFT, GRAPH_BOTTOM));
    window.draw(xAxis);

    sf::RectangleShape yAxis(sf::Vector2f(2.0f, GRAPH_BOTTOM - GRAPH_TOP));
    yAxis.setPosition(sf::Vector2f(GRAPH_LEFT, GRAPH_TOP));
    window.draw(yAxis);

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
