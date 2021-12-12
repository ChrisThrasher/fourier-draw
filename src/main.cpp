#include "epicycles.h"
#include "line.h"

#include <cmath>

static constexpr auto width = 1920u;
static constexpr auto height = 1080u;

static auto transform(const Line& line)
{
    auto x_signal = std::vector<float>();
    auto y_signal = std::vector<float>();
    for (size_t i = 0; i < line.size(); ++i) {
        const auto point = line.at(i);
        x_signal.push_back(point.x - width / 2.0f);
        y_signal.push_back(point.y - height / 2.0f);
    }

    const auto x_epicycles = Epicycles(discrete_fourier_transform(x_signal), { width / 2.0f, 200.0f }, 0.0f);
    const auto y_epicycles = Epicycles(discrete_fourier_transform(y_signal), { 200.0f, height / 2.0f }, half_pi);

    return std::make_pair(x_epicycles, y_epicycles);
}

int main()
{
    auto x_epicycles = Epicycles();
    auto y_epicycles = Epicycles();
    auto signal = Line();
    auto line = Line();
    auto line_shadow = Line();
    auto frame_count = (size_t)0;

    auto font = sf::Font();
    font.loadFromFile(std::string(FONT_PATH) + "/font.ttf");

    auto text = sf::Text("Click and drag to draw a curve", font, 48);
    text.setOrigin({ text.getLocalBounds().left + text.getLocalBounds().width / 2.0f,
                     text.getLocalBounds().top + text.getLocalBounds().height / 2.0f });
    text.setPosition({ width / 2.0f, 150.0f });
    text.setFillColor(sf::Color::White);

    auto window = sf::RenderWindow(sf::VideoMode(width, height), "Fourier Draw");
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Space:
                    signal = {};
                    x_epicycles = {};
                    y_epicycles = {};
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        window.clear();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            const auto mouse = sf::Mouse::getPosition(window);
            const auto is_within_x = mouse.x >= 0 && mouse.x <= (int)window.getSize().x;
            const auto is_within_y = mouse.y >= 0 && mouse.y <= (int)window.getSize().y;
            if (is_within_x && is_within_y) {
                const auto position = sf::Vector2f((float)mouse.x * width / (float)window.getSize().x,
                                                   (float)mouse.y * height / (float)window.getSize().y);
                const auto reset_line
                    = [position, &signal, &line, &line_shadow, &x_epicycles, &y_epicycles, &frame_count]() {
                          signal.push_back(position);
                          line = {};
                          line_shadow = signal;
                          std::tie(x_epicycles, y_epicycles) = transform(signal);
                          frame_count = 0;
                      };
                if (signal.empty()) {
                    reset_line();
                } else {
                    const auto vector = position - signal.back();
                    const auto length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
                    if (length > 2.0f)
                        reset_line();
                }
            }
        }

        if (x_epicycles.empty()) {
            window.draw(text);
            window.display();
            continue;
        }

        line_shadow.set_brightness((sf::Uint8)std::max(196 - 4 * (int)frame_count, 0));
        window.draw(line_shadow);

        if (frame_count % 2 == 0) {
            x_epicycles.update();
            y_epicycles.update();
            line.push_back(signal.at(frame_count / 2));
        }

        window.draw(x_epicycles);
        window.draw(y_epicycles);
        window.draw(line);

        constexpr auto stroke = 2.5f;
        auto vertical = sf::RectangleShape({ stroke, y_epicycles.tip().y - x_epicycles.tip().y });
        vertical.setOrigin({ 0.0f, stroke / 2.0f });
        vertical.setPosition(x_epicycles.tip());
        vertical.setFillColor(sf::Color(255, 255, 255, 128));
        window.draw(vertical);

        auto horizontal = sf::RectangleShape({ x_epicycles.tip().x - y_epicycles.tip().x, stroke });
        horizontal.setOrigin({ 0.0f, stroke / 2.0f });
        horizontal.setPosition(y_epicycles.tip());
        horizontal.setFillColor(sf::Color(255, 255, 255, 128));
        window.draw(horizontal);

        if (++frame_count == 2 * x_epicycles.size()) {
            frame_count = 0;
            line_shadow = line;
            line = {};
        }

        window.display();
    }
}
